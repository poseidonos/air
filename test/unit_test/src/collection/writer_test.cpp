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

#include "writer_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST_F(WriterTest, PerformanceWriter_LogData)
{
    lib::Data* data {new lib::PerformanceData};
    lib::PerformanceData* perf_data {static_cast<lib::PerformanceData*>(data)};

    performance_writer->LogData(data, 512);
    EXPECT_EQ(1, perf_data->period_iops);
    EXPECT_EQ(512, perf_data->period_bandwidth);

    performance_writer->LogData(data, 1024);
    EXPECT_EQ(2, perf_data->period_iops);
    EXPECT_EQ(1536, perf_data->period_bandwidth);

    performance_writer->LogData(data, 2048);
    EXPECT_EQ(3, perf_data->period_packet_cnt.size());
    EXPECT_EQ(1, perf_data->period_packet_cnt[2048]);

    for (uint32_t i = 0; i < 100; i++)
    {
        performance_writer->LogData(data, i);
    }

    EXPECT_EQ(lib::MAX_PACKET_CNT_SIZE, perf_data->period_packet_cnt.size());

    delete data;
}

TEST_F(WriterTest, PerformanceWriter_InformInit)
{
    lib::AccData* data_dirty {new lib::AccPerformanceData};
    lib::AccPerformanceData* perf_data {
        static_cast<lib::AccPerformanceData*>(data_dirty)};

    unsigned int value {0};
    EXPECT_EQ(perf_data->need_erase, value);
    value = 1;
    performance_writer->InformInit(data_dirty);
    EXPECT_EQ(perf_data->need_erase, value);

    data_dirty = nullptr;
    performance_writer->InformInit(data_dirty);
    EXPECT_EQ(nullptr, data_dirty);

    delete data_dirty;
}

TEST_F(WriterTest, LatencyWriter_LogData)
{
    lib::Data* data_dirty {new lib::LatencyData};
    lib::LatencyData* lat_data {static_cast<lib::LatencyData*>(data_dirty)};

    lat_data->start_state = lib::TimeLogState::RUN;
    lat_data->start_token = 100;
    lat_data->end_token = 50;

    latency_writer->LogData(data_dirty, 1234);

    EXPECT_EQ((int)99, lat_data->start_token);
    EXPECT_EQ((int)50, lat_data->end_token);

    lat_data->start_state = lib::TimeLogState::RUN;
    lat_data->start_token = 1;

    latency_writer->LogData(data_dirty, 1234);
    EXPECT_EQ(0, lat_data->start_token);
    EXPECT_EQ(50, lat_data->end_token);
    EXPECT_EQ(lib::TimeLogState::FULL, lat_data->start_state);

    lat_data->end_state = lib::TimeLogState::RUN;
    latency_writer->LogData(data_dirty, 1234);
    EXPECT_EQ(0, lat_data->start_token);
    EXPECT_EQ(49, lat_data->end_token);

    lat_data->end_token = 1;
    latency_writer->LogData(data_dirty, 1234);
    EXPECT_EQ(0, lat_data->end_token);
    EXPECT_EQ(lib::TimeLogState::FULL, lat_data->end_state);

    delete data_dirty;
}

TEST_F(WriterTest, LatencyWriter_InformInit)
{
    lib::AccData* data_dirty {new lib::AccLatencyData};
    lib::AccLatencyData* lat_data {static_cast<lib::AccLatencyData*>(data_dirty)};

    unsigned int value {0};
    EXPECT_EQ(lat_data->need_erase, value);
    value = 1;
    latency_writer->InformInit(data_dirty);
    EXPECT_EQ(lat_data->need_erase, value);

    data_dirty = nullptr;
    latency_writer->InformInit(data_dirty);
    EXPECT_EQ(nullptr, data_dirty);

    delete data_dirty;
}

