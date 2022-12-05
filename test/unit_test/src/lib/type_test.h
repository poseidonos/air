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

#include <gmock/gmock.h>

#include "src/config/ConfigInterface.h"
#include "src/lib/Type.h"

class TypeTest : public ::testing::Test
{
public:
    air::NodeMetaData node_meta {};

protected:
    TypeTest()
    {
    }
    ~TypeTest() override
    {
    }
    void
    SetUp() override
    {
        node_meta.nid =
            cfg::GetSentenceIndex(config::ParagraphType::NODE, "PERF_PSD");
        node_meta.processor_type = air::ProcessorType::PERFORMANCE;
        node_meta.run = true;
        node_meta.group_id = 1;
        node_meta.index_size = 10;
        ;
        node_meta.filter_size = 10;
    }
    void
    TearDown() override
    {
    }
};
