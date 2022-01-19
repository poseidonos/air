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

#include "src/lib/json/Json.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <deque>
#include <fstream>
#include <iostream>

TEST(JsondocTest, UseCase1)
{
    auto& aid0 = air::json("aid0");
    aid0["aid"] = {0};
    aid0["iops_read"] = {979265};
    aid0["iops_write"] = {103239};
    aid0["cnt_1"] = {"4096(sz)-1082504(cnt)"};
    auto& aid1 = air::json("aid1");
    aid1["aid"] = {1};
    aid1["iops_read"] = {301422};
    aid1["iops_write"] = {2144};
    aid1["cnt_1"] = {"4096(sz)-303466(cnt)"};
    auto& aid2 = air::json("aid2");
    aid2["aid"] = {2};
    aid2["iops_read"] = {222222};
    aid2["iops_write"] = {2222};
    aid2["cnt_1"] = {"4096(sz)-224444(cnt)"};
    auto& tid0 = air::json("tid0");
    tid0["tid"] = {30234};
    tid0["tname"] = {"Reactor01"};
    tid0["aid_arr"] += {aid0, aid1, aid2};
    auto& perf = air::json("perf");
    perf["id"] = {0};
    perf["name"] = {"PERF_VOL"};
    perf["tid_arr"] = {tid0};
    auto& air = air::json("air");
    air["timestamp"] = {15849345};
    air["status"] = {"normal"};
    air["interval"] = {1};
    air["node"] += {perf};

    std::ofstream write_file;
    write_file.open("JsonerTest_UseCase1.txt");
    write_file << air << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("JsonerTest_UseCase1.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    EXPECT_EQ(0, read_line.compare("{\"interval\": 1, \"node\": [{\"id\": 0, \"name\": \"PERF_VOL\", \"tid_arr\": {\"aid_arr\": [{\"aid\": 0, \"cnt_1\": \"4096(sz)-1082504(cnt)\", \"iops_read\": 979265, \"iops_write\": 103239}, {\"aid\": 1, \"cnt_1\": \"4096(sz)-303466(cnt)\", \"iops_read\": 301422, \"iops_write\": 2144}, {\"aid\": 2, \"cnt_1\": \"4096(sz)-224444(cnt)\", \"iops_read\": 222222, \"iops_write\": 2222}], \"tid\": 30234, \"tname\": \"Reactor01\"}}], \"status\": \"normal\", \"timestamp\": 15849345}"));
}

TEST(JsondocTest, UseCase2)
{
    air::json("obj") = {};
    air::json("obj")["timestamp"] = {101, 102, 103, 104};
    air::json("obj")["name"] += {"name1", "name2"};

    auto& obj2 = air::json("obj2");
    obj2["age"] = {24, 22};

    auto& obj = air::json("obj");
    obj2["gender"] = {"male", "female"};
    obj["gender"] += {obj2["gender"]};

    std::ofstream write_file;
    write_file.open("JsonerTest_UseCase2.txt");
    write_file << obj << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("JsonerTest_UseCase2.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    EXPECT_EQ(0, read_line.compare("{\"gender\": [[\"male\", \"female\"]], \"name\": [\"name1\", \"name2\"], \"timestamp\": [101, 102, 103, 104]}"));
}

