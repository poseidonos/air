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

#include "src/process/processor/Processor.h"

#include <string.h>

#include "src/lib/Type.h"

void
process::Processor::StreamData(air::string_view& node_name_view, uint32_t tid,
    const char* tname, node::NodeData* node_data, air::ProcessorType ptype,
    double new_time, uint32_t index_size, uint32_t filter_size)
{
    if (nullptr == node_data)
    {
        return;
    }
    lap_time = new_time;

    lib::Data* air_data {nullptr};
    lib::AccData* acc_data {nullptr};
    uint64_t hash_value {0};
    for (uint32_t hash_index = 0; hash_index < index_size; hash_index++)
    {
        for (uint32_t filter_index = 0; filter_index < filter_size; filter_index++)
        {
            air_data = node_data->GetAirData(hash_index, filter_index);
            acc_data = node_data->GetAccData(hash_index, filter_index);
            if (nullptr != air_data && 0 != air_data->access && nullptr != acc_data)
            {
                hash_value = node_data->GetUserHashValue(hash_index);
                _ProcessData(air_data, acc_data);
                _JsonifyData({air_data, acc_data, node_name_view, tid, tname,
                    hash_value, filter_index});
                _InitData(air_data, acc_data);
            }
        }

        node_data->SwapBuffer(hash_index);
    }
}

void
process::Processor::StreamData(air::string_view& node_name_view,
    lib::AccLatencyData* data, uint64_t hash_value, uint32_t filter_index)
{
    if (nullptr == data)
    {
        return;
    }

    if (0 != data->period_sample_count || 0 != data->cumulation_sample_count)
    {
        _ProcessData(nullptr, data);
        _JsonifyData(
            {nullptr, data, node_name_view, 0, "", hash_value, filter_index});
        _InitData(nullptr, data);
    }
}
