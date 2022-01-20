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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>

#include "src/lib/StringView.h"

TEST(StringView, Size)
{
    air::string_view a_str1 = "hello i am a string_view";
    EXPECT_EQ(24, a_str1.size());

    air::string_view a_str2 = "hello ";
    EXPECT_EQ(6, a_str2.size());
}

TEST(StringView, Compare)
{
    air::string_view a_str = "hello i am a string_view";
    EXPECT_EQ(0, a_str.compare("hello i am a string_view"));
    EXPECT_EQ(-1, a_str.compare("hello i am a string_vie"));
}

TEST(StringView, OperatorEqual)
{
    air::string_view a_str = "hi there";
    air::string_view a_str2 = "hi there";
    EXPECT_TRUE(a_str == a_str2);
}

TEST(StringView, Find)
{
    air::string_view a_str = "hello i am a string_view";
    EXPECT_EQ(0, a_str.find("h"));
    EXPECT_EQ(0, a_str.find("hello"));
    EXPECT_EQ(6, a_str.find("i am"));
    EXPECT_EQ(13, a_str.find("str"));
    EXPECT_EQ(23, a_str.find("w"));
}

TEST(StringView, Substr)
{
    air::string_view a_str = "hello i am a string_view";
    EXPECT_EQ(0, a_str.substr(0, 0).compare(""));
    EXPECT_EQ(0, a_str.substr(0, 1).compare("h"));
    EXPECT_EQ(0, a_str.substr(0, 5).compare("hello"));
    EXPECT_EQ(0, a_str.substr(13, 11).compare("string_view"));
    EXPECT_EQ(0, a_str.substr(23, 1).compare("w"));
    EXPECT_TRUE(a_str.substr(13, 12) == "string_view");
    EXPECT_TRUE(a_str.substr(13, 34) == "string_view");
}

TEST(StringView, OperatorStream)
{
    air::string_view a_str = "hi there";
    std::cout << a_str << "!\n";
}

TEST(StringView, ToString)
{
    air::string_view a_str = "hi there";
    std::string str = "";
    str.assign(a_str.data(), a_str.size());
    EXPECT_TRUE(str == "hi there");
}

TEST(StringView, ParsingCase1)
{
    air::string_view sentence =
        "StreamingInterval:1, AirBuild  : True, NodeBuild:True, NodeRun:On, "
        "NodeSamplingRatio: 1000, NodeIndexSize:32";
    size_t start_pos = sentence.find("AirBuild");
    EXPECT_FALSE(air::string_view::npos == start_pos);

    size_t comma_pos = sentence.find(",", start_pos + 1);
    air::string_view key_value = sentence.substr(start_pos, comma_pos - start_pos);
    EXPECT_TRUE(key_value == "AirBuild  : True");

    size_t colon_pos = key_value.find(":");
    air::string_view value =
        key_value.substr(colon_pos + 1, key_value.size() - colon_pos);
    EXPECT_TRUE(value == " True");
}
