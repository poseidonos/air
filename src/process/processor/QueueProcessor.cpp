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

#include "src/process/processor/QueueProcessor.h"

#include <string>

#include "src/config/ConfigInterface.h"
#include "src/lib/json/Json.h"

void
process::QueueProcessor::_ProcessData(lib::Data* air_data,
    lib::AccData* acc_data)
{
    lib::QueueData* air_queue_data {static_cast<lib::QueueData*>(air_data)};
    lib::AccQueueData* acc_queue_data {static_cast<lib::AccQueueData*>(acc_data)};

    if (air_queue_data->num_req > 0)
    {
        air_queue_data->depth_period_avg =
            (float)air_queue_data->sum_depth / air_queue_data->num_req;
    }
    if (acc_queue_data->depth_total_max < air_queue_data->depth_period_max)
    {
        acc_queue_data->depth_total_max = air_queue_data->depth_period_max;
    }

    uint32_t prev_total_num_req {acc_queue_data->total_num_req};
    acc_queue_data->total_num_req += air_queue_data->num_req;

    if (0 != acc_queue_data->total_num_req)
    {
        acc_queue_data->depth_total_avg =
            (acc_queue_data->depth_total_avg / acc_queue_data->total_num_req) * prev_total_num_req +
            (air_queue_data->depth_period_avg / acc_queue_data->total_num_req) * air_queue_data->num_req;
    }
}

void
process::QueueProcessor::_JsonifyData(struct JsonifyData data)
{
    lib::AccQueueData* acc_queue_data {static_cast<lib::AccQueueData*>(data.acc_data)};
    std::string node_name;
    node_name.assign(data.node_name_view.data(), data.node_name_view.size());
    auto& node = air::json(node_name);
    lib::QueueData* air_queue_data {static_cast<lib::QueueData*>(data.air_data)};

    auto& node_obj = air::json(node_name + "_" + std::to_string(data.tid) + "_queue_"
        + std::to_string(data.hash_value) + "_" + std::to_string(data.filter_index));

    std::string filter_item {cfg::GetItemStrWithNodeName(data.node_name_view, data.filter_index)};

    node_obj["filter"] = {filter_item};
    node_obj["target_id"] = {data.tid};
    node_obj["index"] = {data.hash_value};
    node_obj["target_name"] = {data.tname};

    node_obj["num_req"] = {air_queue_data->num_req};
    node_obj["depth_period_avg"] = {air_queue_data->depth_period_avg};
    node_obj["depth_period_max"] = {air_queue_data->depth_period_max};

    node_obj["depth_total_avg"] = {acc_queue_data->depth_total_avg};
    node_obj["depth_total_max"] = {acc_queue_data->depth_total_max};

    node["objs"] += {node_obj};
}

void
process::QueueProcessor::_InitData(lib::Data* air_data,
    lib::AccData* acc_data)
{
    lib::AccQueueData* acc_queue_data {static_cast<lib::AccQueueData*>(acc_data)};
    lib::QueueData* air_queue_data {static_cast<lib::QueueData*>(air_data)};

    air_queue_data->access = 0;
    air_queue_data->num_req = 0;
    air_queue_data->sum_depth = 0;
    air_queue_data->depth_period_avg = 0.0;
    air_queue_data->depth_period_max = 0;

    if (0 != acc_queue_data->need_erase)
    {
        acc_queue_data->depth_total_max = 0;
        acc_queue_data->total_num_req = 0;
        acc_queue_data->depth_total_avg = 0.0;
        acc_queue_data->need_erase = 0;
    }
}
