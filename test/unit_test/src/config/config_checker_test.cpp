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

#include "config_checker_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>

TEST_F(ConfigCheckerTest, CheckParagraphRule_DefaultType)
{
    constexpr air::string_view default_paragraph = R"DEFAULT(
    "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, NodeSamplingRatio: 1000, NodeIndexSize:32"
    )DEFAULT";
    constexpr air::string_view default_paragraph_with_comment = R"DEFAULT(
            // default setting for Mandatory
    "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, NodeSamplingRatio: 1000, NodeIndexSize:32"
    )DEFAULT";
    constexpr air::string_view default_paragraph_multi_line = R"DEFAULT(
            // default setting for Mandatory
    "StreamingInterval:1, AirBuild:True, 
        NodeBuild:True, NodeRun:On, NodeSamplingRatio: 1000, NodeIndexSize:32"
    )DEFAULT";
    constexpr air::string_view default_paragraph_multi_line2 = R"DEFAULT(
            // default setting for Mandatory
    "StreamingInterval:1, AirBuild:True
        ,NodeBuild:True, NodeRun:On, NodeSamplingRatio: 1000, NodeIndexSize:32"
    )DEFAULT";

    EXPECT_EQ(0,
        cfg_checker->CheckParagraphRule(
            config::ParagraphType::DEFAULT, default_paragraph));
    EXPECT_EQ(0,
        cfg_checker->CheckParagraphRule(
            config::ParagraphType::DEFAULT, default_paragraph_with_comment));
    EXPECT_EQ(0,
        cfg_checker->CheckParagraphRule(
            config::ParagraphType::DEFAULT, default_paragraph_multi_line));
    EXPECT_EQ(0,
        cfg_checker->CheckParagraphRule(
            config::ParagraphType::DEFAULT, default_paragraph_multi_line2));
}

TEST_F(ConfigCheckerTest, CheckParagraphRule_GroupType)
{
    constexpr air::string_view group_paragraph = R"GROUP(
    "Group:POS_IO"
    "Group:POS_META"
    "Group: testetsetsetset, NodeRun: On, NodeBuild: False"
    "Group: POS_REBUILD, NodeSamplingRatio: 1000, NodeBuild: True"
    )GROUP";
    constexpr air::string_view group_paragraph_with_comment = R"GROUP(
    "Group:POS_IO"
    "Group:POS_META"// comment test
    "Group: testetsetsetset, NodeRun: On, NodeBuild: False"
    "Group: POS_REBUILD, NodeSamplingRatio: 1000, NodeBuild: True"
    )GROUP";
    constexpr air::string_view group_paragraph_multi_line = R"GROUP(
            // group setting, TBD
    "Group:POS_IO"
    "Group:POS_META"// comment test
    "Group: testetsetsetset, NodeRun: On, NodeBuild: False"
    "Group: POS_REBUILD
        , NodeSamplingRatio: 1000, NodeBuild: True"
    )GROUP";
    EXPECT_EQ(0,
        cfg_checker->CheckParagraphRule(
            config::ParagraphType::GROUP, group_paragraph));
    EXPECT_EQ(0,
        cfg_checker->CheckParagraphRule(
            config::ParagraphType::GROUP, group_paragraph_with_comment));
    EXPECT_EQ(0,
        cfg_checker->CheckParagraphRule(
            config::ParagraphType::GROUP, group_paragraph_multi_line));
}

