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

#include "histogram_processor_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

TEST_F(HistogramProcessorTest, StreamData_DefaultData)
{
    air::string_view node_name {"HISTO_NODE"};

    histogram_processor.StreamData(
        node_name, 0, "thread0", nullptr, air::ProcessorType::HISTOGRAM, 1, 2, 2);
    histogram_processor.StreamData(node_name, 0, "thread0", histogram_data,
        air::ProcessorType::HISTOGRAM, 1, 2, 2);
    histogram_processor.StreamData(node_name, 0, "thread0", histogram_data,
        air::ProcessorType::HISTOGRAM, 1, 2, 2);
}

TEST_F(HistogramProcessorTest, StreamData_DataCase1)
{
    lib::HistogramData* h_data {
        (lib::HistogramData*)histogram_data->GetAirData(1, 0)};
    lib::AccHistogramData* h_acc {
        (lib::AccHistogramData*)histogram_data->GetAccData(1, 0)};

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
    histogram_processor.StreamData(node_name, 0, "thread0", histogram_data,
        air::ProcessorType::HISTOGRAM, 1, 2, 2);

    EXPECT_EQ(0, h_data->period_avg_value); // because it's initialized
    EXPECT_EQ(23, h_acc->cumulation_underflow);
    EXPECT_EQ(9155, h_acc->cumulation_overflow);
    EXPECT_EQ(-345, h_acc->cumulation_min_value);
    EXPECT_EQ(49838, h_acc->cumulation_max_value);
    EXPECT_EQ(20, h_acc->cumulation_bucket[0]);
    EXPECT_EQ(20, h_acc->cumulation_bucket[1]);
    EXPECT_EQ(20, h_acc->cumulation_bucket[2]);
    EXPECT_EQ(15, h_acc->cumulation_avg_value);

    std::string node_obj_name {"HIST_TEST_1_0_histogram_18446744073709551615_0"};
    auto& node_obj = air::json(node_obj_name);
    std::stringstream stream_obj;
    stream_obj << node_obj;
    std::string str_obj = stream_obj.str();
    std::string exp_obj {
        R"({"bucket_lower_bound": 0, "bucket_scale": 10, "bucket_size": 1)"};
    exp_obj += R"(0, "bucket_type": "linear", "bucket_upper_bound": 99, ")";
    exp_obj += R"(bucket_zero_index": 0, "cumulation": {"average_value": )";
    exp_obj += R"(15, "buckets": [20, 20, 20, 0, 0, 0, 0, 0, 0, 0], "maxi)";
    exp_obj += R"(mum_value": 49838, "minimum_value": -345, "overflow_cou)";
    exp_obj += R"(nt": 9155, "since": 0, "underflow_count": 23}, "filter")";
    exp_obj += R"(: "ONE", "index": 18446744073709551615, "period": {"ave)";
    exp_obj += R"(rage_value": 15, "buckets": [20, 20, 20, 0, 0, 0, 0, 0,)";
    exp_obj += R"( 0, 0], "maximum_value": 49838, "minimum_value": -123, )";
    exp_obj += R"("overflow_count": 4921, "underflow_count": 13}, "target)";
    exp_obj += R"(_id": 0, "target_name": "thread0"})";
    EXPECT_EQ(0, str_obj.compare(exp_obj));
}

TEST_F(HistogramProcessorTest, StreamData_DataCase2)
{
    lib::HistogramData* h_data {
        (lib::HistogramData*)histogram_data->GetAirData(1, 0)};
    lib::AccHistogramData* h_acc {
        (lib::AccHistogramData*)histogram_data->GetAccData(1, 0)};

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
    histogram_processor.StreamData(node_name, 0, "thread0", histogram_data,
        air::ProcessorType::HISTOGRAM, 1, 2, 2);

    EXPECT_EQ(0, h_data->period_avg_value); // because it's initialized
    EXPECT_EQ(20, h_acc->cumulation_bucket[0]);
    EXPECT_EQ(20, h_acc->cumulation_bucket[1]);
    EXPECT_EQ(20, h_acc->cumulation_bucket[2]);
    EXPECT_EQ(20, h_acc->cumulation_bucket[8]);
    EXPECT_EQ(20, h_acc->cumulation_bucket[9]);
    EXPECT_EQ(40, h_acc->cumulation_bucket[10]);
    EXPECT_EQ(51, h_acc->cumulation_avg_value);
}

TEST_F(HistogramProcessorTest, StreamData_DataCase3)
{
    lib::HistogramData* h_data {
        (lib::HistogramData*)histogram_data->GetAirData(1, 0)};
    lib::AccHistogramData* h_acc {
        (lib::AccHistogramData*)histogram_data->GetAccData(1, 0)};

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
    histogram_processor.StreamData(node_name, 0, "thread0", histogram_data,
        air::ProcessorType::HISTOGRAM, 1, 2, 2);

    EXPECT_EQ(0, h_data->period_avg_value); // because it's initialized
    EXPECT_EQ(7, h_acc->cumulation_bucket[0]);
    EXPECT_EQ(7, h_acc->cumulation_bucket[4]);
    EXPECT_EQ(7, h_acc->cumulation_bucket[8]);
    EXPECT_EQ(9, h_acc->cumulation_bucket[1]);
    EXPECT_EQ(9, h_acc->cumulation_bucket[2]);
    EXPECT_EQ(9, h_acc->cumulation_bucket[3]);
    EXPECT_EQ(-32, h_acc->cumulation_avg_value);
}

TEST_F(HistogramProcessorTest, StreamData_DataCase4)
{
    lib::HistogramData* h_data {
        (lib::HistogramData*)histogram_data->GetAirData(1, 0)};
    lib::AccHistogramData* h_acc {
        (lib::AccHistogramData*)histogram_data->GetAccData(1, 0)};

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
    histogram_processor.StreamData(node_name, 0, "thread0", histogram_data,
        air::ProcessorType::HISTOGRAM, 1, 2, 2);

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

TEST_F(HistogramProcessorTest, StreamData_DataCase5)
{
    lib::HistogramData* h_data {
        (lib::HistogramData*)histogram_data->GetAirData(1, 0)};
    lib::AccHistogramData* h_acc {
        (lib::AccHistogramData*)histogram_data->GetAccData(1, 0)};

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
    histogram_processor.StreamData(node_name, 0, "thread0", histogram_data,
        air::ProcessorType::HISTOGRAM, 1, 2, 2);

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

TEST_F(HistogramProcessorTest, StreamData_DataCase6)
{
    lib::HistogramData* h_data {
        (lib::HistogramData*)histogram_data->GetAirData(1, 0)};
    lib::AccHistogramData* h_acc {
        (lib::AccHistogramData*)histogram_data->GetAccData(1, 0)};

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

    air::string_view node_name {"HIST_TEST_6"};
    histogram_processor.StreamData(node_name, 0, "thread0", histogram_data,
        air::ProcessorType::HISTOGRAM, 1, 2, 2);

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
