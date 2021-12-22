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
    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::DEFAULT).compare(
        "StreamingInterval:1, AirBuild:True, NodeBuild:False, NodeRun:Off,\n         NodeSamplingRatio: 1000, NodeIndexSize:32"));

    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::GROUP, 1).compare(
        "Group: POS_JOURNAL ,\nNodeBuild: True, NodeIndexSize: 100"));
    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::GROUP, 3).compare(
        "Group: POS_META, NodeBuild: False, NodeRun: On, NodeSamplingRatio: 100"));
    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::GROUP, 5).compare(""));
    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::GROUP, 99).compare(""));

    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::FILTER, 0).compare(
        "Filter: Basic, Item: (BI_0, BI_1, BI_2)"));
    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::FILTER, 1).compare(
        "Filter: Range, Item: (AIR_0 ... AIR_10)"));

    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::HISTOGRAM, 0).compare(
        "Histogram: HTG_1, DataRange: [0, 100), BucketRange: 10"));
    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::HISTOGRAM, 1).compare(
        "Histogram: HTG_2, DataRange: [33, 66), BucketRange: 3"));
    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::HISTOGRAM, 2).compare(
        "Histogram: HTG_3, DataRange: [-100, 80), BucketRange: 20"));
    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::HISTOGRAM, 3).compare(
        "Histogram: HTG_4, DataRange: [2^0, 2^10), BucketRange: 2^"));
    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::HISTOGRAM, 4).compare(
        "Histogram: HTG_5, DataRange: (-4^6, -4^2], BucketRange: 2^"));
    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::HISTOGRAM, 5).compare(
        "Histogram: HTG_6, DataRange: (-10^3, 10^5), BucketRange: 10^"));

    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::NODE, 1).compare(
        "Node: PERF_VOLUME, Type: PERFORMANCE, Build: True, Group: POS, Filter: Basic"));
    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::NODE, 4).compare(
        "Node: PERF_CP, Type: PERFORMANCE, Build: True, Run: On, Filter: Basic, Group: POS_JOURNAL"));
    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::NODE, 9).compare(
        "Node: LAT_CP, Type: LATENCY, Build: False, Run: Off, Group: POS, Filter: Basic"));
    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::NODE, 10).compare(
        "Node: Q_SUBMIT, Type: QUEUE, Group: POS, Filter: Basic"));
    EXPECT_EQ(0, cfg->GetSentenceFromParagraph(config::ParagraphType::NODE, 13).compare(
        "Node: Q_SCHEDULING, Filter: Basic\n, Type: QUEUE, Build   : True, Run : Off, SamplingRatio: 1000,\n          Group: POS"));
}

