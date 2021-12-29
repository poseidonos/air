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

#include "node_manager_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <pthread.h>

TEST_F(NodeManagerTest, SetNodeDataArrayName)
{
    uint32_t tid{0x00000001};

    node_manager->CreateNodeDataArray(tid);
    node::NodeDataArray* node_data_array = node_manager->GetNodeDataArray(tid);
    node_manager->SetNodeDataArrayName(tid);
    const char* str = node_data_array->tname.c_str();
    EXPECT_EQ(strcmp(str, "ut_data_structu"), 0);

    tid = 0x00000002;
    node_manager->CreateNodeDataArray(tid);
    node_data_array = node_manager->GetNodeDataArray(tid);
    pthread_setname_np(pthread_self(), "thread0");
    node_manager->SetNodeDataArrayName(tid);
    str = node_data_array->tname.c_str();
    EXPECT_EQ(strcmp(str, "thread0"), 0);
}

TEST_F(NodeManagerTest, CreateNodeDataArray)
{
    uint32_t tid{0x00000001};

    node_manager->CreateNodeDataArray(tid);
    node::NodeDataArray* node_data_array = node_manager->GetNodeDataArray(tid);
    node::NodeDataArray* node_data_array_ = node_manager->nda_map[tid];
    EXPECT_EQ(node_data_array_, node_data_array);

    tid = 0x00000002;
    node_manager->CreateNodeDataArray(tid);
    node_data_array = node_manager->GetNodeDataArray(tid);
    EXPECT_NE(node_data_array_, node_data_array);

    tid = 0x00000001;
    node_manager->CreateNodeDataArray(tid);
    node_data_array = node_manager->GetNodeDataArray(tid);
    EXPECT_EQ(node_data_array_, node_data_array);
}

TEST_F(NodeManagerTest, DeleteNodeDataArray)
{
    uint32_t tid{0x00000001};

    node_manager->CreateNodeDataArray(tid);
    node::NodeDataArray* node_data_array = node_manager->GetNodeDataArray(tid);
    EXPECT_NE(nullptr, node_data_array->node[0]);

    auto iter = node_manager->nda_map.begin();
    while (iter != node_manager->nda_map.end())
    {
        node_manager->DeleteNodeDataArray(iter->second);
        node_manager->nda_map.erase(iter++);
    }
    EXPECT_EQ(nullptr, node_manager->GetNodeDataArray(tid));
}

TEST_F(NodeManagerTest, CanDelete)
{
    uint32_t tid{0x00000001};

    node_manager->CreateNodeDataArray(tid);
    node::NodeDataArray* node_data_array = node_manager->GetNodeDataArray(tid);
    lib::Data* data = node_data_array->node[0]->GetAirData(0, 0);

    data->access = 1;
    EXPECT_EQ(false, node_manager->CanDeleteNodeDataArray(node_data_array));
    data->access = 0;
    EXPECT_EQ(true, node_manager->CanDeleteNodeDataArray(node_data_array));

    data = node_data_array->node[0]->GetUserDataByHashIndex(1, 0);
    data->access = 1;
    EXPECT_EQ(false, node_manager->CanDeleteNodeDataArray(node_data_array));
    data->access = 0;
    EXPECT_EQ(true, node_manager->CanDeleteNodeDataArray(node_data_array));

    lib::Data* lat_data = node_data_array->node[1]->GetUserDataByHashIndex(0, 0);
    lat_data->access = 1;
    EXPECT_EQ(false, node_manager->CanDeleteNodeDataArray(node_data_array));
    lat_data->access = 0;
    EXPECT_EQ(true, node_manager->CanDeleteNodeDataArray(node_data_array));
}

TEST_F(NodeManagerTest, GetAccLatSeqData)
{
    node_manager->CreateNodeDataArray(0);
    lib::AccLatencyData* seq_data = node_manager->GetAccLatData(0, 0, 0);
    EXPECT_EQ(nullptr, seq_data);
    seq_data = node_manager->GetAccLatData(1, 0, 0);
    EXPECT_EQ(0, seq_data->sample_count);
}
