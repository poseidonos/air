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

#ifndef AIR_TUI_LIB_TO_STRING_H
#define AIR_TUI_LIB_TO_STRING_H

#include <iomanip>
#include <sstream>
#include <string>

namespace air
{
std::string
Double2StringWithBWFormat(double value)
{
    std::stringstream stream;

    stream << std::fixed << std::setprecision(1);

    if (value < 1000ULL)
    {
        stream << value;
    }
    else if (value < 1000000ULL)
    {
        stream << (value / 1000ULL) << "KB";
    }
    else if (value < 1000000000ULL)
    {
        stream << (value / 1000000ULL) << "MB";
    }
    else
    {
        stream << (value / 1000000000ULL) << "GB";
    }

    size_t padding_count = 7 - stream.str().size();
    while (0 < padding_count)
    {
        stream << " ";
        padding_count--;
    }

    return stream.str();
}

std::string
Double2String(double value)
{
    std::stringstream stream;

    stream << std::fixed << std::setprecision(1);

    if (value < 1000ULL)
    {
        stream << value;
    }
    else if (value < 1000000ULL)
    {
        stream << (value / 1000ULL) << "k";
    }
    else if (value < 1000000000ULL)
    {
        stream << (value / 1000000ULL) << "m";
    }
    else
    {
        stream << (value / 1000000000ULL) << "g";
    }

    size_t padding_count = 6 - stream.str().size();
    while (0 < padding_count)
    {
        stream << " ";
        padding_count--;
    }

    return stream.str();
}

std::string
ULL2StringWithLatencyFormat(uint64_t value)
{
    std::stringstream stream;
    std::string str_unit;
    uint64_t num_unit;

    if (value < 1000ULL)
    {
        str_unit = "ns";
        num_unit = 1;
    }
    else if (value < 1000000ULL)
    {
        str_unit = "us";
        num_unit = 1000;
    }
    else if (value < 1000000000ULL)
    {
        str_unit = "ms";
        num_unit = 1000000;
    }
    else
    {
        str_unit = "s";
        num_unit = 1000000000;
    }

    if (10 > (value / num_unit))
    {
        stream << std::fixed << std::setprecision(1);
        stream << (double)value / num_unit;
    }
    else
    {
        stream << value / num_unit;
    }
    stream << str_unit;

    if (5 > stream.str().size())
    {
        size_t padding_count = 5 - stream.str().size();
        while (0 < padding_count)
        {
            stream << " ";
            padding_count--;
        }
    }

    return stream.str();
}

std::string
LL2String(int64_t value)
{
    std::stringstream stream;

    stream << std::fixed << std::setprecision(1);

    if (value < 1000LL && value > -1000LL)
    {
        stream << value;
    }
    else if (value < 1000000LL && value > -1000000LL)
    {
        stream << ((double)value / 1000LL) << "k";
    }
    else if (value < 1000000000LL && value > -1000000000LL)
    {
        stream << ((double)value / 1000000LL) << "m";
    }
    else
    {
        stream << ((double)value / 1000000000LL) << "g";
    }

    size_t padding_count = 7 - stream.str().size();
    while (0 < padding_count)
    {
        stream << " ";
        padding_count--;
    }

    return stream.str();
}

} // namespace air

#endif // AIR_TUI_LIB_TO_STRING_H