TEST_F(ConfigCheckerTest, CheckParagraphRule_FilterType)
{
    constexpr air::string_view filter_paragraph1 =
        R"FILTER("Filter:F1, Item: (F1_0)")FILTER";
    constexpr air::string_view filter_paragraph2 =
        R"FILTER("Filter:_F1, Item: (F1_0,F1_1,F1_2) ")FILTER";
    constexpr air::string_view filter_paragraph3 =
        R"FILTER("Filter:F1_, Item: (F1_0...F1_13)   ")FILTER";
    constexpr air::string_view filter_paragraph_error1 =
        R"FILTER("Filter: F1, Item: (F1_0, F1_1),")FILTER";
    constexpr air::string_view filter_paragraph_error2 =
        R"FILTER("Filter: F1, Item: (F1_0, F1_1")FILTER";
    constexpr air::string_view filter_paragraph_error3 =
        R"FILTER("Filter: F1, Item: F1_0, F1_1)")FILTER";
    constexpr air::string_view filter_paragraph_error4 =
        R"FILTER("Filter: F1, Item: ((F1_0, F1_1)")FILTER";
    constexpr air::string_view filter_paragraph_error5 =
        R"FILTER("Filter: F1, Item: (F1_0, F1_1))")FILTER";
    constexpr air::string_view filter_paragraph_error6 =
        R"FILTER(": F1, Item: (F1_0, F1_1)")FILTER";
    constexpr air::string_view filter_paragraph_error7 =
        R"FILTER("Filter: , Item: (F1_0, F1_1)")FILTER";
    constexpr air::string_view filter_paragraph_error8 =
        R"FILTER("Filter: F1, Item: ")FILTER";
    constexpr air::string_view filter_paragraph_error9 =
        R"FILTER("Filter: F1, Item: ()")FILTER";

    EXPECT_EQ(0,
        cfg_checker->CheckParagraphRule(
            config::ParagraphType::FILTER, filter_paragraph1));
    EXPECT_EQ(0,
        cfg_checker->CheckParagraphRule(
            config::ParagraphType::FILTER, filter_paragraph2));
    EXPECT_EQ(0,
        cfg_checker->CheckParagraphRule(
            config::ParagraphType::FILTER, filter_paragraph3));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::FILTER, filter_paragraph_error1));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::FILTER, filter_paragraph_error2));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::FILTER, filter_paragraph_error3));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::FILTER, filter_paragraph_error4));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::FILTER, filter_paragraph_error5));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::FILTER, filter_paragraph_error6));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::FILTER, filter_paragraph_error7));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::FILTER, filter_paragraph_error8));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::FILTER, filter_paragraph_error9));
}

TEST_F(ConfigCheckerTest, CheckParagraphRule_BucketType)
{
    constexpr air::string_view bucket_paragraph = R"BUCKET(
        "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10"
        "Scale: 3, Bucket: BUCKET_2, Bounds: [33, 66)"
        "Bounds: [-100, 80), Scale: 20, Bucket: BUCKET_3"
        )BUCKET";

    EXPECT_EQ(0,
        cfg_checker->CheckParagraphRule(
            config::ParagraphType::BUCKET, bucket_paragraph));
}

TEST_F(ConfigCheckerTest, CheckParagraphRule_NodeType)
{
    constexpr air::string_view node_paragraph = R"NODE(
    "Node: Q_SUBMIT, Type:Queue, Build:True"
    "Node:Q_EVENT , Type:Queue, Build:False"
    "Node:PERF_VOLUME, Type:Performance, Build:True"
    "Node: LAT_VOLUME  , Type:Latency, Build:True"
    "Node: Q_REACTOR"
    )NODE";
    constexpr air::string_view node_paragraph_with_comment = R"NODE(
            // node setting
    "Node: Q_SUBMIT, Type:Queue, Build:True"
    // this comment is okay
    "Node:Q_EVENT , Type:Queue, Build:False"
    // but do not insert comment within double quotation marks!
    "Node:PERF_VOLUME, Type:Performance, Build:True"
    "Node: LAT_VOLUME  , Type:Latency, Build:True"
    "Node: Q_REACTOR"   // valid comment
    )NODE";
    constexpr air::string_view node_paragraph_multi_line = R"NODE(
            // node setting
    "Node: Q_SUBMIT, Type:Queue, Build:True"
    // this comment is okay
    "Node:Q_EVENT , Type:Queue, Build:False"
    // but do not insert comment within double quotation marks!
    "Node:PERF_VOLUME,
         
        Type:Performance, 
        Build:True"
    "Node: LAT_VOLUME  , Type:Latency, Build:True"
    "Node: Q_REACTOR"
    )NODE";
    constexpr air::string_view node_paragraph_multi_line2 = R"NODE(
            // node setting
    "Node: Q_SUBMIT, Type:Queue, Build:True"
    // this comment is okay
    "Node:Q_EVENT     , Type:Queue, Build:False"
    // but do not insert comment within double quotation marks!
    "Node:PERF_VOLUME           
         
        ,Type:Performance, 
        Build:True"
    "Node: LAT_VOLUME  , Type:Latency, Build:True"
    "Node: Q_REACTOR"
    )NODE";

    EXPECT_EQ(0,
        cfg_checker->CheckParagraphRule(
            config::ParagraphType::NODE, node_paragraph));
    EXPECT_EQ(0,
        cfg_checker->CheckParagraphRule(
            config::ParagraphType::NODE, node_paragraph_with_comment));
    EXPECT_EQ(0,
        cfg_checker->CheckParagraphRule(
            config::ParagraphType::NODE, node_paragraph_multi_line));
    EXPECT_EQ(0,
        cfg_checker->CheckParagraphRule(
            config::ParagraphType::NODE, node_paragraph_multi_line2));
}

