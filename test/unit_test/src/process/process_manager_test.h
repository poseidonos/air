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

#include "mock_global_meta_getter.h"
#include "mock_node_manager.h"
#include "mock_node_meta_getter.h"
#include "src/process/ProcessManager.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;

class ProcessManagerTest : public ::testing::Test
{
public:
    NiceMock<meta::MockGlobalMetaGetter> mock_global_meta_getter;
    NiceMock<meta::MockNodeMetaGetter> mock_node_meta_getter;
    NiceMock<node::MockNodeManager> mock_node_manager {
        &mock_global_meta_getter, &mock_node_meta_getter};
    process::ProcessManager process_manager {
        &mock_global_meta_getter, &mock_node_meta_getter, &mock_node_manager};

protected:
    void
    SetUp() override
    {
        ON_CALL(mock_global_meta_getter, AirPlay()).WillByDefault(Return(true));
        ON_CALL(mock_global_meta_getter, StreamingInterval())
            .WillByDefault(Return(3));

        ON_CALL(mock_node_meta_getter, ProcessorType(_))
            .WillByDefault(Return(air::ProcessorType::PROCESSORTYPE_NULL));
        mock_node_manager.Init();
    }
    void
    TearDown() override
    {
    }
};
