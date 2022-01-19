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

#include <fstream>
#include <iostream>

TEST(JsontypeTest, NumberTypeValue)
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

TEST(JsontypeTest, BooleanTypeValue)
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

TEST(JsontypeTest, StringTypeValue)
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

TEST(JsontypeTest, NullptrTypeValue)
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

TEST(JsontypeTest, ArrayTypeValue)
{
    auto& obj = air::json("obj");

    obj["arr1"] += {1};
    obj["arr1"] += {2.2, 3.3};
    obj["arr1"] += {nullptr};

    obj["arr2"] = {"a", "b", "c"};
    obj["arr2"] += {std::string("def")};
    obj["arr2"] += {true, false};

    std::ofstream write_file;
    write_file.open("JsontypeTest_Array.txt");
    write_file << obj << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("JsontypeTest_Array.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    EXPECT_EQ(0, read_line.compare("{\"arr1\": [1, 2.2, 3.3, null], \"arr2\": [\"a\", \"b\", \"c\", \"def\", true, false]}"));
}

TEST(JsontypeTest, ObjectTypeValue)
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
    write_file.open("JsontypeTest_Object.txt");
    write_file << obj << std::endl;
    write_file.close();

    air::json_clear();

    std::ifstream read_file;
    read_file.open("JsontypeTest_Object.txt");
    std::string read_line;
    std::getline(read_file, read_line);
    read_file.close();

    EXPECT_EQ(0, read_line.compare("{\"obj_item\": \"item1\", \"sub_1\": {}, \"sub_2\": {\"s2_item\": \"itemS2\"}}"));
}
