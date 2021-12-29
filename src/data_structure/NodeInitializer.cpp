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

#include "src/data_structure/NodeInitializer.h"

#include <ctime>
#include <iostream>
#include <string>

#include "src/config/ConfigInterface.h"

void
node::NodeInitializer::InitNodeData(uint32_t nid, air::ProcessorType type, NodeData* data)
{
    if (nullptr == data)
    {
        return;
    }

    switch (type)
    {
        case (air::ProcessorType::HISTOGRAM):
            _InitHistogramData(nid, data);
            break;
        default:
            break;
    }
}

void
node::NodeInitializer::_InitHistogramData(uint32_t nid, NodeData* data)
{
    air::string_view node_name {cfg::GetSentenceName(config::ParagraphType::NODE, nid)};
    air::string_view bucket_name {cfg::GetStrValue(config::ParagraphType::NODE, "Bucket", node_name)};
    int64_t bucket_lower_bound {cfg::GetLowerBoundWithBucketName(bucket_name)};
    int64_t bucket_upper_bound {cfg::GetUpperBoundWithBucketName(bucket_name)};
    bool bucket_linear_type {cfg::IsLinearTypeWithBucketName(bucket_name)};
    int32_t bucket_scale {cfg::GetIntValue(config::ParagraphType::BUCKET, "Scale", bucket_name)};

    uint32_t hash_size {data->GetIndexSize()};
    uint32_t filter_size {data->GetFilterSize()};

    time_t curr_time {time(NULL)};

    for (uint32_t hash_index = 0; hash_index < hash_size; hash_index++)
    {
        for (uint32_t filter_index = 0; filter_index < filter_size; filter_index++)
        {
            lib::HistogramData* user_data =
                static_cast<lib::HistogramData*>(data->GetUserDataByHashIndex(
                    hash_index, filter_index));
            lib::HistogramData* air_data =
                static_cast<lib::HistogramData*>(data->GetAirData(
                    hash_index, filter_index));
            if (bucket_linear_type)
            {
                _InitHistogramLinearType(user_data, bucket_lower_bound,
                    bucket_upper_bound, bucket_scale);
                _InitHistogramLinearType(air_data, bucket_lower_bound,
                    bucket_upper_bound, bucket_scale);
            }
            else
            {
                _InitHistogramExponentialType(user_data, bucket_lower_bound,
                    bucket_upper_bound, bucket_scale);
                _InitHistogramExponentialType(air_data, bucket_lower_bound,
                    bucket_upper_bound, bucket_scale);
            }
            lib::AccHistogramData* acc_data =
                static_cast<lib::AccHistogramData*>(data->GetAccData(
                    hash_index, filter_index));
            acc_data->since = curr_time;
        }
    }
}

void
node::NodeInitializer::_InitHistogramLinearType(lib::HistogramData* data,
    int64_t lower_bound, int64_t upper_bound, int32_t scale)
{
    if (0 == scale)
    {
        std::cout << "bucket_scale is zero\n";
        scale = 2;
    }
    data->bucket_type = lib::BucketType::LINEAR;
    data->bucket_scale = scale;
    data->bucket_lower_bound = lower_bound;
    data->bucket_upper_bound = upper_bound - 1;
    data->bucket_size = (upper_bound - lower_bound) / scale;
    if (20 < data->bucket_size)
    {
        std::cout << "BucketSize is bigger than 20\n";
        data->bucket_size = 20;
    }
    for (uint64_t bucket_index = 0; bucket_index < data->bucket_size; bucket_index++)
    {
        data->bucket_name[bucket_index] = std::to_string(lower_bound + ((int64_t)bucket_index * scale));
    }
}

void
node::NodeInitializer::_InitHistogramExponentialType(lib::HistogramData* data,
    int64_t lower_bound, int64_t upper_bound, int32_t scale)
{
    if (0 == scale)
    {
        std::cout << "bucket_scale is zero\n";
        scale = 2;
    }
    int32_t lower_exponent {0};
    int32_t upper_exponent {0};
    data->bucket_type = lib::BucketType::EXPONENTIAL;
    data->bucket_scale = scale;
    if (0 > upper_bound)
    {
        data->bucket_lower_bound = lower_bound + 1;
        data->bucket_upper_bound = upper_bound;
        while (-1 > lower_bound)
        {
            lower_bound /= scale;
            lower_exponent--;
        }
        while (-1 > upper_bound)
        {
            upper_bound /= scale;
            upper_exponent--;
        }
        data->bucket_size = upper_exponent - lower_exponent;
        data->bucket_zero_index = -1 * lower_exponent;

        int32_t curr_exponent {-1 * lower_exponent};
        for (uint64_t bucket_index = 0; bucket_index < data->bucket_size; bucket_index++)
        {
            data->bucket_name[bucket_index] =
                "-" + std::to_string(scale) + "^" + std::to_string(curr_exponent);
            curr_exponent--;
        }
    }
    else if (0 < lower_bound)
    {
        data->bucket_lower_bound = lower_bound;
        data->bucket_upper_bound = upper_bound - 1;
        while (1 < lower_bound)
        {
            lower_bound /= scale;
            lower_exponent++;
        }
        while (1 < upper_bound)
        {
            upper_bound /= scale;
            upper_exponent++;
        }
        data->bucket_size = upper_exponent - lower_exponent;
        data->bucket_zero_index = (data->bucket_size - 1) - upper_exponent;

        int32_t curr_exponent {lower_exponent};
        for (uint64_t bucket_index = 0; bucket_index < data->bucket_size; bucket_index++)
        {
            data->bucket_name[bucket_index] =
                std::to_string(scale) + "^" + std::to_string(curr_exponent);
            curr_exponent++;
        }
    }
    else
    {
        data->bucket_lower_bound = lower_bound + 1;
        data->bucket_upper_bound = upper_bound - 1;
        while (-1 > lower_bound)
        {
            lower_bound /= scale;
            lower_exponent--;
        }
        while (1 < upper_bound)
        {
            upper_bound /= scale;
            upper_exponent++;
        }
        data->bucket_size = upper_exponent - lower_exponent + 1;
        data->bucket_zero_index = data->bucket_size - upper_exponent - 1;

        int32_t curr_exponent {-1 * lower_exponent};
        uint64_t negative_unit_size = static_cast<uint64_t>(-1 * lower_exponent);
        for (uint64_t bucket_index = 0; bucket_index < negative_unit_size; bucket_index++)
        {
            data->bucket_name[bucket_index] =
                "-" + std::to_string(scale) + "^" + std::to_string(curr_exponent);
            curr_exponent--;
        }
        data->bucket_name[data->bucket_zero_index] = "0";
        curr_exponent = 0;
        for (uint64_t bucket_index = data->bucket_zero_index + 1; bucket_index < data->bucket_size; bucket_index++)
        {
            data->bucket_name[bucket_index] =
                std::to_string(scale) + "^" + std::to_string(curr_exponent);
            curr_exponent++;
        }
    }

    if (20 < data->bucket_size)
    {
        std::cout << "BucketSize is bigger than 20\n";
        data->bucket_size = 20;
    }
}
