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

#include "jsonparser_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>
#include <iostream>

TEST(JsonparserTest, Case1)
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
    write_file.open("JsonparserTest_Case1.txt");
    write_file << obj << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("JsonparserTest_Case1.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    auto& parsed_obj = air::parse("parsed_data", read_line);

    write_file.open("JsonparserTest_Case1_Parsed.txt");
    write_file << parsed_obj << std::endl;
    write_file.close();

    air::json_clear();

    read_file.open("JsonparserTest_Case1_Parsed.txt");
    std::string read_line_parsed;
    std::getline(read_file, read_line_parsed);
    read_file.close();

    EXPECT_EQ(0, read_line.compare(read_line_parsed));
}

TEST(JsonparserTest, Case2)
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
    write_file.open("JsonparserTest_Case2.txt");
    write_file << obj << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("JsonparserTest_Case2.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    auto& parsed_obj = air::parse("parsed_data", read_line);

    write_file.open("JsonparserTest_Case2_Parsed.txt");
    write_file << parsed_obj << std::endl;
    write_file.close();

    air::json_clear();

    read_file.open("JsonparserTest_Case2_Parsed.txt");
    std::string read_line_parsed;
    std::getline(read_file, read_line_parsed);
    read_file.close();

    EXPECT_EQ(0, read_line.compare(read_line_parsed));
}

TEST(JsonparserTest, Case3)
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
    write_file.open("JsonparserTest_Case3.txt");
    write_file << obj << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("JsonparserTest_Case3.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    auto& parsed_obj = air::parse("parsed_data", read_line);

    write_file.open("JsonparserTest_Case3_Parsed.txt");
    write_file << parsed_obj << std::endl;
    write_file.close();

    air::json_clear();

    read_file.open("JsonparserTest_Case3_Parsed.txt");
    std::string read_line_parsed;
    std::getline(read_file, read_line_parsed);
    read_file.close();

    EXPECT_EQ(0, read_line.compare(read_line_parsed));
}

TEST(JsonparserTest, Exception)
{
    auto& obj = air::json("obj");

    static_cast<JSONparserTest&>(JSONparserTest::GetInstance())
        .TEST_AddJson(obj, "obj", air::JSONtype::NULLVAL, "null", "null");

    try
    {
        static_cast<JSONparserTest&>(JSONparserTest::GetInstance())
            .TEST_AddJson(obj, "obj", air::JSONtype::LONG, "long", "1234");
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    auto& arr = air::json("arr");
    arr["int"] += {1, 2, 3};

    static_cast<JSONparserTest&>(JSONparserTest::GetInstance())
        .TEST_AddJsonArray(arr, "arr", 3, air::JSONtype::INT64, "int", "4");

    try
    {
        static_cast<JSONparserTest&>(JSONparserTest::GetInstance())
            .TEST_AddJsonArray(arr, "arr", 4, air::JSONtype::LONG, "int", "5");
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    air::json_clear();
}
