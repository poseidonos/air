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

#ifndef AIR_RULER_H
#define AIR_RULER_H

#include "src/config/ConfigInterface.h"
#include "src/lib/Protocol.h"
#include "src/meta/GlobalMeta.h"
#include "src/meta/NodeMeta.h"

namespace policy
{
class Ruler
{
public:
    Ruler(meta::NodeMeta* node_meta, meta::GlobalMeta* global_meta)
    : node_meta(node_meta),
      global_meta(global_meta)
    {
    }
    int CheckRule(uint32_t type1, uint32_t type2, uint32_t value1, uint32_t value2);
    bool SetRule(uint32_t type1, uint32_t type2, uint32_t value1, uint32_t value2);
    void
    SetStreamingInterval(int streaming_interval)
    {
        global_meta->SetStreamingInterval(streaming_interval);
    }
    void
    SetCpuNum(uint32_t cpu_num)
    {
        global_meta->SetCpuNum(cpu_num);
    }
    void
    SetAirBuild(bool air_build)
    {
        global_meta->SetAirBuild(air_build);
    }

private:
    int _CheckEnableNodeRule(
        uint32_t type1, uint32_t type2, uint32_t value1, uint32_t value2);
    int _CheckInitNodeRule(
        uint32_t type1, uint32_t type2, uint32_t value1, uint32_t value2);
    int _CheckSetSamplingRatioRule(
        uint32_t type1, uint32_t type2, uint32_t value1, uint32_t value2);
    int _CheckStreamInterval(
        uint32_t type1, uint32_t type2, uint32_t value1, uint32_t value2);
    bool _SetEnableNodeRule(
        uint32_t type1, uint32_t type2, uint32_t value1, uint32_t value2);
    // bool SetInitNodeRule(uint32_t type1, uint32_t type2,
    //                     uint32_t value1, uint32_t value2);
    bool _SetSamplingRatioRule(
        uint32_t type1, uint32_t type2, uint32_t value1, uint32_t value2);

    meta::NodeMeta* node_meta {nullptr};
    meta::GlobalMeta* global_meta {nullptr};
    const uint32_t MAX_NID_SIZE {
        cfg::GetSentenceCount(config::ParagraphType::NODE)};
};

} // namespace policy

#endif // AIR_RULER_H
