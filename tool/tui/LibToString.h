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
#include <type_traits>

namespace air
{

template<typename T>
std::string
NumberToStrSIFmt(T value, std::string suffix = "", uint32_t padding = 0)
{
    static_assert(
        std::is_same<uint64_t, T>::value ||
        std::is_same<int64_t, T>::value ||
        std::is_same<double, T>::value
        , "");

    std::stringstream stream;
    bool overflow {false};
    int32_t max_size {7};

    stream << std::fixed << std::setprecision(1);

    if (0 > value)
    {
        value *= -1;
        stream << "-";
    }

    if (value < 1e3)
    {
        stream << value;
    }
    else if (value < 1e6)
    {
        stream << (value / 1e3) << "K";
    }
    else if (value < 1e9)
    {
        stream << (value / 1e6) << "M";
    }
    else if (value < 1e12)
    {
        stream << (value / 1e9) << "G";
    }
    else if (value < 1e15)
    {
        stream << (value / 1e12) << "T";
    }
    else if (value < 1e18)
    {
        stream << (value / 1e15) << "P";
    }
    else if (value < 1e21)
    {
        stream << (value / 1e18) << "E";
    }
    else
    {
        overflow = true;
    }

    int32_t padding_count {0};
    if (overflow)
    {
        stream << "NaN";
    }
    else
    {
        if (stream.str().size() < padding)
        {
            padding_count = padding - stream.str().size();
            while (0 < padding_count)
            {
                stream << " ";
                padding_count--;
            }
        }

        if (0 != suffix.compare(""))
        {
            stream << suffix;
            max_size += suffix.size();
        }
    }
    padding_count = max_size - stream.str().size();
    while (0 < padding_count)
    {
        stream << " ";
        padding_count--;
    }

    return stream.str();
}

std::string
ULLToStrLatFmt(uint64_t value)
{
    std::stringstream stream;
    std::string str_unit;
    uint64_t num_unit;
    bool overflow {false};
    int32_t max_size {5};

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
    else if (value < 4000000000ULL)
    {
        str_unit = "s";
        num_unit = 1000000000;
    }
    else
    {
        overflow = true;
    }

    if (overflow)
    {
        stream << "NaN";
    }
    else
    {
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
    }

    int32_t padding_count = max_size - stream.str().size();
    while (0 < padding_count)
    {
        stream << " ";
        padding_count--;
    }

    return stream.str();
}

} // namespace air

#endif // AIR_TUI_LIB_TO_STRING_H
