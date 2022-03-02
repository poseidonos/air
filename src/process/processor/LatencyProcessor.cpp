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

#include "src/process/processor/LatencyProcessor.h"

#include <algorithm>
#include <cmath>
#include <string>

#include "src/config/ConfigInterface.h"
#include "src/lib/json/Json.h"

void
process::LatencyProcessor::_ProcessData(lib::Data* air_data, lib::AccData* acc_data)
{
    lib::AccLatencyData* acc_lat_data {static_cast<lib::AccLatencyData*>(acc_data)};
    _Calculate(acc_lat_data);
}

void
process::LatencyProcessor::_Calculate(lib::AccLatencyData* lat_data)
{
    if (1 > lat_data->period_sample_count)
    {
        return;
    }
    uint32_t sample_count {lat_data->period_sample_count};

    // calculate period statistics
    std::sort(lat_data->timelag, lat_data->timelag + sample_count);
    uint64_t sum_value {0};
    for (uint32_t sample_index {0}; sample_index < sample_count; sample_index++)
    {
        sum_value += lat_data->timelag[sample_index];
    }
    lat_data->period_mean = (uint32_t)(sum_value / sample_count);
    lat_data->period_median = lat_data->timelag[sample_count / 2];
    lat_data->period_lower_quartile = lat_data->timelag[sample_count / 4];
    lat_data->period_upper_quartile =
        lat_data->timelag[sample_count - (sample_count / 4)];

    // calculate total statistics
    uint64_t total_count {lat_data->cumulation_sample_count +
        (uint64_t)lat_data->period_sample_count};
    if (total_count >= OVERFLOW_THRESHOLD)
    {
        lat_data->overflow_warning = true;
        return;
    }
    if (0 < lat_data->cumulation_sample_count)
    {
        double divisor1 {(double)(lat_data->cumulation_sample_count) /
            (double)(lat_data->cumulation_sample_count +
                lat_data->period_sample_count)};
        double divisor2 {(double)(lat_data->period_sample_count) /
            (double)(lat_data->cumulation_sample_count +
                lat_data->period_sample_count)};
        lat_data->cumulation_mean = (double)(lat_data->cumulation_mean * divisor1) +
            (double)(lat_data->period_mean * divisor2);
        lat_data->cumulation_median =
            (double)(lat_data->cumulation_median * divisor1) +
            (double)(lat_data->period_median * divisor2);
        lat_data->cumulation_lower_quartile =
            (double)(lat_data->cumulation_lower_quartile * divisor1) +
            (double)(lat_data->period_lower_quartile * divisor2);
        lat_data->cumulation_upper_quartile =
            (double)(lat_data->cumulation_upper_quartile * divisor1) +
            (double)(lat_data->period_upper_quartile * divisor2);
    }
    else
    {
        lat_data->cumulation_mean = lat_data->period_mean;
        lat_data->cumulation_median = lat_data->period_median;
        lat_data->cumulation_lower_quartile = lat_data->period_lower_quartile;
        lat_data->cumulation_upper_quartile = lat_data->period_upper_quartile;
    }

    lat_data->cumulation_sample_count = total_count;
    if (lat_data->period_max > lat_data->cumulation_max)
    {
        lat_data->cumulation_max = lat_data->period_max;
    }
    if ((0 != lat_data->period_min) &&
        ((0 == lat_data->cumulation_min) ||
            (lat_data->period_min < lat_data->cumulation_min)))
    {
        lat_data->cumulation_min = lat_data->period_min;
    }
}

void
process::LatencyProcessor::_JsonifyData(struct JsonifyData data)
{
    std::string node_name;
    node_name.assign(data.node_name_view.data(), data.node_name_view.size());
    std::string node_obj_name {node_name + "_" + std::to_string(data.tid) +
        "_lat_" + std::to_string(data.hash_value) + "_" +
        std::to_string(data.filter_index)};

    std::string filter_range {
        cfg::GetItemStrWithNodeName(data.node_name_view, data.filter_index)};
    filter_range += "~";
    filter_range +=
        cfg::GetItemStrWithNodeName(data.node_name_view, data.filter_index + 1);

    auto& node_obj = air::json(node_obj_name);
    node_obj["index"] = {data.hash_value};
    node_obj["filter"] = {filter_range};
    node_obj["target_id"] = {data.tid};
    node_obj["target_name"] = {data.tname};

    lib::AccLatencyData* acc_lat_data {
        static_cast<lib::AccLatencyData*>(data.acc_data)};

    auto& node_obj_period = air::json(node_obj_name + "_period");
    node_obj_period["mean"] = {acc_lat_data->period_mean};
    node_obj_period["min"] = {acc_lat_data->period_min};
    node_obj_period["max"] = {acc_lat_data->period_max};
    node_obj_period["median"] = {acc_lat_data->period_median};
    node_obj_period["low_qt"] = {acc_lat_data->period_lower_quartile};
    node_obj_period["up_qt"] = {acc_lat_data->period_upper_quartile};
    node_obj_period["sample_cnt"] = {acc_lat_data->period_sample_count};
    node_obj["period"] = {node_obj_period};

    auto& node_obj_cumulation = air::json(node_obj_name + "_cumulation");
    node_obj_cumulation["mean"] = {acc_lat_data->cumulation_mean};
    node_obj_cumulation["min"] = {acc_lat_data->cumulation_min};
    node_obj_cumulation["max"] = {acc_lat_data->cumulation_max};
    node_obj_cumulation["median"] = {acc_lat_data->cumulation_median};
    node_obj_cumulation["low_qt"] = {acc_lat_data->cumulation_lower_quartile};
    node_obj_cumulation["up_qt"] = {acc_lat_data->cumulation_upper_quartile};
    node_obj_cumulation["sample_cnt"] = {acc_lat_data->cumulation_sample_count};
    node_obj["cumulation"] = {node_obj_cumulation};

    auto& node = air::json(node_name);
    node["objs"] += {node_obj};
}

void
process::LatencyProcessor::_InitData(lib::Data* air_data, lib::AccData* acc_data)
{
    lib::AccLatencyData* acc_lat_data {static_cast<lib::AccLatencyData*>(acc_data)};

    acc_lat_data->period_mean = 0;
    acc_lat_data->period_min = 0;
    acc_lat_data->period_max = 0;
    acc_lat_data->period_median = 0;
    acc_lat_data->period_lower_quartile = 0;
    acc_lat_data->period_upper_quartile = 0;
    for (uint32_t i {0}; i < lib::TIMELAG_SIZE; i++)
    {
        acc_lat_data->timelag[i] = 0;
    }

    if ((true == acc_lat_data->overflow_warning) || (0 != acc_lat_data->need_erase))
    {
        acc_lat_data->cumulation_mean = 0;
        acc_lat_data->cumulation_min = 0;
        acc_lat_data->cumulation_max = 0;
        acc_lat_data->cumulation_median = 0;
        acc_lat_data->cumulation_lower_quartile = 0;
        acc_lat_data->cumulation_upper_quartile = 0;
        acc_lat_data->cumulation_sample_count = 0;
        acc_lat_data->overflow_warning = 0;
        acc_lat_data->need_erase = 0;
    }
}
