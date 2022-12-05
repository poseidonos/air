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

#include "collector_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST_F(CollectorTest, PerformanceCollector_LogData)
{
    lib::Data* data {new lib::PerformanceData};
    lib::PerformanceData* perf_data {static_cast<lib::PerformanceData*>(data)};

    performance_collector->LogData(data, 512);
    EXPECT_EQ(1, perf_data->period_iops);
    EXPECT_EQ(512, perf_data->period_bandwidth);

    delete data;
}

TEST_F(CollectorTest, PerformanceCollector_InformInit)
{
    lib::AccData* data_dirty {new lib::AccPerformanceData};
    lib::AccPerformanceData* perf_data {
        static_cast<lib::AccPerformanceData*>(data_dirty)};
    unsigned int value {0};

    EXPECT_EQ(value, perf_data->need_erase);
    performance_collector->InformInit(data_dirty);
    value = 1;
    EXPECT_EQ(value, perf_data->need_erase);

    delete data_dirty;
    data_dirty = nullptr;
    performance_collector->InformInit(data_dirty);
}

TEST_F(CollectorTest, LeatencyCollector_LogData)
{
    lib::Data* data {new lib::LatencyData};
    lib::LatencyData* latency_data {static_cast<lib::LatencyData*>(data)};

    latency_data->start_state = lib::TimeLogState::RUN;
    latency_data->start_token = 43;
    latency_collector->LogData(data, 1234);

    EXPECT_EQ(42, latency_data->start_token);

    delete data;
}

TEST_F(CollectorTest, LatencyCollector_InformInit)
{
    lib::AccData* data_dirty {new lib::AccLatencyData};
    lib::AccLatencyData* lat_data {static_cast<lib::AccLatencyData*>(data_dirty)};

    uint32_t value {0};
    EXPECT_EQ(value, lat_data->need_erase);
    latency_collector->InformInit(data_dirty);
    value = 1;
    EXPECT_EQ(value, lat_data->need_erase);

    delete data_dirty;
}

TEST_F(CollectorTest, QueueCollector_LogData)
{
    lib::Data* data {new lib::QueueData};
    lib::QueueData* queue_data {static_cast<lib::QueueData*>(data)};

    queue_collector->LogData(data, 10);
    EXPECT_EQ(10, queue_data->period_qd_sum);
    EXPECT_EQ(1, queue_data->period_num_req);

    delete data;
}

TEST_F(CollectorTest, QueueCollector_InformInit)
{
    lib::AccData* data_dirty {new lib::AccQueueData};
    lib::AccQueueData* q_data {static_cast<lib::AccQueueData*>(data_dirty)};

    uint32_t value {0};
    EXPECT_EQ(value, q_data->need_erase);
    queue_collector->InformInit(data_dirty);
    value = 1;
    EXPECT_EQ(value, q_data->need_erase);

    delete data_dirty;
}

TEST_F(CollectorTest, CountCollector_LogData)
{
    lib::Data* data {new lib::CountData};
    lib::CountData* count_data {static_cast<lib::CountData*>(data)};

    count_collector->LogData(data, 10);
    count_collector->LogData(data, -1);
    count_collector->LogData(data, -10000);
    count_collector->LogData(data, 0);

    EXPECT_EQ(10, count_data->period_count_positive);
    EXPECT_EQ(2, count_data->period_num_req_positive);
    EXPECT_EQ(10001, count_data->period_count_negative);
    EXPECT_EQ(2, count_data->period_num_req_negative);

    delete data;
}

TEST_F(CollectorTest, CountCollector_InformInit)
{
    lib::AccData* data_dirty {new lib::AccCountData};
    lib::AccCountData* count_data {static_cast<lib::AccCountData*>(data_dirty)};

    EXPECT_EQ(0, count_data->need_erase);
    count_collector->InformInit(data_dirty);
    EXPECT_EQ(1, count_data->need_erase);

    delete data_dirty;
    data_dirty = nullptr;
    count_collector->InformInit(data_dirty);
}

TEST_F(CollectorTest, UtilizationCollector_LogData)
{
    lib::Data* data {new lib::UtilizationData};
    lib::UtilizationData* util_data {static_cast<lib::UtilizationData*>(data)};

    util_collector->LogData(data, 131230);
    EXPECT_EQ(131230, util_data->period_usage);

    delete data;
}

TEST_F(CollectorTest, UtilizationCollector_InformInit)
{
    lib::AccData* data_dirty {new lib::AccUtilizationData};
    lib::AccUtilizationData* util_data {
        static_cast<lib::AccUtilizationData*>(data_dirty)};

    EXPECT_EQ(0, util_data->need_erase);
    util_collector->InformInit(data_dirty);
    EXPECT_EQ(1, util_data->need_erase);

    delete data_dirty;
    data_dirty = nullptr;
    util_collector->InformInit(data_dirty);
}

TEST_F(CollectorTest, HistogramCollector_LogData)
{
    lib::Data* data {new lib::HistogramData};
    lib::HistogramData* histogram_data {static_cast<lib::HistogramData*>(data)};
    histogram_data->bucket_lower_bound = 30;

    histogram_collector->LogData(data, 29);
    EXPECT_EQ(1, histogram_data->period_underflow);
    delete data;
}

TEST_F(CollectorTest, HistogramCollector_InformInit)
{
    lib::AccData* data_dirty {new lib::AccHistogramData};
    lib::AccHistogramData* histogram_data {
        static_cast<lib::AccHistogramData*>(data_dirty)};

    EXPECT_EQ(0, histogram_data->need_erase);
    histogram_collector->InformInit(data_dirty);
    EXPECT_EQ(1, histogram_data->need_erase);

    delete data_dirty;
    data_dirty = nullptr;
    histogram_collector->InformInit(data_dirty);
}
