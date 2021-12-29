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

#include "json_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>

TEST_F(JsonTest, json_value_type_number)
{
    auto& obj = air::json("obj");

    int16_t int16_value = -1;
    obj["int16"] = {int16_value};
    int32_t int32_value = -2;
    obj["int32"] = {int32_value};
    int64_t int64_value = -3;
    obj["int64"] = {int64_value};

    uint16_t uint16_value = 1;
    obj["uint16"] = {uint16_value};
    uint32_t uint32_value = 2;
    obj["uint32"] = {uint32_value};
    uint64_t uint64_value = 3;
    obj["uint64"] = {uint64_value};

    float float_value = 100.77;
    obj["float"] = {float_value};
    double double_value = 1.23;
    obj["double"] = {double_value};

    for (auto i : air::range(obj))
    {
        switch (i.type)
        {
            case air::JSONtype::INT16:
            {
                void* vp = i.value;
                int16_t value = *((int16_t*)vp);
                EXPECT_EQ(-1, value);
                break;
            }
            case air::JSONtype::INT32:
            {
                void* vp = i.value;
                int32_t value = *((int32_t*)vp);
                EXPECT_EQ(-2, value);
                break;
            }
            case air::JSONtype::INT64:
            {
                void* vp = i.value;
                int64_t value = *((int64_t*)vp);
                EXPECT_EQ(-3, value);
                break;
            }
            case air::JSONtype::UINT16:
            {
                void* vp = i.value;
                uint16_t value = *((uint16_t*)vp);
                EXPECT_EQ((uint16_t)1, value);
                break;
            }
            case air::JSONtype::UINT32:
            {
                void* vp = i.value;
                uint32_t value = *((uint32_t*)vp);
                EXPECT_EQ((uint32_t)2, value);
                break;
            }
            case air::JSONtype::UINT64:
            {
                void* vp = i.value;
                uint64_t value = *((uint64_t*)vp);
                EXPECT_EQ((uint64_t)3, value);
                break;
            }
            case air::JSONtype::FLOAT:
            {
                void* vp = i.value;
                float value = *((float*)vp);
                EXPECT_EQ(true, ((100.7 < value) && (100.8 > value)));
                break;
            }
            case air::JSONtype::DOUBLE:
            {
                void* vp = i.value;
                double value = *((double*)vp);
                EXPECT_EQ(true, ((1.22 < value) && (1.24 > value)));
                break;
            }
            default:
            {
                break;
            }
        }
    }
    air::json_clear();
}

TEST_F(JsonTest, json_value_type_bool)
{
    auto& obj = air::json("obj");

    obj["bool_true"] = {true};
    obj["bool_false"] = {false};

    for (auto i : air::range(obj))
    {
        if (0 == i.key.compare("bool_true"))
        {
            void* vp = i.value;
            bool value = *((bool*)vp);
            EXPECT_EQ(true, value);
        }
        else if (0 == i.key.compare("bool_false"))
        {
            void* vp = i.value;
            bool value = *((bool*)vp);
            EXPECT_EQ(false, value);
        }
    }
    air::json_clear();
}

TEST_F(JsonTest, json_value_type_string)
{
    auto& obj = air::json("obj");

    obj["charp"] = {"const char p"};
    obj["string"] = {std::string("stringstring")};

    for (auto i : air::range(obj))
    {
        if (0 == i.key.compare("charp"))
        {
            void* vp = i.value;
            std::string value = *((std::string*)vp);
            EXPECT_EQ(0, value.compare("const char p"));
        }
        else if (0 == i.key.compare("string"))
        {
            void* vp = i.value;
            std::string value = *((std::string*)vp);
            EXPECT_EQ(0, value.compare("stringstring"));
        }
    }
    air::json_clear();
}

TEST_F(JsonTest, json_value_type_null)
{
    auto& obj = air::json("obj");

    obj["null"] = {nullptr};

    for (auto i : air::range(obj))
    {
        if (0 == i.key.compare("null"))
        {
            EXPECT_EQ(true, air::JSONtype::NULLVAL == i.type);
        }
    }
    air::json_clear();
}

TEST_F(JsonTest, json_export)
{
    auto& obj = air::json("obj");

    obj["num1"] = {1, 2, 3};
    obj["null"] = {nullptr};

    std::ofstream write_file;
    write_file.open("json_export.txt");
    write_file << obj << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("json_export.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    EXPECT_EQ(0, read_line.compare("{\"null\": null, \"num1\": [1, 2, 3]}"));
}

