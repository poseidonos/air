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

#include "src/process/processor/HistogramProcessor.h"

#include <ctime>
#include <string>

#include "src/config/ConfigInterface.h"
#include "src/lib/StringView.h"
#include "src/lib/json/Json.h"

double
process::HistogramProcessor::_GetExponentialTypeBucketStartValue(
    int32_t bucket_zero_index, int32_t bucket_index, int64_t bucket_scale)
{
    double value {0};
    if (bucket_zero_index > bucket_index)
    {
        int32_t exponent {bucket_zero_index - bucket_index};
        value = 1;
        while (0 < exponent)
        {
            value *= bucket_scale;
            exponent--;
        }
        value *= -1;
        value += 1;
    }
    else if (bucket_zero_index < bucket_index)
    {
        int32_t exponent {bucket_index - bucket_zero_index - 1};
        value = 1;
        while (0 < exponent)
        {
            value *= bucket_scale;
            exponent--;
        }
    }
    return value;
}

double
process::HistogramProcessor::_GetExponentialTypeBucketEndValue(
    int32_t bucket_zero_index, int32_t bucket_index, int64_t bucket_scale)
{
    double value {0};
    if (bucket_zero_index > bucket_index)
    {
        int32_t exponent {bucket_zero_index - bucket_index - 1};
        value = 1;
        while (0 < exponent)
        {
            value *= bucket_scale;
            exponent--;
        }
        value *= -1;
    }
    else if (bucket_zero_index < bucket_index)
    {
        int32_t exponent {bucket_index - bucket_zero_index};
        value = 1;
        while (0 < exponent)
        {
            value *= bucket_scale;
            exponent--;
        }
        value -= 1;
    }
    return value;
}

void
process::HistogramProcessor::_ProcessData(lib::Data* air_data, lib::AccData* acc_data)
{
    lib::AccHistogramData* acc_hist_data {static_cast<lib::AccHistogramData*>(acc_data)};
    lib::HistogramData* hist_data {static_cast<lib::HistogramData*>(air_data)};

    int64_t total_count {0};
    int64_t total_sum {0};
    if (lib::BucketType::LINEAR == hist_data->bucket_type)
    {
        for (uint64_t bucket_index {0}; bucket_index < hist_data->bucket_size; bucket_index++)
        {
            double bucket_mid_value {hist_data->bucket_lower_bound +
                (bucket_index * hist_data->bucket_scale + ((double)hist_data->bucket_scale / 2))};
            double bucket_sum {bucket_mid_value * hist_data->period_bucket[bucket_index]};
            total_sum += (int64_t)bucket_sum;
            total_count += hist_data->period_bucket[bucket_index];
        }
    }
    else if (lib::BucketType::EXPONENTIAL == hist_data->bucket_type)
    {
        for (uint64_t bucket_index {0}; bucket_index < hist_data->bucket_size; bucket_index++)
        {
            double bucket_start_value {_GetExponentialTypeBucketStartValue(hist_data->bucket_zero_index,
                (int32_t)bucket_index, hist_data->bucket_scale)};
            double bucket_end_value {_GetExponentialTypeBucketEndValue(hist_data->bucket_zero_index,
                (int32_t)bucket_index, hist_data->bucket_scale)};
            double bucket_mid_value {(bucket_start_value + bucket_end_value) / 2};
            double bucket_sum {bucket_mid_value * hist_data->period_bucket[bucket_index]};
            total_sum += (int64_t)bucket_sum;
            total_count += hist_data->period_bucket[bucket_index];
        }
    }
    if (0 == total_count)
    {
        hist_data->period_avg_value = 0;
    }
    else
    {
        hist_data->period_avg_value = total_sum / total_count;
    }

    acc_hist_data->cumulation_underflow += hist_data->period_underflow;
    acc_hist_data->cumulation_overflow += hist_data->period_overflow;
    for (uint64_t bucket_index {0}; bucket_index < hist_data->bucket_size; bucket_index++)
    {
        acc_hist_data->cumulation_bucket[bucket_index]
            += hist_data->period_bucket[bucket_index];
    }
    if (acc_hist_data->cumulation_min_value > hist_data->period_min_value)
    {
        acc_hist_data->cumulation_min_value = hist_data->period_min_value;
    }
    if (acc_hist_data->cumulation_max_value < hist_data->period_max_value)
    {
        acc_hist_data->cumulation_max_value = hist_data->period_max_value;
    }

    total_sum = 0;
    total_count = 0;
    if (lib::BucketType::LINEAR == hist_data->bucket_type)
    {
        for (uint64_t bucket_index {0}; bucket_index < hist_data->bucket_size; bucket_index++)
        {
            total_count += acc_hist_data->cumulation_bucket[bucket_index];
            double bucket_mid_value {(bucket_index * hist_data->bucket_scale +
                ((double)hist_data->bucket_scale / 2))};
            bucket_mid_value += hist_data->bucket_lower_bound;
            double bucket_sum {bucket_mid_value * acc_hist_data->cumulation_bucket[bucket_index]};
            total_sum += (int64_t)bucket_sum;
        }
    }
    else if (lib::BucketType::EXPONENTIAL == hist_data->bucket_type)
    {
        for (uint64_t bucket_index {0}; bucket_index < hist_data->bucket_size; bucket_index++)
        {
            total_count += acc_hist_data->cumulation_bucket[bucket_index];
            double bucket_end_value {_GetExponentialTypeBucketEndValue(hist_data->bucket_zero_index,
                (int32_t)bucket_index, hist_data->bucket_scale)};
            double bucket_start_value {_GetExponentialTypeBucketStartValue(hist_data->bucket_zero_index,
                (int32_t)bucket_index, hist_data->bucket_scale)};
            double bucket_mid_value {(bucket_start_value + bucket_end_value) / 2};
            double bucket_sum {bucket_mid_value * acc_hist_data->cumulation_bucket[bucket_index]};
            total_sum += (int64_t)bucket_sum;
        }
    }
    if (0 > total_count)
    {
        acc_hist_data->cumulation_avg_value = 0;
        acc_hist_data->need_erase = true;
    }
    else if (0 == total_count)
    {
        acc_hist_data->cumulation_avg_value = 0;
    }
    else
    {
        acc_hist_data->cumulation_avg_value = total_sum / total_count;
    }
}