TEST_F(ConfigCheckerTest, SentenceFormatViolation)
{
    constexpr air::string_view default_no_quote = R"DEFAULT(
    StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, NodeSamplingRatio: 1000, NodeIndexSize:32
    )DEFAULT";
    constexpr air::string_view default_no_key_viol = R"DEFAULT(
    "StreamingInterval:1, :True, NodeBuild:True, NodeRun:On, NodeSamplingRatio: 1000, NodeIndexSize:32"
    )DEFAULT";
    constexpr air::string_view default_no_value_viol = R"DEFAULT(
    "StreamingInterval:1, AirBuild:, NodeBuild:True, NodeRun:On, NodeSamplingRatio: 1000, NodeIndexSize:32"
    )DEFAULT";
    constexpr air::string_view default_no_colon_viol = R"DEFAULT(
    "StreamingInterval:1, AirBuild True, NodeBuild True, NodeRun:On, NodeSamplingRatio: 1000, NodeIndexSize:32"
    )DEFAULT";
    constexpr air::string_view default_no_colon_viol2 = R"DEFAULT(
    "StreamingInterval 1, AirBuild True, NodeBuild True, NodeRun On, NodeSamplingRatio 1000, NodeIndexSize 32"
    )DEFAULT";
    constexpr air::string_view default_comment_viol = R"DEFAULT(
    "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On,   // comment violation
        NodeSamplingRatio: 1000, NodeIndexSize:32"
    )DEFAULT";
    constexpr air::string_view default_no_end_quote = R"DEFAULT(
    "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On,
        NodeSamplingRatio: 1000, NodeIndexSize:32
    )DEFAULT";
    constexpr air::string_view default_comma_count_viol = R"DEFAULT(
    "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On,
        NodeSamplingRatio: 1000, NodeIndexSize:32, "
    )DEFAULT";

    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::DEFAULT, default_no_quote));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::DEFAULT, default_no_key_viol));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::DEFAULT, default_no_value_viol));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::DEFAULT, default_no_colon_viol));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::DEFAULT, default_no_colon_viol2));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::DEFAULT, default_comment_viol));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::DEFAULT, default_no_end_quote));
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::DEFAULT, default_comma_count_viol));
}

TEST_F(ConfigCheckerTest, NameDuplicatedViolation)
{
    constexpr air::string_view group_key_duplicated(R"GROUP(
    "Group:POS_IO, Build:False"
    "Group:POS_REBUILD"
    "Group: testetsetsetset, NodeRun: on, NodeBuild: False"
    "Group: POS_REBUILD, NodeSamplingRatio: 1000, NodeBuild: True"
    )GROUP");
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::GROUP, group_key_duplicated));

    constexpr air::string_view filter_key_duplicated(R"FILTER(
    "Filter:F1, Item:(F1_0, F1_1)"
    "Filter:F1, Item:(F2_0 ... F2_3)"
    )FILTER");
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::FILTER, filter_key_duplicated));

    constexpr air::string_view bucket_key_duplicated(R"BUCKET(
    "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10"
    "Bounds: [33, 66), Scale: 3, Bucket: BUCKET_1"
    )BUCKET");
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::BUCKET, bucket_key_duplicated));

    constexpr air::string_view node_key_duplicated = R"NODE(
    "Node: Q_SUBMIT, Type:Queue, Build:True"
    "Node:Q_EVENT , Type:Queue, Build:False"
    "Node:PERF_VOLUME, Type:Performance, Build:True"
    "Node: LAT_VOLUME  , Type:Latency, Build:True"
    "Node: Q_EVENT"
    )NODE";
    EXPECT_ANY_THROW(cfg_checker->CheckParagraphRule(
        config::ParagraphType::NODE, node_key_duplicated));
}

TEST_F(ConfigCheckerTest, CheckKeyRule_DefaultType)
{
    // DEFAULT
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
            "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
            "NodeSamplingRatio: 1000, NodeIndexSize:32"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
            "NodeSamplingRatio: 1000, NodeRun:On, NodeBuild:True, AirBuild:True, "
            "StreamingInterval:1, NodeIndexSize:32"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
            "NodeSamplingRatio: 1000, NodeRun:On, NodeBuild:True, AirBuild:True, "
            "StreamingInterval:1, NodeIndexSize : 32"));
}