TEST_F(JsonTest, json_value_type_array)
{
    auto& obj = air::json("obj");

    obj["arr1"] += {1};
    obj["arr1"] += {2.2, 3.3};
    obj["arr1"] += {nullptr};

    obj["arr2"] = {"a", "b", "c"};
    obj["arr2"] += {std::string("def")};
    obj["arr2"] += {true, false};

    std::ofstream write_file;
    write_file.open("json_value_type_array.txt");
    write_file << obj << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("json_value_type_array.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    EXPECT_EQ(0, read_line.compare("{\"arr1\": [1, 2.2, 3.3, null], \"arr2\": [\"a\", \"b\", \"c\", \"def\", true, false]}"));
}

TEST_F(JsonTest, json_value_type_object)
{
    auto& obj = air::json("obj");

    obj["sub_1"] = {air::json("sub_1")};
    auto& sub_2 = air::json("sub_2");
    sub_2["s2_item"];
    obj["sub_2"] = {sub_2};

    obj["obj_item"] = {"item1"};
    air::json("sub_1")["s1_item"] = {"itemS1"};
    sub_2["s2_item"] = {"itemS2"};

    std::ofstream write_file;
    write_file.open("json_value_type_object.txt");
    write_file << obj << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("json_value_type_object.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    EXPECT_EQ(0, read_line.compare("{\"obj_item\": \"item1\", \"sub_1\": {}, \"sub_2\": {\"s2_item\": \"itemS2\"}}"));
}

TEST_F(JsonTest, json_print)
{
    auto& obj = air::json("obj");

    long long longlong_value = 1234;
    obj["longlong"] = {longlong_value};
    unsigned long long ulonglong_value = 1234;
    obj["ulonglong"] = {ulonglong_value};

    int16_t int16_value = -1;
    obj["int16"] = {int16_value};
    int32_t int32_value = -2;
    obj["int32"] = {int32_value};
    int64_t int64_value = -3;
    obj["int64"] = {int64_value};

    uint16_t uint16_value = 1;
    obj["uint16"] = {uint16_value};
    uint32_t uint32_value = 2;
    obj["uint32"] = {uint32_value};
    uint64_t uint64_value = 3;
    obj["uint64"] = {uint64_value};

    float float_value = 100.77;
    obj["float"] = {float_value};
    double double_value = 1.23;
    obj["double"] = {double_value};

    air::json_clear();
}

TEST_F(JsonTest, json_range)
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
        // i.key : obj[i.key]
    }

    air::json_clear();
}

TEST_F(JsonTest, json_use_case_1)
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
    write_file.open("json_use_case_1.txt");
    write_file << air << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("json_use_case_1.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    EXPECT_EQ(0, read_line.compare("{\"interval\": 1, \"node\": [{\"id\": 0, \"name\": \"PERF_VOL\", \"tid_arr\": {\"aid_arr\": [{\"aid\": 0, \"cnt_1\": \"4096(sz)-1082504(cnt)\", \"iops_read\": 979265, \"iops_write\": 103239}, {\"aid\": 1, \"cnt_1\": \"4096(sz)-303466(cnt)\", \"iops_read\": 301422, \"iops_write\": 2144}, {\"aid\": 2, \"cnt_1\": \"4096(sz)-224444(cnt)\", \"iops_read\": 222222, \"iops_write\": 2222}], \"tid\": 30234, \"tname\": \"Reactor01\"}}], \"status\": \"normal\", \"timestamp\": 15849345}"));
}

TEST_F(JsonTest, json_use_case_2)
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
    write_file.open("json_use_case_2.txt");
    write_file << obj << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("json_use_case_2.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    EXPECT_EQ(0, read_line.compare("{\"gender\": [[\"male\", \"female\"]], \"name\": [\"name1\", \"name2\"], \"timestamp\": [101, 102, 103, 104]}"));
}

TEST_F(JsonTest, json_parse_case_1)
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
    write_file.open("json_parse_case_1.txt");
    write_file << obj << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("json_parse_case_1.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    auto& parsed_obj = air::parse("parsed_data", read_line);

    write_file.open("json_parse_case_1_parsed.txt");
    write_file << parsed_obj << std::endl;
    write_file.close();

    air::json_clear();

    read_file.open("json_parse_case_1_parsed.txt");
    std::string read_line_parsed;
    std::getline(read_file, read_line_parsed);
    read_file.close();

    EXPECT_EQ(0, read_line.compare(read_line_parsed));
}

