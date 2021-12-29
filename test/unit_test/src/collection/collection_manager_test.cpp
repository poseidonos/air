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

#include "collection_manager_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST_F(CollectionManagerTest, Subject_Notify)
{
    EXPECT_EQ(-1, collection_subject->Notify(1, 0, 0, 0, 0, 0, 0, 0));
}

TEST_F(CollectionManagerTest, CreateThread)
{
    mock_node_manager->CreateNodeDataArray(0);
    EXPECT_NE(nullptr, mock_node_manager->GetNodeDataArray(0));
    EXPECT_EQ(nullptr, mock_node_manager->GetNodeDataArray(1));
}

TEST_F(CollectionManagerTest, GetThread)
{
    mock_node_manager->CreateNodeDataArray(0);
    EXPECT_NE(nullptr, collection_manager->GetNodeDataArray(0));
    EXPECT_EQ(nullptr, collection_manager->GetNodeDataArray(1));
}

TEST_F(CollectionManagerTest, IsLog)
{
    collection_manager->Init();
    EXPECT_TRUE(collection_manager->IsLog(0)); // performance
}

TEST_F(CollectionManagerTest, LogData)
{
    collection_manager->Init();

    mock_node_manager->CreateNodeDataArray(0);
    node::NodeDataArray* node_data_array = collection_manager->GetNodeDataArray(0);
    node::NodeData* node_data = node_data_array->node[0];

    collection_manager->LogData(0, 0, node_data_array, 0, 128);
    lib::Data* data = node_data->GetUserDataByHashIndex(0, 0);
    lib::PerformanceData* perf_data = static_cast<lib::PerformanceData*>(data);
    EXPECT_EQ(1, perf_data->iops);
    EXPECT_EQ(128, perf_data->bandwidth);

    collection_manager->LogData(0, 0, node_data_array, 1, 128);
    collection_manager->LogData(0, 0, node_data_array, 2, 256);
    collection_manager->LogData(0, 0, node_data_array, 3, 512);

    data = node_data->GetUserDataByHashIndex(1, 0);
    perf_data = static_cast<lib::PerformanceData*>(data);
    EXPECT_EQ(1, perf_data->iops);
    EXPECT_EQ(128, perf_data->bandwidth);
    data = node_data->GetUserDataByHashIndex(2, 0);
    perf_data = static_cast<lib::PerformanceData*>(data);
    EXPECT_EQ(1, perf_data->iops);
    EXPECT_EQ(256, perf_data->bandwidth);
    data = node_data->GetUserDataByHashIndex(3, 0);
    EXPECT_EQ(nullptr, data);
}

TEST_F(CollectionManagerTest, UpdateCollection)
{
    EXPECT_EQ(0, collection_manager->UpdateCollection(0, to_dtype(pi::Type2::ENABLE_AIR), 1, 0));
    EXPECT_EQ(0, collection_manager->UpdateCollection(0, to_dtype(pi::Type2::ENABLE_AIR), 0, 0));
    EXPECT_EQ(-1, collection_manager->UpdateCollection(0, to_dtype(pi::Type2::ENABLE_AIR), 99, 0));
    EXPECT_EQ(-1, collection_manager->UpdateCollection(0, 99, 0, 0));

    collection_manager->Init();
    mock_node_manager->Init();

    // init node 0 (perf)
    mock_node_manager->CreateNodeDataArray(1);
    node::NodeDataArray* node_data_array = collection_manager->GetNodeDataArray(1);
    node::NodeData* node_data = node_data_array->node[0];
    collection_manager->LogData(0, 0, node_data_array, 0, 128);
    lib::Data* data = node_data->GetUserDataByHashIndex(0, 0);
    lib::PerformanceData* perf_data = static_cast<lib::PerformanceData*>(data);
    EXPECT_EQ(1, perf_data->iops);
    EXPECT_EQ(128, perf_data->bandwidth);

    lib::AccData* acc_data = node_data->GetAccData(0, 0);
    lib::AccPerformanceData* acc_perf_data = static_cast<lib::AccPerformanceData*>(acc_data);
    EXPECT_EQ(0, acc_perf_data->need_erase);
    collection_manager->UpdateCollection(0, to_dtype(pi::Type2::INITIALIZE_NODE), 0, 0);
    EXPECT_EQ(1, acc_perf_data->need_erase);

    // init node 5 (latency)
    collection_manager->UpdateCollection(0, to_dtype(pi::Type2::INITIALIZE_NODE), 5, 0);

    // init node 0,12
    node_data = node_data_array->node[0];
    collection_manager->LogData(0, 0, node_data_array, 0, 128);
    data = node_data->GetUserDataByHashIndex(0, 0);
    perf_data = static_cast<lib::PerformanceData*>(data);
    EXPECT_EQ(2, perf_data->iops);
    EXPECT_EQ(256, perf_data->bandwidth); // not initialized yet

    node_data = node_data_array->node[12];
    collection_manager->LogData(12, 0, node_data_array, 0, 128);
    data = node_data->GetUserDataByHashIndex(0, 0);
    lib::QueueData* queue_data = static_cast<lib::QueueData*>(data);
    EXPECT_EQ(128, queue_data->sum_depth);
    EXPECT_EQ(1, queue_data->num_req);
}

TEST_F(CollectionManagerTest, Observer)
{
    collection::Observer* observer = new collection::Observer {collection_manager};
    collection::CollectionCoRHandler* collection_cor_handler = new collection::CollectionCoRHandler {observer};
    MockOutputObserver* mock_output_observer = new MockOutputObserver;
    collection_subject->Attach(mock_output_observer, 0);

    // send init msg
    observer->Update(0, to_dtype(pi::Type2::INITIALIZE_NODE), 0, 0, 0, 0, 0);

    mock_node_manager->CreateNodeDataArray(1);
    collection_manager->Init();
    node::NodeDataArray* node_data_array = collection_manager->GetNodeDataArray(1);
    node::NodeData* node_data = node_data_array->node[0];
    collection_manager->LogData(0, 0, node_data_array, 0, 128);
    lib::Data* data = node_data->GetUserDataByHashIndex(0, 0);
    lib::PerformanceData* perf_data = static_cast<lib::PerformanceData*>(data);
    lib::AccData* acc_data = node_data->GetAccData(0, 0);
    lib::AccPerformanceData* acc_perf_data =
        static_cast<lib::AccPerformanceData*>(acc_data);
    EXPECT_EQ(1, perf_data->iops);
    EXPECT_EQ(128, perf_data->bandwidth);
    EXPECT_EQ(0, acc_perf_data->need_erase);

    // handle init msg
    collection_cor_handler->HandleRequest();

    // handle error
    EXPECT_EQ(1, perf_data->iops);
    EXPECT_EQ(128, perf_data->bandwidth); // not initialized yet
    EXPECT_EQ(1, acc_perf_data->need_erase);

    observer->Update(0, to_dtype(pi::Type2::SET_SAMPLING_RATE_ALL), 0, 0, 0, 0, 0);
    collection_cor_handler->HandleRequest();

    delete observer;
    delete collection_cor_handler;
}
