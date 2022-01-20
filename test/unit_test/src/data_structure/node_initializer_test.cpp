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

#include "node_initializer_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST_F(NodeInitializerTest, InitNodeData_NullData)
{
    node_initializer.InitNodeData(
        0, air::ProcessorType::PROCESSORTYPE_NULL, nullptr);
}

TEST_F(NodeInitializerTest, InitNodeData_HistogramType_LinearDataSample1)
{
    // Node: HIST_TEST_1, Filter: One, Type: Histogram, Group: POS, IndexSize: 1,
    // Bucket: BUCKET_1 Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10
    node_initializer.InitNodeData(
        17, air::ProcessorType::HISTOGRAM, &histogram_node_data);

    lib::HistogramData* user_data = static_cast<lib::HistogramData*>(
        histogram_node_data.GetUserDataByHashIndex(0, 0));

    EXPECT_EQ(lib::BucketType::LINEAR, user_data->bucket_type);
    EXPECT_EQ(10, user_data->bucket_scale);
    EXPECT_EQ(0, user_data->bucket_lower_bound);
    EXPECT_EQ(99, user_data->bucket_upper_bound);
    EXPECT_EQ(10, user_data->bucket_size);
    EXPECT_EQ("0", user_data->bucket_name[0]);
    EXPECT_EQ("90", user_data->bucket_name[9]);
}

TEST_F(NodeInitializerTest, InitNodeData_HistogramType_LinearDataSample2)
{
    // Node: HIST_TEST_2, Filter: One, Type: Histogram, Group: POS, IndexSize: 1,
    // Bucket: BUCKET_2 Scale: 3, Bucket: BUCKET_2, Bounds: [33, 66)
    node_initializer.InitNodeData(
        18, air::ProcessorType::HISTOGRAM, &histogram_node_data);

    lib::HistogramData* user_data = static_cast<lib::HistogramData*>(
        histogram_node_data.GetUserDataByHashIndex(0, 0));

    EXPECT_EQ(lib::BucketType::LINEAR, user_data->bucket_type);
    EXPECT_EQ(3, user_data->bucket_scale);
    EXPECT_EQ(33, user_data->bucket_lower_bound);
    EXPECT_EQ(65, user_data->bucket_upper_bound);
    EXPECT_EQ(11, user_data->bucket_size);
    EXPECT_EQ("33", user_data->bucket_name[0]);
    EXPECT_EQ("63", user_data->bucket_name[10]);
}

TEST_F(NodeInitializerTest, InitNodeData_HistogramType_LinearDataSample3)
{
    // Node: HIST_TEST_3, Filter: One, Type: Histogram, Group: POS, IndexSize: 1,
    // Bucket: BUCKET_3 Bounds: [-100, 80), Scale: 20, Bucket: BUCKET_3
    node_initializer.InitNodeData(
        19, air::ProcessorType::HISTOGRAM, &histogram_node_data);

    lib::HistogramData* user_data = static_cast<lib::HistogramData*>(
        histogram_node_data.GetUserDataByHashIndex(0, 0));

    EXPECT_EQ(lib::BucketType::LINEAR, user_data->bucket_type);
    EXPECT_EQ(20, user_data->bucket_scale);
    EXPECT_EQ(-100, user_data->bucket_lower_bound);
    EXPECT_EQ(79, user_data->bucket_upper_bound);
    EXPECT_EQ(9, user_data->bucket_size);
    EXPECT_EQ("-100", user_data->bucket_name[0]);
    EXPECT_EQ("60", user_data->bucket_name[8]);
}

TEST_F(NodeInitializerTest, InitNodeData_HistogramType_ExponentialDataSample1)
{
    // Node: HIST_TEST_4, Filter: One, Type: Histogram, Group: POS, IndexSize: 1,
    // Bucket: BUCKET_4 Bucket: BUCKET_4, Bounds: [2^0, 2^10), Scale: 2^
    node_initializer.InitNodeData(
        20, air::ProcessorType::HISTOGRAM, &histogram_node_data);

    lib::HistogramData* user_data = static_cast<lib::HistogramData*>(
        histogram_node_data.GetUserDataByHashIndex(0, 0));

    EXPECT_EQ(lib::BucketType::EXPONENTIAL, user_data->bucket_type);
    EXPECT_EQ(2, user_data->bucket_scale);
    EXPECT_EQ(1, user_data->bucket_lower_bound);
    EXPECT_EQ(1023, user_data->bucket_upper_bound);
    EXPECT_EQ(10, user_data->bucket_size);
    EXPECT_EQ("2^0", user_data->bucket_name[0]);
    EXPECT_EQ("2^9", user_data->bucket_name[9]);
    EXPECT_EQ(-1, user_data->bucket_zero_index);
}

TEST_F(NodeInitializerTest, InitNodeData_HistogramType_ExponentialDataSample2)
{
    // Node: HIST_TEST_5, Filter: One, Type: Histogram, Group: POS, IndexSize: 1,
    // Bucket: BUCKET_5 Bucket: BUCKET_5, Bounds: (-4^6, -4^2], Scale: 2^
    node_initializer.InitNodeData(
        21, air::ProcessorType::HISTOGRAM, &histogram_node_data);

    lib::HistogramData* user_data = static_cast<lib::HistogramData*>(
        histogram_node_data.GetUserDataByHashIndex(0, 0));

    EXPECT_EQ(lib::BucketType::EXPONENTIAL, user_data->bucket_type);
    EXPECT_EQ(2, user_data->bucket_scale);
    EXPECT_EQ(-4095, user_data->bucket_lower_bound);
    EXPECT_EQ(-16, user_data->bucket_upper_bound);
    EXPECT_EQ(8, user_data->bucket_size);
    EXPECT_EQ("-2^12", user_data->bucket_name[0]);
    EXPECT_EQ("-2^5", user_data->bucket_name[7]);
    EXPECT_EQ(12, user_data->bucket_zero_index);
}

TEST_F(NodeInitializerTest, InitNodeData_HistogramType_ExponentialDataSample3)
{
    // Node: HIST_TEST_6, Filter: One, Type: Histogram, Group: POS, IndexSize: 1,
    // Bucket: BUCKET_6 Bucket: BUCKET_6, Bounds: (-10^3, 10^5), Scale: 10^
    node_initializer.InitNodeData(
        22, air::ProcessorType::HISTOGRAM, &histogram_node_data);

    lib::HistogramData* user_data = static_cast<lib::HistogramData*>(
        histogram_node_data.GetUserDataByHashIndex(0, 0));

    EXPECT_EQ(lib::BucketType::EXPONENTIAL, user_data->bucket_type);
    EXPECT_EQ(10, user_data->bucket_scale);
    EXPECT_EQ(-999, user_data->bucket_lower_bound);
    EXPECT_EQ(99999, user_data->bucket_upper_bound);
    EXPECT_EQ(9, user_data->bucket_size);
    EXPECT_EQ("-10^3", user_data->bucket_name[0]);
    EXPECT_EQ("-10^1", user_data->bucket_name[2]);
    EXPECT_EQ("0", user_data->bucket_name[3]);
    EXPECT_EQ("10^0", user_data->bucket_name[4]);
    EXPECT_EQ("10^4", user_data->bucket_name[8]);
    EXPECT_EQ(3, user_data->bucket_zero_index);
}
