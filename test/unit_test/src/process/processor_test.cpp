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

#include "processor_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST_F(ProcessorTest, ThreadAwareStreamData_PerformanceType)
{
    int tid {123};

    air::string_view node_name_perf1 {"PERF_BENCHMARK"};
    perf_processor.StreamData(node_name_perf1, tid, "thread0", nullptr, air::ProcessorType::PERFORMANCE, 1, 32, 32);
    air::string_view node_name_perf2 {"PERF_BENCHMARK"};
    perf_processor.StreamData(node_name_perf2, tid, "thread0", &perf_data, air::ProcessorType::PERFORMANCE, 1, 32, 32);
    air::string_view node_name_perf3 {"PERF_BENCHMARK"};
    perf_processor.StreamData(node_name_perf3, tid, "thread0", &perf_data, air::ProcessorType::PERFORMANCE, 1, 32, 32);
}

TEST_F(ProcessorTest, ThreadAwareStreamData_QueueType)
{
    int tid {123};

    air::string_view node_name_q1 {"Q_SUBMISSION"};
    queue_processor.StreamData(node_name_q1, tid, "thread0", &queue_data, air::ProcessorType::QUEUE, 1, 32, 32);
    air::string_view node_name_q2 {"Q_COMPLETION"};
    queue_processor.StreamData(node_name_q2, tid, "thread0", &queue_data, air::ProcessorType::QUEUE, 1, 32, 32);
}

TEST_F(ProcessorTest, QueueProcessData)
{
    lib::QueueData* q_data {(lib::QueueData*)queue_data.GetAirData(0, 0)};
    lib::AccQueueData* acc_q_data {(lib::AccQueueData*)queue_data.GetAccData(0, 0)};

    q_data->num_req = 12;
    q_data->sum_depth = 120;
    q_data->access = true;
    acc_q_data->need_erase = false;
    air::string_view node_name_q1 {"Q_SUBMISSION"};
    queue_processor.StreamData(node_name_q1, 0, "thread0", &queue_data, air::ProcessorType::QUEUE, 1, 32, 32);
    EXPECT_EQ(true, ((10.1f > acc_q_data->depth_total_avg) && (9.9f < acc_q_data->depth_total_avg)));

    queue_processor.StreamData(node_name_q1, 0, "thread0", &queue_data, air::ProcessorType::QUEUE, 1, 32, 32); // for swap

    q_data->num_req = 12;
    q_data->sum_depth = 240;
    q_data->access = true;
    acc_q_data->need_erase = false;
    air::string_view node_name_q2 {"Q_SUBMISSION"};
    queue_processor.StreamData(node_name_q2, 0, "thread0", &queue_data, air::ProcessorType::QUEUE, 1, 32, 32);
    EXPECT_EQ(true, ((15.1f > acc_q_data->depth_total_avg) && (14.9f < acc_q_data->depth_total_avg)));
}

TEST_F(ProcessorTest, UtilizationProcessData)
{
    lib::UtilizationData* u_data {(lib::UtilizationData*)util_data.GetAirData(0, 0)};
    lib::AccUtilizationData* u_acc {(lib::AccUtilizationData*)util_data.GetAccData(0, 0)};

    u_data->usage = 400;
    u_data->access = true;
    air::string_view node_name_util1 {"UTIL_SUBMIT_THR"};
    util_processor.StreamData(node_name_util1, 0, "thread0", &util_data, air::ProcessorType::UTILIZATION, 1, 32, 32);

    EXPECT_EQ(400, u_acc->total_usage);
}

TEST_F(ProcessorTest, CountProcessData_Case1)
{
    lib::CountData* c_data {(lib::CountData*)count_data.GetAirData(1, 0)};
    lib::AccCountData* c_acc {(lib::AccCountData*)count_data.GetAccData(1, 0)};

    c_data->access = true;
    c_data->count_positive = 100;
    c_data->num_req_positive = 45;
    c_data->count_negative = 1000;
    c_data->num_req_negative = 23;

    c_acc->negative = 0;
    c_acc->total_count = 0;
    c_acc->total_num_req_positive = 0;
    c_acc->total_num_req_negative = 0;

    air::string_view node_name_count1 {"CNT_TEST_EVENT"};
    count_processor.StreamData(node_name_count1, 0, "thread0", &count_data, air::ProcessorType::COUNT, 1, 32, 32);

    EXPECT_EQ(1, c_acc->negative);
    EXPECT_EQ(900, c_acc->total_count);
    EXPECT_EQ(45, c_acc->total_num_req_positive);
    EXPECT_EQ(23, c_acc->total_num_req_negative);
}