TEST_F(ConfigCheckerTest, CheckKeyRule_GroupType)
{
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::GROUP,
            "Group: POS_RSC, NodeBuild: False, NodeRun: On, NodeSamplingRatio: "
            "10"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::GROUP,
            "NodeBuild: False, Group: POS_RSC, NodeSamplingRatio: 10, NodeRun: "
            "On"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::GROUP,
            "NodeBuild: False, NodeRun: On, NodeSamplingRatio: 10, Group: "
            "POS_RSC"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::GROUP,
            "Group: POS_RSC, NodeBuild: False, NodeRun: On, NodeSamplingRatio: "
            "100"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::GROUP,
            "Group: POS_RSC, NodeBuild: False, NodeRun: On"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(
            config::ParagraphType::GROUP, "Group: POS_RSC, NodeBuild: False"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::GROUP, "Group: POS_RSC"));
}

TEST_F(ConfigCheckerTest, CheckKeyRule_FilterType)
{
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(
            config::ParagraphType::FILTER, "Filter: F1, Item: (F1_0, F1_1)"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(
            config::ParagraphType::FILTER, "Filter: F1, Item: (F1_0 ... F1_1)"));
}

TEST_F(ConfigCheckerTest, CheckKeyRule_BucketType)
{
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::BUCKET,
            "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::BUCKET,
            "Scale: 3, Bucket: BUCKET_2, Bounds: [33, 66)"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::BUCKET,
            "Bounds: [-100, 80), Scale: 20, Bucket: BUCKET_3"));
}

TEST_F(ConfigCheckerTest, CheckKeyRule_NodeType)
{
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
            "Node: PERF_CP, Type: PERFORMANCE, Build: True, Run: On, Group: "
            "POS_Journal, Filter: Basic"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
            "Type: PERFORMANCE, Group: POS_Journal, Run: On, Build: True, Node: "
            "PERF_CP, Filter: Basic"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
            "Node: PERF_CP, Type: PERFORMANCE, Build: True, Filter: Basic, Run: "
            "On, Group: UNGROUPED"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
            "Node: PERF_CP, Type: PERFORMANCE,Filter:Basic, Build: True, Run: On, "
            "Group: UNGROUPED"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
            "Node: PERF_CP, Type: PERFORMANCE, Build: True, Run: On, Group: "
            "UNGROUPED, Filter: Basic"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
            "Node: PERF_CP, Type: PERFORMANCE, Filter: Basic, Build: True, Group: "
            "UNGROUPED"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
            "Filter: Basic, Node: PERF_CP, Type: PERFORMANCE, Group: UNGROUPED"));
    EXPECT_EQ(0,
        cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
            "Node: HIST_PSD, Type : HISTOGRAM, Filter: Basic,  Group: POS, Bucket: "
            "BUCKET_6"));
}

TEST_F(ConfigCheckerTest, KeyTypoViolation_DefaultType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AAABuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 1000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, N0deBuild:True, NodeRun:On, "
        "NodeSamplingRati0: 1000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NR:On, "
        "NodeSamplingRatio: 1000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "SI:1, AirBuild:True, NodeBuild:True, NodeRun:On, NodeSamplingRatio: 1000, "
        "NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, NSR: "
        "1333, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 1000, AS:33"));
}

TEST_F(ConfigCheckerTest, KeyTypoViolation_GroupType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::GROUP,
        "Group: POS_RSC, NB: False, NodeRun: On, NodeSamplingRatio: 10"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::GROUP,
        "Group: POS_RSC, NodeBuild: False, RN: On, NodeSamplingRatio: 10"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::GROUP,
        "Group: POS_RSC, NodeBuild: False, NodeRun: On, NSR: 10"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::GROUP,
        "Group: POS_RSC, NodeBuild: False, Node: On, NodeSamplingRatio: 10"));
}

TEST_F(ConfigCheckerTest, KeyTypoViolation_FilterType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(
        config::ParagraphType::FILTER, "Filte: F1, Item: (AIR_BASE)"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(
        config::ParagraphType::FILTER, "Filter: F1, Itemm: (AIR_BASE)"));
}

TEST_F(ConfigCheckerTest, KeyTypoViolation_BucketType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::BUCKET,
        "Buckett: BUCKET_1, Bounds: [0, 100), Scale: 10"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_1, Bound: [0, 100), Scale: 10"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_1, Bounds: [0, 100), Scal: 10"));
}

