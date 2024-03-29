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

#include "test/system_test/sparsely_log/sparsely_log.h"

#include <pthread.h>
#include <unistd.h>

#include <future>
#include <iostream>
#include <thread>

#include "src/api/Air.h"

bool SparselyLog::run = true;

void
SparselyLog::Run(void)
{
    run = true;
}

void
SparselyLog::Stop(void)
{
    run = false;
}

void
SparselyLog::TestLog(void)
{
    auto job1 {std::async(std::launch::async, _Log)};

    job1.get();
}

void
SparselyLog::_Log(void)
{
    cpu_set_t cpu {0};
    CPU_SET(1, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);
    pthread_setname_np(pthread_self(), "LogThread");
    std::cout << "LogThread run\n";

    uint64_t key {0};
    while (run)
    {
        airlog("PERF_BENCHMARK", "read", 0, 4096);
        airlog("PERF_BENCHMARK", "read", 0, 4096);
        airlog("PERF_BENCHMARK", "read", 0, 4096);
        airlog("LAT_SUBMIT", "range_0", 0, key);
        airlog("LAT_SUBMIT", "range_1", 0, key);
        key++;
        airlog("Q_SUBMISSION", "base", 0, 10);
        airlog("Q_SUBMISSION", "base", 0, 20);
        airlog("Q_SUBMISSION", "base", 0, 30);
        airlog("UTIL_SUBMIT_THR", "submit", 0, 3);
        airlog("UTIL_SUBMIT_THR", "submit", 0, 3);
        airlog("UTIL_SUBMIT_THR", "submit", 0, 3);
        airlog("CNT_TEST_EVENT", "submit", 0, 2);
        airlog("CNT_TEST_EVENT", "submit", 0, 2);
        airlog("CNT_TEST_EVENT", "submit", 0, 2);
        airlog("HIST_SAMPLE_3", "range_0", 0, 10);
        airlog("HIST_SAMPLE_3", "range_0", 0, -40);
        airlog("HIST_SAMPLE_3", "range_3", 0, 50);

        sleep(10);
    }
}
