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

#include "src/process/processor/UtilizationProcessor.h"

#include <string>

#include "src/config/ConfigInterface.h"
#include "src/lib/json/Json.h"

void
process::UtilizationProcessor::_ProcessData(
    lib::Data* air_data, lib::AccData* acc_data)
{
    lib::UtilizationData* air_util_data {
        static_cast<lib::UtilizationData*>(air_data)};
    lib::AccUtilizationData* acc_util_data {
        static_cast<lib::AccUtilizationData*>(acc_data)};

    acc_util_data->cumulation_usage += air_util_data->period_usage;
}

void
process::UtilizationProcessor::_JsonifyData(struct JsonifyData data)
{
    std::string node_name;
    lib::UtilizationData* air_util_data {
        static_cast<lib::UtilizationData*>(data.air_data)};

    node_name.assign(data.node_name_view.data(), data.node_name_view.size());
    std::string node_obj_name {node_name + "_" + std::to_string(data.tid) +
        "_util_" + std::to_string(data.hash_value) + "_" +
        std::to_string(data.filter_index)};
    auto& node_obj = air::json(node_obj_name);

    node_obj["target_id"] = {data.tid};
    node_obj["target_name"] = {data.tname};
    node_obj["index"] = {data.hash_value};

    std::string filter_item {
        cfg::GetItemStrWithNodeName(data.node_name_view, data.filter_index)};
    node_obj["filter"] = {filter_item};

    auto& node_obj_period = air::json(node_obj_name + "_period");
    node_obj_period["usage"] = {air_util_data->period_usage};
    node_obj["period"] = {node_obj_period};

    auto& node_obj_cumulation = air::json(node_obj_name + "_cumulation");
    lib::AccUtilizationData* acc_util_data {
        static_cast<lib::AccUtilizationData*>(data.acc_data)};
    node_obj_cumulation["usage"] = {acc_util_data->cumulation_usage};
    node_obj["cumulation"] = {node_obj_cumulation};

    auto& node = air::json(node_name);
    node["objs"] += {node_obj};
}

void
process::UtilizationProcessor::_InitData(
    lib::Data* air_data, lib::AccData* acc_data)
{
    lib::AccUtilizationData* acc_util_data {
        static_cast<lib::AccUtilizationData*>(acc_data)};
    lib::UtilizationData* air_util_data {
        static_cast<lib::UtilizationData*>(air_data)};

    air_util_data->access = 0;
    air_util_data->period_usage = 0;

    if (0 != acc_util_data->need_erase)
    {
        acc_util_data->cumulation_usage = 0;
        acc_util_data->need_erase = 0;
        acc_util_data->updated_count = 0;
    }
}