TEST_F(WriterTest, QueueWriter_LogData)
{
    lib::Data* data {new lib::QueueData};
    lib::QueueData* queue_data {static_cast<lib::QueueData*>(data)};

    queue_writer->LogData(data, 10);
    EXPECT_EQ(10, queue_data->period_qd_sum);
    EXPECT_EQ(1, queue_data->period_num_req);

    delete data;
}

TEST_F(WriterTest, QueueWriter_InformInit)
{
    lib::AccData* data_dirty {new lib::AccQueueData};
    lib::AccQueueData* q_data {static_cast<lib::AccQueueData*>(data_dirty)};

    uint64_t value {0};
    EXPECT_EQ(q_data->need_erase, value);
    queue_writer->InformInit(data_dirty);
    value = 1;
    EXPECT_EQ(q_data->need_erase, value);

    delete data_dirty;
    data_dirty = nullptr;
    queue_writer->InformInit(data_dirty);
    EXPECT_EQ(nullptr, data_dirty);
}

TEST_F(WriterTest, CountWriter_LogData)
{
    lib::Data* data {new lib::CountData};
    lib::CountData* count_data {static_cast<lib::CountData*>(data)};

    count_writer->LogData(data, 10);

    int16_t val1 = -10;
    count_writer->LogData(data, val1);
    int32_t val2 = -100;
    count_writer->LogData(data, val2);
    int64_t val3 = -1000;
    count_writer->LogData(data, val3);
    count_writer->LogData(data, -1);
    count_writer->LogData(data, -10000);

    EXPECT_EQ(10, count_data->period_count_positive);
    EXPECT_EQ(1, count_data->period_num_req_positive);
    EXPECT_EQ(11111, count_data->period_count_negative);
    EXPECT_EQ(5, count_data->period_num_req_negative);

    delete data;
}

TEST_F(WriterTest, CountWriter_InformInit)
{
    lib::AccData* data_dirty {new lib::AccCountData};
    lib::AccCountData* count_data {static_cast<lib::AccCountData*>(data_dirty)};

    EXPECT_EQ(0, count_data->need_erase);
    count_writer->InformInit(data_dirty);
    EXPECT_EQ(1, count_data->need_erase);

    delete data_dirty;
    data_dirty = nullptr;
    count_writer->InformInit(data_dirty);
    EXPECT_EQ(nullptr, data_dirty);
}

TEST_F(WriterTest, UtilizationWriter_LogData)
{
    lib::Data* data {new lib::UtilizationData};
    lib::UtilizationData* util_data {static_cast<lib::UtilizationData*>(data)};

    util_writer->LogData(data, 10);
    EXPECT_EQ(10, util_data->period_usage);

    delete data;
}

TEST_F(WriterTest, UtilizationWriter_InformInit)
{
    lib::AccData* data_dirty {new lib::AccUtilizationData};
    lib::AccUtilizationData* util_data {
        static_cast<lib::AccUtilizationData*>(data_dirty)};

    EXPECT_EQ(0, util_data->need_erase);
    util_writer->InformInit(data_dirty);
    EXPECT_EQ(1, util_data->need_erase);

    delete data_dirty;
    data_dirty = nullptr;
    util_writer->InformInit(data_dirty);
    EXPECT_EQ(nullptr, data_dirty);
}

TEST_F(WriterTest, HistogramWriter_InformInit)
{
    lib::AccData* data_dirty {new lib::AccHistogramData};
    lib::AccHistogramData* histogram_data {
        static_cast<lib::AccHistogramData*>(data_dirty)};

    EXPECT_EQ(0, histogram_data->need_erase);
    histogram_writer->InformInit(data_dirty);
    EXPECT_EQ(1, histogram_data->need_erase);

    delete data_dirty;
    data_dirty = nullptr;
    histogram_writer->InformInit(data_dirty);
    EXPECT_EQ(nullptr, data_dirty);
}

