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

#include "src/process/processor/PerformanceProcessor.h"

#include <string>

#include "src/config/ConfigInterface.h"
#include "src/lib/json/Json.h"

void
process::PerformanceProcessor::_ProcessData(
    lib::Data* air_data, lib::AccData* acc_data)
{
    lib::PerformanceData* perf_data {static_cast<lib::PerformanceData*>(air_data)};
    lib::AccPerformanceData* acc_perf_data {
        static_cast<lib::AccPerformanceData*>(acc_data)};

    if (lap_time > 0.0)
    {
        perf_data->period_iops = (double)perf_data->period_iops / lap_time;
        perf_data->period_bandwidth =
            (double)perf_data->period_bandwidth / lap_time;

        double common_divisor {acc_perf_data->duration_second + lap_time};

        acc_perf_data->cumulation_iops =
            ((double)acc_perf_data->cumulation_iops / common_divisor *
                acc_perf_data->duration_second) +
            ((double)perf_data->period_iops / common_divisor * lap_time);

        acc_perf_data->cumulation_bandwidth =
            ((double)acc_perf_data->cumulation_bandwidth / common_divisor *
                acc_perf_data->duration_second) +
            ((double)perf_data->period_bandwidth / common_divisor * lap_time);
    }
    acc_perf_data->duration_second += lap_time;
}

void
process::PerformanceProcessor::_JsonifyData(struct JsonifyData data)
{
    std::string node_name;
    lib::PerformanceData* perf_data {
        static_cast<lib::PerformanceData*>(data.air_data)};

    node_name.assign(data.node_name_view.data(), data.node_name_view.size());
    std::string node_obj_name {node_name + "_" + std::to_string(data.tid) +
        "_perf_" + std::to_string(data.hash_value) + "_" +
        std::to_string(data.filter_index)};

    auto& node_obj = air::json(node_obj_name);
    node_obj["target_name"] = {data.tname};
    node_obj["index"] = {data.hash_value};
    node_obj["target_id"] = {data.tid};
    std::string filter_item {
        cfg::GetItemStrWithNodeName(data.node_name_view, data.filter_index)};
    node_obj["filter"] = {filter_item};

    auto& node_obj_period = air::json(node_obj_name + "_period");
    node_obj_period["iops"] = {perf_data->period_iops};
    node_obj_period["bw"] = {perf_data->period_bandwidth};
    for (const auto& pair : perf_data->period_packet_cnt)
    {
        auto& node_obj_period_cnt =
            air::json(node_obj_name + "_period_cnt_" + std::to_string(pair.first));
        node_obj_period_cnt[std::to_string(pair.first)] = {pair.second};
        node_obj_period["count"] += {node_obj_period_cnt};
    }
    node_obj["period"] = {node_obj_period};

    auto& node_obj_cumulation = air::json(node_obj_name + "_cumulation");
    lib::AccPerformanceData* acc_perf_data {
        static_cast<lib::AccPerformanceData*>(data.acc_data)};
    node_obj_cumulation["iops"] = {acc_perf_data->cumulation_iops};
    node_obj_cumulation["bw"] = {acc_perf_data->cumulation_bandwidth};
    node_obj["cumulation"] = {node_obj_cumulation};

    auto& node = air::json(node_name);
    node["objs"] += {node_obj};
}

void
process::PerformanceProcessor::_InitData(
    lib::Data* air_data, lib::AccData* acc_data)
{
    lib::AccPerformanceData* acc_perf_data {
        static_cast<lib::AccPerformanceData*>(acc_data)};
    lib::PerformanceData* perf_data {static_cast<lib::PerformanceData*>(air_data)};

    perf_data->access = 0;
    perf_data->period_bandwidth = 0;
    perf_data->period_iops = 0;
    perf_data->period_packet_cnt.clear();

    if (0 != acc_perf_data->need_erase)
    {
        acc_perf_data->cumulation_bandwidth = 0;
        acc_perf_data->cumulation_iops = 0;
        acc_perf_data->duration_second = 0.0;
        acc_perf_data->need_erase = 0;
        acc_perf_data->updated_count = 0;
    }
}
