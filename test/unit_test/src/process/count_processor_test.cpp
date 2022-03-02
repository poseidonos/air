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

#include "count_processor_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

TEST_F(CountProcessorTest, StreamData_DefaultData)
{
    air::string_view node_name {"COUNT_NODE"};

    count_processor.StreamData(
        node_name, 0, "thread0", nullptr, air::ProcessorType::HISTOGRAM, 1, 2, 2);
    count_processor.StreamData(node_name, 0, "thread0", count_data,
        air::ProcessorType::HISTOGRAM, 1, 2, 2);
    count_processor.StreamData(node_name, 0, "thread0", count_data,
        air::ProcessorType::HISTOGRAM, 1, 2, 2);
}

TEST_F(CountProcessorTest, StreamData_DataCase1)
{
    lib::CountData* c_data {(lib::CountData*)count_data->GetAirData(1, 0)};
    lib::AccCountData* c_acc {(lib::AccCountData*)count_data->GetAccData(1, 0)};

    c_data->access = true;
    c_data->period_count_positive = 100;
    c_data->period_num_req_positive = 45;
    c_data->period_count_negative = 1000;
    c_data->period_num_req_negative = 23;

    c_acc->cumulation_negative = 0;
    c_acc->cumulation_count = 0;
    c_acc->cumulation_num_req_positive = 0;
    c_acc->cumulation_num_req_negative = 0;

    air::string_view node_name_count1 {"CNT_PSD"};
    count_processor.StreamData(node_name_count1, 0, "thread0", count_data,
        air::ProcessorType::COUNT, 1, 2, 2);

    EXPECT_EQ(1, c_acc->cumulation_negative);
    EXPECT_EQ(900, c_acc->cumulation_count);
    EXPECT_EQ(45, c_acc->cumulation_num_req_positive);
    EXPECT_EQ(23, c_acc->cumulation_num_req_negative);
}

TEST_F(CountProcessorTest, StreamData_DataCase2)
{
    lib::CountData* c_data {(lib::CountData*)count_data->GetAirData(1, 0)};
    lib::AccCountData* c_acc {(lib::AccCountData*)count_data->GetAccData(1, 0)};

    c_data->access = true;
    c_data->period_count_positive = 100;
    c_data->period_num_req_positive = 45;
    c_data->period_count_negative = 1000;
    c_data->period_num_req_negative = 23;

    c_acc->cumulation_negative = 0;
    c_acc->cumulation_count = 10000;
    c_acc->cumulation_num_req_positive = 10;
    c_acc->cumulation_num_req_negative = 30;

    air::string_view node_name_count1 {"CNT_PSD"};
    count_processor.StreamData(node_name_count1, 0, "thread0", count_data,
        air::ProcessorType::COUNT, 1, 2, 2);

    EXPECT_EQ(0, c_acc->cumulation_negative);
    EXPECT_EQ(9100, c_acc->cumulation_count);
    EXPECT_EQ(55, c_acc->cumulation_num_req_positive);
    EXPECT_EQ(53, c_acc->cumulation_num_req_negative);
}

TEST_F(CountProcessorTest, StreamData_DataCase3)
{
    lib::CountData* c_data {(lib::CountData*)count_data->GetAirData(1, 0)};
    lib::AccCountData* c_acc {(lib::AccCountData*)count_data->GetAccData(1, 0)};

    c_data->access = true;
    c_data->period_count_positive = 100;
    c_data->period_num_req_positive = 45;
    c_data->period_count_negative = 0;
    c_data->period_num_req_negative = 0;

    c_acc->cumulation_negative = 0;
    c_acc->cumulation_count = 10000;
    c_acc->cumulation_num_req_positive = 10;
    c_acc->cumulation_num_req_negative = 30;

    air::string_view node_name_count1 {"CNT_PSD"};
    count_processor.StreamData(node_name_count1, 0, "thread0", count_data,
        air::ProcessorType::COUNT, 1, 2, 2);

    EXPECT_EQ(0, c_acc->cumulation_negative);
    EXPECT_EQ(10100, c_acc->cumulation_count);
    EXPECT_EQ(55, c_acc->cumulation_num_req_positive);
    EXPECT_EQ(30, c_acc->cumulation_num_req_negative);
}

TEST_F(CountProcessorTest, StreamData_DataCase4)
{
    lib::CountData* c_data {(lib::CountData*)count_data->GetAirData(1, 0)};
    lib::AccCountData* c_acc {(lib::AccCountData*)count_data->GetAccData(1, 0)};

    c_data->access = true;
    c_data->period_count_positive = 100;
    c_data->period_num_req_positive = 45;
    c_data->period_count_negative = 90000;
    c_data->period_num_req_negative = 323;

    c_acc->cumulation_negative = 0;
    c_acc->cumulation_count = 10000;
    c_acc->cumulation_num_req_positive = 10;
    c_acc->cumulation_num_req_negative = 30;

    air::string_view node_name_count1 {"CNT_PSD"};
    count_processor.StreamData(node_name_count1, 0, "thread0", count_data,
        air::ProcessorType::COUNT, 1, 2, 2);

    EXPECT_EQ(1, c_acc->cumulation_negative);
    EXPECT_EQ(79900, c_acc->cumulation_count);
    EXPECT_EQ(55, c_acc->cumulation_num_req_positive);
    EXPECT_EQ(353, c_acc->cumulation_num_req_negative);

    std::string node_obj_name {"CNT_PSD_0_count_18446744073709551615_0"};
    auto& node_obj = air::json(node_obj_name);
    std::stringstream stream_obj;
    stream_obj << node_obj;
    std::string str_obj = stream_obj.str();
    std::string exp_obj {
        R"({"cumulation": {"count": -79900, "negative": 1, "num_req_minus)"};
    exp_obj += R"(": 353, "num_req_plus": 55}, "filter": "BI_0", "index":)";
    exp_obj += R"( 18446744073709551615, "period": {"count": -89900, "cou)";
    exp_obj += R"(nt_minus": 90000, "count_plus": 100, "negative": 1, "nu)";
    exp_obj += R"(m_req_minus": 323, "num_req_plus": 45}, "target_id": 0,)";
    exp_obj += R"( "target_name": "thread0"})";
    EXPECT_EQ(0, str_obj.compare(exp_obj));
}