TEST_F(WriterTest, HistogramWriter_LogData_LinearSample1)
{
    // Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10
    lib::Data* data {new lib::HistogramData};
    lib::HistogramData* histogram_data {static_cast<lib::HistogramData*>(data)};
    histogram_data->bucket_lower_bound = 0;
    histogram_data->bucket_upper_bound = 99;
    histogram_data->bucket_scale = 10;
    histogram_data->bucket_size = 10;
    histogram_data->bucket_type = lib::BucketType::LINEAR;

    // underflow
    histogram_writer->LogData(histogram_data, -123);
    histogram_writer->LogData(histogram_data, -345);
    histogram_writer->LogData(histogram_data, -1);
    // bucket[0]: 0~9
    histogram_writer->LogData(histogram_data, 0);
    histogram_writer->LogData(histogram_data, 3);
    histogram_writer->LogData(histogram_data, 3);
    histogram_writer->LogData(histogram_data, 9);
    // bucket[1]: 10~19
    histogram_writer->LogData(histogram_data, 10);
    histogram_writer->LogData(histogram_data, 19);
    // bucket[9]: 90~99
    histogram_writer->LogData(histogram_data, 90);
    histogram_writer->LogData(histogram_data, 97);
    histogram_writer->LogData(histogram_data, 99);
    // overflow
    histogram_writer->LogData(histogram_data, 100);
    histogram_writer->LogData(histogram_data, 4356);

    EXPECT_EQ(-345, histogram_data->period_min_value);
    EXPECT_EQ(3, histogram_data->period_underflow);
    EXPECT_EQ(4, histogram_data->period_bucket[0]);
    EXPECT_EQ(2, histogram_data->period_bucket[1]);
    EXPECT_EQ(3, histogram_data->period_bucket[9]);
    EXPECT_EQ(4356, histogram_data->period_max_value);
    EXPECT_EQ(2, histogram_data->period_overflow);

    delete data;
}

TEST_F(WriterTest, HistogramWriter_LogData_LinearSample2)
{
    // Scale: 3, Bucket: BUCKET_2, Bounds: [33, 66)
    lib::Data* data {new lib::HistogramData};
    lib::HistogramData* histogram_data {static_cast<lib::HistogramData*>(data)};
    histogram_data->bucket_lower_bound = 33;
    histogram_data->bucket_upper_bound = 65;
    histogram_data->bucket_scale = 3;
    histogram_data->bucket_size = 11;
    histogram_data->bucket_type = lib::BucketType::LINEAR;

    // underflow
    histogram_writer->LogData(histogram_data, 32);
    // bucket[0]: 33~35
    histogram_writer->LogData(histogram_data, 33);
    histogram_writer->LogData(histogram_data, 33);
    histogram_writer->LogData(histogram_data, 34);
    histogram_writer->LogData(histogram_data, 35);
    // bucket[10]: 63~65
    histogram_writer->LogData(histogram_data, 63);
    histogram_writer->LogData(histogram_data, 64);
    histogram_writer->LogData(histogram_data, 65);
    // overflow
    histogram_writer->LogData(histogram_data, 66);
    histogram_writer->LogData(histogram_data, 67);

    EXPECT_EQ(32, histogram_data->period_min_value);
    EXPECT_EQ(1, histogram_data->period_underflow);
    EXPECT_EQ(4, histogram_data->period_bucket[0]);
    EXPECT_EQ(3, histogram_data->period_bucket[10]);
    EXPECT_EQ(67, histogram_data->period_max_value);
    EXPECT_EQ(2, histogram_data->period_overflow);

    delete data;
}

