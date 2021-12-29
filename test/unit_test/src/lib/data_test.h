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

#include "src/lib/Data.h"

#include <gmock/gmock.h>

class DataTest : public ::testing::Test
{
public:
    lib::PerformanceData* perf_data {nullptr};
    lib::AccLatencyData* lat_data {nullptr};
    lib::QueueData* queue_data {nullptr};
    lib::HistogramData* histogram_data {nullptr};
    lib::AccHistogramData* acc_histogram_data {nullptr};

protected:
    DataTest()
    {
        perf_data = new lib::PerformanceData;
        lat_data = new lib::AccLatencyData;
        queue_data = new lib::QueueData;
        histogram_data = new lib::HistogramData;
        acc_histogram_data = new lib::AccHistogramData;
    }
    ~DataTest() override
    {
        if (nullptr != perf_data)
        {
            delete perf_data;
            perf_data = nullptr;
        }
        if (nullptr != lat_data)
        {
            delete lat_data;
            lat_data = nullptr;
        }
        if (nullptr != queue_data)
        {
            delete queue_data;
            queue_data = nullptr;
        }
        if (nullptr != histogram_data)
        {
            delete histogram_data;
            histogram_data = nullptr;
        }
        if (nullptr != acc_histogram_data)
        {
            delete acc_histogram_data;
            acc_histogram_data = nullptr;
        }
    }
    void
    SetUp() override
    {
        perf_data->iops = 100;
        lat_data->max = 0x0000FEAD;
        queue_data->sum_depth = 0xFF00EE00DD00CA00;
        queue_data->depth_period_avg = 3.21f;
    }
    void
    TearDown() override
    {
    }
};
