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

#ifndef AIR_STRING_VIEW_H
#define AIR_STRING_VIEW_H

#include <iostream>
#include <stdexcept>
#include <string>

namespace air
{
class string_view
{
public:
    template<std::size_t N>
    constexpr string_view(const char (&a)[N])
    : str(a),
      len(N - 1)
    {
    }
    constexpr string_view(const char* a, std::size_t N)
    : str(a),
      len(N)
    {
    }
    constexpr std::size_t
    size() const
    {
        return len;
    }
    constexpr const char*
    data() const noexcept
    {
        return str;
    }
    constexpr char operator[](std::size_t n) const
    {
        return n < len ? str[n] : throw std::out_of_range("air::string_view operator[] invalid");
    }
    constexpr bool
    operator==(const air::string_view& arg) const
    {
        if (0 == compare(arg))
        {
            return true;
        }
        return false;
    }
    constexpr bool
    operator!=(const air::string_view& arg) const
    {
        if (0 == compare(arg))
        {
            return false;
        }
        return true;
    }
    constexpr int
    compare(air::string_view arg) const
    {
        if (arg.size() != len)
        {
            return -1;
        }
        for (std::size_t i = 0; i < len; i++)
        {
            if (arg[i] != str[i])
            {
                return -1;
            }
        }
        return 0;
    }
    constexpr std::size_t
    find(air::string_view arg, std::size_t pos = 0) const
    {
        if (len > pos)
        {
            std::size_t cnt{0};
            std::size_t curr_pos{pos};
            while (len > curr_pos)
            {
                if (arg[cnt] == str[curr_pos])
                {
                    cnt++;
                }
                else
                {
                    cnt = 0;
                }
                if (arg.size() == cnt)
                {
                    if (curr_pos + 1 < cnt)
                    {
                        return air::string_view::npos;
                    }
                    else
                    {
                        return curr_pos + 1 - cnt;
                    }
                }

                curr_pos++;
            }
        }
        return air::string_view::npos;
    }
    constexpr air::string_view
    substr(std::size_t start, std::size_t length) const
    {
        std::size_t left_size = length;
        if (len < start + length)
        {
            if (len >= start)
            {
                left_size = len - start;
            }
        }
        return air::string_view{str + start, left_size};
    }
    friend std::ostream&
    operator<<(std::ostream& os, const air::string_view& arg)
    {
        for (std::size_t i = 0; i < arg.size(); i++)
        {
            os << arg[i];
        }
        return os;
    }
    const static std::size_t npos{0x0FFFFFFF};

private:
    const char* str;
    std::size_t len;
};

} // namespace air

#endif // AIR_STRING_VIEW_H
