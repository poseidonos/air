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

#include <fstream>
#include <iostream>

#include "src/lib/json/Json.h"

TEST(JsonprintTest, DirectCall)
{
    int32_t value {7};
    air::PrintValue(std::cout, air::JSONtype::INT32, &value) << std::endl;
}

TEST(JsonprintTest, Exception)
{
    EXPECT_ANY_THROW(air::PrintValue(std::cout, air::JSONtype::LONG, nullptr));
}

TEST(JsonprintTest, ObjectStream)
{
    auto& obj = air::json("obj");

    obj["signed number"] = {-100};
    obj["unsigned number"] = {100U};
    obj["double"] = {333.333};
    obj["string"] = {"string value"};
    obj["null"] = {nullptr};

    std::cout << obj << std::endl;

    air::json_clear();
}

TEST(JsonprintTest, ExportTextFile)
{
    auto& obj = air::json("obj");

    obj["num1"] = {1, 2, 3};
    obj["null"] = {nullptr};

    std::ofstream write_file;
    write_file.open("JsonprintTest_ExportTextFile.txt");
    write_file << obj << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("JsonprintTest_ExportTextFile.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    EXPECT_EQ(0, read_line.compare("{\"null\": null, \"num1\": [1, 2, 3]}"));
}
