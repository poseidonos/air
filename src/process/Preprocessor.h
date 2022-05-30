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

#ifndef AIR_PREPROCESSOR_H
#define AIR_PREPROCESSOR_H

#include <map>
#include <vector>

#include "src/collection/writer/LatencyWriter.h"
#include "src/config/ConfigInterface.h"
#include "src/data_structure/NodeManager.h"
#include "src/lib/Data.h"
#include "src/meta/NodeMeta.h"

namespace process
{
class Preprocessor
{
public:
    Preprocessor(meta::NodeMetaGetter* new_node_meta_getter,
        node::NodeManager* new_node_manager)
    : node_meta_getter(new_node_meta_getter),
      node_manager(new_node_manager)
    {
    }
    virtual ~Preprocessor(void)
    {
    }
    void Run(int option);

private:
    void _GetStopOrFullData(void);
    void _ConvertData(void);
    void _MatchData(void);
    void _CleanData(int option);

    static const uint32_t MAX_TIMELOG_TO_SIZE {100000};
    struct match_st
    {
        std::vector<lib::LatencyData*> done_from;
        std::vector<lib::LatencyData*> done_to;
        std::map<uint64_t, timespec> timestamp_from;
        lib::TimeLog timelog_to[MAX_TIMELOG_TO_SIZE];
        uint32_t timelog_to_size {0};
        bool done {false};
        bool update {false};
    };
    std::map<uint64_t, struct match_st> match_map;
    meta::NodeMetaGetter* node_meta_getter {nullptr};
    node::NodeManager* node_manager {nullptr};
    collection::LatencyWriter latency_writer {};
    static const uint64_t MAX_TIME {900000000}; // 900 ms
    const uint32_t MAX_NID_SIZE {
        cfg::GetSentenceCount(config::ParagraphType::NODE)};
};

} // namespace process

#endif // AIR_PREPROCESSOR_H
