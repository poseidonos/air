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

#include "config_interface_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>

TEST_F(ConfigInterfaceTest, GetSentenceName)
{
    EXPECT_EQ(
        0, cfg::GetSentenceName(config::ParagraphType::DEFAULT, 0).compare(""));
    EXPECT_EQ(0,
        cfg::GetSentenceName(config::ParagraphType::GROUP, 3).compare("POS_META"));
    EXPECT_EQ(
        0, cfg::GetSentenceName(config::ParagraphType::FILTER, 1).compare("Range"));
    EXPECT_EQ(0,
        cfg::GetSentenceName(config::ParagraphType::BUCKET, 0).compare("BUCKET_1"));
    EXPECT_EQ(0,
        cfg::GetSentenceName(config::ParagraphType::BUCKET, 5).compare("BUCKET_6"));
    EXPECT_EQ(0,
        cfg::GetSentenceName(config::ParagraphType::NODE, 0).compare("PERF_PSD"));
    EXPECT_EQ(0,
        cfg::GetSentenceName(config::ParagraphType::NODE, 1)
            .compare("PERF_VOLUME"));
}

TEST_F(ConfigInterfaceTest, GetSentenceCount)
{
    EXPECT_EQ(1, cfg::GetSentenceCount(config::ParagraphType::DEFAULT));
    EXPECT_EQ(5, cfg::GetSentenceCount(config::ParagraphType::GROUP));
    EXPECT_EQ(3, cfg::GetSentenceCount(config::ParagraphType::FILTER));
    EXPECT_EQ(6, cfg::GetSentenceCount(config::ParagraphType::BUCKET));
    EXPECT_EQ(23, cfg::GetSentenceCount(config::ParagraphType::NODE));
}

TEST_F(ConfigInterfaceTest, GetSentenceIndex)
{
    EXPECT_EQ(2, cfg::GetSentenceIndex(config::ParagraphType::GROUP, "POS_IO"));
    EXPECT_EQ(0, cfg::GetSentenceIndex(config::ParagraphType::FILTER, "Basic"));
    EXPECT_EQ(0, cfg::GetSentenceIndex(config::ParagraphType::BUCKET, "BUCKET_1"));
    EXPECT_EQ(5, cfg::GetSentenceIndex(config::ParagraphType::BUCKET, "BUCKET_6"));
    EXPECT_EQ(0, cfg::GetSentenceIndex(config::ParagraphType::NODE, "PERF_PSD"));
    EXPECT_EQ(4, cfg::GetSentenceIndex(config::ParagraphType::NODE, "PERF_CP"));
    EXPECT_EQ(8, cfg::GetSentenceIndex(config::ParagraphType::NODE, "LAT_REBUILD"));
    EXPECT_EQ(
        13, cfg::GetSentenceIndex(config::ParagraphType::NODE, "Q_SCHEDULING"));
    EXPECT_EQ(-1, cfg::GetSentenceIndex(config::ParagraphType::NODE, "UNKNOWN"));
}

