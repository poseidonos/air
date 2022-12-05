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

#ifndef AIR_COLLECTION_MANAGER_H
#define AIR_COLLECTION_MANAGER_H

#include <queue>

#include "src/collection/Collector.h"
#include "src/config/ConfigInterface.h"
#include "src/data_structure/NodeManager.h"
#include "src/lib/Design.h"
#include "src/lib/Msg.h"
#include "src/meta/GlobalMeta.h"
#include "src/meta/NodeMeta.h"

namespace collection
{
class Subject : public lib_design::Subject
{
public:
    Subject(void)
    {
    }
    virtual ~Subject(void)
    {
    }
    virtual int Notify(uint32_t index, uint32_t type1, uint32_t type2,
        uint32_t value1, uint32_t value2, int pid, int cmd_type, int cmd_order);
};

class CollectionManager
{
public:
    CollectionManager(meta::GlobalMetaGetter* global_meta_getter,
        meta::NodeMetaGetter* node_meta_getter, node::NodeManager* node_manager,
        collection::Subject* subject)
    : global_meta_getter(global_meta_getter),
      node_meta_getter(node_meta_getter),
      node_manager(node_manager),
      subject(subject)
    {
    }
    virtual ~CollectionManager(void);
    void Init(void);

    node::NodeDataArray*
    GetNodeDataArray(uint32_t tid)
    {
        return node_manager->GetNodeDataArray(tid);
    }

    virtual inline bool
    IsLog(uint32_t nid)
    {
        return (air_run & node_run[nid]);
    }

    virtual inline void
    LogData(uint32_t nid, uint32_t filter_index,
        node::NodeDataArray* node_data_array, uint64_t node_index, uint64_t value)
    {
        node::NodeData* node_data = node_data_array->node[nid];
        if (nullptr == node_data)
        {
            return;
        }
        lib::Data* user_data =
            node_data->GetUserDataByNodeIndex(node_index, filter_index);
        if (nullptr == user_data)
        {
            return;
        }

        collector[nid]->LogData(user_data, value);
    }

    void HandleMsg(void);
    int UpdateCollection(
        uint32_t type1, uint32_t type2, uint32_t value1, uint32_t value2);
    void
    EnqueueMsg(uint32_t type1, uint32_t type2, uint32_t value1, uint32_t value2,
        int pid, int cmd_type, int cmd_order)
    {
        int dummy = 0;
        msg.push({type1, type2, value1, value2, pid, cmd_type, cmd_order});
        dummy++;
    }

private:
    int _EnableNode(uint32_t node_index, uint32_t value2);
    int _EnableRangeNode(uint32_t start_idx, uint32_t end_idx, uint32_t value2);
    int _EnableGroupNode(uint32_t gid, uint32_t value2);
    int _UpdateEnable(
        uint32_t type1, uint32_t type2, uint32_t value1, uint32_t value2);
    void _InitNode(uint32_t node_index);
    int _UpdateInit(
        uint32_t type1, uint32_t type2, uint32_t value1, uint32_t value2);

    Collector* collector[cfg::GetSentenceCount(config::ParagraphType::NODE)] {
        nullptr,
    };

    meta::GlobalMetaGetter* global_meta_getter {nullptr};
    bool air_run {false};

    meta::NodeMetaGetter* node_meta_getter {nullptr};
    bool node_run[cfg::GetSentenceCount(config::ParagraphType::NODE)] {
        false,
    };

    node::NodeManager* node_manager {nullptr};

    const uint32_t MAX_NID_SIZE {
        cfg::GetSentenceCount(config::ParagraphType::NODE)};
    const uint32_t MAX_SEQ_IDX {10};

    std::queue<lib::MsgEntry> msg;

    Subject* subject {nullptr};
};

} // namespace collection

#endif // AIR_COLLECTION_MANAGER_H