TEST_F(WriterTest, HistogramWriter_LogData_LinearSample3)
{
    // Bounds: [-100, 80), Scale: 20, Bucket: BUCKET_3
    lib::Data* data {new lib::HistogramData};
    lib::HistogramData* histogram_data {static_cast<lib::HistogramData*>(data)};
    histogram_data->bucket_lower_bound = -100;
    histogram_data->bucket_upper_bound = 79;
    histogram_data->bucket_scale = 20;
    histogram_data->bucket_size = 9;
    histogram_data->bucket_type = lib::BucketType::LINEAR;

    // underflow
    histogram_writer->LogData(histogram_data, -101);
    histogram_writer->LogData(histogram_data, -3245);
    // bucket[0]: -100 ~ -81
    histogram_writer->LogData(histogram_data, -100);
    histogram_writer->LogData(histogram_data, -81);
    histogram_writer->LogData(histogram_data, -93);
    // bucket[8]: 60~79
    histogram_writer->LogData(histogram_data, 60);
    histogram_writer->LogData(histogram_data, 72);
    histogram_writer->LogData(histogram_data, 79);
    // overflow
    histogram_writer->LogData(histogram_data, 80);
    histogram_writer->LogData(histogram_data, 9875632);

    EXPECT_EQ(-3245, histogram_data->period_min_value);
    EXPECT_EQ(2, histogram_data->period_underflow);
    EXPECT_EQ(3, histogram_data->period_bucket[0]);
    EXPECT_EQ(3, histogram_data->period_bucket[8]);
    EXPECT_EQ(9875632, histogram_data->period_max_value);
    EXPECT_EQ(2, histogram_data->period_overflow);

    delete data;
}

TEST_F(WriterTest, HistogramWriter_LogData_ExponentialSample1)
{
    // Bucket: BUCKET_4, Bounds: [2^0, 2^10), Scale: 2^
    lib::Data* data {new lib::HistogramData};
    lib::HistogramData* histogram_data {static_cast<lib::HistogramData*>(data)};
    histogram_data->bucket_lower_bound = 1;
    histogram_data->bucket_upper_bound = 1023;
    histogram_data->bucket_scale = 2;
    histogram_data->bucket_size = 10;
    histogram_data->bucket_type = lib::BucketType::EXPONENTIAL;
    histogram_data->bucket_zero_index = -1; // 10(size) - 1 - 10(upper_exponent)

    // underflow
    histogram_writer->LogData(histogram_data, 0);
    histogram_writer->LogData(histogram_data, -333222111);
    // bucket[0]: 1 ~ 1
    histogram_writer->LogData(histogram_data, 1);
    histogram_writer->LogData(histogram_data, 1);
    histogram_writer->LogData(histogram_data, 1);
    // bucket[1]: 2 ~ 3
    histogram_writer->LogData(histogram_data, 2);
    histogram_writer->LogData(histogram_data, 3);
    // bucket[9]: 512~1023
    histogram_writer->LogData(histogram_data, 512);
    histogram_writer->LogData(histogram_data, 732);
    histogram_writer->LogData(histogram_data, 1023);
    // overflow
    histogram_writer->LogData(histogram_data, 1024);
    histogram_writer->LogData(histogram_data, 102410241024);

    EXPECT_EQ(-333222111, histogram_data->period_min_value);
    EXPECT_EQ(2, histogram_data->period_underflow);
    EXPECT_EQ(3, histogram_data->period_bucket[0]);
    EXPECT_EQ(2, histogram_data->period_bucket[1]);
    EXPECT_EQ(3, histogram_data->period_bucket[9]);
    EXPECT_EQ(102410241024, histogram_data->period_max_value);
    EXPECT_EQ(2, histogram_data->period_overflow);

    delete data;
}

