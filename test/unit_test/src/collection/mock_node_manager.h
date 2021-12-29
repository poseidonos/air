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

#include "src/data_structure/NodeData.cpp"
#include "src/data_structure/NodeInitializer.cpp"
#include "src/data_structure/NodeInitializer.h"
#include "src/data_structure/NodeManager.cpp"
#include "src/data_structure/NodeManager.h"
#include "src/lib/Hash.cpp"

class MockNodeManager : public node::NodeManager
{
public:
    virtual ~MockNodeManager()
    {
        nda_map.clear();
    }
    MockNodeManager(meta::GlobalMetaGetter* new_global_meta_getter,
        meta::NodeMetaGetter* new_node_meta_getter)
    : node::NodeManager(new_global_meta_getter, new_node_meta_getter)
    {
    }
    node::NodeDataArray*
    GetNodeDataArray(uint32_t tid)
    {
        auto nda_iter = nda_map.find(tid);
        if (nda_iter != nda_map.end())
        {
            return nda_iter->second;
        }

        return nullptr;
    }

    lib::AccLatencyData*
    GetAccLatData(uint32_t nid, uint32_t aid)
    {
        return &(mock_acc_lat_data[aid]);
    }

    void
    CreateNodeDataArray(uint32_t tid)
    {
        node::NodeDataArray* node_data_array = GetNodeDataArray(tid);
        if (nullptr != node_data_array)
        {
            return; // already create
        }

        node::NodeDataArray* nda = new node::NodeDataArray;
        nda->node[0] = new node::NodeData(air::ProcessorType::PERFORMANCE, 3, 3);
        nda->node[1] = new node::NodeData(air::ProcessorType::PERFORMANCE, 3, 3);
        nda->node[2] = new node::NodeData(air::ProcessorType::PERFORMANCE, 3, 3);
        nda->node[3] = new node::NodeData(air::ProcessorType::PERFORMANCE, 3, 3);
        nda->node[4] = new node::NodeData(air::ProcessorType::PERFORMANCE, 3, 3);
        nda->node[5] = new node::NodeData(air::ProcessorType::LATENCY, 3, 3);
        nda->node[6] = new node::NodeData(air::ProcessorType::LATENCY, 3, 3);
        nda->node[7] = new node::NodeData(air::ProcessorType::LATENCY, 3, 3);
        nda->node[8] = new node::NodeData(air::ProcessorType::LATENCY, 3, 3);
        nda->node[9] = new node::NodeData(air::ProcessorType::LATENCY, 3, 3);
        nda->node[10] = new node::NodeData(air::ProcessorType::QUEUE, 3, 3);
        nda->node[11] = new node::NodeData(air::ProcessorType::QUEUE, 3, 3);
        nda->node[12] = new node::NodeData(air::ProcessorType::QUEUE, 3, 3);
        nda->node[13] = new node::NodeData(air::ProcessorType::QUEUE, 3, 3);
        nda->node[14] = new node::NodeData(air::ProcessorType::QUEUE, 3, 3);

        nda_map.insert(std::make_pair(tid, nda));
    }

private:
    lib::AccLatencyData mock_acc_lat_data[32];
};