TEST(JsondocTest, jsondocApiException)
{
    auto& obj = air::json("obj");

    try
    {
        obj = {"invalid usage"};
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        obj += {1};
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        obj += {"appending const cp"};
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        obj += {air::json("obj2")};
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        uint8_t uint8_value{1};
        obj["uint8_t"] += {uint8_value};
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    auto& obj_whitespace = air::json("obj_whitespace");
    obj_whitespace.SetType(air::JSONtype::WHITESPACE);
    std::cout << obj_whitespace << std::endl;

    auto& obj_undefined = air::json("obj_undefined");
    obj_undefined.SetType(air::JSONtype::UNDEFINED);
    try
    {
        std::cout << obj_undefined << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    air::json_clear();
}

TEST(JsondocTest, jsondocClearException)
{
    auto& obj = air::json("obj");
    long long longlong_value{1234};
    obj["longlong"] = {longlong_value};
    unsigned long long ulonglong_value{1234};
    obj["ulonglong"] = {ulonglong_value};
    obj = {};

    try
    {
        obj.DeleteValue(air::JSONtype::ARRAY, nullptr);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    air::json_clear();
}

TEST(JsondocTest, Compound)
{
    auto& obj = air::json("obj");
    obj["interval"] = {3};
    obj["timestamp"] = {1639469944};
    auto& groupTop = air::json("groupTop");
    auto& groupA = air::json("groupA");
    auto& nodeA = air::json("nodeA");
    auto& nodeApple = air::json("nodeApple");
    auto& nodeAnt = air::json("nodeAnt");
    auto& groupB = air::json("groupB");
    auto& nodeB = air::json("nodeB");
    auto& nodeBanana = air::json("nodeBanana");
    auto& nodeBacon = air::json("nodeBacon");
    nodeApple["color"] = {"red"};
    nodeApple["weight(kg)"] = {0.93};
    nodeAnt["color"] = {"black"};
    nodeAnt["type"] = {"queen"};
    nodeA["Apple"] = {nodeApple};
    nodeA["Ant"] = {nodeAnt};
    groupA["node"] = {nodeA};
    groupTop["groupA"] = {groupA};
    nodeBanana["color"] = {"yellow"};
    nodeBanana["weight(kg)"] = {2.34};
    nodeBacon["color"] = {"pink"};
    nodeB["Banana"] = {nodeBanana};
    nodeB["Bacon"] = {nodeBacon};
    groupB["node"] = {nodeB};
    groupTop["groupB"] = {groupB};
    obj["group"] = {groupTop};

    std::deque<std::string> q{"Apple", "Banana"};
    auto compound = obj.Compound(q);

    std::stringstream stream;
    stream << compound;
    std::string str = stream.str();
    EXPECT_EQ(0, str.compare("{\"Apple\": {\"color\": \"red\", \"weight(kg)\": 0.93}, \"Banana\": {\"color\": \"yellow\", \"weight(kg)\": 2.34}, \"interval\": 3, \"timestamp\": 1639469944}"));

    air::json_clear();
}

TEST(JsondocTest, GetType_Array)
{
    auto& obj = air::json("obj");
    auto& arr = air::json("arr");
    arr["bucket"] += {100};
    arr["bucket"] += {200, 300};
    obj["arr"] = {arr};

    auto& bucket = arr["bucket"];
    bucket += {400, 500};

    // 0: Object
    EXPECT_EQ(0, (uint32_t)arr.GetType());

    // 1: Array
    EXPECT_EQ(1, (uint32_t)bucket.GetType());
    EXPECT_EQ(1, (uint32_t)obj["arr"]["bucket"].GetType());
    EXPECT_EQ(1, (uint32_t)arr.GetType("bucket"));
    EXPECT_EQ(1, (uint32_t)obj["arr"].GetType("bucket"));

    air::json_clear();
}

TEST(JsondocTest, GetType_Number)
{
    auto& obj = air::json("obj");
    auto& integer = air::json("integer");
    integer["num"] = {100};
    obj["integer"] = {integer};
    auto& rational_num = air::json("rational_num");
    rational_num["num"] = {333.5};
    obj["rational_num"] = {rational_num};

    // 11: INT32
    EXPECT_EQ(11, (uint32_t)integer["num"].GetType());
    EXPECT_EQ(11, (uint32_t)integer.GetType("num"));

    // 17: DOUBLE
    EXPECT_EQ(17, (uint32_t)rational_num["num"].GetType());
    EXPECT_EQ(17, (uint32_t)rational_num.GetType("num"));

    air::json_clear();
}

TEST(JsondocTest, ArrayOrder)
{
    auto& obj = air::json("obj");
    obj["arr"] += {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    std::stringstream stream;
    stream << obj["arr"];
    std::string str = stream.str();
    EXPECT_EQ(0, str.compare("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]"));

    air::json_clear();
}
