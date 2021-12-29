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

#include "type_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST_F(TypeTest, Node)
{
    EXPECT_EQ((uint32_t)cfg::GetSentenceIndex(config::ParagraphType::NODE, "PERF_PSD"), node_meta.nid);
    EXPECT_EQ(air::ProcessorType::PERFORMANCE, node_meta.processor_type);
    EXPECT_EQ(true, node_meta.run);
    EXPECT_EQ(1000, node_meta.sample_ratio);

    node_meta.nid = (uint32_t)cfg::GetSentenceIndex(config::ParagraphType::NODE, "PERF_VOLUME");
    EXPECT_EQ(0, cfg::GetSentenceName(config::ParagraphType::NODE, node_meta.nid).compare("PERF_VOLUME"));
    node_meta.nid = (uint32_t)cfg::GetSentenceIndex(config::ParagraphType::NODE, "LAT_PSD");
    EXPECT_EQ(0, cfg::GetSentenceName(config::ParagraphType::NODE, node_meta.nid).compare("LAT_PSD"));
    node_meta.nid = (uint32_t)cfg::GetSentenceIndex(config::ParagraphType::NODE, "LAT_METAFS");
    EXPECT_EQ(0, cfg::GetSentenceName(config::ParagraphType::NODE, node_meta.nid).compare("LAT_METAFS"));
    node_meta.nid = (uint32_t)cfg::GetSentenceIndex(config::ParagraphType::NODE, "LAT_REBUILD");
    EXPECT_EQ(0, cfg::GetSentenceName(config::ParagraphType::NODE, node_meta.nid).compare("LAT_REBUILD"));
    node_meta.nid = (uint32_t)cfg::GetSentenceIndex(config::ParagraphType::NODE, "Q_SUBMIT");
    EXPECT_EQ(0, cfg::GetSentenceName(config::ParagraphType::NODE, node_meta.nid).compare("Q_SUBMIT"));
    node_meta.nid = (uint32_t)cfg::GetSentenceIndex(config::ParagraphType::NODE, "Q_IOWORER");
    EXPECT_EQ(0, cfg::GetSentenceName(config::ParagraphType::NODE, node_meta.nid).compare("Q_IOWORER"));
}
