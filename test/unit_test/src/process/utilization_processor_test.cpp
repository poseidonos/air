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

#include "utilization_processor_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

TEST_F(UtilizationProcessorTest, StreamData_DefaultData)
{
    air::string_view node_name {"UTIL_NODE"};

    util_processor.StreamData(node_name, 0, "thread0", nullptr,
        air::ProcessorType::PERFORMANCE, 1.0, 1, 1);
    util_processor.StreamData(node_name, 0, "thread0", util_data,
        air::ProcessorType::PERFORMANCE, 1.0, 1, 1);
    util_processor.StreamData(node_name, 0, "thread0", util_data,
        air::ProcessorType::PERFORMANCE, 1.0, 1, 1);
}

TEST_F(UtilizationProcessorTest, StreamData_DataCase1)
{
    lib::UtilizationData* u_data {
        (lib::UtilizationData*)util_data->GetAirData(0, 0)};
    lib::AccUtilizationData* u_acc {
        (lib::AccUtilizationData*)util_data->GetAccData(0, 0)};

    u_data->period_usage = 400;
    u_data->access = true;
    air::string_view node_name_util1 {"UTIL_PSD"};
    util_processor.StreamData(node_name_util1, 0, "thread0", util_data,
        air::ProcessorType::UTILIZATION, 1, 2, 2);

    EXPECT_EQ(400, u_acc->cumulation_usage);

    std::string node_obj_name {"UTIL_PSD_0_util_18446744073709551615_0"};
    auto& node_obj = air::json(node_obj_name);
    std::stringstream stream_obj;
    stream_obj << node_obj;
    std::string str_obj = stream_obj.str();
    std::string exp_obj {
        R"({"cumulation": {"usage": 400}, "filter": "BI_0", "index": )"};
    exp_obj += R"(18446744073709551615, "period": {"usage": 400}, )";
    exp_obj += R"("target_id": 0, "target_name": "thread0"})";
    EXPECT_EQ(0, str_obj.compare(exp_obj));
}
