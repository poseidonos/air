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

#include <utility>

#include "mock_global_meta_getter.h"
#include "mock_node_manager.h"
#include "mock_node_meta_getter.h"
#include "src/process/Preprocessor.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;

class PreprocessorTest : public ::testing::Test
{
public:
    NiceMock<meta::MockGlobalMetaGetter> mock_global_meta_getter;
    NiceMock<meta::MockNodeMetaGetter> mock_node_meta_getter;
    NiceMock<node::MockNodeManager> mock_node_manager {
        &mock_global_meta_getter, &mock_node_meta_getter};
    lib::AccLatencyData acc_data[32];
    process::Preprocessor preprocessor {&mock_node_meta_getter, &mock_node_manager};

protected:
    void
    SetUp() override
    {
        ON_CALL(mock_global_meta_getter, AirBuild()).WillByDefault(Return(true));
        ON_CALL(mock_global_meta_getter, AirPlay()).WillByDefault(Return(true));

        ON_CALL(mock_node_meta_getter, IndexSize(_)).WillByDefault(Return(32));
        ON_CALL(mock_node_meta_getter, FilterSize(_)).WillByDefault(Return(32));
        ON_CALL(mock_node_meta_getter, Run(_)).WillByDefault(Return(true));
        ON_CALL(mock_node_meta_getter, ProcessorType(_))
            .WillByDefault(Invoke([](uint32_t nid) {
                switch (nid)
                {
                    case 0:
                        return air::ProcessorType::PERFORMANCE;
                    case 1:
                    case 2:
                    case 3:
                        return air::ProcessorType::LATENCY;
                    case 4:
                        return air::ProcessorType::PROCESSORTYPE_NULL;
                    case 5:
                    case 6:
                        return air::ProcessorType::QUEUE;
                    default:
                        return air::ProcessorType::PROCESSORTYPE_NULL;
                }
            }));

        mock_node_manager.Init();
        node::NodeDataArray* node_data_array = new node::NodeDataArray;
        node_data_array->node[0] =
            new node::NodeData(air::ProcessorType::PERFORMANCE, 32, 32);
        node_data_array->node[1] =
            new node::NodeData(air::ProcessorType::LATENCY, 32, 32);
        node_data_array->node[2] =
            new node::NodeData(air::ProcessorType::LATENCY, 32, 32);
        node_data_array->node[3] =
            new node::NodeData(air::ProcessorType::LATENCY, 32, 32);
        node_data_array->node[4] =
            new node::NodeData(air::ProcessorType::LATENCY, 32, 32);
        node_data_array->node[5] =
            new node::NodeData(air::ProcessorType::QUEUE, 32, 32);
        node_data_array->node[6] =
            new node::NodeData(air::ProcessorType::QUEUE, 32, 32);
        mock_node_manager.nda_map.insert(std::make_pair(123, node_data_array));
        ON_CALL(mock_node_manager, GetAccLatData(_, _, _))
            .WillByDefault(Invoke(
                [this](uint32_t nid, uint32_t hash_index, uint32_t filter_index) {
                    return &(acc_data[filter_index]);
                }));
    }
    void
    TearDown() override
    {
    }
};
