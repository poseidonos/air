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

#include "config_parser_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST_F(ConfigParserTest, GetSentenceFromParagraph)
{
    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::DEFAULT)
            .compare(
                "StreamingInterval:1, FileWrite: On, AirBuild:True,\n"
                "         NodeBuild:False, NodeRun:Off, NodeSamplingRatio: 1000,\n"
                "         NodeIndexSize:32, RemainingFileCount: 3"));

    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::GROUP, 1)
            .compare("Group: POS_JOURNAL ,\nNodeBuild: True, NodeIndexSize: 100"));
    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::GROUP, 3)
            .compare("Group: POS_META, NodeBuild: False, NodeRun: On, "
                     "NodeSamplingRatio: 100"));
    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::GROUP, 5).compare(""));
    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::GROUP, 99)
            .compare(""));

    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::FILTER, 0)
            .compare("Filter: Basic, Item: (BI_0, BI_1, BI_2)"));
    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::FILTER, 1)
            .compare("Filter: Range, Item: (AIR_0 ... AIR_10)"));

    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::BUCKET, 0)
            .compare("Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10"));
    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::BUCKET, 1)
            .compare("Scale: 3, Bucket: BUCKET_2, Bounds: [33, 66)"));
    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::BUCKET, 2)
            .compare("Bounds: [-100, 80), Scale: 20, Bucket: BUCKET_3"));
    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::BUCKET, 3)
            .compare("Bucket: BUCKET_4, Bounds: [2^0, 2^10), Scale: 2^"));
    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::BUCKET, 4)
            .compare("Bucket: BUCKET_5, Bounds: (-4^6, -4^2], Scale: 2^"));
    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::BUCKET, 5)
            .compare("Bucket: BUCKET_6, Bounds: (-10^3, 10^5), Scale: 10^"));

    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::NODE, 1)
            .compare("Node: PERF_VOLUME, Type: PERFORMANCE, Build: True, Group: "
                     "POS, Filter: Basic"));
    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::NODE, 4)
            .compare("Node: PERF_CP, Type: PERFORMANCE, Build: True, Run: On, "
                     "Filter: Basic, Group: POS_JOURNAL"));
    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::NODE, 9)
            .compare("Node: LAT_CP, Type: LATENCY, Build: False, Run: Off, Group: "
                     "POS, Filter: Basic"));
    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::NODE, 10)
            .compare("Node: Q_SUBMIT, Type: QUEUE, Group: POS, Filter: Basic"));
    EXPECT_EQ(0,
        cfg->GetSentenceFromParagraph(config::ParagraphType::NODE, 13)
            .compare(
                "Node: Q_SCHEDULING, Filter: Basic\n, Type: QUEUE, Build   : True, "
                "Run : Off, SamplingRatio: 1000,\n          Group: POS"));
}

