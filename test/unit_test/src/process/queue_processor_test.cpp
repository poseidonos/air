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

#include "queue_processor_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

TEST_F(QueueProcessorTest, StreamData_DefaultData)
{
    air::string_view node_name {"Q_NODE"};

    queue_processor.StreamData(node_name, 0, "thread0", nullptr,
        air::ProcessorType::PERFORMANCE, 1.0, 1, 1);
    queue_processor.StreamData(node_name, 0, "thread0", queue_data,
        air::ProcessorType::PERFORMANCE, 1.0, 1, 1);
    queue_processor.StreamData(node_name, 0, "thread0", queue_data,
        air::ProcessorType::PERFORMANCE, 1.0, 1, 1);
}

TEST_F(QueueProcessorTest, StreamData_DataCase1)
{
    air::string_view node_name_q1 {"Q_SUBMIT"};
    queue_processor.StreamData(node_name_q1, 123, "thread0", queue_data,
        air::ProcessorType::QUEUE, 1, 2, 2);
    air::string_view node_name_q2 {"Q_UNKNOWN"};
    queue_processor.StreamData(node_name_q2, 123, "thread0", queue_data,
        air::ProcessorType::QUEUE, 1, 2, 2);
}

TEST_F(QueueProcessorTest, StreamData_DataCase2)
{
    lib::QueueData* q_data {(lib::QueueData*)queue_data->GetAirData(0, 0)};
    lib::AccQueueData* acc_q_data {
        (lib::AccQueueData*)queue_data->GetAccData(0, 0)};

    q_data->period_num_req = 12;
    q_data->period_qd_sum = 120;
    q_data->period_qd_max = 30;
    q_data->access = true;
    acc_q_data->need_erase = false;
    acc_q_data->updated_count = 0;
    air::string_view node_name_q1 {"Q_SUBMIT"};
    queue_processor.StreamData(
        node_name_q1, 0, "thread0", queue_data, air::ProcessorType::QUEUE, 1, 1, 1);
    EXPECT_EQ(true,
        ((10.1f > acc_q_data->cumulation_qd_avg) &&
            (9.9f < acc_q_data->cumulation_qd_avg)));
    air::json_clear();

    // for swap buffer
    queue_processor.StreamData(
        node_name_q1, 0, "thread0", queue_data, air::ProcessorType::QUEUE, 1, 1, 1);
    air::json_clear();

    q_data->period_num_req = 12;
    q_data->period_qd_sum = 240;
    q_data->access = true;
    acc_q_data->need_erase = false;
    air::string_view node_name_q2 {"Q_SUBMIT"};
    queue_processor.StreamData(
        node_name_q2, 0, "thread0", queue_data, air::ProcessorType::QUEUE, 1, 1, 1);
    EXPECT_EQ(true,
        ((15.1f > acc_q_data->cumulation_qd_avg) &&
            (14.9f < acc_q_data->cumulation_qd_avg)));
}

TEST_F(QueueProcessorTest, StreamData_DataCase3)
{
    lib::QueueData* q_data {(lib::QueueData*)queue_data->GetAirData(0, 1)};
    lib::AccQueueData* acc_q_data {
        (lib::AccQueueData*)queue_data->GetAccData(0, 1)};

    q_data->period_num_req = 12;
    q_data->period_qd_sum = 120;
    q_data->period_qd_max = 19;
    q_data->access = true;
    acc_q_data->need_erase = false;
    air::string_view node_name_q1 {"Q_SUBMIT"};
    queue_processor.StreamData(
        node_name_q1, 0, "thread1", queue_data, air::ProcessorType::QUEUE, 1, 2, 2);

    std::string node_obj_name {"Q_SUBMIT_0_queue_18446744073709551615_1"};
    auto& node_obj = air::json(node_obj_name);
    std::stringstream stream_obj;
    stream_obj << node_obj;
    std::string str_obj = stream_obj.str();
    std::string exp_obj {
        R"({"cumulation": {"num_req": 12, "qd_avg": 10, "qd_max": 19}, "filter": )"};
    exp_obj += R"("BI_1", "index": 18446744073709551615, "period": {"num_req":)";
    exp_obj += R"( 12, "qd_avg": 10, "qd_max": 19}, "target_id": 0,)";
    exp_obj += R"( "target_name": "thread1"})";
    EXPECT_EQ(0, str_obj.compare(exp_obj));
}
