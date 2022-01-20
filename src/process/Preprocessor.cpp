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

#include "src/lib/Protocol.h"
#include "src/lib/Timelag.h"

void
process::Preprocessor::Run(int option)
{
    _GetStopOrFullData();
    _ConvertData();
    _MatchData();
    _CleanData(option);
}

void
process::Preprocessor::_GetStopOrFullData(void)
{
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
                    for (uint32_t filter_index = 0; filter_index < filter_size - 1;
                         filter_index++)
                    {
                        lib::LatencyData* from = static_cast<lib::LatencyData*>(
                            kv.second->node[nid]->GetUserDataByHashIndex(
                                hash_index, filter_index));
                        if (lib::TimeLogState::STOP == from->start_state ||
                            lib::TimeLogState::FULL == from->start_state)
                        {
                            from->start_state = lib::TimeLogState::DONE;
                            uint64_t key {0};
                            key = ((uint64_t)nid << 48) +
                                ((uint64_t)hash_index << 32) +
                                ((uint64_t)filter_index);
                            match_map[key].done_from.push_back(from);
                        }

                        lib::LatencyData* to = static_cast<lib::LatencyData*>(
                            kv.second->node[nid]->GetUserDataByHashIndex(
                                hash_index, filter_index + 1));
                        if (lib::TimeLogState::STOP == to->end_state ||
                            lib::TimeLogState::FULL == to->end_state)
                        {
                            to->end_state = lib::TimeLogState::DONE;
                            uint64_t key {0};
                            key = ((uint64_t)nid << 48) +
                                ((uint64_t)hash_index << 32) +
                                ((uint64_t)filter_index);
                            match_map[key].done_to.push_back(to);
                        }
                    }
                }
            }
        }
    }
}

void
process::Preprocessor::_ConvertData(void)
{
    for (auto& it : match_map)
    {
        auto& data = it.second;
        if (data.done)
        {
            continue;
        }

        bool need_update {false};
        while (!data.done_from.empty())
        {
            lib::LatencyData* lat_data = data.done_from.back();
            for (auto& timelog : lat_data->start_v)
            {
                data.timestamp_from[timelog.key] = timelog.timestamp;
                need_update = true;
            }
            data.done_from.pop_back();
        }
        while (!data.done_to.empty())
        {
            lib::LatencyData* lat_data = data.done_to.back();
            for (auto& timelog : lat_data->end_v)
            {
                data.timelog_to.push_back(timelog);
                need_update = true;
            }
            data.done_to.pop_back();
        }

        if (need_update)
        {
            data.update = true;
        }
    }
}

void
process::Preprocessor::_MatchData(void)
{
    for (auto& it : match_map)
    {
        auto& data = it.second;
        if (data.done || !data.update)
        {
            continue;
        }
        data.update = false;

        for (auto& timelog : data.timelog_to)
        {
            auto it_match = data.timestamp_from.find(timelog.key);
            if (it_match != data.timestamp_from.end())
            {
                uint64_t key = it.first;
                uint32_t nid = ((key >> 48) & 0xFFFF);
                uint32_t hash_index = (key >> 32 & 0xFFFF);
                uint32_t filter_index = (key & 0xFFFFFFFF);
                lib::AccLatencyData* acc_data =
                    node_manager->GetAccLatData(nid, hash_index, filter_index);

                uint64_t timelag =
                    Timelag::GetDiff(timelog.timestamp, it_match->second);
                data.timestamp_from.erase(it_match);

                if (MAX_TIME > timelag)
                {
                    if (!latency_writer.AddTimelag(acc_data, timelag))
                    {
                        data.done = true;
                        break;
                    }
                }
            }
        }
    }
}

void
process::Preprocessor::_CleanData(int option)
{
    if (to_dtype(pi::PreprocessOption::FORCED) == option)
    {
        for (auto& it : match_map)
        {
            auto& data = it.second;
            data.done_from.clear();
            data.done_to.clear();
            data.timestamp_from.clear();
            data.timelog_to.clear();
        }
        match_map.clear();
    }
}