TEST_F(ConfigParserTest, GetIndexFromParagraph)
{
    EXPECT_EQ(0, cfg->GetIndexFromParagraph(config::ParagraphType::DEFAULT));

    EXPECT_EQ(0, cfg->GetIndexFromParagraph(config::ParagraphType::GROUP, "POS"));
    EXPECT_EQ(
        1, cfg->GetIndexFromParagraph(config::ParagraphType::GROUP, "POS_JOURNAL"));
    EXPECT_EQ(
        2, cfg->GetIndexFromParagraph(config::ParagraphType::GROUP, "POS_IO"));
    EXPECT_EQ(
        3, cfg->GetIndexFromParagraph(config::ParagraphType::GROUP, "POS_META"));
    EXPECT_EQ(
        4, cfg->GetIndexFromParagraph(config::ParagraphType::GROUP, "POS_RSC"));
    EXPECT_EQ(-1,
        cfg->GetIndexFromParagraph(config::ParagraphType::GROUP, "INVALIE_NAME"));
    EXPECT_EQ(
        -1, cfg->GetIndexFromParagraph(config::ParagraphType::GROUP, "PERF_PSD"));

    EXPECT_EQ(
        0, cfg->GetIndexFromParagraph(config::ParagraphType::FILTER, "Basic"));
    EXPECT_EQ(
        1, cfg->GetIndexFromParagraph(config::ParagraphType::FILTER, "Range"));
    EXPECT_EQ(
        -1, cfg->GetIndexFromParagraph(config::ParagraphType::FILTER, "Basiic"));

    EXPECT_EQ(
        0, cfg->GetIndexFromParagraph(config::ParagraphType::BUCKET, "BUCKET_1"));
    EXPECT_EQ(
        1, cfg->GetIndexFromParagraph(config::ParagraphType::BUCKET, "BUCKET_2"));
    EXPECT_EQ(
        2, cfg->GetIndexFromParagraph(config::ParagraphType::BUCKET, "BUCKET_3"));
    EXPECT_EQ(
        3, cfg->GetIndexFromParagraph(config::ParagraphType::BUCKET, "BUCKET_4"));
    EXPECT_EQ(
        4, cfg->GetIndexFromParagraph(config::ParagraphType::BUCKET, "BUCKET_5"));
    EXPECT_EQ(
        5, cfg->GetIndexFromParagraph(config::ParagraphType::BUCKET, "BUCKET_6"));
    EXPECT_EQ(
        -1, cfg->GetIndexFromParagraph(config::ParagraphType::BUCKET, "BUCKET_7"));

    EXPECT_EQ(
        0, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "PERF_PSD"));
    EXPECT_EQ(
        1, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "PERF_VOLUME"));
    EXPECT_EQ(
        2, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "PERF_METAFS"));
    EXPECT_EQ(
        4, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "PERF_CP"));
    EXPECT_EQ(
        6, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "LAT_SUBMIT"));
    EXPECT_EQ(
        8, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "LAT_REBUILD"));
    EXPECT_EQ(13,
        cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "Q_SCHEDULING"));
    EXPECT_EQ(-1,
        cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "INVALIE_NAME"));
    EXPECT_EQ(
        -1, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "POS_RSC"));
}

