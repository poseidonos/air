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
#include "src/data_structure/NodeData.h"
#include "src/process/processor/CountProcessor.h"
#include "src/process/processor/HistogramProcessor.h"
#include "src/process/processor/PerformanceProcessor.h"
#include "src/process/processor/QueueProcessor.h"
#include "src/process/processor/UtilizationProcessor.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;

class ProcessorTest : public ::testing::Test
{
public:
    process::PerformanceProcessor perf_processor;
    node::NodeData perf_data {air::ProcessorType::PERFORMANCE, 32, 32};
    process::QueueProcessor queue_processor;
    node::NodeData queue_data {air::ProcessorType::QUEUE, 32, 32};
    process::UtilizationProcessor util_processor;
    node::NodeData util_data {air::ProcessorType::UTILIZATION, 32, 32};
    process::CountProcessor count_processor;
    node::NodeData count_data {air::ProcessorType::COUNT, 32, 32};
    process::HistogramProcessor histogram_processor;
    node::NodeData histogram_data {air::ProcessorType::HISTOGRAM, 32, 32};

protected:
    void
    SetUp() override
    {
    }
    void
    TearDown() override
    {
    }
};