TEST_F(ProcessorTest, CountProcessData_Case2)
{
    lib::CountData* c_data {(lib::CountData*)count_data.GetAirData(1, 0)};
    lib::AccCountData* c_acc {(lib::AccCountData*)count_data.GetAccData(1, 0)};

    c_data->access = true;
    c_data->count_positive = 100;
    c_data->num_req_positive = 45;
    c_data->count_negative = 1000;
    c_data->num_req_negative = 23;

    c_acc->negative = 0;
    c_acc->total_count = 10000;
    c_acc->total_num_req_positive = 10;
    c_acc->total_num_req_negative = 30;

    air::string_view node_name_count1 {"CNT_TEST_EVENT"};
    count_processor.StreamData(node_name_count1, 0, "thread0", &count_data, air::ProcessorType::COUNT, 1, 32, 32);

    EXPECT_EQ(0, c_acc->negative);
    EXPECT_EQ(9100, c_acc->total_count);
    EXPECT_EQ(55, c_acc->total_num_req_positive);
    EXPECT_EQ(53, c_acc->total_num_req_negative);
}

TEST_F(ProcessorTest, CountProcessData_Case3)
{
    lib::CountData* c_data {(lib::CountData*)count_data.GetAirData(1, 0)};
    lib::AccCountData* c_acc {(lib::AccCountData*)count_data.GetAccData(1, 0)};

    c_data->access = true;
    c_data->count_positive = 100;
    c_data->num_req_positive = 45;
    c_data->count_negative = 0;
    c_data->num_req_negative = 0;

    c_acc->negative = 0;
    c_acc->total_count = 10000;
    c_acc->total_num_req_positive = 10;
    c_acc->total_num_req_negative = 30;

    air::string_view node_name_count1 {"CNT_TEST_EVENT"};
    count_processor.StreamData(node_name_count1, 0, "thread0", &count_data, air::ProcessorType::COUNT, 1, 32, 32);

    EXPECT_EQ(0, c_acc->negative);
    EXPECT_EQ(10100, c_acc->total_count);
    EXPECT_EQ(55, c_acc->total_num_req_positive);
    EXPECT_EQ(30, c_acc->total_num_req_negative);
}

TEST_F(ProcessorTest, CountProcessData_Case4)
{
    lib::CountData* c_data {(lib::CountData*)count_data.GetAirData(1, 0)};
    lib::AccCountData* c_acc {(lib::AccCountData*)count_data.GetAccData(1, 0)};

    c_data->access = true;
    c_data->count_positive = 100;
    c_data->num_req_positive = 45;
    c_data->count_negative = 90000;
    c_data->num_req_negative = 323;

    c_acc->negative = 0;
    c_acc->total_count = 10000;
    c_acc->total_num_req_positive = 10;
    c_acc->total_num_req_negative = 30;

    air::string_view node_name_count1 {"CNT_TEST_EVENT"};
    count_processor.StreamData(node_name_count1, 0, "thread0", &count_data, air::ProcessorType::COUNT, 1, 32, 32);

    EXPECT_EQ(1, c_acc->negative);
    EXPECT_EQ(79900, c_acc->total_count);
    EXPECT_EQ(55, c_acc->total_num_req_positive);
    EXPECT_EQ(353, c_acc->total_num_req_negative);
}

TEST_F(ProcessorTest, HitogramProcessData_Case1)
{
    lib::HistogramData* h_data {(lib::HistogramData*)histogram_data.GetAirData(1, 0)};
    lib::AccHistogramData* h_acc {(lib::AccHistogramData*)histogram_data.GetAccData(1, 0)};

    h_data->access = true;
    h_data->bucket_lower_bound = 0;
    h_data->bucket_upper_bound = 99;
    h_data->bucket_scale = 10;
    h_data->bucket_size = 10;
    h_data->bucket_type = lib::BucketType::LINEAR;

    h_data->period_min_value = -123;
    h_data->period_max_value = 49838;
    h_data->period_underflow = 13;
    h_data->period_overflow = 4921;
    h_data->period_bucket[0] = 20;
    h_data->period_bucket[1] = 20;
    h_data->period_bucket[2] = 20;

    h_acc->is_first = false;
    h_acc->cumulation_underflow = 10;
    h_acc->cumulation_overflow = 4234;
    h_acc->cumulation_min_value = -345;
    h_acc->cumulation_max_value = 3455;

    air::string_view node_name {"HIST_TEST_1"};
    histogram_processor.StreamData(node_name, 0, "thread0", &histogram_data, air::ProcessorType::HISTOGRAM, 1, 32, 32);

    EXPECT_EQ(0, h_data->period_avg_value); // because it's initialized
    EXPECT_EQ(23, h_acc->cumulation_underflow);
    EXPECT_EQ(9155, h_acc->cumulation_overflow);
    EXPECT_EQ(-345, h_acc->cumulation_min_value);
    EXPECT_EQ(49838, h_acc->cumulation_max_value);
    EXPECT_EQ(20, h_acc->cumulation_bucket[0]);
    EXPECT_EQ(20, h_acc->cumulation_bucket[1]);
    EXPECT_EQ(20, h_acc->cumulation_bucket[2]);
    EXPECT_EQ(15, h_acc->cumulation_avg_value);
}

