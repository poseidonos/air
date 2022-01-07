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

#include "test/system_test/api_histogram/histogram_log.h"

#include <air/Air.h>
#include <pthread.h>
#include <unistd.h>

bool HistogramLog::run = true;

void
HistogramLog::Run(void)
{
    run = true;
}

void
HistogramLog::Stop(void)
{
    run = false;
}

void
HistogramLog::TestLog(void)
{
    cpu_set_t cpu{0};
    CPU_SET(2, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);
    pthread_setname_np(pthread_self(), "HistogramLog");

    while (run)
    {
        airlog("HIST_SAMPLE_1", "AIR_0", 0, 33); // bucket[3]
        airlog("HIST_SAMPLE_1", "AIR_1", 0, 33); // bucket[3]
        airlog("HIST_SAMPLE_1", "AIR_2", 0, 33); // bucket[3]
        airlog("HIST_SAMPLE_1", "AIR_3", 0, 33); // bucket[3]
        airlog("HIST_SAMPLE_1", "AIR_3", 0, 33); // bucket[3]
        airlog("HIST_SAMPLE_1", "AIR_3", 0, 33); // bucket[3]

        airlog("HIST_SAMPLE_2", "AIR_0", 0, 33); // bucket[0]
        airlog("HIST_SAMPLE_2", "AIR_0", 0, 65); // bucket[10]

        airlog("HIST_SAMPLE_3", "AIR_0", 0, -101); // underflow
        airlog("HIST_SAMPLE_3", "AIR_0", 0, -100); // bucket[0]
        airlog("HIST_SAMPLE_3", "AIR_0", 0, -80); // bucket[1]
        airlog("HIST_SAMPLE_3", "AIR_0", 0, -60); // bucket[2]
        airlog("HIST_SAMPLE_3", "AIR_0", 0, -40); // bucket[3]
        airlog("HIST_SAMPLE_3", "AIR_0", 0, -20); // bucket[4]
        airlog("HIST_SAMPLE_3", "AIR_0", 1, 0); // bucket[5]
        airlog("HIST_SAMPLE_3", "AIR_0", 1, 20); // bucket[6]
        airlog("HIST_SAMPLE_3", "AIR_0", 1, 40); // bucket[7]
        airlog("HIST_SAMPLE_3", "AIR_0", 1, 60); // bucket[8]
        airlog("HIST_SAMPLE_3", "AIR_0", 1, 80); // overflow

        airlog("HIST_SAMPLE_4", "AIR_0", 0, 0); // underflow
        airlog("HIST_SAMPLE_4", "AIR_0", 0, 1); // bucket[0]
        airlog("HIST_SAMPLE_4", "AIR_0", 0, 2); // bucket[1]
        airlog("HIST_SAMPLE_4", "AIR_0", 0, 1023); // bucket[9]
        airlog("HIST_SAMPLE_4", "AIR_0", 0, 1024); // overflow

        airlog("HIST_SAMPLE_5", "AIR_0", 0, -8966555); // underflow
        airlog("HIST_SAMPLE_5", "AIR_0", 0, -16); // bucket[7]
        airlog("HIST_SAMPLE_5", "AIR_0", 0, -15); // overflow

        airlog("HIST_SAMPLE_6", "AIR_0", 0, -1000); // underflow
        airlog("HIST_SAMPLE_6", "AIR_0", 0, -100); // bucket[0]
        airlog("HIST_SAMPLE_6", "AIR_0", 0, -10); // bucket[1]
        airlog("HIST_SAMPLE_6", "AIR_0", 0, -1); // bucket[2]
        airlog("HIST_SAMPLE_6", "AIR_0", 0, 0); // bucket[3]
        airlog("HIST_SAMPLE_6", "AIR_0", 0, 9); // bucket[4]
        airlog("HIST_SAMPLE_6", "AIR_0", 0, 99); // bucket[5]
        airlog("HIST_SAMPLE_6", "AIR_0", 0, 999); // bucket[6]
        airlog("HIST_SAMPLE_6", "AIR_0", 0, 9999); // bucket[7]
        airlog("HIST_SAMPLE_6", "AIR_0", 0, 99999); // bucket[8]
        airlog("HIST_SAMPLE_6", "AIR_0", 0, 100000); // overflow

        usleep(100);
    }
}
