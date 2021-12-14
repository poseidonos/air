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
#include <string>

#include "tool/tui/LibToString.h"

TEST(LibToStringTest, NumberToStrSIFmt_uint64)
{
    // Given: uint64_t, std::string type variables
    uint64_t number;
    std::string value;

    // When 1: Call air::NumberToStrSIFmt with number
    number = std::stoull("1234");
    value = air::NumberToStrSIFmt(number);
    // Then 1: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("1.2K   "));

    // When 2: Call air::NumberToStrSIFmt with number
    number = std::stoull("56781234");
    value = air::NumberToStrSIFmt(number);
    // Then 2: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("56.8M  "));

    // When 3: Call air::NumberToStrSIFmt with number
    number = std::stoull("912356781234");
    value = air::NumberToStrSIFmt(number);
    // Then 3: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("912.4G "));

    // When 4: Call air::NumberToStrSIFmt with number
    number = std::stoull("1343000000000");
    value = air::NumberToStrSIFmt(number);
    // Then 4: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("1.3T   "));

    // When 5: Call air::NumberToStrSIFmt with number
    number = 0xFFFFFFFFFFFFFFFF; // maximum value
    value = air::NumberToStrSIFmt(number);
    // Then 4: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("18.4E  "));
}

TEST(LibToStringTest, NumberToStrSIFmt_int64)
{
    // Given: int64_t, std::string type variables
    int64_t number;
    std::string value;

    // When 1: Call air::NumberToStrSIFmt with number
    number = std::stoull("1234");
    value = air::NumberToStrSIFmt(number);
    // Then 1: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("1.2K   "));

    // When 2: Call air::NumberToStrSIFmt with number
    number = std::stoull("56781234");
    value = air::NumberToStrSIFmt(number);
    // Then 2: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("56.8M  "));

    // When 3: Call air::NumberToStrSIFmt with number
    number = std::stoull("912356781234");
    value = air::NumberToStrSIFmt(number);
    // Then 3: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("912.4G "));

    // When 4: Call air::NumberToStrSIFmt with number
    number = std::stoull("1343000000000");
    value = air::NumberToStrSIFmt(number);
    // Then 4: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("1.3T   "));

    // When 5: Call air::NumberToStrSIFmt with number
    number = 0xFFFFFFFFFFFFFFFF; // -1
    value = air::NumberToStrSIFmt(number);
    // Then 4: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("-1     "));

    // When 6: Call air::NumberToStrSIFmt with number
    number = 0xEFFFFFFFFFFFFFFF; // -1.2E
    value = air::NumberToStrSIFmt(number);
    // Then 6: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("-1.2E  "));

    // When 7: Call air::NumberToStrSIFmt with number
    number = -987654321098;
    value = air::NumberToStrSIFmt(number);
    // Then 7: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("-987.7G"));
}

TEST(LibToStringTest, NumberToStrSIFmt_double)
{
    // Given: double, std::string type variables
    double number;
    std::string value;

    // When 1: Call air::NumberToStrSIFmt with number
    number = std::stod("1.343e12");
    value = air::NumberToStrSIFmt(number);
    // Then 1: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("1.3T   "));

    // When 2: Call air::NumberToStrSIFmt with number
    number = std::stod("34.1893e15");
    value = air::NumberToStrSIFmt(number);
    // Then 2: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("34.2P  "));

    // When 3: Call air::NumberToStrSIFmt with number
    number = std::stod("452.997e18");
    value = air::NumberToStrSIFmt(number);
    // Then 3: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("453.0E "));

    // When 4: Call air::NumberToStrSIFmt with number
    number = std::stod("1234.5678e18");
    value = air::NumberToStrSIFmt(number);
    // Then 4: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("NaN    "));

    // When 5: Call air::NumberToStrSIFmt with number
    number = std::stod("9.12e21");
    value = air::NumberToStrSIFmt(number);
    // Then 5: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("NaN    "));

    // When 6: Call air::NumberToStrSIFmt with number
    number = std::stod("-483.34e18");
    value = air::NumberToStrSIFmt(number);
    // Then 6: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("-483.3E"));

    // When 7: Call air::NumberToStrSIFmt with number
    number = std::stod("-56e21");
    value = air::NumberToStrSIFmt(number);
    // Then 7: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("-NaN   "));
}

TEST(LibToStringTest, NumberToStrSIFmt_WithSuffix)
{
    // Given: std::string type variable
    std::string value;

    // When 1: Call air::NumberToStrSIFmt with number & suffix
    value = air::NumberToStrSIFmt(73.2e9, "iB/s");
    // Then 1: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("73.2GiB/s  "));

    // When 1: Call air::NumberToStrSIFmt with number & suffix
    value = air::NumberToStrSIFmt(52.87e15, "iops");
    // Then 1: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("52.9Piops  "));

    // When 3: Call air::NumberToStrSIFmt with number & suffix
    value = air::NumberToStrSIFmt(-56e21, "Meaningless");
    // Then 3: Expect the value is converted to SI expression with padding
    EXPECT_EQ(0, value.compare("-NaN   "));
}

TEST(LibToStringTest, NumberToStrSIFmt_NotNumber)
{
    // Given: double, std::string type variables
    double number;
    std::string value {""};

    // When: Call air::NumberToStrSIFmt with not number
    try
    {
        number = std::stod("fesdfe");
        value = air::NumberToStrSIFmt(number);
    }
    // Then: Expect to catch exception and the value isn't updated
    catch(...)
    {
        EXPECT_EQ(0, value.compare(""));
    }
}

TEST(LibToStringTest, ULLToStrLatFmt_Normal)
{
    // Given: uint64_t, std::string type variables
    uint64_t number;
    std::string value;

    // When 1: Call air::ULLToStrLatFmt with number
    number = 42;
    value = air::ULLToStrLatFmt(number);
    // Then 1: Expect the value is converted to latency format with padding
    EXPECT_EQ(0, value.compare("42ns "));

    // When 2: Call air::ULLToStrLatFmt with number
    number = 294942;
    value = air::ULLToStrLatFmt(number);
    // Then 2: Expect the value is converted to latency format with padding
    EXPECT_EQ(0, value.compare("294us"));

    // When 3: Call air::ULLToStrLatFmt with number
    number = 3998294942;
    value = air::ULLToStrLatFmt(number);
    // Then 3: Expect the value is converted to latency format with padding
    EXPECT_EQ(0, value.compare("4.0s "));
}

TEST(LibToStringTest, ULLToStrLatFmt_Overflow)
{
    // Given: uint64_t, std::string type variables
    uint64_t number;
    std::string value;

    // When: Call air::ULLToStrLatFmt with number
    number = 4000000001;
    value = air::ULLToStrLatFmt(number);

    // Then: Expect the value is converted to latency format with padding
    EXPECT_EQ(0, value.compare("NaN  "));
}
