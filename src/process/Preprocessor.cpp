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

#include "src/process/Preprocessor.h"

#include <algorithm>
#include <map>
#include <vector>

#include "src/lib/Protocol.h"
#include "src/lib/Timelag.h"

void
process::Preprocessor::Run(int option)
{
    struct match_st
    {
        uint32_t run_state_count{0};
        std::vector<lib::LatencyData*> curr_v;
        std::vector<lib::LatencyData*> next_v;
    };

    std::map<uint64_t, struct match_st> match_map;
    match_map.clear();

    for (auto& kv : node_manager->nda_map)
    {
        for (uint32_t nid = 0; nid < MAX_NID_SIZE; nid++)
        {
            if (air::ProcessorType::LATENCY == node_meta_getter->ProcessorType(nid))
            {
                uint32_t index_size = node_meta_getter->IndexSize(nid);
                uint32_t filter_size = node_meta_getter->FilterSize(nid);
                for (uint32_t hash_index = 0; hash_index < index_size; hash_index++)
                {
                    for (uint32_t filter_index = 0; filter_index < filter_size - 1; filter_index++)
                    {
                        lib::LatencyData* to = static_cast<lib::LatencyData*>(
                            kv.second->node[nid]->GetUserDataByHashIndex(hash_index, filter_index + 1));
                        lib::LatencyData* from = static_cast<lib::LatencyData*>(
                            kv.second->node[nid]->GetUserDataByHashIndex(hash_index, filter_index));

                        if (from->access || to->access)
                        {
                            uint64_t key{0};
                            key = ((uint64_t)nid << 48) + ((uint64_t)hash_index << 32) +
                                ((uint64_t)filter_index);
                            struct match_st match_value;
                            match_value.run_state_count = 0;
                            match_value.curr_v.clear();
                            match_value.next_v.clear();

                            auto it_finder = match_map.find(key);
                            if (it_finder == match_map.end())
                            {
                                match_map.insert({key, match_value});
                            }

                            switch (from->start_state)
                            {
                                case (lib::TimeLogState::RUN):
                                    match_map[key].run_state_count += 1;
                                    break;
                                case (lib::TimeLogState::STOP):
                                case (lib::TimeLogState::FULL):
                                    if (!from->start_v.empty())
                                    {
                                        match_map[key].curr_v.push_back(from);
                                    }
                                    break;
                                case (lib::TimeLogState::DONE):
                                case (lib::TimeLogState::IDLE):
                                default:
                                    break;
                            }

                            switch (to->end_state)
                            {
                                case (lib::TimeLogState::RUN):
                                    match_map[key].run_state_count += 1;
                                    break;
                                case (lib::TimeLogState::STOP):
                                case (lib::TimeLogState::FULL):
                                    if (!to->end_v.empty())
                                    {
                                        match_map[key].next_v.push_back(to);
                                    }
                                    break;
                                case (lib::TimeLogState::DONE):
                                case (lib::TimeLogState::IDLE):
                                default:
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }

    for (auto& kv : match_map)
    {
        if (0 == kv.second.run_state_count)
        {
            if ((!kv.second.curr_v.empty()) && (!kv.second.next_v.empty()))
            {
                uint32_t nid = ((kv.first >> 48) & 0xFFFF);
                uint32_t hash_index = (kv.first >> 32 & 0xFFFF);
                uint32_t filter_index = (kv.first & 0xFFFFFFFF);

                for (auto curr_v : kv.second.curr_v)
                {
                    for (auto next_v : kv.second.next_v)
                    {
                        _MatchKey(curr_v, next_v,
                            node_manager->GetAccLatData(nid, hash_index, filter_index));
                    }
                }

                for (auto curr_v : kv.second.curr_v)
                {
                    for (auto next_v : kv.second.next_v)
                    {
                        curr_v->start_v.clear();
                        next_v->end_v.clear();
                    }
                }
            }
        }
    }
}

void
process::Preprocessor::_MatchKey(lib::LatencyData* curr_data,
    lib::LatencyData* next_data,
    lib::AccLatencyData* acc_data)
{
    curr_data->start_state = lib::TimeLogState::DONE;
    next_data->end_state = lib::TimeLogState::DONE;

    uint64_t timelag{0};

    for (auto it_start = curr_data->start_v.begin(); it_start != curr_data->start_v.end();)
    {
        for (auto it_end = next_data->end_v.begin(); it_end != next_data->end_v.end();)
        {
            if (it_start->key == it_end->key)
            {
                timelag = Timelag::GetDiff(it_end->timestamp, it_start->timestamp);
                if (MAX_TIME > timelag)
                {
                    latency_writer.AddTimelag(acc_data, timelag);
                    curr_data->start_match_count++;
                    next_data->end_match_count++;
                }

                break;
            }
            it_end++;
        }
        it_start++;
    }
}
