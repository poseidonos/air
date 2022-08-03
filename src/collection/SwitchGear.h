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

#ifndef AIR_SWITCH_GEAR_H
#define AIR_SWITCH_GEAR_H

#include "src/config/ConfigInterface.h"
#include "src/data_structure/NodeManager.h"
#include "src/lib/Data.h"
#include "src/meta/GlobalMeta.h"
#include "src/meta/NodeMeta.h"

namespace collection
{
class SwitchGear
{
public:
    SwitchGear(meta::NodeMetaGetter* new_node_meta_getter,
        meta::GlobalMetaGetter* new_global_meta_getter,
        node::NodeManager* new_node_manager)
    : node_meta_getter(new_node_meta_getter),
      global_meta_getter(new_global_meta_getter),
      node_manager(new_node_manager)
    {
    }
    virtual ~SwitchGear(void)
    {
    }
    void Run(void);

private:
    void _CheckDeadline(lib::Data* data, uint32_t deadline);
    meta::NodeMetaGetter* node_meta_getter {nullptr};
    meta::GlobalMetaGetter* global_meta_getter {nullptr};
    node::NodeManager* node_manager {nullptr};
    const uint32_t MAX_NID_SIZE {
        cfg::GetSentenceCount(config::ParagraphType::NODE)};
};

} // namespace collection

#endif // AIR_SWITCH_GEAR_H