TEST_F(ConfigParserTest, GetIndexFromParagraph)
{
    EXPECT_EQ(0, cfg->GetIndexFromParagraph(config::ParagraphType::DEFAULT));

    EXPECT_EQ(0, cfg->GetIndexFromParagraph(config::ParagraphType::GROUP, "POS"));
    EXPECT_EQ(1, cfg->GetIndexFromParagraph(config::ParagraphType::GROUP, "POS_JOURNAL"));
    EXPECT_EQ(2, cfg->GetIndexFromParagraph(config::ParagraphType::GROUP, "POS_IO"));
    EXPECT_EQ(3, cfg->GetIndexFromParagraph(config::ParagraphType::GROUP, "POS_META"));
    EXPECT_EQ(4, cfg->GetIndexFromParagraph(config::ParagraphType::GROUP, "POS_RSC"));
    EXPECT_EQ(-1, cfg->GetIndexFromParagraph(config::ParagraphType::GROUP, "INVALIE_NAME"));
    EXPECT_EQ(-1, cfg->GetIndexFromParagraph(config::ParagraphType::GROUP, "PERF_PSD"));

    EXPECT_EQ(0, cfg->GetIndexFromParagraph(config::ParagraphType::FILTER, "Basic"));
    EXPECT_EQ(1, cfg->GetIndexFromParagraph(config::ParagraphType::FILTER, "Range"));
    EXPECT_EQ(-1, cfg->GetIndexFromParagraph(config::ParagraphType::FILTER, "Basiic"));

    EXPECT_EQ(0, cfg->GetIndexFromParagraph(config::ParagraphType::HISTOGRAM, "HTG_1"));
    EXPECT_EQ(1, cfg->GetIndexFromParagraph(config::ParagraphType::HISTOGRAM, "HTG_2"));
    EXPECT_EQ(2, cfg->GetIndexFromParagraph(config::ParagraphType::HISTOGRAM, "HTG_3"));
    EXPECT_EQ(3, cfg->GetIndexFromParagraph(config::ParagraphType::HISTOGRAM, "HTG_4"));
    EXPECT_EQ(4, cfg->GetIndexFromParagraph(config::ParagraphType::HISTOGRAM, "HTG_5"));
    EXPECT_EQ(5, cfg->GetIndexFromParagraph(config::ParagraphType::HISTOGRAM, "HTG_6"));
    EXPECT_EQ(-1, cfg->GetIndexFromParagraph(config::ParagraphType::HISTOGRAM, "HTG_7"));

    EXPECT_EQ(0, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "PERF_PSD"));
    EXPECT_EQ(1, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "PERF_VOLUME"));
    EXPECT_EQ(2, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "PERF_METAFS"));
    EXPECT_EQ(4, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "PERF_CP"));
    EXPECT_EQ(6, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "LAT_SUBMIT"));
    EXPECT_EQ(8, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "LAT_REBUILD"));
    EXPECT_EQ(13, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "Q_SCHEDULING"));
    EXPECT_EQ(-1, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "INVALIE_NAME"));
    EXPECT_EQ(-1, cfg->GetIndexFromParagraph(config::ParagraphType::NODE, "POS_RSC"));
}

