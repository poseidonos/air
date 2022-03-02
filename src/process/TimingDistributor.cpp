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

#include "src/process/TimingDistributor.h"

#include <map>

void
process::TimingDistributor::SetTiming(
    meta::NodeMetaGetter* node_meta_getter, node::NodeManager* node_manager)
{
    std::map<uint64_t, int32_t> timing_map;
    timing_map.clear();

    for (auto& kv : node_manager->nda_map)
    {
        for (uint32_t nid = 0; nid < MAX_NID_SIZE; nid++)
        {
            uint32_t index_size = node_meta_getter->IndexSize(nid);
            uint32_t filter_size = node_meta_getter->FilterSize(nid);

            if (air::ProcessorType::LATENCY == node_meta_getter->ProcessorType(nid))
            {
                for (uint32_t hash_index = 0; hash_index < index_size; hash_index++)
                {
                    for (uint32_t filter_index = 0; filter_index < filter_size - 1;
                         filter_index++)
                    {
                        lib::LatencyData* to = static_cast<lib::LatencyData*>(
                            kv.second->node[nid]->GetUserDataByHashIndex(
                                hash_index, filter_index + 1));
                        lib::LatencyData* from = static_cast<lib::LatencyData*>(
                            kv.second->node[nid]->GetUserDataByHashIndex(
                                hash_index, filter_index));
                        lib::AccLatencyData* acc = node_manager->GetAccLatData(
                            nid, hash_index, filter_index);

                        uint64_t key {0};
                        int32_t value {0};
                        key = ((uint64_t)nid << 48) + ((uint64_t)hash_index << 32) +
                            ((uint64_t)filter_index);
                        value = dist(rand_engine);

                        auto it_finder = timing_map.find(key);
                        if (it_finder == timing_map.end())
                        {
                            timing_map.insert({key, value});
                        }

                        _ResetTiming(
                            from, to, timing_map[key], acc->period_sample_count);
                    }
                }
            }
        }
    }

    for (uint32_t nid = 0; nid < MAX_NID_SIZE; nid++)
    {
        if (node_meta_getter->ProcessorType(nid) == air::ProcessorType::LATENCY)
        {
            uint32_t index_size = node_meta_getter->IndexSize(nid);
            for (uint32_t hash_index = 0; hash_index < index_size; hash_index++)
            {
                uint32_t filter_size = node_meta_getter->FilterSize(nid);
                for (uint32_t filter_index = 0; filter_index < filter_size - 1;
                     filter_index++)
                {
                    lib::AccLatencyData* acc =
                        node_manager->GetAccLatData(nid, hash_index, filter_index);
                    acc->period_sample_count = 0;
                }
            }
        }
    }
}

void
process::TimingDistributor::_ResetTiming(lib::LatencyData* curr_data,
    lib::LatencyData* next_data, int32_t time_value, uint32_t sampled_count)
{
    int32_t token {curr_data->start_token_size};
    uint32_t minimum_sampled_count {lib::TIMELAG_SIZE / 4};         // 25%
    uint32_t enough_sampled_count {(lib::TIMELAG_SIZE * 97) / 100}; // 97%
    if (minimum_sampled_count > sampled_count)
    {
        if (MAX_TOKEN > token)
        {
            token += MIN_TOKEN;
        }
    }
    else if (enough_sampled_count < sampled_count)
    {
        if (MIN_TOKEN < token)
        {
            token -= MIN_TOKEN;
        }
    }

    curr_data->start_deadline = time_value;
    curr_data->start_v.clear();
    if (curr_data->access)
    {
        curr_data->start_v.reserve(token + 4);
    }
    curr_data->start_token_size = token;
    curr_data->start_state = lib::TimeLogState::IDLE;
    curr_data->access = false;

    next_data->end_deadline = time_value;
    next_data->end_v.clear();
    if (next_data->access)
    {
        next_data->end_v.reserve(token + 4);
    }
    next_data->end_token_size = token;
    next_data->end_state = lib::TimeLogState::IDLE;
    next_data->access = false;
}
