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

#ifndef AIR_HISTOGRAM_PROCESSOR_TEST_H
#define AIR_HISTOGRAM_PROCESSOR_TEST_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/data_structure/NodeData.h"
#include "src/lib/json/Json.h"
#include "src/process/processor/HistogramProcessor.h"

class HistogramProcessorTest : public ::testing::Test
{
public:
    process::HistogramProcessor histogram_processor;
    node::NodeData* histogram_data {nullptr};

protected:
    void
    SetUp() override
    {
        histogram_data = new node::NodeData {air::ProcessorType::HISTOGRAM, 2, 2};
    }
    void
    TearDown() override
    {
        delete histogram_data;
        air::json_clear();
    }
};

#endif // AIR_HISTOGRAM_PROCESSOR_TEST_H