TEST_F(ConfigParserTest, GetIntValueFromSentence)
{
    // DEFAULT
    EXPECT_EQ(1, cfg->GetIntValueFromSentence(
        "StreamingInterval:1, AirBuild  : True, NodeBuild:True, NodeRun:On, NodeSamplingRatio: 1000, NodeIndexSize:32", "AirBuild"));
    EXPECT_EQ(1, cfg->GetIntValueFromSentence(
        "StreamingInterval:1, AirBuild  : True, NodeBuild:True, NodeRun:On, NodeSamplingRatio: 1000, NodeIndexSize:32", "NodeBuild"));
    EXPECT_EQ(0, cfg->GetIntValueFromSentence(
        "StreamingInterval:1, AirBuild  : True, NodeBuild:True, NodeRun:Off, NodeSamplingRatio: 1000, NodeIndexSize:32", "NodeRun"));
    EXPECT_EQ(3, cfg->GetIntValueFromSentence(
        "StreamingInterval:  3   , AirBuild  : True, NodeBuild:True, NodeRun:Off, NodeSamplingRatio: 1000, NodeIndexSize:32", "StreamingInterval"));
    EXPECT_EQ(1000, cfg->GetIntValueFromSentence(
        "StreamingInterval:1, AirBuild  : True, NodeBuild:True, NodeRun:off, NodeSamplingRatio: 1000, NodeIndexSize:32", "NodeSamplingRatio"));
    EXPECT_EQ(-1, cfg->GetIntValueFromSentence(
        "StreamingInterval:1, AirBuild  : True, NodeBuild:True, NodeRun:off, NodeSamplingRatio: 1000, NodeIndexSize:32", "Condition"));
    EXPECT_EQ(32, cfg->GetIntValueFromSentence(
        "StreamingInterval:1, AirBuild  : True, NodeBuild:True, NodeRun:Off, NodeSamplingRatio: 1000, NodeIndexSize:32", "NodeIndexSize"));

    // GROUP
    EXPECT_EQ(0, cfg->GetIntValueFromSentence("Group: POS_RSC, NodeBuild: False, NodeRun: On", "NodeBuild"));
    EXPECT_EQ(1, cfg->GetIntValueFromSentence("Group: POS_RSC, NodeBuild: False, NodeRun: On", "NodeRun"));
    EXPECT_EQ(-1, cfg->GetIntValueFromSentence("Group: POS_RSC, NodeBuild: False, NodeRun: On", "Condition"));

    // FILTER
    EXPECT_EQ(0, cfg->GetIntValueFromSentence("Filter: F1, Item: (AIR_10 ... AIR_32)", "Item", "AIR_10"));
    EXPECT_EQ(9, cfg->GetIntValueFromSentence("Filter: F1, Item: (AIR_10 ... AIR_32)", "Item", "AIR_19"));
    EXPECT_EQ(22, cfg->GetIntValueFromSentence("Filter: F1, Item: (AIR_10 ... AIR_32)", "Item", "AIR_32"));
    EXPECT_EQ(-1, cfg->GetIntValueFromSentence("Filter: F1, Item: (AIR_10 ... AIR_32)", "Item", "AIR_33"));
    EXPECT_EQ(0, cfg->GetIntValueFromSentence("Filter: F1, Item: (AIR_READ, AIR_WRITE)", "Item", "AIR_READ"));
    EXPECT_EQ(1, cfg->GetIntValueFromSentence("Filter: F1, Item: (AIR_READ, AIR_WRITE)", "Item", "AIR_WRITE"));
    EXPECT_EQ(-1, cfg->GetIntValueFromSentence("Filter: F1, Item: (AIR_READ, AIR_WRITE)", "Item", "AIR_WRITEE"));
    EXPECT_EQ(0, cfg->GetIntValueFromSentence("Filter: F1, Item: (AIR_ONE)", "Item", "AIR_ONE"));

    // HISTOGRAM
    EXPECT_EQ(10, cfg->GetIntValueFromSentence("Histogram: HTG_1, DataRange: [0, 100), BucketRange: 10", "BucketRange"));
    EXPECT_EQ(3, cfg->GetIntValueFromSentence("Histogram: HTG_2, DataRange: [33, 66), BucketRange: 3", "BucketRange"));
    EXPECT_EQ(20, cfg->GetIntValueFromSentence("Histogram: HTG_3, DataRange: [-100, 80), BucketRange: 20", "BucketRange"));
    EXPECT_EQ(2, cfg->GetIntValueFromSentence("Histogram: HTG_4, DataRange: [2^0, 2^10), BucketRange: 2^", "BucketRange"));
    EXPECT_EQ(2, cfg->GetIntValueFromSentence("Histogram: HTG_5, DataRange: (-4^6, 4^2], BucketRange: 2^", "BucketRange"));
    EXPECT_EQ(10, cfg->GetIntValueFromSentence("Histogram: HTG_6, DataRange: (-10^3, 10^5), BucketRange: 10^", "BucketRange"));

    // NODE
    EXPECT_EQ(1, cfg->GetIntValueFromSentence("Node: Q_IOWORER, Type: QUEUE, Build: True, Filter: Basic, Run: Off, SamplingRatio: 10", "Build"));
    EXPECT_EQ(0, cfg->GetIntValueFromSentence("Node: Q_IOWORER, Type: QUEUE, Build: True, Filter: Basic, Run: Off, SamplingRatio: 10", "Run"));
    EXPECT_EQ(-1, cfg->GetIntValueFromSentence("Node: Q_IOWORER, Type: QUEUE, Build: True, Filter: Basic, Run: Off, SamplingRatio: 10", "AirBuild"));
    EXPECT_EQ(10, cfg->GetIntValueFromSentence("Node: Q_IOWORER, Type: QUEUE, Build: True, Filter: Basic, Run: Off, SamplingRatio: 10", "SamplingRatio"));
}

