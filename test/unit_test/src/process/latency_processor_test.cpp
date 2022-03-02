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

#include "latency_processor_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

TEST_F(LatencyProcessorTest, StreamData_DefaultData)
{
    air::string_view node_name {"LAT_NODE"};

    lat_processor.StreamData(node_name, nullptr, 1, 1);
    lat_processor.StreamData(node_name, &acc_data, 1, 1);
    lat_processor.StreamData(node_name, &acc_data, 1, 1);
}

TEST_F(LatencyProcessorTest, StreamData_DataCase1)
{
    air::string_view node_name {"LAT_PSD"};

    acc_data.period_sample_count = 50;
    for (uint32_t idx {0}; idx < 50; idx++)
    {
        acc_data.timelag[idx] = idx * 1000;
    }

    lat_processor.StreamData(node_name, &acc_data, 1, 1);

    EXPECT_EQ(50, acc_data.cumulation_sample_count);
    EXPECT_EQ(24500, acc_data.cumulation_mean);
}

TEST_F(LatencyProcessorTest, StreamData_DataCase2)
{
    air::string_view node_name {"LAT_PSD"};

    acc_data.period_sample_count = 50;
    for (uint32_t idx {0}; idx < 50; idx++)
    {
        acc_data.timelag[idx] = idx * 1000;
    }
    acc_data.period_max = 1234567;
    acc_data.period_min = 123;

    lat_processor.StreamData(node_name, &acc_data, 1, 1);

    std::string node_obj_name {"LAT_PSD_0_lat_1_1"};
    auto& node_obj = air::json(node_obj_name);
    std::stringstream stream_obj;
    stream_obj << node_obj;
    std::string str_obj = stream_obj.str();
    std::string exp_obj {
        R"({"cumulation": {"low_qt": 12000, "max": 1234567, "mean": 24500)"};
    exp_obj += R"(, "median": 25000, "min": 123, "sample_cnt": 50, "up_qt)";
    exp_obj += R"(": 38000}, "filter": "BI_1~BI_2", "index": 1, "period":)";
    exp_obj += R"( {"low_qt": 12000, "max": 1234567, "mean": 24500, "medi)";
    exp_obj += R"(an": 25000, "min": 123, "sample_cnt": 50, "up_qt": 3800)";
    exp_obj += R"(0}, "target_id": 0, "target_name": ""})";
    EXPECT_EQ(0, str_obj.compare(exp_obj));
}

TEST_F(LatencyProcessorTest, StreamData_DataCase3)
{
    air::string_view node_name {"LAT_PSD"};

    acc_data.period_sample_count = 50;
    for (uint32_t idx {0}; idx < 50; idx++)
    {
        acc_data.timelag[idx] = idx * 1000;
    }
    acc_data.period_max = 1234567;
    acc_data.period_min = 123;

    lat_processor.StreamData(node_name, &acc_data, 1, 1);

    air::json_clear();

    acc_data.period_sample_count = 50;
    for (uint32_t idx {0}; idx < 50; idx++)
    {
        acc_data.timelag[idx] = idx * 1000 + 24500;
    }
    acc_data.period_max = 12345678;
    acc_data.period_min = 1234;

    lat_processor.StreamData(node_name, &acc_data, 1, 1);

    std::string node_obj_name {"LAT_PSD_0_lat_1_1"};
    auto& node_obj = air::json(node_obj_name);
    std::stringstream stream_obj;
    stream_obj << node_obj;
    std::string str_obj = stream_obj.str();
    std::string exp_obj {
        R"({"cumulation": {"low_qt": 24250, "max": 12345678, "mean": 3675)"};
    exp_obj += R"(0, "median": 37250, "min": 123, "sample_cnt": 100, "up_)";
    exp_obj += R"(qt": 50250}, "filter": "BI_1~BI_2", "index": 1, "period)";
    exp_obj += R"(": {"low_qt": 36500, "max": 12345678, "mean": 49000, "m)";
    exp_obj += R"(edian": 49500, "min": 1234, "sample_cnt": 50, "up_qt": )";
    exp_obj += R"(62500}, "target_id": 0, "target_name": ""})";
    EXPECT_EQ(0, str_obj.compare(exp_obj));
}