TEST_F(ConfigInterfaceTest, GetIntValue)
{
    EXPECT_EQ(
        1, cfg::GetIntValue(config::ParagraphType::DEFAULT, "StreamingInterval"));
    EXPECT_EQ(1000,
        cfg::GetIntValue(config::ParagraphType::DEFAULT, "NodeSamplingRatio"));
    EXPECT_EQ(
        32, cfg::GetIntValue(config::ParagraphType::DEFAULT, "NodeIndexSize"));
    EXPECT_EQ(1, cfg::GetIntValue(config::ParagraphType::DEFAULT, "AirBuild"));
    EXPECT_EQ(0, cfg::GetIntValue(config::ParagraphType::DEFAULT, "NodeBuild"));
    EXPECT_EQ(0, cfg::GetIntValue(config::ParagraphType::DEFAULT, "NodeRun"));

    EXPECT_EQ(
        0, cfg::GetIntValue(config::ParagraphType::GROUP, "NodeBuild", "POS_META"));
    EXPECT_EQ(
        0, cfg::GetIntValue(config::ParagraphType::GROUP, "NodeBuild", "POS"));
    EXPECT_EQ(0, cfg::GetIntValue(config::ParagraphType::GROUP, "NodeRun", "POS"));
    EXPECT_EQ(
        32, cfg::GetIntValue(config::ParagraphType::GROUP, "NodeIndexSize", "POS"));
    EXPECT_EQ(100,
        cfg::GetIntValue(
            config::ParagraphType::GROUP, "NodeIndexSize", "POS_JOURNAL"));

    EXPECT_EQ(
        10, cfg::GetIntValue(config::ParagraphType::BUCKET, "Scale", "BUCKET_1"));
    EXPECT_EQ(
        3, cfg::GetIntValue(config::ParagraphType::BUCKET, "Scale", "BUCKET_2"));
    EXPECT_EQ(
        20, cfg::GetIntValue(config::ParagraphType::BUCKET, "Scale", "BUCKET_3"));
    EXPECT_EQ(
        2, cfg::GetIntValue(config::ParagraphType::BUCKET, "Scale", "BUCKET_4"));
    EXPECT_EQ(
        2, cfg::GetIntValue(config::ParagraphType::BUCKET, "Scale", "BUCKET_5"));
    EXPECT_EQ(
        10, cfg::GetIntValue(config::ParagraphType::BUCKET, "Scale", "BUCKET_6"));

    EXPECT_EQ(
        0, cfg::GetIntValue(config::ParagraphType::NODE, "Build", "PERF_PSD"));
    EXPECT_EQ(0, cfg::GetIntValue(config::ParagraphType::NODE, "Run", "PERF_PSD"));
    EXPECT_EQ(
        1, cfg::GetIntValue(config::ParagraphType::NODE, "Build", "PERF_VOLUME"));
    EXPECT_EQ(
        0, cfg::GetIntValue(config::ParagraphType::NODE, "Run", "PERF_VOLUME"));
    EXPECT_EQ(
        1, cfg::GetIntValue(config::ParagraphType::NODE, "Build", "PERF_METAFS"));
    EXPECT_EQ(
        0, cfg::GetIntValue(config::ParagraphType::NODE, "Run", "PERF_METAFS"));
    EXPECT_EQ(
        0, cfg::GetIntValue(config::ParagraphType::NODE, "Build", "PERF_REBUILD"));
    EXPECT_EQ(
        0, cfg::GetIntValue(config::ParagraphType::NODE, "Run", "PERF_REBUILD"));
    EXPECT_EQ(32,
        cfg::GetIntValue(config::ParagraphType::NODE, "IndexSize", "PERF_REBUILD"));
    EXPECT_EQ(
        100, cfg::GetIntValue(config::ParagraphType::NODE, "IndexSize", "PERF_CP"));
    EXPECT_EQ(0, cfg::GetIntValue(config::ParagraphType::NODE, "Build", "LAT_PSD"));
    EXPECT_EQ(100,
        cfg::GetIntValue(config::ParagraphType::NODE, "SamplingRatio", "LAT_PSD"));
    EXPECT_EQ(
        77, cfg::GetIntValue(config::ParagraphType::NODE, "IndexSize", "LAT_PSD"));
    EXPECT_EQ(1000,
        cfg::GetIntValue(
            config::ParagraphType::NODE, "SamplingRatio", "LAT_SUBMIT"));
}

TEST_F(ConfigInterfaceTest, GetIntValue_Filter)
{
    EXPECT_EQ(
        0, cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "One", "ONE"));
    EXPECT_EQ(
        -1, cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "One", "TWO"));
    EXPECT_EQ(
        -1, cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "One", "ONEE"));

    EXPECT_EQ(0,
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "Basic", "BI_0"));
    EXPECT_EQ(1,
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "Basic", "BI_1"));
    EXPECT_EQ(2,
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "Basic", "BI_2"));
    EXPECT_EQ(-1,
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "Basic", "BI_3"));
    EXPECT_EQ(-1,
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "Basic", "XX_1"));

    EXPECT_EQ(0,
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "Range", "AIR_0"));
    EXPECT_EQ(3,
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "Range", "AIR_3"));
    EXPECT_EQ(7,
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "Range", "AIR_7"));
    EXPECT_EQ(10,
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "Range", "AIR_10"));
    EXPECT_EQ(-1,
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "Range", "AIR_11"));
    EXPECT_EQ(-1,
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "Range", "AIRR_3"));
    EXPECT_EQ(-1,
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "Range", "AOR_3"));
    EXPECT_EQ(-1,
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "Range", "AI_3"));
    EXPECT_EQ(-1,
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "Range", "AIR3"));
    EXPECT_EQ(
        -1, cfg::GetIntValue(config::ParagraphType::FILTER, "Item", "Range", "_3"));
}

TEST_F(ConfigInterfaceTest, GetStrValue)
{
    EXPECT_EQ(0,
        cfg::GetStrValue(config::ParagraphType::NODE, "Type", "PERF_PSD")
            .compare("PERFORMANCE"));
    EXPECT_EQ(0,
        cfg::GetStrValue(config::ParagraphType::NODE, "Bucket", "HIST_TEST_1")
            .compare("BUCKET_1"));
    EXPECT_EQ(0,
        cfg::GetStrValue(config::ParagraphType::NODE, "Bucket", "HIST_TEST_2")
            .compare("BUCKET_2"));
    EXPECT_EQ(0,
        cfg::GetStrValue(config::ParagraphType::NODE, "Bucket", "HIST_TEST_3")
            .compare("BUCKET_3"));
    EXPECT_EQ(0,
        cfg::GetStrValue(config::ParagraphType::NODE, "Bucket", "HIST_TEST_4")
            .compare("BUCKET_4"));
    EXPECT_EQ(0,
        cfg::GetStrValue(config::ParagraphType::NODE, "Bucket", "HIST_TEST_5")
            .compare("BUCKET_5"));
    EXPECT_EQ(0,
        cfg::GetStrValue(config::ParagraphType::NODE, "Bucket", "HIST_TEST_6")
            .compare("BUCKET_6"));
}