TEST_F(ConfigParserTest, GetStrValueFromSentence)
{
    // DEFAULT
    EXPECT_EQ(0, cfg->GetStrValueFromSentence(
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, NodeSamplingRatio: 1000, NodeIndexSize:32", "AirBuild").compare(""));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence(
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, NodeSamplingRatio: 1000, NodeIndexSize:32", "NodeBuild").compare(""));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence(
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:Off, NodeSamplingRatio: 1000, NodeIndexSize:32", "StreamingInterval").compare(""));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence(
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:Off, NodeSamplingRatio: 1000, NodeIndexSize:32", "SamplingRatio").compare(""));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence(
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:Off, NodeSamplingRatio: 1000, NodeIndexSize:32", "Condition").compare(""));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence(
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:off, NodeSamplingRatio: 1000, NodeIndexSize:32", "NodeRun").compare(""));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence(
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:off, NodeSamplingRatio: 1000, NodeIndexSize:32", "NodeIndexSize").compare(""));

    // GROUP
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Group: POS_RSC  , NodeBuild: False, NodeRun: On", "Group").compare("POS_RSC"));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Group: POS_RSC", "Group").compare("POS_RSC"));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Group: POS_RSC  , NodeBuild: False, NodeRun: On", "NodeRun").compare(""));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Group: POS_RSC  , NodeBuild: False, NodeRun: On", "NodeBuild").compare(""));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Group: POS_RSC  , NodeBuild: False, NodeRun: On", "NodeRun").compare(""));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Group: POS_RSC  , NodeBuild: False, NodeRun: On", "SamplingRatio").compare(""));

    // FILTER
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Filter: F1, Item: (AIR_9 ... AIR_32)", "Filter").compare("F1"));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Filter: F2, Item: (AIR_12, AIR_34, AIR_39)", "Filter").compare("F2"));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Filter: F3, Item: (AIR_ONE)", "Filter").compare("F3"));

    // HISTOGRAM
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Histogram: HTG_1, DataRange: [0, 100), BucketRange: 10", "Histogram").compare("HTG_1"));

    // NODE
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Node:   Q_IOWORKER    , Type: QUEUE, Build: True, Run: Off, SamplingRatio: 10", "Node").compare("Q_IOWORKER"));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Node: Q_IOWORKER, Type: QUEUE, Build: True, Run: Off, SamplingRatio: 10", "Type").compare("QUEUE"));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Node: Q_IOWORKER, Type: QUEUE, Build: True, Run: Off, SamplingRatio: 10, Group:  IO     ", "Group").compare("IO"));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Node: Q_IOWORKER, Type: QUEUE, Build: True, Run: Off, SamplingRatio: 10", "Run").compare(""));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Node: Q_IOWORKER, Type: QUEUE, Build: True, Run: Off, SamplingRatio: 10", "SamplingRatio").compare(""));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Node: Q_IOWORKER, Type: QUEUE, Build: True, Run: Off, SamplingRatio: 10", "AirBuild").compare(""));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Node: Q_IOWORKER, Type: QUEUE, Build: True, Run: Off, SamplingRatio: 10", "Build").compare(""));
    EXPECT_EQ(0, cfg->GetStrValueFromSentence("Node: Q_IOWORKER, Type: QUEUE, Build: True, Run: Off, SamplingRatio: 10", "Group").compare(""));
}

TEST_F(ConfigParserTest, GetSentenceCount)
{
    EXPECT_EQ(1, cfg->GetSentenceCount(config::ParagraphType::DEFAULT));
    EXPECT_EQ(5, cfg->GetSentenceCount(config::ParagraphType::GROUP));
    EXPECT_EQ(2, cfg->GetSentenceCount(config::ParagraphType::FILTER));
    EXPECT_EQ(6, cfg->GetSentenceCount(config::ParagraphType::HISTOGRAM));
    EXPECT_EQ(18, cfg->GetSentenceCount(config::ParagraphType::NODE));
}

TEST_F(ConfigParserTest, GetItemStrFromFilterSentence)
{
    EXPECT_EQ(0, cfg->GetItemStrFromFilterSentence("Filter: F1, Item: (AIR_10 ... AIR_32)", 0).compare("AIR_10"));
    EXPECT_EQ(0, cfg->GetItemStrFromFilterSentence("Filter: F1, Item: (AIR_10 ... AIR_32)", 9).compare("AIR_19"));
    EXPECT_EQ(0, cfg->GetItemStrFromFilterSentence("Filter: F1, Item: (AIR_10 ... AIR_32)", 22).compare("AIR_32"));
    EXPECT_EQ(0, cfg->GetItemStrFromFilterSentence("Filter: F1, Item: (AIR_READ, AIR_WRITE)", 0).compare("AIR_READ"));
    EXPECT_EQ(0, cfg->GetItemStrFromFilterSentence("Filter: F1, Item: (AIR_READ, AIR_WRITE)", 1).compare("AIR_WRITE"));
    EXPECT_EQ(0, cfg->GetItemStrFromFilterSentence("Filter: F1, Item: (AIR_ONE)", 0).compare("AIR_ONE"));
}

