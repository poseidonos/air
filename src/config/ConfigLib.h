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

#ifndef AIR_CONFIG_LIB_H
#define AIR_CONFIG_LIB_H

#include <cstdint>
#include <type_traits>

#include "src/lib/StringView.h"

template<typename E>
constexpr auto
dtype(E e) noexcept // to primitive data types
{
    return static_cast<std::underlying_type_t<E>>(e);
}

namespace config
{
static constexpr uint32_t NUM_DEFAULT_KEY {8};
static constexpr uint32_t NUM_GROUP_KEY {5};
static constexpr uint32_t NUM_FILTER_KEY {2};
static constexpr uint32_t NUM_BUCKET_KEY {3};
static constexpr uint32_t NUM_NODE_KEY {9};

enum class ParagraphType : uint32_t
{
    DEFAULT = 0,
    GROUP,
    FILTER,
    BUCKET,
    NODE,

    COUNT,
    CONFIGTYPE_NULL
};

static constexpr bool
HasSpace(air::string_view value)
{
    size_t pos {0};

    if (0 == value.size())
    {
        return false;
    }

    while (pos < value.size())
    {
        if (' ' == value[pos] || '\t' == value[pos] || '\n' == value[pos] ||
            '\v' == value[pos] || '\f' == value[pos] || '\r' == value[pos])
        {
            return true;
        }
        pos++;
    }

    return false;
}

static constexpr bool
IsSpace(char c)
{
    if (' ' == c || '\t' == c || '\n' == c || '\v' == c || '\f' == c || '\r' == c)
    {
        return true;
    }

    return false;
}

static constexpr bool
IsUInt(air::string_view value)
{
    size_t pos {0};

    if (0 == value.size())
    {
        return false;
    }

    while (pos < value.size())
    {
        if ('0' > value[pos] || '9' < value[pos])
        {
            return false;
        }
        pos++;
    }
    return true;
}

static constexpr air::string_view
Strip(air::string_view str)
{
    if (0 != str.size())
    {
        std::size_t tail {str.size() - 1};

        if (IsSpace(str[0]))
        {
            return Strip(str.substr(1, str.size() - 1));
        }
        else if (IsSpace(str[tail]))
        {
            return Strip(str.substr(0, str.size() - 1));
        }
    }
    return str;
}

static constexpr int64_t
Stoi(air::string_view str)
{
    int64_t result {0};
    uint32_t len {static_cast<uint32_t>(str.size())};
    int64_t pow_of_ten {1};
    for (int32_t len_idx = len - 1; len_idx >= 0; len_idx--)
    {
        uint32_t num {static_cast<uint32_t>(str[len_idx]) - '0'};
        if (9 < num)
        {
            continue;
        }
        result += num * pow_of_ten;
        pow_of_ten *= 10;
    }
    return result;
}

static constexpr int64_t
StolWithExponent(air::string_view str)
{
    int64_t result {0};
    uint32_t len {static_cast<uint32_t>(str.size())};
    int64_t pow_of_ten {1};
    int32_t len_last {0};
    bool is_positive {true};
    if ('-' == str[0])
    {
        is_positive = false;
        len_last = 1;
    }

    size_t pow_pos {str.find("^")};
    if (air::string_view::npos != pow_pos)
    {
        int64_t base {StolWithExponent(str.substr(0, pow_pos))};
        int64_t exponent {
            StolWithExponent(str.substr(pow_pos + 1, str.size() - pow_pos - 1))};
        if (0 == exponent)
        {
            if (is_positive)
            {
                return 1;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            result = 1;
            while (exponent)
            {
                result *= base;
                exponent--;
            }

            if (!is_positive && 0 < result)
            {
                result *= -1;
            }

            return result;
        }
    }

    for (int32_t len_idx = len - 1; len_idx >= len_last; len_idx--)
    {
        uint32_t num {static_cast<uint32_t>(str[len_idx]) - '0'};
        result += num * pow_of_ten;
        pow_of_ten *= 10;
    }

    if (is_positive)
    {
        return result;
    }
    else
    {
        return -1 * result;
    }
}

} // namespace config

#endif // AIR_CONFIG_LIB_H
