#include "test/system_test/data_transfer/dummy_log.h"

#include <pthread.h>
#include <unistd.h>

#include "src/api/Air.h"

bool DummyLog::run = true;

void
DummyLog::Run(void)
{
    run = true;
}

void
DummyLog::Stop(void)
{
    run = false;
}

void
DummyLog::TestLog(void)
{
    cpu_set_t cpu {0};
    CPU_SET(2, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);
    pthread_setname_np(pthread_self(), "DummyLog");
    uint64_t lat_id {0};

    while (run)
    {
        airlog("PERF_BENCHMARK", "read", 0, 4096);
        airlog("PERF_BENCHMARK", "read", 0, 4096);
        airlog("PERF_BENCHMARK", "read", 3, 4096);
        airlog("PERF_BENCHMARK", "write", 0, 16384);
        airlog("LAT_IO_PATH", "range_0", 1, lat_id);
        usleep(100);
        airlog("LAT_IO_PATH", "range_1", 1, lat_id);
        lat_id++;
    }
}