TEST_F(ConfigParserTest, GetIntValueFromSentence)
{
    // DEFAULT
    EXPECT_EQ(1,
        cfg->GetIntValueFromSentence(
            "StreamingInterval:1, AirBuild  : True, NodeBuild:True, NodeRun:On, "
            "NodeSamplingRatio: 1000, NodeIndexSize:32, FileWrite: On, "
            "RemainingFileCount: 3",
            "AirBuild"));
    EXPECT_EQ(1,
        cfg->GetIntValueFromSentence(
            "StreamingInterval:1, AirBuild  : True, NodeBuild:True, NodeRun:On, "
            "NodeSamplingRatio: 1000, NodeIndexSize:32, FileWrite: On, "
            "RemainingFileCount: 3",
            "NodeBuild"));
    EXPECT_EQ(0,
        cfg->GetIntValueFromSentence(
            "StreamingInterval:1, AirBuild  : True, NodeBuild:True, NodeRun:Off, "
            "NodeSamplingRatio: 1000, NodeIndexSize:32, FileWrite: On, "
            "RemainingFileCount: 3",
            "NodeRun"));
    EXPECT_EQ(3,
        cfg->GetIntValueFromSentence(
            "StreamingInterval:  3   , AirBuild  : True, NodeBuild:True, "
            "NodeRun:Off, NodeSamplingRatio: 1000, NodeIndexSize:32, FileWrite: "
            "On, RemainingFileCount: 3",
            "StreamingInterval"));
    EXPECT_EQ(1000,
        cfg->GetIntValueFromSentence(
            "StreamingInterval:1, AirBuild  : True, NodeBuild:True, NodeRun:off, "
            "NodeSamplingRatio: 1000, NodeIndexSize:32 ,FileWrite: On, "
            "RemainingFileCount: 3",
            "NodeSamplingRatio"));
    EXPECT_EQ(-1,
        cfg->GetIntValueFromSentence(
            "StreamingInterval:1, AirBuild  : True, NodeBuild:True, NodeRun:off, "
            "NodeSamplingRatio: 1000, NodeIndexSize:32, FileWrite: On, "
            "RemainingFileCount: 3",
            "Condition"));
    EXPECT_EQ(32,
        cfg->GetIntValueFromSentence(
            "StreamingInterval:1, AirBuild  : True, NodeBuild:True, NodeRun:Off, "
            "NodeSamplingRatio: 1000, NodeIndexSize:32, FileWrite: On, "
            "RemainingFileCount: 3",
            "NodeIndexSize"));
    EXPECT_EQ(1,
        cfg->GetIntValueFromSentence(
            "StreamingInterval:1, AirBuild  : True, NodeBuild:True, NodeRun:Off, "
            "NodeSamplingRatio: 1000, NodeIndexSize:32, FileWrite: On, "
            "RemainingFileCount: 3",
            "FileWrite"));
    EXPECT_EQ(7,
        cfg->GetIntValueFromSentence(
            "StreamingInterval:1, AirBuild  : True, NodeBuild:True, NodeRun:Off, "
            "NodeSamplingRatio: 1000, NodeIndexSize:32, FileWrite: On, "
            "RemainingFileCount: 7",
            "RemainingFileCount"));

    // GROUP
    EXPECT_EQ(0,
        cfg->GetIntValueFromSentence(
            "Group: POS_RSC, NodeBuild: False, NodeRun: On", "NodeBuild"));
    EXPECT_EQ(1,
        cfg->GetIntValueFromSentence(
            "Group: POS_RSC, NodeBuild: False, NodeRun: On", "NodeRun"));
    EXPECT_EQ(-1,
        cfg->GetIntValueFromSentence(
            "Group: POS_RSC, NodeBuild: False, NodeRun: On", "Condition"));

    // FILTER
    EXPECT_EQ(0,
        cfg->GetIntValueFromSentence(
            "Filter: F1, Item: (AIR_10 ... AIR_32)", "Item", "AIR_10"));
    EXPECT_EQ(9,
        cfg->GetIntValueFromSentence(
            "Filter: F1, Item: (AIR_10 ... AIR_32)", "Item", "AIR_19"));
    EXPECT_EQ(22,
        cfg->GetIntValueFromSentence(
            "Filter: F1, Item: (AIR_10 ... AIR_32)", "Item", "AIR_32"));
    EXPECT_EQ(-1,
        cfg->GetIntValueFromSentence(
            "Filter: F1, Item: (AIR_10 ... AIR_32)", "Item", "AIR_33"));
    EXPECT_EQ(0,
        cfg->GetIntValueFromSentence(
            "Filter: F1, Item: (AIR_READ, AIR_WRITE)", "Item", "AIR_READ"));
    EXPECT_EQ(1,
        cfg->GetIntValueFromSentence(
            "Filter: F1, Item: (AIR_READ, AIR_WRITE)", "Item", "AIR_WRITE"));
    EXPECT_EQ(-1,
        cfg->GetIntValueFromSentence(
            "Filter: F1, Item: (AIR_READ, AIR_WRITE)", "Item", "AIR_WRITEE"));
    EXPECT_EQ(0,
        cfg->GetIntValueFromSentence(
            "Filter: F1, Item: (AIR_ONE)", "Item", "AIR_ONE"));

    // BUCKET
    EXPECT_EQ(10,
        cfg->GetIntValueFromSentence(
            "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10", "Scale"));
    EXPECT_EQ(3,
        cfg->GetIntValueFromSentence(
            "Scale: 3, Bucket: BUCKET_2, Bounds: [33, 66)", "Scale"));
    EXPECT_EQ(20,
        cfg->GetIntValueFromSentence(
            "Bounds: [-100, 80), Scale: 20, Bucket: BUCKET_3", "Scale"));
    EXPECT_EQ(2,
        cfg->GetIntValueFromSentence(
            "Bucket: BUCKET_4, Bounds: [2^0, 2^10), Scale: 2^", "Scale"));
    EXPECT_EQ(2,
        cfg->GetIntValueFromSentence(
            "Bucket: BUCKET_5, Bounds: (-4^6, -4^2], Scale: 2^", "Scale"));
    EXPECT_EQ(10,
        cfg->GetIntValueFromSentence(
            "Bucket: BUCKET_6, Bounds: (-10^3, 10^5), Scale: 10^", "Scale"));

    // NODE
    EXPECT_EQ(1,
        cfg->GetIntValueFromSentence("Node: Q_IOWORER, Type: QUEUE, Build: True, "
                                     "Filter: Basic, Run: Off, SamplingRatio: 10",
            "Build"));
    EXPECT_EQ(0,
        cfg->GetIntValueFromSentence("Node: Q_IOWORER, Type: QUEUE, Build: True, "
                                     "Filter: Basic, Run: Off, SamplingRatio: 10",
            "Run"));
    EXPECT_EQ(-1,
        cfg->GetIntValueFromSentence("Node: Q_IOWORER, Type: QUEUE, Build: True, "
                                     "Filter: Basic, Run: Off, SamplingRatio: 10",
            "AirBuild"));
    EXPECT_EQ(10,
        cfg->GetIntValueFromSentence("Node: Q_IOWORER, Type: QUEUE, Build: True, "
                                     "Filter: Basic, Run: Off, SamplingRatio: 10",
            "SamplingRatio"));
}

