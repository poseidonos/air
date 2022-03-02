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

#include "performance_processor_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

TEST_F(PerformanceProcessorTest, StreamData_DefaultData)
{
    air::string_view node_name {"PERF_NODE"};

    perf_processor.StreamData(node_name, 0, "thread0", nullptr,
        air::ProcessorType::PERFORMANCE, 1.0, 1, 1);
    perf_processor.StreamData(node_name, 0, "thread0", perf_data,
        air::ProcessorType::PERFORMANCE, 1.0, 1, 1);
    perf_processor.StreamData(node_name, 0, "thread0", perf_data,
        air::ProcessorType::PERFORMANCE, 1.0, 1, 1);
}

TEST_F(PerformanceProcessorTest, StreamData_DataCase1)
{
    air::string_view node_name {"PERF_NODE"};
    lib::PerformanceData* air_data {
        static_cast<lib::PerformanceData*>(perf_data->GetAirData(0, 0))};
    lib::AccPerformanceData* acc_data {
        static_cast<lib::AccPerformanceData*>(perf_data->GetAccData(0, 0))};

    air_data->access = true;
    air_data->period_iops = 100;
    air_data->period_bandwidth = 100 * 4096;

    perf_processor.StreamData(node_name, 0, "thread_name", perf_data,
        air::ProcessorType::PERFORMANCE, 1.0, 1, 1);

    EXPECT_EQ(100, acc_data->cumulation_iops);
    EXPECT_EQ(409600, acc_data->cumulation_bandwidth);
}

TEST_F(PerformanceProcessorTest, StreamData_DataCase2)
{
    air::string_view node_name {"PERF_PSD"};
    lib::PerformanceData* air_data {
        static_cast<lib::PerformanceData*>(perf_data->GetAirData(0, 0))};
    lib::AccPerformanceData* acc_data {
        static_cast<lib::AccPerformanceData*>(perf_data->GetAccData(0, 0))};

    air_data->access = true;
    air_data->period_iops = 1100;
    air_data->period_bandwidth = (100 * 4096) + (1000 * 512);
    air_data->period_packet_cnt.insert({4096, 100});
    air_data->period_packet_cnt.insert({512, 1000});

    perf_processor.StreamData(node_name, 0, "thread_0", perf_data,
        air::ProcessorType::PERFORMANCE, 1.0, 1, 1);

    EXPECT_EQ(1100, acc_data->cumulation_iops);
    EXPECT_EQ(921600, acc_data->cumulation_bandwidth);

    std::string node_obj_name {"PERF_PSD_0_perf_18446744073709551615_0"};
    auto& node_obj = air::json(node_obj_name);
    std::stringstream stream_obj;
    stream_obj << node_obj;
    std::string str_obj = stream_obj.str();
    std::string exp_obj {
        R"({"cumulation": {"bw": 921600, "iops": 1100}, "filter": "BI_0",)"};
    exp_obj += R"( "index": 18446744073709551615, "period": {"bw": 921600)";
    exp_obj += R"(, "count": [{"512": 1000}, {"4096": 100}], "iops": 1100)";
    exp_obj += R"(}, "target_id": 0, "target_name": "thread_0"})";
    EXPECT_EQ(0, str_obj.compare(exp_obj));
}

TEST_F(PerformanceProcessorTest, StreamData_DataCase3)
{
    air::string_view node_name {"PERF_PSD"};
    lib::PerformanceData* air_data {
        static_cast<lib::PerformanceData*>(perf_data->GetAirData(0, 0))};
    lib::PerformanceData* user_data {static_cast<lib::PerformanceData*>(
        perf_data->GetUserDataByHashIndex(0, 0))};
    lib::AccPerformanceData* acc_data {
        static_cast<lib::AccPerformanceData*>(perf_data->GetAccData(0, 0))};

    air_data->access = true;
    air_data->period_iops = 300;
    air_data->period_bandwidth = 300 * 4096;
    air_data->period_packet_cnt.insert({4096, 300});

    perf_processor.StreamData(node_name, 0, "thread_0", perf_data,
        air::ProcessorType::PERFORMANCE, 3.0, 1, 1);

    EXPECT_EQ(100, acc_data->cumulation_iops);
    EXPECT_EQ(409600, acc_data->cumulation_bandwidth);
    EXPECT_EQ(false, air_data->access);
    EXPECT_EQ(0, air_data->period_iops);
    EXPECT_EQ(0, air_data->period_bandwidth);
    EXPECT_EQ(true, air_data->period_packet_cnt.empty());

    air::json_clear();

    user_data->access = true;
    user_data->period_iops = 200;
    user_data->period_bandwidth = 200 * 1024;
    user_data->period_packet_cnt.insert({1024, 200});

    perf_processor.StreamData(node_name, 0, "thread_0", perf_data,
        air::ProcessorType::PERFORMANCE, 1.0, 1, 1);

    EXPECT_EQ(125, acc_data->cumulation_iops);
    EXPECT_EQ(358400, acc_data->cumulation_bandwidth);

    std::string node_obj_name {"PERF_PSD_0_perf_18446744073709551615_0"};
    auto& node_obj = air::json(node_obj_name);
    std::stringstream stream_obj;
    stream_obj << node_obj;
    std::string str_obj = stream_obj.str();
    std::string exp_obj {
        R"({"cumulation": {"bw": 358400, "iops": 125}, "filter": "BI_0", )"};
    exp_obj += R"("index": 18446744073709551615, "period": {"bw": 204800,)";
    exp_obj += R"( "count": [{"1024": 200}], "iops": 200}, "target_id": 0)";
    exp_obj += R"(, "target_name": "thread_0"})";
    EXPECT_EQ(0, str_obj.compare(exp_obj));
}
