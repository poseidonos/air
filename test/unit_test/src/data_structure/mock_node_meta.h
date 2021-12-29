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

#include "src/meta/NodeMeta.h"

class MockNodeMetaGetter : public meta::NodeMetaGetter
{
public:
    virtual ~MockNodeMetaGetter()
    {
    }

    virtual air::ProcessorType
    ProcessorType(uint32_t i) const
    {
        switch (i)
        {
            case 0:
                return air::ProcessorType::PERFORMANCE;
            case 1:
                return air::ProcessorType::LATENCY;
            case 2:
                return air::ProcessorType::QUEUE;
            case 3:
                return air::ProcessorType::UTILIZATION;
            default:
                return air::ProcessorType::PROCESSORTYPE_NULL;
        }
    }

    virtual inline uint32_t
    IndexSize(uint32_t nid) const
    {
        return 10;
    }

    virtual inline uint32_t
    FilterSize(uint32_t nid) const
    {
        return 10;
    }

private:
};