TEST_F(ConfigParserTest, GetStrValueFromSentence)
{
    // DEFAULT
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence(
               "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
               "NodeSamplingRatio: 1000, NodeIndexSize:32, FileWrite: On, "
               "RemainingFileCount: 3",
               "AirBuild")
            .compare(""));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence(
               "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
               "NodeSamplingRatio: 1000, NodeIndexSize:32, FileWrite: On, "
               "RemainingFileCount: 3",
               "NodeBuild")
            .compare(""));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence(
               "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:Off, "
               "NodeSamplingRatio: 1000, NodeIndexSize:32, FileWrite: On, "
               "RemainingFileCount: 3",
               "StreamingInterval")
            .compare(""));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence(
               "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:Off, "
               "NodeSamplingRatio: 1000, NodeIndexSize:32, FileWrite: On, "
               "RemainingFileCount: 3",
               "SamplingRatio")
            .compare(""));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence(
               "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:Off, "
               "NodeSamplingRatio: 1000, NodeIndexSize:32, FileWrite: On, "
               "RemainingFileCount: 3",
               "Condition")
            .compare(""));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence(
               "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:off, "
               "NodeSamplingRatio: 1000, NodeIndexSize:32, FileWrite: On, "
               "RemainingFileCount: 3",
               "NodeRun")
            .compare(""));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence(
               "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:off, "
               "NodeSamplingRatio: 1000, NodeIndexSize:32, FileWrite: On, "
               "RemainingFileCount: 3",
               "NodeIndexSize")
            .compare(""));

    // GROUP
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence(
               "Group: POS_RSC  , NodeBuild: False, NodeRun: On", "Group")
            .compare("POS_RSC"));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence("Group: POS_RSC", "Group").compare("POS_RSC"));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence(
               "Group: POS_RSC  , NodeBuild: False, NodeRun: On", "NodeRun")
            .compare(""));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence(
               "Group: POS_RSC  , NodeBuild: False, NodeRun: On", "NodeBuild")
            .compare(""));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence(
               "Group: POS_RSC  , NodeBuild: False, NodeRun: On", "NodeRun")
            .compare(""));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence(
               "Group: POS_RSC  , NodeBuild: False, NodeRun: On", "SamplingRatio")
            .compare(""));

    // FILTER
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence(
               "Filter: F1, Item: (AIR_9 ... AIR_32)", "Filter")
            .compare("F1"));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence(
               "Filter: F2, Item: (AIR_12, AIR_34, AIR_39)", "Filter")
            .compare("F2"));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence("Filter: F3, Item: (AIR_ONE)", "Filter")
            .compare("F3"));

    // BUCKET
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence(
               "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10", "Bucket")
            .compare("BUCKET_1"));

    // NODE
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence("Node:   Q_IOWORKER    , Type: QUEUE, Build: "
                                     "True, Run: Off, SamplingRatio: 10",
               "Node")
            .compare("Q_IOWORKER"));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence("Node: Q_IOWORKER, Type: QUEUE, Build: True, "
                                     "Run: Off, SamplingRatio: 10",
               "Type")
            .compare("QUEUE"));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence("Node: Q_IOWORKER, Type: QUEUE, Build: True, "
                                     "Run: Off, SamplingRatio: 10, Group:  IO     ",
               "Group")
            .compare("IO"));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence("Node: Q_IOWORKER, Type: QUEUE, Build: True, "
                                     "Run: Off, SamplingRatio: 10",
               "Run")
            .compare(""));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence("Node: Q_IOWORKER, Type: QUEUE, Build: True, "
                                     "Run: Off, SamplingRatio: 10",
               "SamplingRatio")
            .compare(""));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence("Node: Q_IOWORKER, Type: QUEUE, Build: True, "
                                     "Run: Off, SamplingRatio: 10",
               "AirBuild")
            .compare(""));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence("Node: Q_IOWORKER, Type: QUEUE, Build: True, "
                                     "Run: Off, SamplingRatio: 10",
               "Build")
            .compare(""));
    EXPECT_EQ(0,
        cfg->GetStrValueFromSentence("Node: Q_IOWORKER, Type: QUEUE, Build: True, "
                                     "Run: Off, SamplingRatio: 10",
               "Group")
            .compare(""));
}