TEST_F(JsonTest, json_parse_case_2)
{
    auto& obj = air::json("obj");
    auto& sub = air::json("sub");
    auto& subsub = air::json("subsub");

    subsub["heart"] = {"beating"};
    sub["subsub"] = {subsub};
    sub["time"] = {2342354};

    obj["name"] = {"unknown"};
    obj["sub"] = {sub};
    obj["x-lay-tested"] = {true};
    obj["y-lay-tested"] = {false};
    obj["zoo"] = {"like"};
    obj["null"] = {nullptr};
    obj["age"] = {34};
    obj["condition"] = {-3};
    obj["luck"] = {0.77};

    std::ofstream write_file;
    write_file.open("json_parse_case_2.txt");
    write_file << obj << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("json_parse_case_2.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    auto& parsed_obj = air::parse("parsed_data", read_line);

    write_file.open("json_parse_case_2_parsed.txt");
    write_file << parsed_obj << std::endl;
    write_file.close();

    air::json_clear();

    read_file.open("json_parse_case_2_parsed.txt");
    std::string read_line_parsed;
    std::getline(read_file, read_line_parsed);
    read_file.close();

    EXPECT_EQ(0, read_line.compare(read_line_parsed));
}

TEST_F(JsonTest, json_parse_case_3)
{
    auto& obj = air::json("obj");
    auto& sub = air::json("sub");

    obj["arr1"] += {0.77};
    obj["arr1"] += {1, 2, 3};
    obj["arr1"] += {nullptr};
    obj["arr1"] += {true, false};
    obj["arr1"] += {-324.2};
    obj["arr1"] += {-44};
    sub["bool"] = {false};
    obj["arr1"] += {sub};

    std::ofstream write_file;
    write_file.open("json_parse_case_3.txt");
    write_file << obj << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("json_parse_case_3.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    auto& parsed_obj = air::parse("parsed_data", read_line);

    write_file.open("json_parse_case_3_parsed.txt");
    write_file << parsed_obj << std::endl;
    write_file.close();

    air::json_clear();

    read_file.open("json_parse_case_3_parsed.txt");
    std::string read_line_parsed;
    std::getline(read_file, read_line_parsed);
    read_file.close();

    EXPECT_EQ(0, read_line.compare(read_line_parsed));
}

TEST_F(JsonTest, jsondoc_api_exception)
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

TEST_F(JsonTest, jsondoc_clear_exception)
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

TEST_F(JsonTest, jsonprint_exception)
{
    try
    {
        air::PrintValue(std::cout, air::JSONtype::LONG, nullptr);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

TEST_F(JsonTest, jsonparser_exception)
{
    auto& obj = air::json("obj");

    static_cast<JSONparserTest&>(JSONparserTest::GetInstance()).TEST_AddJson(obj, "obj", air::JSONtype::NULLVAL, "null", "null");

    try
    {
        static_cast<JSONparserTest&>(JSONparserTest::GetInstance()).TEST_AddJson(obj, "obj", air::JSONtype::LONG, "long", "1234");
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    auto& arr = air::json("arr");
    arr["int"] += {1, 2, 3};

    static_cast<JSONparserTest&>(JSONparserTest::GetInstance()).TEST_AddJsonArray(arr, "arr", 3, air::JSONtype::INT64, "int", "4");

    try
    {
        static_cast<JSONparserTest&>(JSONparserTest::GetInstance()).TEST_AddJsonArray(arr, "arr", 4, air::JSONtype::LONG, "int", "5");
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    air::json_clear();
}

TEST_F(JsonTest, json_copy)
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
    EXPECT_EQ(0, str.compare("{\"bool\": true, \"double_arr\": [4.44], \"int\": 123, \"int_arr\": [1, 2, 3], \"multi_arr\": [\"str\", 5, {\"dd\": 394.231}], \"obj\": {\"str2\": \"strstr\"}, \"str\": \"string\"}"));

    auto copy_obj = air::json_copy("obj");
    std::stringstream stream_copy_obj;
    stream_copy_obj << copy_obj;
    std::string copy_str = stream_copy_obj.str();
    EXPECT_EQ(0, copy_str.compare("{\"bool\": true, \"double_arr\": [4.44], \"int\": 123, \"int_arr\": [1, 2, 3], \"multi_arr\": [\"str\", 5, {\"dd\": 394.231}], \"obj\": {\"str2\": \"strstr\"}, \"str\": \"string\"}"));

    air::json_clear();
    std::stringstream stream_copy_obj2;
    stream_copy_obj2 << copy_obj;
    std::string copy_str2 = stream_copy_obj2.str();
    EXPECT_EQ(0, copy_str2.compare("{\"bool\": true, \"double_arr\": [4.44], \"int\": 123, \"int_arr\": [1, 2, 3], \"multi_arr\": [\"str\", 5, {\"dd\": 394.231}], \"obj\": {\"str2\": \"strstr\"}, \"str\": \"string\"}"));

    copy_obj.Clear();
    std::stringstream stream_copy_obj3;
    stream_copy_obj3 << copy_obj;
    std::string copy_str3 = stream_copy_obj3.str();
    EXPECT_EQ(0, copy_str3.compare("{}"));
}

TEST_F(JsonTest, json_compound)
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