TEST_F(ConfigCheckerTest, KeyTypoViolation_NodeType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "NN: PERF_CP, Type: PERFORMANCE, Build: True, Run: On, Group: POS_Journal, "
        "SamplingRatio: 100"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: PERF_CP, t: PERFORMANCE, Build: True, Run: On, Group: POS_Journal, "
        "SamplingRatio: 100"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: PERF_CP, Type: PERFORMANCE, B: True, Run: On, Group: POS_Journal, "
        "SamplingRatio: 100"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: PERF_CP, Type: PERFORMANCE, Build: True, RN: On, Group: "
        "POS_Journal, SamplingRatio: 100"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: PERF_CP, Type: PERFORMANCE, Build: True, Run: On, Group: "
        "POS_Journal, SamplingRatio: 100"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: PERF_CP, Type: PERFORMANCE, Build: True, Run: On, GN: POS_Journal, "
        "SamplingRatio: 100"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: PERF_CP, Type: PERFORMANCE, Build: True, Run: On, Group: "
        "POS_Journal, SR: 100"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: PERF_CP, Type: PERFORMANCE, SamplingRatio: 10"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: LAT_TEST, Type: LATENCY, SamplingRatio : 1000"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: HIST_PSD, Type : HISTOGRAM, Filter: Basic,  Group: POS, Bucke: "
        "BUCKET_6"));
}

TEST_F(ConfigCheckerTest, KeyMandatoryViolation_DefaultType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, "
        "NodeSamplingRatio:1000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeRun:On, NodeSamplingRatio:1000, "
        "NodeIndexSize: 50"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, NodeBuild:True, NodeRun:On, NodeSamplingRatio:1000, "
        "NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "AirBuild:True, NodeBuild:True, NodeRun:On, NodeSamplingRatio:1000, "
        "NodeIndexSize:10"));
}

TEST_F(ConfigCheckerTest, KeyMandatoryViolation_GroupType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::GROUP,
        "NodeBuild: False, NodeRun: On, NodeSamplingRatio: 10"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(
        config::ParagraphType::GROUP, "NodeBuild: False, NodeSamplingRatio: 10"));
}

TEST_F(ConfigCheckerTest, KeyMandatoryViolation_FilterType)
{
    EXPECT_ANY_THROW(
        cfg_checker->CheckKeyRule(config::ParagraphType::FILTER, "Filter: F1"));
    EXPECT_ANY_THROW(
        cfg_checker->CheckKeyRule(config::ParagraphType::FILTER, "Item: (AIR_0)"));
}

TEST_F(ConfigCheckerTest, KeyMandatoryViolation_BucketType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(
        config::ParagraphType::BUCKET, "Bounds: [0, 100), Scale: 10"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(
        config::ParagraphType::BUCKET, "Bucket: BUCKET_1, Scale: 10"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(
        config::ParagraphType::BUCKET, "Bucket: BUCKET_1, Bounds: [0, 100)"));
}

TEST_F(ConfigCheckerTest, KeyMandatoryViolation_NodeType)
{
    EXPECT_ANY_THROW(
        cfg_checker->CheckKeyRule(config::ParagraphType::NODE, "Node: PERF_CP"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(
        config::ParagraphType::NODE, "Type: PERFORMANCE"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Build: True, Run: On, Type: PERFORMANCE, Node: Test1, Filter: F2"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Run: On, Node: PERF_CP, Type: QUEUE, Group: G2"));
    EXPECT_ANY_THROW(
        cfg_checker->CheckKeyRule(config::ParagraphType::NODE, "Run: On"));
}

TEST_F(ConfigCheckerTest, KeyDuplicationViolation_DefaultType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, StreamingInterval: 10, AirBuild:True, "
        "NodeBuild:True, NodeRun:On, NodeSamplingRatio: 1000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, AirBuild: False, NodeBuild:True, "
        "NodeRun:On, NodeSamplingRatio: 1000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 1000, NodeBuild:True, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 1000, NodeRun:On, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "NodeSamplingRatio: 10, StreamingInterval:1, AirBuild:True, "
        "NodeBuild:True, NodeRun:On, NodeSamplingRatio: 1000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::DEFAULT,
        "NodeSamplingRatio: 10, NodeIndexSize: 100, StreamingInterval:1, "
        "AirBuild:True, NodeBuild:True, NodeRun:On, NodeIndexSize: 10"));
}

