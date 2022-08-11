/*
 *   MIT License
 *
 *   Copyright (c) 2021 Samsung Electronics Corporation
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

#include "test/system_test/latency_resolution/latency_log.h"

#include <pthread.h>
#include <unistd.h>

#include <future>
#include <iostream>
#include <thread>

#include "src/api/Air.h"

bool LatencyLog::run = true;

void
LatencyLog::Run(void)
{
    run = true;
}

void
LatencyLog::Stop(void)
{
    run = false;
}

void
LatencyLog::TestLog(void)
{
    auto job1 {std::async(std::launch::async, _LogNano)};
    auto job2 {std::async(std::launch::async, _LogMicro)};
    auto job3 {std::async(std::launch::async, _LogMilli_1)};
    auto job4 {std::async(std::launch::async, _LogMilli_2)};
    auto job5 {std::async(std::launch::async, _LogMilli_3)};
    auto job6 {std::async(std::launch::async, _LogSecond)};

    job1.get();
    job2.get();
    job3.get();
    job4.get();
    job5.get();
    job6.get();
}

void
LatencyLog::_LogNano(void)
{
    cpu_set_t cpu {0};
    CPU_SET(1, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);
    pthread_setname_np(pthread_self(), "LogNano");
    std::cout << "LogNano thread run\n";

    uint64_t key {0};
    while (run)
    {
        airlog("LAT_DUMMY_00", "range_0", 0, key);
        airlog("LAT_DUMMY_00", "range_1", 0, key);
        key++;
    }
}

void
LatencyLog::_LogMicro(void)
{
    cpu_set_t cpu {0};
    CPU_SET(2, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);
    pthread_setname_np(pthread_self(), "LogMicro");
    std::cout << "LogMicro thread run\n";

    uint64_t key {0};
    while (run)
    {
        airlog("LAT_DUMMY_01", "range_0", 0, key);
        usleep(1);
        airlog("LAT_DUMMY_01", "range_1", 0, key);

        airlog("LAT_DUMMY_01", "range_0", 1, key);
        usleep(10);
        airlog("LAT_DUMMY_01", "range_1", 1, key);

        airlog("LAT_DUMMY_01", "range_0", 2, key);
        usleep(100);
        airlog("LAT_DUMMY_01", "range_1", 2, key);

        airlog("LAT_DUMMY_01", "range_0", 3, key);
        usleep(200);
        airlog("LAT_DUMMY_01", "range_1", 3, key);

        key++;
    }
}

void
LatencyLog::_LogMilli_1(void)
{
    cpu_set_t cpu {0};
    CPU_SET(3, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);
    pthread_setname_np(pthread_self(), "LogMilli_1");
    std::cout << "LogMilli_1 thread run\n";

    uint64_t key {0};
    while (run)
    {
        airlog("LAT_DUMMY_02", "range_0", 0, key);
        usleep(1000); // 1ms
        airlog("LAT_DUMMY_02", "range_1", 0, key);

        airlog("LAT_DUMMY_02", "range_0", 1, key);
        usleep(10000); // 10ms
        airlog("LAT_DUMMY_02", "range_1", 1, key);

        airlog("LAT_DUMMY_02", "range_0", 2, key);
        usleep(100000); // 100ms
        airlog("LAT_DUMMY_02", "range_1", 2, key);

        key++;
    }
}

void
LatencyLog::_LogMilli_2(void)
{
    cpu_set_t cpu {0};
    CPU_SET(4, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);
    pthread_setname_np(pthread_self(), "LogMilli_2");
    std::cout << "LogMilli_2 thread run\n";

    uint64_t key {0};
    while (run)
    {
        airlog("LAT_DUMMY_03", "range_0", 0, key);
        usleep(300000); // 300ms
        airlog("LAT_DUMMY_03", "range_1", 0, key);

        key++;
    }
}

void
LatencyLog::_LogMilli_3(void)
{
    cpu_set_t cpu {0};
    CPU_SET(5, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);
    pthread_setname_np(pthread_self(), "LogMilli_3");
    std::cout << "LogMilli_3 thread run\n";

    uint64_t key {0};
    while (run)
    {
        airlog("LAT_DUMMY_04", "range_0", 0, key);
        usleep(500000); // 500ms
        airlog("LAT_DUMMY_04", "range_1", 0, key);

        key++;
    }
}

void
LatencyLog::_LogSecond(void)
{
    cpu_set_t cpu {0};
    CPU_SET(6, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);
    pthread_setname_np(pthread_self(), "LogSecond");
    std::cout << "LogSecond thread run\n";

    uint64_t key {0};
    while (run)
    {
        airlog("LAT_DUMMY_05", "range_0", 0, key);
        usleep(1100000); // 1.1s
        airlog("LAT_DUMMY_05", "range_1", 0, key);

        key++;
    }
}