TEST_F(ProcessorTest, HitogramProcessData_Case2)
{
    lib::HistogramData* h_data {(lib::HistogramData*)histogram_data.GetAirData(1, 0)};
    lib::AccHistogramData* h_acc {(lib::AccHistogramData*)histogram_data.GetAccData(1, 0)};

    h_data->access = true;
    h_data->bucket_lower_bound = 33;
    h_data->bucket_upper_bound = 65;
    h_data->bucket_scale = 3;
    h_data->bucket_size = 11;
    h_data->bucket_type = lib::BucketType::LINEAR;

    h_data->period_bucket[0] = 20;
    h_data->period_bucket[1] = 20;
    h_data->period_bucket[2] = 20;
    h_data->period_bucket[10] = 20;

    h_acc->cumulation_bucket[8] = 20;
    h_acc->cumulation_bucket[9] = 20;
    h_acc->cumulation_bucket[10] = 20;

    air::string_view node_name {"HIST_TEST_2"};
    histogram_processor.StreamData(node_name, 0, "thread0", &histogram_data, air::ProcessorType::HISTOGRAM, 1, 32, 32);

    EXPECT_EQ(0, h_data->period_avg_value); // because it's initialized
    EXPECT_EQ(20, h_acc->cumulation_bucket[0]);
    EXPECT_EQ(20, h_acc->cumulation_bucket[1]);
    EXPECT_EQ(20, h_acc->cumulation_bucket[2]);
    EXPECT_EQ(20, h_acc->cumulation_bucket[8]);
    EXPECT_EQ(20, h_acc->cumulation_bucket[9]);
    EXPECT_EQ(40, h_acc->cumulation_bucket[10]);
    EXPECT_EQ(51, h_acc->cumulation_avg_value);
}

TEST_F(ProcessorTest, HitogramProcessData_Case3)
{
    lib::HistogramData* h_data {(lib::HistogramData*)histogram_data.GetAirData(1, 0)};
    lib::AccHistogramData* h_acc {(lib::AccHistogramData*)histogram_data.GetAccData(1, 0)};

    h_data->access = true;
    h_data->bucket_lower_bound = -100;
    h_data->bucket_upper_bound = 79;
    h_data->bucket_scale = 20;
    h_data->bucket_size = 9;
    h_data->bucket_type = lib::BucketType::LINEAR;

    h_data->period_bucket[0] = 7;
    h_data->period_bucket[4] = 7;
    h_data->period_bucket[8] = 7;

    h_acc->cumulation_bucket[1] = 9;
    h_acc->cumulation_bucket[2] = 9;
    h_acc->cumulation_bucket[3] = 9;

    air::string_view node_name {"HIST_TEST_3"};
    histogram_processor.StreamData(node_name, 0, "thread0", &histogram_data, air::ProcessorType::HISTOGRAM, 1, 32, 32);

    EXPECT_EQ(0, h_data->period_avg_value); // because it's initialized
    EXPECT_EQ(7, h_acc->cumulation_bucket[0]);
    EXPECT_EQ(7, h_acc->cumulation_bucket[4]);
    EXPECT_EQ(7, h_acc->cumulation_bucket[8]);
    EXPECT_EQ(9, h_acc->cumulation_bucket[1]);
    EXPECT_EQ(9, h_acc->cumulation_bucket[2]);
    EXPECT_EQ(9, h_acc->cumulation_bucket[3]);
    EXPECT_EQ(-32, h_acc->cumulation_avg_value);
}

