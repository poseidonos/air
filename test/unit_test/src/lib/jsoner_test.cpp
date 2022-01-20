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

#include <sstream>

#include "src/lib/json/Json.h"

TEST(JsonerTest, json)
{
    auto& obj = air::json("obj");
    obj["bool"] = {true};
    obj["int"] = {123};
    obj["str"] = {"string"};

    auto& obj2 = air::json("obj");

    std::stringstream stream_obj;
    stream_obj << obj2;
    std::string str = stream_obj.str();
    EXPECT_EQ(
        0, str.compare("{\"bool\": true, \"int\": 123, \"str\": \"string\"}"));
}

TEST(JsonerTest, json_clear)
{
    auto& obj = air::json("obj");
    obj["bool"] = {true};
    obj["int"] = {123};
    obj["str"] = {"string"};

    air::json_clear();
    auto& obj2 = air::json("obj");

    std::stringstream stream_obj;
    stream_obj << obj2;
    std::string str = stream_obj.str();
    EXPECT_EQ(0, str.compare("{}"));
}

TEST(JsonerTest, json_copy)
{
    auto& obj = air::json("obj");
    obj["bool"] = {true};
    obj["int"] = {123};
    obj["str"] = {"string"};
    obj["double_arr"] += {4.44};
    auto& obj2 = air::json("obj2");
    obj2["str2"] = {"strstr"};
    obj["obj"] = {obj2};
    obj["int_arr"] = {1, 2, 3};
    obj["multi_arr"] += {"str"};
    obj["multi_arr"] += {5};
    auto& obj3 = air::json("obj3");
    obj3["dd"] = {394.2311};
    obj["multi_arr"] += {obj3};
    std::stringstream stream_obj;
    stream_obj << obj;
    std::string str = stream_obj.str();
    EXPECT_EQ(0,
        str.compare(
            "{\"bool\": true, \"double_arr\": [4.44], \"int\": 123, \"int_arr\": "
            "[1, 2, 3], \"multi_arr\": [\"str\", 5, {\"dd\": 394.231}], \"obj\": "
            "{\"str2\": \"strstr\"}, \"str\": \"string\"}"));

    auto copy_obj = air::json_copy("obj");
    std::stringstream stream_copy_obj;
    stream_copy_obj << copy_obj;
    std::string copy_str = stream_copy_obj.str();
    EXPECT_EQ(0,
        copy_str.compare(
            "{\"bool\": true, \"double_arr\": [4.44], \"int\": 123, \"int_arr\": "
            "[1, 2, 3], \"multi_arr\": [\"str\", 5, {\"dd\": 394.231}], \"obj\": "
            "{\"str2\": \"strstr\"}, \"str\": \"string\"}"));

    air::json_clear();
    std::stringstream stream_copy_obj2;
    stream_copy_obj2 << copy_obj;
    std::string copy_str2 = stream_copy_obj2.str();
    EXPECT_EQ(0,
        copy_str2.compare(
            "{\"bool\": true, \"double_arr\": [4.44], \"int\": 123, \"int_arr\": "
            "[1, 2, 3], \"multi_arr\": [\"str\", 5, {\"dd\": 394.231}], \"obj\": "
            "{\"str2\": \"strstr\"}, \"str\": \"string\"}"));

    copy_obj.Clear();
    std::stringstream stream_copy_obj3;
    stream_copy_obj3 << copy_obj;
    std::string copy_str3 = stream_copy_obj3.str();
    EXPECT_EQ(0, copy_str3.compare("{}"));
}