TEST_F(ConfigParserTest, GetSentenceCount)
{
    EXPECT_EQ(1, cfg->GetSentenceCount(config::ParagraphType::DEFAULT));
    EXPECT_EQ(5, cfg->GetSentenceCount(config::ParagraphType::GROUP));
    EXPECT_EQ(3, cfg->GetSentenceCount(config::ParagraphType::FILTER));
    EXPECT_EQ(6, cfg->GetSentenceCount(config::ParagraphType::BUCKET));
    EXPECT_EQ(23, cfg->GetSentenceCount(config::ParagraphType::NODE));
}

TEST_F(ConfigParserTest, GetItemStrFromFilterSentence)
{
    EXPECT_EQ(0,
        cfg->GetItemStrFromFilterSentence(
               "Filter: F1, Item: (AIR_10 ... AIR_32)", 0)
            .compare("AIR_10"));
    EXPECT_EQ(0,
        cfg->GetItemStrFromFilterSentence(
               "Filter: F1, Item: (AIR_10 ... AIR_32)", 9)
            .compare("AIR_19"));
    EXPECT_EQ(0,
        cfg->GetItemStrFromFilterSentence(
               "Filter: F1, Item: (AIR_10 ... AIR_32)", 22)
            .compare("AIR_32"));
    EXPECT_EQ(0,
        cfg->GetItemStrFromFilterSentence(
               "Filter: F1, Item: (AIR_READ, AIR_WRITE)", 0)
            .compare("AIR_READ"));
    EXPECT_EQ(0,
        cfg->GetItemStrFromFilterSentence(
               "Filter: F1, Item: (AIR_READ, AIR_WRITE)", 1)
            .compare("AIR_WRITE"));
    EXPECT_EQ(0,
        cfg->GetItemStrFromFilterSentence("Filter: F1, Item: (AIR_ONE)", 0)
            .compare("AIR_ONE"));
}

