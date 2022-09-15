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

#ifndef AIR_PROCESS_MANAGER_H
#define AIR_PROCESS_MANAGER_H

#include <string>

#include "src/config/ConfigInterface.h"
#include "src/data_structure/NodeManager.h"
#include "src/lib/StringView.h"
#include "src/lib/Timelag.h"
#include "src/lib/json/Json.h"
#include "src/meta/GlobalMeta.h"
#include "src/meta/NodeMeta.h"
#include "src/process/TimingDistributor.h"
#include "src/process/processor/Processor.h"

namespace process
{
class ProcessManager
{
public:
    ProcessManager(meta::GlobalMetaGetter* global_meta_getter,
        meta::NodeMetaGetter* node_meta_getter, node::NodeManager* node_manager)
    : global_meta_getter(global_meta_getter),
      node_meta_getter(node_meta_getter),
      node_manager(node_manager)
    {
    }
    ~ProcessManager(void);
    int Init(void);
    void StreamData(void);
    void SetTimeSlot(void);

private:
    void _AddGroupInfo(air::JSONdoc& doc);
    void _AddNodeInfo(air::string_view& group_name,
        air::string_view& node_name_view, uint32_t nid, air::ProcessorType type);

    Processor* processor[cfg::GetSentenceCount(config::ParagraphType::NODE)] {
        nullptr,
    };
    meta::GlobalMetaGetter* global_meta_getter {nullptr};
    meta::NodeMetaGetter* node_meta_getter {nullptr};
    node::NodeManager* node_manager {nullptr};
    TimingDistributor timing_distributor;
    const uint32_t MAX_NID_SIZE {
        cfg::GetSentenceCount(config::ParagraphType::NODE)};
    timespec last_timestamp {0, 0};
    double time_unit {0.0};
};

} // namespace process

#endif // AIR_PROCESS_MANAGER_H
