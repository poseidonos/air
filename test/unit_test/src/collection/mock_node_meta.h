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
    virtual inline bool
    Run(uint32_t node_index) const
    {
        return true;
    }
    virtual inline air::ProcessorType
    ProcessorType(uint32_t node_index) const
    {
        air::ProcessorType ptype = air::ProcessorType::PROCESSORTYPE_NULL;

        switch (node_index)
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                ptype = air::ProcessorType::PERFORMANCE;
                break;
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                ptype = air::ProcessorType::LATENCY;
                break;
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
                ptype = air::ProcessorType::QUEUE;
                break;
            case 15:
                ptype = air::ProcessorType::UTILIZATION;
                break;
            case 16:
                ptype = air::ProcessorType::COUNT;
                break;
            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
                ptype = air::ProcessorType::HISTOGRAM;
                break;
            default:
                break;
        }

        return ptype;
    }
    virtual inline uint32_t
    GroupId(uint32_t node_index) const
    {
        return group_id[node_index];
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
    uint32_t group_id[7] {1, 0, 1, 3, 2, 0, 1};
};