TEST_F(ConfigParserTest, GetItemSizeFromFilterSentence)
{
    EXPECT_EQ(13, cfg->GetItemSizeFromFilterSentence("Filter: F1, Item: (AIR_0 ... AIR_12)"));
    EXPECT_EQ(4, cfg->GetItemSizeFromFilterSentence("Filter: F1, Item: (AIR_0, AIR_1, AIR_2, AIR_8)"));
    EXPECT_EQ(1, cfg->GetItemSizeFromFilterSentence("Filter: F1, Item: (AIR_ONE)"));
}

TEST_F(ConfigParserTest, GetMinValueFromHistogramSentence)
{
    EXPECT_EQ(0, cfg->GetMinValueFromHistogramSentence("Histogram: HTG_1, DataRange:[0, 100), BucketRange: 10"));
    EXPECT_EQ(33, cfg->GetMinValueFromHistogramSentence("Histogram: HTG_2, DataRange: [33, 66), BucketRange: 3"));
    EXPECT_EQ(-100, cfg->GetMinValueFromHistogramSentence("Histogram: HTG_3, DataRange: [-100, 80), BucketRange: 20"));
    EXPECT_EQ(1, cfg->GetMinValueFromHistogramSentence("Histogram: HTG_4, DataRange: [2^0, 2^10), BucketRange: 2^"));
    EXPECT_EQ(-4096, cfg->GetMinValueFromHistogramSentence("Histogram: HTG_5, DataRange: (-4^6, -4^2], BucketRange: 2^"));
    EXPECT_EQ(-1000, cfg->GetMinValueFromHistogramSentence("Histogram: HTG_6, BucketRange: 10^, DataRange: (-10^3, 10^5)"));
}

TEST_F(ConfigParserTest, GetMaxValueFromHistogramSentence)
{
    EXPECT_EQ(100, cfg->GetMaxValueFromHistogramSentence("Histogram: HTG_1, DataRange:[0, 100), BucketRange: 10"));
    EXPECT_EQ(66, cfg->GetMaxValueFromHistogramSentence("Histogram: HTG_2, DataRange: [33, 66), BucketRange: 3"));
    EXPECT_EQ(80, cfg->GetMaxValueFromHistogramSentence("Histogram: HTG_3, DataRange: [-100, 80), BucketRange: 20"));
    EXPECT_EQ(1024, cfg->GetMaxValueFromHistogramSentence("Histogram: HTG_4, DataRange: [2^0, 2^10), BucketRange: 2^"));
    EXPECT_EQ(-16, cfg->GetMaxValueFromHistogramSentence("Histogram: HTG_5, DataRange: (-4^6, -4^2], BucketRange: 2^"));
    EXPECT_EQ(100000, cfg->GetMaxValueFromHistogramSentence("Histogram: HTG_6, BucketRange: 10^, DataRange: (-10^3, 10^5)"));
}

TEST_F(ConfigParserTest, GetRangeTypeFromHistogramSentence)
{
    EXPECT_EQ(true, cfg->IsLinearTypeFromHistogramSentence("Histogram: HTG_1, DataRange:[0, 100), BucketRange:10 "));
    EXPECT_EQ(true, cfg->IsLinearTypeFromHistogramSentence("Histogram: HTG_2, DataRange: [33, 66), BucketRange:3"));
    EXPECT_EQ(true, cfg->IsLinearTypeFromHistogramSentence("Histogram: HTG_3, DataRange: [-100, 80), BucketRange: 20"));
    EXPECT_EQ(false, cfg->IsLinearTypeFromHistogramSentence("Histogram: HTG_4, DataRange: [2^0, 2^10), BucketRange: 2^"));
    EXPECT_EQ(false, cfg->IsLinearTypeFromHistogramSentence("Histogram: HTG_5, DataRange: (-4^6, -4^2], BucketRange: 2^ "));
    EXPECT_EQ(false, cfg->IsLinearTypeFromHistogramSentence("Histogram: HTG_6, BucketRange:10^, DataRange: (-10^3, 10^5)"));
}