TEST_F(ConfigCheckerTest, KeyDuplicationViolation_GroupType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::GROUP,
        "Group: POS_RSC, NodeBuild: False, NodeRun: On, NodeSamplingRatio: 10, "
        "Group: POS_RSC"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::GROUP,
        "Group: POS_RSC, NodeBuild: False, NodeRun: On, NodeSamplingRatio: 10, "
        "NodeBuild: False"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::GROUP,
        "Group: POS_RSC, NodeBuild: False, NodeRun: On, NodeSamplingRatio: 10, "
        "NodeRun: On"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::GROUP,
        "Group: POS_RSC, NodeBuild: False, NodeRun: On, NodeSamplingRatio: 10, "
        "NodeSamplingRatio: 1"));
}

TEST_F(ConfigCheckerTest, KeyDuplicationViolation_FilterType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(
        config::ParagraphType::FILTER, "Filter: F1, Item: (AIR_0), Filter: F2"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(
        config::ParagraphType::FILTER, "Filter: F1, Item: (AIR_0), Item: (AIR_2)"));
}

TEST_F(ConfigCheckerTest, KeyDuplicationViolation_BucketType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10, Bucket: BUCKET_2"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10, Bounds: [0, 140)"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10, Scale: 100"));
}

TEST_F(ConfigCheckerTest, KeyDuplicationViolation_NodeType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: LAT_CP, Type: LATENCY, Build: True, Run: On, Group: POS_Journal, "
        "Node: LAT_CP"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: LAT_CP, Type: LATENCY, Type: LATENCY, Build: True, Run: On, Group: "
        "POS_Journal"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: LAT_CP, Type: LATENCY, Build: True, Build: True, Run: On, Group: "
        "POS_Journal"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: LAT_CP, Type: LATENCY, Build: True, Run: On, Run: On, Group: "
        "POS_Journal"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: LAT_CP, Type: LATENCY, Build: True, Run: On, Group: POS_Journal, "
        "Filter: F1, Filter: F2"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: LAT_CP, Type: LATENCY, Build: True, Run: On, Group: POS_Journal, "
        "Group: POS_Journal"));
    EXPECT_ANY_THROW(cfg_checker->CheckKeyRule(config::ParagraphType::NODE,
        "Node: HIST_PSD, Type : HISTOGRAM, Filter: Basic,  Group: POS, Bucket: "
        "BUCKET_2, Bucket: BUCKET_4"));
}

TEST_F(ConfigCheckerTest, CheckValueRulePass)
{
    EXPECT_EQ(0,
        cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
            "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
            "NodeSamplingRatio: 1000, NodeIndexSize: 30"));
    EXPECT_EQ(0,
        cfg_checker->CheckValueRule(config::ParagraphType::GROUP,
            "Group: POS_RSC, NodeBuild: False, NodeRun: On, NodeSamplingRatio: "
            "100"));
    EXPECT_EQ(0,
        cfg_checker->CheckValueRule(
            config::ParagraphType::GROUP, "Group: POS_RSC"));
    EXPECT_EQ(0,
        cfg_checker->CheckValueRule(
            config::ParagraphType::FILTER, "Filter: F1, Item: (BASE_0 , BASE1)"));
    EXPECT_EQ(0,
        cfg_checker->CheckValueRule(config::ParagraphType::FILTER,
            "Filter: F2, Item: (BASE_0 ... BASE_3)"));
    EXPECT_EQ(0,
        cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
            "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10"));
    EXPECT_EQ(0,
        cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
            "Scale: 3, Bucket: BUCKET_2, Bounds: [33, 66)"));
    EXPECT_EQ(0,
        cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
            "Bounds: [-100, 80), Scale: 20, Bucket: BUCKET_3"));
    EXPECT_EQ(0,
        cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
            "Bucket: BUCKET_4, Bounds: [2^0, 2^10), Scale: 2^"));
    EXPECT_EQ(0,
        cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
            "Bucket: BUCKET_5, Bounds: (-4^6, -4^2], Scale: 2^"));
    EXPECT_EQ(0,
        cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
            "Bucket: BUCKET_6, Bounds: (-10^3, 10^5), Scale: 10^"));
    EXPECT_EQ(0,
        cfg_checker->CheckValueRule(config::ParagraphType::NODE,
            "Node: LAT_CP, Type: LATENCY, Build: True, Run: On, Group: POS, "
            "Filter: Basic"));
}

TEST_F(ConfigCheckerTest, ValueValidityViolation_NodeType_ForbiddenWords)
{
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::FILTER,
        "Filter: FILTER_IoGroup, Item: (F1_AAA, F1_BBB)"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::GROUP,
        "Group: NodeA, NodeBuild: True, NodeRun: On"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::NODE,
        "Node: PERF_TypeA, Type: PERFORMANCE, Build: True, Run: On, "
        "Group: POS_JOURNAL"));
}