TEST_F(WriterTest, HistogramWriter_LogData_ExponentialSample2)
{
    // Bucket: BUCKET_5, Bounds: (-4^6, -4^2], Scale: 2^
    lib::Data* data {new lib::HistogramData};
    lib::HistogramData* histogram_data {static_cast<lib::HistogramData*>(data)};
    histogram_data->bucket_lower_bound = -4095;
    histogram_data->bucket_upper_bound = -16;
    histogram_data->bucket_scale = 2;
    histogram_data->bucket_size = 8;
    histogram_data->bucket_type = lib::BucketType::EXPONENTIAL;
    histogram_data->bucket_zero_index = 12; // -1 * -12(lower_exponent)

    // underflow
    histogram_writer->LogData(histogram_data, -4096);
    histogram_writer->LogData(histogram_data, -8181);
    // bucket[0]: -4095 ~ -2048
    histogram_writer->LogData(histogram_data, -4095);
    histogram_writer->LogData(histogram_data, -3030);
    histogram_writer->LogData(histogram_data, -2048);
    // bucket[1]: -2047 ~ -1024
    histogram_writer->LogData(histogram_data, -2047);
    histogram_writer->LogData(histogram_data, -1024);
    // bucket[7]: -31 ~ -16
    histogram_writer->LogData(histogram_data, -31);
    histogram_writer->LogData(histogram_data, -25);
    histogram_writer->LogData(histogram_data, -16);
    // overflow
    histogram_writer->LogData(histogram_data, 1234567);

    EXPECT_EQ(-8181, histogram_data->period_min_value);
    EXPECT_EQ(2, histogram_data->period_underflow);
    EXPECT_EQ(3, histogram_data->period_bucket[0]);
    EXPECT_EQ(2, histogram_data->period_bucket[1]);
    EXPECT_EQ(3, histogram_data->period_bucket[7]);
    EXPECT_EQ(1234567, histogram_data->period_max_value);
    EXPECT_EQ(1, histogram_data->period_overflow);

    delete data;
}

TEST_F(WriterTest, HistogramWriter_LogData_ExponentialSample3)
{
    // Bucket: BUCKET_6, Bounds: (-10^3, 10^5), Scale: 10^
    lib::Data* data {new lib::HistogramData};
    lib::HistogramData* histogram_data {static_cast<lib::HistogramData*>(data)};
    histogram_data->bucket_lower_bound = -999;
    histogram_data->bucket_upper_bound = 99999;
    histogram_data->bucket_scale = 10;
    histogram_data->bucket_size = 9;
    histogram_data->bucket_type = lib::BucketType::EXPONENTIAL;
    histogram_data->bucket_zero_index = 3; // 9(size) - 5(upper exponent) - 1

    // underflow
    histogram_writer->LogData(histogram_data, -1000);
    histogram_writer->LogData(histogram_data, -1000200030004000);
    // bucket[0]: -999 ~ -100
    histogram_writer->LogData(histogram_data, -999);
    histogram_writer->LogData(histogram_data, -303);
    histogram_writer->LogData(histogram_data, -100);
    // bucket[1]: -99 ~ -10
    histogram_writer->LogData(histogram_data, -99);
    histogram_writer->LogData(histogram_data, -10);
    // bucket[2]: -9 ~ -1
    histogram_writer->LogData(histogram_data, -9);
    histogram_writer->LogData(histogram_data, -1);
    // bucket[3]: 0
    histogram_writer->LogData(histogram_data, 0);
    // bucket[4]: 1 ~ 9
    histogram_writer->LogData(histogram_data, 1);
    histogram_writer->LogData(histogram_data, 9);
    // bucket[8]: 10000 ~ 99999
    histogram_writer->LogData(histogram_data, 10000);
    histogram_writer->LogData(histogram_data, 33456);
    histogram_writer->LogData(histogram_data, 99999);
    // overflow
    histogram_writer->LogData(histogram_data, 123456789012345);

    EXPECT_EQ(-1000200030004000, histogram_data->period_min_value);
    EXPECT_EQ(2, histogram_data->period_underflow);
    EXPECT_EQ(3, histogram_data->period_bucket[0]);
    EXPECT_EQ(2, histogram_data->period_bucket[1]);
    EXPECT_EQ(2, histogram_data->period_bucket[2]);
    EXPECT_EQ(1, histogram_data->period_bucket[3]);
    EXPECT_EQ(2, histogram_data->period_bucket[4]);
    EXPECT_EQ(3, histogram_data->period_bucket[8]);
    EXPECT_EQ(123456789012345, histogram_data->period_max_value);
    EXPECT_EQ(1, histogram_data->period_overflow);

    delete data;
}