TEST_F(ConfigParserTest, GetItemSizeFromFilterSentence)
{
    EXPECT_EQ(13,
        cfg->GetItemSizeFromFilterSentence("Filter: F1, Item: (AIR_0 ... AIR_12)"));
    EXPECT_EQ(4,
        cfg->GetItemSizeFromFilterSentence(
            "Filter: F1, Item: (AIR_0, AIR_1, AIR_2, AIR_8)"));
    EXPECT_EQ(1, cfg->GetItemSizeFromFilterSentence("Filter: F1, Item: (AIR_ONE)"));
}

TEST_F(ConfigParserTest, GetLowerBoundFromBucketSentence)
{
    EXPECT_EQ(0,
        cfg->GetLowerBoundFromBucketSentence(
            "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10"));
    EXPECT_EQ(33,
        cfg->GetLowerBoundFromBucketSentence(
            "Scale: 3, Bucket: BUCKET_2, Bounds: [33, 66)"));
    EXPECT_EQ(-100,
        cfg->GetLowerBoundFromBucketSentence(
            "Bounds: [-100, 80), Scale: 20, Bucket: BUCKET_3"));
    EXPECT_EQ(1,
        cfg->GetLowerBoundFromBucketSentence(
            "Bucket: BUCKET_4, Bounds: [2^0, 2^10), Scale: 2^"));
    EXPECT_EQ(-4096,
        cfg->GetLowerBoundFromBucketSentence(
            "Bucket: BUCKET_5, Bounds: (-4^6, -4^2], Scale: 2^"));
    EXPECT_EQ(-1000,
        cfg->GetLowerBoundFromBucketSentence(
            "Bucket: BUCKET_6, Bounds: (-10^3, 10^5), Scale: 10^"));
}

TEST_F(ConfigParserTest, GetUpperBoundFromBucketSentence)
{
    EXPECT_EQ(100,
        cfg->GetUpperBoundFromBucketSentence(
            "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10"));
    EXPECT_EQ(66,
        cfg->GetUpperBoundFromBucketSentence(
            "Scale: 3, Bucket: BUCKET_2, Bounds: [33, 66)"));
    EXPECT_EQ(80,
        cfg->GetUpperBoundFromBucketSentence(
            "Bounds: [-100, 80), Scale: 20, Bucket: BUCKET_3"));
    EXPECT_EQ(1024,
        cfg->GetUpperBoundFromBucketSentence(
            "Bucket: BUCKET_4, Bounds: [2^0, 2^10), Scale: 2^"));
    EXPECT_EQ(-16,
        cfg->GetUpperBoundFromBucketSentence(
            "Bucket: BUCKET_5, Bounds: (-4^6, -4^2], Scale: 2^"));
    EXPECT_EQ(100000,
        cfg->GetUpperBoundFromBucketSentence(
            "Bucket: BUCKET_6, Bounds: (-10^3, 10^5), Scale: 10^"));
}

TEST_F(ConfigParserTest, IsLinearTypeFromBucketSentence)
{
    EXPECT_EQ(true,
        cfg->IsLinearTypeFromBucketSentence(
            "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10"));
    EXPECT_EQ(true,
        cfg->IsLinearTypeFromBucketSentence(
            "Scale: 3, Bucket: BUCKET_2, Bounds: [33, 66)"));
    EXPECT_EQ(true,
        cfg->IsLinearTypeFromBucketSentence(
            "Bounds: [-100, 80), Scale: 20, Bucket: BUCKET_3"));
    EXPECT_EQ(false,
        cfg->IsLinearTypeFromBucketSentence(
            "Bucket: BUCKET_4, Bounds: [2^0, 2^10), Scale: 2^"));
    EXPECT_EQ(false,
        cfg->IsLinearTypeFromBucketSentence(
            "Bucket: BUCKET_5, Bounds: (-4^6, -4^2], Scale: 2^"));
    EXPECT_EQ(false,
        cfg->IsLinearTypeFromBucketSentence(
            "Bucket: BUCKET_6, Bounds: (-10^3, 10^5), Scale: 10^"));
}