TEST_F(ConfigCheckerTest, ValueValidityViolation_DefaultType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:199, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 1000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval: b , AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 1000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval: 1a, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 1000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval: aaaaaa, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 1000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild: 100, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 1000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:-99, NodeRun:On, "
        "NodeSamplingRatio: 1000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:abcd, "
        "NodeSamplingRatio: 1000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:abcd, "
        "NodeSamplingRatio: 1000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:abcd, "
        "NodeSamplingRatio: 10000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: -1, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 0.0000000000000001, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 0.a001, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 1ra01, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 1000, NodeIndexSize: -10"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 100000, NodeIndexSize:32"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 1000, NodeIndexSize: 10000000000"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 1000, NodeIndexSize: 5000000000"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::DEFAULT,
        "StreamingInterval:1, AirBuild:True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 1000, NodeIndexSize: 4294967296"));
}

TEST_F(ConfigCheckerTest, ValueValidityViolation_GroupType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::GROUP,
        "Group: CANNOT_EXCEED_30_CHARACTERS_IN_STRING_TYPE_DATA, NodeBuild: False, "
        "NodeRun: On, NodeSamplingRatio: 10"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::GROUP,
        "Group: POS_RSC, NodeBuild: 123, NodeRun: On, NodeSamplingRatio: 10"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::GROUP,
        "Group: POS_RSC, NodeBuild: False, NodeRun: 123, NodeSamplingRatio: 10"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::GROUP,
        "Group: POS_RSC, NodeBuild: False, NodeRun: On, NodeSamplingRatio: 0"));
}

TEST_F(ConfigCheckerTest, ValueValidityViolation_FilterType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::FILTER,
        "Filter: CANNOT_EXCEED_30_CHARACTERS_IN_STRING_TYPE_DATA, Item: (F1_AAA, "
        "F1_BBB)"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::FILTER,
        "Filter: F2, Item: (F2_0... F2_3), Run: False"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(
        config::ParagraphType::FILTER, "Filter: F3, Item: (F3_0, ... F3_2)"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(
        config::ParagraphType::FILTER, "Filter: F4, Item: (F4_0 F4_1)"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(
        config::ParagraphType::FILTER, "Filter: F4, Item: (.)"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(
        config::ParagraphType::FILTER, "Filter: F4, Item: (..)"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(
        config::ParagraphType::FILTER, "Filter: F4, Item: ()"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(
        config::ParagraphType::FILTER, "Filter: F4, Item: (     )"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(
        config::ParagraphType::FILTER, "Filter: F5, Item: (F5_0 ... )"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(
        config::ParagraphType::FILTER, "Filter: F5, Item: ( ... F5_3 )"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::FILTER,
        "Filter: F5, Item: (F5_1 ... F5)")); // underscope(_) missing
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::FILTER,
        "Filter: F5, Item: (F5 ... F5_3)")); // underscope(_) missing
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::FILTER,
        "Filter: F5, Item: (F5_3 ... F55_6)")); // prefix has to be same
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::FILTER,
        "Filter: F5, Item: (F5_a3 ... F5_3)")); // suffix has to be integer
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::FILTER,
        "Filter: F5, Item: (F5_1 ... F5_-3)")); // suffix has to be integer
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::FILTER,
        "Filter: F5, Item: (F5_13 ... F5_3)")); // suffix underflow
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::FILTER,
        "Filter: F5, Item: (F5_42 ... F5_42)")); // suffix underflow
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::FILTER,
        "Filter: F5, Item: (F5_0 ... F5_100)")); // max size: 100 (suffix overflow)
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(
        config::ParagraphType::FILTER, "Filter: F6, Item: (, F6_1)"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(
        config::ParagraphType::FILTER, "Filter: F6, Item: (F6_0, F6_1 F6_2)"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(
        config::ParagraphType::FILTER, "Filter: F6, Item: (F6_0,, F6_1)"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(
        config::ParagraphType::FILTER, "Filter: F6, Item: (F6_0, F6_1 F6_2, )"));
}

TEST_F(ConfigCheckerTest, ValueValidityViolation_BucketType_ScaleMissing)
{
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_1, Bounds: [0, 100), Scale: "));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_1, Bounds: (-100, 100), Scale: "));
}

