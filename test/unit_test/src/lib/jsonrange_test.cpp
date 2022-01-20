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
#include <sstream>

#include "src/lib/json/Json.h"

TEST(JsonrangeTest, Object)
{
    auto& obj = air::json("obj");
    obj["int_arr"] = {1, 2, 3};
    obj["str"] = {"string"};
    obj["bool"] = {true};
    obj["double_arr"] += {4.44};
    auto& obj2 = air::json("obj2");
    obj2["str2"] = {"strstr"};
    obj["obj"] = {obj2};

    for (auto i : air::range(obj))
    {
        std::stringstream stream_value;
        stream_value << obj[i.key];
        std::string str_value = stream_value.str();

        if (0 == i.key.compare("bool"))
        {
            EXPECT_EQ(0, str_value.compare("true"));
        }
        else if (0 == i.key.compare("double_arr"))
        {
            EXPECT_EQ(0, str_value.compare("[4.44]"));
        }
        else if (0 == i.key.compare("int_arr"))
        {
            EXPECT_EQ(0, str_value.compare("[1, 2, 3]"));
        }
        else if (0 == i.key.compare("obj"))
        {
            EXPECT_EQ(0, str_value.compare("{\"str2\": \"strstr\"}"));
        }
        else if (0 == i.key.compare("str"))
        {
            EXPECT_EQ(0, str_value.compare("\"string\""));
        }
        else
        {
            FAIL();
        }
    }

    air::json_clear();
}

TEST(JsonrangeTest, Array)
{
    auto& obj = air::json("obj");
    auto& arr = air::json("arr");
    arr["bucket"] += {100};
    arr["bucket"] += {200, 300};
    obj["arr"] = {arr};

    auto& bucket = arr["bucket"];
    bucket += {400, 500};

    for (auto i : air::range(bucket))
    {
        int32_t value {*static_cast<int32_t*>(i.value)};
        if (0 == i.key.compare("00"))
        {
            EXPECT_EQ(100, value);
        }
        else if (0 == i.key.compare("01"))
        {
            EXPECT_EQ(200, value);
        }
        else if (0 == i.key.compare("02"))
        {
            EXPECT_EQ(300, value);
        }
        else if (0 == i.key.compare("03"))
        {
            EXPECT_EQ(400, value);
        }
        else if (0 == i.key.compare("04"))
        {
            EXPECT_EQ(500, value);
        }
        else
        {
            FAIL();
        }
    }

    air::json_clear();
}