void
process::HistogramProcessor::_JsonifyData(struct JsonifyData data)
{
    lib::HistogramData* air_hist_data {static_cast<lib::HistogramData*>(data.air_data)};
    lib::AccHistogramData* acc_hist_data {static_cast<lib::AccHistogramData*>(data.acc_data)};
    std::string node_name;
    node_name.assign(data.node_name_view.data(), data.node_name_view.size());
    std::string node_obj_name {node_name + "_" + std::to_string(data.tid) +
        "_histogram_" + std::to_string(data.hash_value) + "_" + std::to_string(data.filter_index)};
    std::string filter_item = cfg::GetItemStrWithNodeName(data.node_name_view, data.filter_index);

    auto& node = air::json(node_name);
    auto& node_obj = air::json(node_obj_name);

    node_obj["target_id"] = {data.tid};
    node_obj["target_name"] = {data.tname};
    node_obj["index"] = {data.hash_value};
    node_obj["filter"] = {filter_item};
    if (lib::BucketType::LINEAR == air_hist_data->bucket_type)
    {
        node_obj["bucket_type"] = {"linear"};
    }
    else
    {
        node_obj["bucket_type"] = {"exponential"};
    }
    node_obj["bucket_scale"] = {air_hist_data->bucket_scale};
    node_obj["bucket_size"] = {air_hist_data->bucket_size};
    node_obj["bucket_zero_index"] = {air_hist_data->bucket_zero_index};
    node_obj["bucket_lower_bound"] = {air_hist_data->bucket_lower_bound};
    node_obj["bucket_upper_bound"] = {air_hist_data->bucket_upper_bound};

    auto& node_obj_period = air::json(node_obj_name + "_period");
    node_obj_period["underflow_count"] = {air_hist_data->period_underflow};
    node_obj_period["overflow_count"] = {air_hist_data->period_overflow};
    node_obj_period["minimum_value"] = {air_hist_data->period_min_value};
    node_obj_period["maximum_value"] = {air_hist_data->period_max_value};
    node_obj_period["average_value"] = {air_hist_data->period_avg_value};
    for (uint64_t bucket_index {0}; bucket_index < air_hist_data->bucket_size; bucket_index++)
    {
        node_obj_period["buckets"] += {air_hist_data->period_bucket[bucket_index]};
    }
    node_obj["period"] = {node_obj_period};

    auto& node_obj_cumulation = air::json(node_obj_name + "_cumulation");
    node_obj_cumulation["since"] = {(int64_t)acc_hist_data->since};
    node_obj_cumulation["underflow_count"] = {acc_hist_data->cumulation_underflow};
    node_obj_cumulation["overflow_count"] = {acc_hist_data->cumulation_overflow};
    node_obj_cumulation["minimum_value"] = {acc_hist_data->cumulation_min_value};
    node_obj_cumulation["maximum_value"] = {acc_hist_data->cumulation_max_value};
    node_obj_cumulation["average_value"] = {acc_hist_data->cumulation_avg_value};
    for (uint64_t bucket_index {0}; bucket_index < air_hist_data->bucket_size; bucket_index++)
    {
        node_obj_cumulation["buckets"] += {acc_hist_data->cumulation_bucket[bucket_index]};
    }
    node_obj["cumulation"] = {node_obj_cumulation};

    node["objs"] += {node_obj};
}

void
process::HistogramProcessor::_InitData(lib::Data* air_data, lib::AccData* acc_data)
{
    lib::HistogramData* hist_data {static_cast<lib::HistogramData*>(air_data)};
    lib::AccHistogramData* acc_hist_data {static_cast<lib::AccHistogramData*>(acc_data)};

    hist_data->access = 0;
    hist_data->period_underflow = 0;
    hist_data->period_overflow = 0;
    hist_data->period_min_value = 0;
    hist_data->period_max_value = 0;
    hist_data->period_avg_value = 0;
    for (uint64_t bucket_index {0}; bucket_index < hist_data->bucket_size; bucket_index++)
    {
        hist_data->period_bucket[bucket_index] = 0;
    }

    if (0 != acc_hist_data->need_erase)
    {
        acc_hist_data->cumulation_underflow = 0;
        acc_hist_data->cumulation_overflow = 0;
        acc_hist_data->cumulation_min_value = 0;
        acc_hist_data->cumulation_max_value = 0;
        acc_hist_data->cumulation_avg_value = 0;
        for (uint64_t bucket_index {0}; bucket_index < hist_data->bucket_size; bucket_index++)
        {
            acc_hist_data->cumulation_bucket[bucket_index] = 0;
        }
        time(&(acc_hist_data->since));
    }
}