TEST_F(ConfigCheckerTest, ValueValidityViolation_BucketType_InvalidScale)
{
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_1, Bounds: [0, 100), Scale: -10"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10.0"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: [2^0, 2^10), Scale: 3^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: [8, 16), Scale: -2^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_5, Bounds: (-4^6, -4^2], Scale: 16^"));
}

TEST_F(ConfigCheckerTest, ValueValidityViolation_BucketType_BucketSizeOverflow)
{
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 2"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_1, Bounds: (-2^9, 2^9), Scale: 2^"));
}

TEST_F(ConfigCheckerTest, ValueValidityViolation_BucketType_BucketSizeUnderflow)
{
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: [8, 4), Scale: 2^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: [8, 8), Scale: 2^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: (-9, -27], Scale: 3^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: [-10, -22), Scale: 2"));
}

TEST_F(ConfigCheckerTest, ValueValidityViolation_BucketType_MissingBounds)
{
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(
        config::ParagraphType::BUCKET, "Bucket: BUCKET_1, Bounds: , Scale: 10"));
}

TEST_F(ConfigCheckerTest, ValueValidityViolation_BucketType_MissingBrace)
{
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_1, Bounds: [0, 100, Scale: 10"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_1, Bounds: 0, 100), Scale: 10"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_1, Bounds: 0, 100, Scale: 10"));
}

TEST_F(ConfigCheckerTest, ValueValidityViolation_BucketType_MissingComma)
{
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: [2^0 2^10), Scale: 2^"));
}

TEST_F(ConfigCheckerTest, ValueValidityViolation_BucketType_InvalidBrace)
{
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: (2^0, 2^10), Scale: 2^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: [2^0, 2^10], Scale: 2^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: (2^0, 2^10], Scale: 2^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_5, Bounds: (-4^6, -4^2), Scale: 2^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_5, Bounds: [-4^6, -4^2], Scale: 2^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_5, Bounds: [-4^6, -4^2), Scale: 2^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_6, Bounds: [-10^3, 10^5), Scale: 10^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_6, Bounds: (-10^3, 10^5], Scale: 10^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_6, Bounds: [-10^3, 10^5], Scale: 10^"));
}

TEST_F(ConfigCheckerTest, ValueValidityViolation_BucketType_InvalidBounds)
{
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: [3, 2^10), Scale: 2^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: [4, 1023), Scale: 2^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: [4, 1025), Scale: 2^"));
}

TEST_F(ConfigCheckerTest, ValueValidityViolation_BucketType_BoundsOverflow)
{
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: [10^12, 10^19), Scale: 10^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: (-10^19, -10^3], Scale: 10^"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: [0, 10^19), Scale: 100000000000000000"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::BUCKET,
        "Bucket: BUCKET_4, Bounds: [-10^19, 0), Scale: 100000000000000000"));
}

TEST_F(ConfigCheckerTest, ValueValidityViolation_NodeType)
{
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::NODE,
        "Node: CANNOT_EXCEED_30_CHARACTERS_IN_STRING_TYPE_DATA, Type: PERFORMANCE, "
        "Build: True, Run: On, SamplingRatio:1000, Group: POS_JOURNAL, Filter: "
        "Basic"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::NODE,
        "Node: PERF_CP, Type: NOTYPE, Build: True, Run: On, SamplingRatio:1000, "
        "Group: POS_JOURNAL"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::NODE,
        "Node: PERF_CP, Type: PERFORMANCE, Build: 123, Run: On, "
        "SamplingRatio:1000, Group: POS_JOURNAL"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::NODE,
        "Node: PERF_CP, Type: PERFORMANCE, Build: True, Run: 123, "
        "SamplingRatio:1000, Group: POS_JOURNAL"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::NODE,
        "Node: PERF_CP, Type: PERFORMANCE, Build: True, Run: On, "
        "SamplingRatio:0.9, Group: POS_JOURNAL"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::NODE,
        "Node: PERF_CP, Type: PERFORMANCE, Build: True, Run: On, "
        "SamplingRatio:1000, Group: POS_JOURNALLLL"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::NODE,
        "Node: PERF_CP, Type: PERFORMANCE, Build: True, Run: On, "
        "SamplingRatio:1000, Group: POS_JOURNAL, Filter: Basiccc"));
    EXPECT_ANY_THROW(cfg_checker->CheckValueRule(config::ParagraphType::NODE,
        "Node: HIST_PSD, Type : HISTOGRAM, Filter: Basic,  Group: POS, Bucket: "
        "BUCKET_NOT_EXIST"));
}
