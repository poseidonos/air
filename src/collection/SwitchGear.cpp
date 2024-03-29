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

#include "src/collection/SwitchGear.h"

void
collection::SwitchGear::Run(void)
{
    // streaming_interval x 20(50ms resolution) / 2(half streaming_interval)
    uint32_t deadline {global_meta_getter->StreamingInterval() * 10};

    for (uint32_t nid = 0; nid < MAX_NID_SIZE; nid++)
    {
        if (air::ProcessorType::LATENCY == node_meta_getter->ProcessorType(nid))
        {
            uint32_t filter_size = node_meta_getter->FilterSize(nid);
            uint32_t index_size = node_meta_getter->IndexSize(nid);
            if (false == node_meta_getter->Run(nid))
            {
                continue;
            }
            for (uint32_t hash_index = 0; hash_index < index_size; hash_index++)
            {
                for (uint32_t filter_index = 0; filter_index < filter_size;
                     filter_index++)
                {
                    for (auto& kv : node_manager->nda_map)
                    {
                        _CheckDeadline(kv.second->node[nid]->GetUserDataByHashIndex(
                                           hash_index, filter_index),
                            deadline);
                    }
                }
            }
        }
    }
}

void
collection::SwitchGear::_CheckDeadline(lib::Data* data, uint32_t deadline)
{
    lib::LatencyData* lat_data = static_cast<lib::LatencyData*>(data);
    int32_t threshold {-1 * static_cast<int32_t>(deadline)};

    if (false == lat_data->access)
    {
        return;
    }

    if (lib::TimeLogState::IDLE == lat_data->start_state)
    {
        lat_data->start_deadline--;
        if (0 >= lat_data->start_deadline)
        {
            lat_data->start_token = lat_data->start_token_size;
            lat_data->start_state = lib::TimeLogState::RUN;
        }
    }
    else if (lib::TimeLogState::RUN == lat_data->start_state)
    {
        lat_data->start_deadline--;
        if (threshold > lat_data->start_deadline)
        {
            lat_data->start_state = lib::TimeLogState::STOP;
        }
    }

    if (lib::TimeLogState::IDLE == lat_data->end_state)
    {
        lat_data->end_deadline--;
        if (0 >= lat_data->end_deadline)
        {
            lat_data->end_token = lat_data->end_token_size;
            lat_data->end_state = lib::TimeLogState::RUN;
        }
    }
    else if (lib::TimeLogState::RUN == lat_data->end_state)
    {
        lat_data->end_deadline--;
        if (threshold > lat_data->end_deadline)
        {
            lat_data->end_state = lib::TimeLogState::STOP;
        }
    }
}
