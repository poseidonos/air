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

#include "preprocessor_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST_F(PreprocessorTest, Run)
{
    lib::Data* data =
        mock_node_manager.nda_map[123]->node[1]->GetUserDataByHashIndex(0, 0);
    lib::LatencyData* lat_data_0 = static_cast<lib::LatencyData*>(data);
    lat_data_0->access = true;
    data = mock_node_manager.nda_map[123]->node[1]->GetUserDataByHashIndex(0, 1);
    lib::LatencyData* lat_data_1 = static_cast<lib::LatencyData*>(data);
    data = mock_node_manager.nda_map[123]->node[1]->GetUserDataByHashIndex(0, 2);
    lib::LatencyData* lat_data_2 = static_cast<lib::LatencyData*>(data);

    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    lat_data_0->start_v.push_back({123, ts});
    lat_data_0->start_v.push_back({124, ts});
    lat_data_0->start_state = lib::TimeLogState::STOP;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    lat_data_1->end_v.push_back({123, ts});
    lat_data_1->end_v.push_back({125, ts});
    lat_data_1->end_state = lib::TimeLogState::STOP;

    lat_data_1->start_state = lib::TimeLogState::RUN;

    lat_data_2->end_state = lib::TimeLogState::RUN;

    preprocessor.Run(0);

    EXPECT_EQ(lib::TimeLogState::DONE, lat_data_0->start_state);
    EXPECT_EQ(lib::TimeLogState::DONE, lat_data_1->end_state);
    lib::AccLatencyData* acc_data = mock_node_manager.GetAccLatData(1, 0, 0);
    EXPECT_EQ(1, acc_data->period_sample_count);
}
