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

#include "src/process/processor/CountProcessor.h"

#include <string>

#include "src/config/ConfigInterface.h"
#include "src/lib/StringView.h"
#include "src/lib/json/Json.h"

void
process::CountProcessor::_ProcessData(lib::Data* air_data, lib::AccData* acc_data)
{
    lib::CountData* air_count_data {static_cast<lib::CountData*>(air_data)};
    lib::AccCountData* acc_count_data {static_cast<lib::AccCountData*>(acc_data)};

    if (air_count_data->period_count_positive >
        air_count_data->period_count_negative)
    {
        air_count_data->period_count = air_count_data->period_count_positive -
            air_count_data->period_count_negative;
        air_count_data->period_negative = 0;
    }
    else
    {
        air_count_data->period_count = air_count_data->period_count_negative -
            air_count_data->period_count_positive;
        air_count_data->period_negative = 1;
    }

    acc_count_data->cumulation_num_req_positive +=
        air_count_data->period_num_req_positive;
    acc_count_data->cumulation_num_req_negative +=
        air_count_data->period_num_req_negative;

    if (air_count_data->period_negative == acc_count_data->cumulation_negative)
    {
        acc_count_data->cumulation_count += air_count_data->period_count;
    }
    else
    {
        if (air_count_data->period_count > acc_count_data->cumulation_count)
        {
            acc_count_data->cumulation_negative = air_count_data->period_negative;
            acc_count_data->cumulation_count =
                air_count_data->period_count - acc_count_data->cumulation_count;
        }
        else
        {
            acc_count_data->cumulation_count -= air_count_data->period_count;
        }
    }
}

void
process::CountProcessor::_JsonifyData(struct JsonifyData data)
{
    std::string node_name;
    lib::CountData* air_count_data {static_cast<lib::CountData*>(data.air_data)};

    node_name.assign(data.node_name_view.data(), data.node_name_view.size());
    std::string node_obj_name {node_name + "_" + std::to_string(data.tid) +
        "_count_" + std::to_string(data.hash_value) + "_" +
        std::to_string(data.filter_index)};

    lib::AccCountData* acc_count_data {
        static_cast<lib::AccCountData*>(data.acc_data)};

    auto& node_obj = air::json(node_obj_name);

    std::string filter_item {
        cfg::GetItemStrWithNodeName(data.node_name_view, data.filter_index)};

    node_obj["target_id"] = {data.tid};
    node_obj["target_name"] = {data.tname};
    node_obj["index"] = {data.hash_value};
    node_obj["filter"] = {filter_item};

    auto& node_obj_period = air::json(node_obj_name + "_period");
    if (0 == air_count_data->period_negative)
    {
        node_obj_period["count"] = {air_count_data->period_count};
    }
    else
    {
        node_obj_period["count"] = {((int64_t)(air_count_data->period_count)) * -1};
    }
    node_obj_period["count_plus"] = {air_count_data->period_count_positive};
    node_obj_period["count_minus"] = {air_count_data->period_count_negative};
    node_obj_period["num_req_plus"] = {air_count_data->period_num_req_positive};
    node_obj_period["num_req_minus"] = {air_count_data->period_num_req_negative};
    node_obj_period["negative"] = {air_count_data->period_negative};
    node_obj["period"] = {node_obj_period};

    auto& node_obj_cumulation = air::json(node_obj_name + "_cumulation");
    if (0 == acc_count_data->cumulation_negative)
    {
        node_obj_cumulation["count"] = {acc_count_data->cumulation_count};
    }
    else
    {
        node_obj_cumulation["count"] = {
            ((int64_t)(acc_count_data->cumulation_count)) * -1};
    }
    node_obj_cumulation["num_req_plus"] = {
        acc_count_data->cumulation_num_req_positive};
    node_obj_cumulation["num_req_minus"] = {
        acc_count_data->cumulation_num_req_negative};
    node_obj_cumulation["negative"] = {acc_count_data->cumulation_negative};
    node_obj["cumulation"] = {node_obj_cumulation};

    auto& node = air::json(node_name);
    node["objs"] += {node_obj};
}

void
process::CountProcessor::_InitData(lib::Data* air_data, lib::AccData* acc_data)
{
    lib::AccCountData* acc_count_data {static_cast<lib::AccCountData*>(acc_data)};
    lib::CountData* air_count_data {static_cast<lib::CountData*>(air_data)};

    air_count_data->access = 0;
    air_count_data->period_count_positive = 0;
    air_count_data->period_count_negative = 0;
    air_count_data->period_num_req_positive = 0;
    air_count_data->period_num_req_negative = 0;
    air_count_data->period_count = 0;
    air_count_data->period_negative = 0;

    if (0 != acc_count_data->need_erase)
    {
        acc_count_data->cumulation_count = 0;
        acc_count_data->cumulation_num_req_positive = 0;
        acc_count_data->cumulation_num_req_negative = 0;
        acc_count_data->cumulation_negative = 0;
        acc_count_data->need_erase = 0;
    }
}