TEST_F(ProcessorTest, HitogramProcessData_Case4)
{
    lib::HistogramData* h_data {(lib::HistogramData*)histogram_data.GetAirData(1, 0)};
    lib::AccHistogramData* h_acc {(lib::AccHistogramData*)histogram_data.GetAccData(1, 0)};

    h_data->access = true;
    h_data->bucket_lower_bound = 1;
    h_data->bucket_upper_bound = 1023;
    h_data->bucket_scale = 2;
    h_data->bucket_size = 10;
    h_data->bucket_type = lib::BucketType::EXPONENTIAL;
    h_data->bucket_zero_index = -1;

    h_data->period_bucket[0] = 10;
    h_data->period_bucket[1] = 10;
    h_data->period_bucket[2] = 10;
    h_data->period_bucket[3] = 10;
    h_data->period_bucket[4] = 10;
    h_data->period_bucket[9] = 10;

    air::string_view node_name {"HIST_TEST_4"};
    histogram_processor.StreamData(node_name, 0, "thread0", &histogram_data, air::ProcessorType::HISTOGRAM, 1, 32, 32);

    EXPECT_EQ(0, h_data->period_avg_value); // because it's initialized
    EXPECT_EQ(10, h_acc->cumulation_bucket[0]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[1]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[2]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[3]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[4]);
    EXPECT_EQ(0, h_acc->cumulation_bucket[5]);
    EXPECT_EQ(0, h_acc->cumulation_bucket[6]);
    EXPECT_EQ(0, h_acc->cumulation_bucket[7]);
    EXPECT_EQ(0, h_acc->cumulation_bucket[8]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[9]);
    EXPECT_EQ(135, h_acc->cumulation_avg_value);
}

TEST_F(ProcessorTest, HitogramProcessData_Case5)
{
    lib::HistogramData* h_data {(lib::HistogramData*)histogram_data.GetAirData(1, 0)};
    lib::AccHistogramData* h_acc {(lib::AccHistogramData*)histogram_data.GetAccData(1, 0)};

    h_data->access = true;
    h_data->bucket_lower_bound = -4095;
    h_data->bucket_upper_bound = -16;
    h_data->bucket_scale = 2;
    h_data->bucket_size = 8;
    h_data->bucket_type = lib::BucketType::EXPONENTIAL;
    h_data->bucket_zero_index = 12;

    h_data->period_bucket[0] = 10;
    h_data->period_bucket[1] = 10;
    h_data->period_bucket[2] = 10;
    h_data->period_bucket[3] = 10;
    h_data->period_bucket[4] = 10;
    h_data->period_bucket[7] = 10;

    h_acc->cumulation_bucket[5] = 10;
    h_acc->cumulation_bucket[6] = 10;
    h_acc->cumulation_bucket[7] = 10;

    air::string_view node_name {"HIST_TEST_5"};
    histogram_processor.StreamData(node_name, 0, "thread0", &histogram_data, air::ProcessorType::HISTOGRAM, 1, 32, 32);

    EXPECT_EQ(0, h_data->period_avg_value); // because it's initialized
    EXPECT_EQ(10, h_acc->cumulation_bucket[0]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[1]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[2]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[3]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[4]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[5]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[6]);
    EXPECT_EQ(20, h_acc->cumulation_bucket[7]);
    EXPECT_EQ(-682, h_acc->cumulation_avg_value);
}

TEST_F(ProcessorTest, HitogramProcessData_Case6)
{
    lib::HistogramData* h_data {(lib::HistogramData*)histogram_data.GetAirData(1, 0)};
    lib::AccHistogramData* h_acc {(lib::AccHistogramData*)histogram_data.GetAccData(1, 0)};

    h_data->access = true;
    h_data->bucket_lower_bound = -999;
    h_data->bucket_upper_bound = 99999;
    h_data->bucket_scale = 10;
    h_data->bucket_size = 9;
    h_data->bucket_type = lib::BucketType::EXPONENTIAL;
    h_data->bucket_zero_index = 3;

    h_data->period_bucket[0] = 10;
    h_data->period_bucket[1] = 10;
    h_data->period_bucket[2] = 10;
    h_data->period_bucket[3] = 10;
    h_data->period_bucket[4] = 10;
    h_data->period_bucket[5] = 10;
    h_data->period_bucket[6] = 10;
    h_data->period_bucket[7] = 10;
    h_data->period_bucket[8] = 10;

    h_acc->cumulation_bucket[8] = 10;

    air::string_view node_name {"HIST_TEST_5"};
    histogram_processor.StreamData(node_name, 0, "thread0", &histogram_data, air::ProcessorType::HISTOGRAM, 1, 32, 32);

    EXPECT_EQ(0, h_data->period_avg_value); // because it's initialized
    EXPECT_EQ(10, h_acc->cumulation_bucket[0]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[1]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[2]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[3]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[4]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[5]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[6]);
    EXPECT_EQ(10, h_acc->cumulation_bucket[7]);
    EXPECT_EQ(20, h_acc->cumulation_bucket[8]);
    EXPECT_EQ(11549, h_acc->cumulation_avg_value);
}
