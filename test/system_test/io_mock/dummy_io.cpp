
#include "dummy_io.h"

#include <pthread.h>

#include <string>
#include <thread>

#include "src/api/Air.h"

bool DummyIO::run = true;

unsigned int DummyIO::read_iops = 0;
std::mutex DummyIO::iops_lock;

std::queue<int> DummyIO::sq[2];
std::mutex DummyIO::sq_lock[2];

std::queue<int> DummyIO::cq[2];
std::mutex DummyIO::cq_lock[2];

void
DummyIO::Run()
{
    run = true;
}

void
DummyIO::Stop()
{
    run = false;
}

void
DummyIO::Print()
{
    // core affinity 2
    cpu_set_t cpu {0};
    CPU_SET(2, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);
    pthread_setname_np(pthread_self(), "Print");

    while (run)
    {
        sleep(1);
        printf(" [DummyIO] Read_IOPS: %8u\r", read_iops);
        fflush(stdout);
        std::lock_guard<std::mutex> guard(iops_lock);
        read_iops = 0;
    }
}

void
DummyIO::SubmitIO()
{
    // core affinity 3
    cpu_set_t cpu {0};
    CPU_SET(3, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);
    pthread_setname_np(pthread_self(), "SubmitIO");

    int i {0};
    bool sq_push {false};

    while (run)
    {
        if (1000000000 < i)
            i = 0;

        {
            airlog("LAT_SUBMIT", "range_0", 0, i);
            airlog("LAT_IO_PATH", "range_0", 0, i);

            std::lock_guard<std::mutex> guard(sq_lock[0]);
            if (sq[0].size() < 256)
            {
                sq[0].push(i);
                sq_push = true;
            }
            else
            {
                sq_push = false;
            }

            if (sq_push)
            {
                airlog("LAT_IO_PATH", "range_1", 0, i);
                airlog("LAT_SUBMIT", "range_1", 0, i);
                i++;
            }
            airlog("UTIL_SUBMIT_THR", "submit", 0, 10);
        }

        {
            airlog("LAT_IO_PATH", "range_0", 1, i);

            std::lock_guard<std::mutex> guard(sq_lock[1]);
            if (sq[1].size() < 256)
            {
                sq[1].push(i);
                sq_push = true;
            }
            else
            {
                sq_push = false;
            }

            if (sq_push)
            {
                airlog("LAT_IO_PATH", "range_1", 1, i);
                i++;
            }
            airlog("UTIL_SUBMIT_THR", "submit", 1, 10);
        }
    }
}

void
DummyIO::ProcessIO(int qid)
{
    // core affinity 4, 5
    cpu_set_t cpu {0};
    CPU_SET(4 + qid, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);
    pthread_setname_np(pthread_self(), "ProcessIO");

    while (run)
    {
        bool valid {false};
        int value {0};

        {
            std::lock_guard<std::mutex> guard(sq_lock[qid]);
            if (!sq[qid].empty())
            {
                airlog("Q_SUBMISSION", "base", qid, sq[qid].size());
                valid = true;
                value = sq[qid].front();
                sq[qid].pop();

                airlog("LAT_PROCESS", "range_0", 512 + qid, value);
                airlog("LAT_IO_PATH", "range_2", qid, value);
                airlog("UTIL_SUBMIT_THR", "process", qid, 10);
            }
        }

        if (valid)
        {
            std::lock_guard<std::mutex> guard(cq_lock[qid]);
            if (cq[qid].size() < 256)
            {
                cq[qid].push(value);
                airlog("LAT_PROCESS", "range_1", 512 + qid, value);
            }
        }
    }
}

void
DummyIO::CompleteIO()
{
    // core affinity 6
    cpu_set_t cpu {0};
    CPU_SET(6, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);
    pthread_setname_np(pthread_self(), "CompleteIO");

    while (run)
    {
        int count {0};
        int value {0};

        {
            std::lock_guard<std::mutex> guard(cq_lock[0]);
            if (!cq[0].empty())
            {
                airlog("Q_COMPLETION", "base", 0, cq[0].size());
                count++;
                value = cq[0].front();

                airlog("LAT_COMPLETE", "range_0", 0, value);

                cq[0].pop();

                airlog("PERF_BENCHMARK", "read", 512, 4096);
                airlog("CNT_TEST_EVENT", "complete", 0, 1);
                airlog("LAT_IO_PATH", "range_3", 0, value);
                airlog("LAT_COMPLETE", "range_1", 0, value);
                airlog("UTIL_SUBMIT_THR", "complete", 0, 3);
            }
        }

        {
            std::lock_guard<std::mutex> guard(cq_lock[1]);
            if (!cq[1].empty())
            {
                airlog("Q_COMPLETION", "base", 1, cq[1].size());
                count++;
                value = cq[1].front();
                cq[1].pop();

                airlog("PERF_BENCHMARK", "read", 513, 4096);
                airlog("CNT_TEST_EVENT", "complete", 1, -1);

                airlog("LAT_IO_PATH", "range_3", 1, value);
                airlog("UTIL_SUBMIT_THR", "complete", 1, 10);
            }
        }

        if (count)
        {
            std::lock_guard<std::mutex> guard(iops_lock);
            read_iops += count;
        }
    }
}