TEST_F(ConfigInterfaceTest, GetNodeType)
{
    EXPECT_EQ(air::ProcessorType::PERFORMANCE, cfg::GetNodeType("PERF_PSD"));
    EXPECT_EQ(air::ProcessorType::LATENCY, cfg::GetNodeType("LAT_PSD"));
    EXPECT_EQ(air::ProcessorType::QUEUE, cfg::GetNodeType("Q_REACTOR"));
    EXPECT_EQ(air::ProcessorType::UTILIZATION, cfg::GetNodeType("UTIL_PSD"));
    EXPECT_EQ(air::ProcessorType::COUNT, cfg::GetNodeType("CNT_PSD"));
    EXPECT_EQ(air::ProcessorType::HISTOGRAM, cfg::GetNodeType("HIST_TEST_1"));
}

TEST_F(ConfigInterfaceTest, GetItemStrWithFilterName)
{
    EXPECT_EQ(0, cfg::GetItemStrWithFilterName("Basic", 0).compare("BI_0"));
    EXPECT_EQ(0, cfg::GetItemStrWithFilterName("Basic", 1).compare("BI_1"));
    EXPECT_EQ(0, cfg::GetItemStrWithFilterName("Basic", 2).compare("BI_2"));
    EXPECT_EQ(0, cfg::GetItemStrWithFilterName("Range", 4).compare("AIR_4"));
    EXPECT_EQ(0, cfg::GetItemStrWithFilterName("Range", 8).compare("AIR_8"));
    EXPECT_EQ(0, cfg::GetItemStrWithFilterName("Range", 9).compare("AIR_9"));
}

TEST_F(ConfigInterfaceTest, GetItemSizeWithFilterName)
{
    EXPECT_EQ(3, cfg::GetItemSizeWithFilterName("Basic"));
    EXPECT_EQ(11, cfg::GetItemSizeWithFilterName("Range"));
}

TEST_F(ConfigInterfaceTest, GetLowerBoundWithBucketName)
{
    EXPECT_EQ(0, cfg::GetLowerBoundWithBucketName("BUCKET_1"));
    EXPECT_EQ(33, cfg::GetLowerBoundWithBucketName("BUCKET_2"));
    EXPECT_EQ(-100, cfg::GetLowerBoundWithBucketName("BUCKET_3"));
    EXPECT_EQ(1, cfg::GetLowerBoundWithBucketName("BUCKET_4"));
    EXPECT_EQ(-4096, cfg::GetLowerBoundWithBucketName("BUCKET_5"));
    EXPECT_EQ(-1000, cfg::GetLowerBoundWithBucketName("BUCKET_6"));
}

TEST_F(ConfigInterfaceTest, GetUpperBoundWithBucketName)
{
    EXPECT_EQ(100, cfg::GetUpperBoundWithBucketName("BUCKET_1"));
    EXPECT_EQ(66, cfg::GetUpperBoundWithBucketName("BUCKET_2"));
    EXPECT_EQ(80, cfg::GetUpperBoundWithBucketName("BUCKET_3"));
    EXPECT_EQ(1024, cfg::GetUpperBoundWithBucketName("BUCKET_4"));
    EXPECT_EQ(-16, cfg::GetUpperBoundWithBucketName("BUCKET_5"));
    EXPECT_EQ(100000, cfg::GetUpperBoundWithBucketName("BUCKET_6"));
}

TEST_F(ConfigInterfaceTest, IsLinearTypeWithBucketName)
{
    EXPECT_EQ(true, cfg::IsLinearTypeWithBucketName("BUCKET_1"));
    EXPECT_EQ(true, cfg::IsLinearTypeWithBucketName("BUCKET_2"));
    EXPECT_EQ(true, cfg::IsLinearTypeWithBucketName("BUCKET_3"));
    EXPECT_EQ(false, cfg::IsLinearTypeWithBucketName("BUCKET_4"));
    EXPECT_EQ(false, cfg::IsLinearTypeWithBucketName("BUCKET_5"));
    EXPECT_EQ(false, cfg::IsLinearTypeWithBucketName("BUCKET_6"));
}
