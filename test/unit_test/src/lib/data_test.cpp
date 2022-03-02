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

#include "data_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST_F(DataTest, QueueData)
{
    bool b {false};
    uint64_t value {0xFF00EE00DD00CA00};
    EXPECT_EQ(b, queue_data->access);
    EXPECT_EQ(value, queue_data->period_qd_sum);
    EXPECT_LT(3.2, queue_data->period_qd_avg);
}

TEST_F(DataTest, LatencyData)
{
    uint32_t value {0};
    EXPECT_EQ(value, lat_data->period_min);

    value = 0x0000FEAD;
    EXPECT_EQ(value, lat_data->period_max);
}

TEST_F(DataTest, PerformanceData)
{
    EXPECT_EQ(false, perf_data->access);
    EXPECT_EQ(100, perf_data->period_iops);
}

TEST_F(DataTest, HistogramData)
{
    EXPECT_EQ(0, histogram_data->period_underflow);
    EXPECT_EQ(0, histogram_data->period_overflow);
    EXPECT_EQ(0, histogram_data->period_min_value);
    EXPECT_EQ(0, histogram_data->period_max_value);
    EXPECT_EQ(0, histogram_data->period_avg_value);
    for (int i = 0; i < 20; i++)
    {
        EXPECT_EQ(0, histogram_data->period_bucket[i]);
    }

    EXPECT_EQ(0, histogram_data->bucket_lower_bound);
    EXPECT_EQ(0, histogram_data->bucket_upper_bound);
    EXPECT_EQ(0, histogram_data->bucket_scale);
    EXPECT_EQ(0, histogram_data->bucket_size);
    EXPECT_EQ(lib::BucketType::LINEAR, histogram_data->bucket_type);
    EXPECT_EQ(0, histogram_data->bucket_zero_index);
    for (int i = 0; i < 20; i++)
    {
        EXPECT_EQ("", histogram_data->bucket_name[i]);
    }
}

TEST_F(DataTest, AccHistogramData)
{
    EXPECT_EQ(0, acc_histogram_data->cumulation_underflow);
    EXPECT_EQ(0, acc_histogram_data->cumulation_overflow);
    EXPECT_EQ(0, acc_histogram_data->cumulation_min_value);
    EXPECT_EQ(0, acc_histogram_data->cumulation_max_value);
    EXPECT_EQ(0, acc_histogram_data->cumulation_avg_value);
    for (int i = 0; i < 20; i++)
    {
        EXPECT_EQ(0, acc_histogram_data->cumulation_bucket[i]);
    }
    EXPECT_EQ(0, acc_histogram_data->since);
}
