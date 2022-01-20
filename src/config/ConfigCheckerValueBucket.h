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

#ifndef AIR_CONFIG_CHECKER_VALUE_BUCKET_H
#define AIR_CONFIG_CHECKER_VALUE_BUCKET_H

#include <iostream>
#include <stdexcept>

#include "src/config/ConfigLib.h"
#include "src/config/ConfigParser.h"
#include "src/lib/StringView.h"

namespace config
{
class ConfigCheckerValueBucket
{
public:
    ConfigCheckerValueBucket(void)
    {
    }

    virtual ~ConfigCheckerValueBucket(void)
    {
    }

    static constexpr int32_t
    CheckBucketValue(air::string_view key, air::string_view value,
        int64_t& bucket_scale, int64_t& bucket_lower_bound,
        int64_t& bucket_upper_bound, bool& bucket_type_linear,
        bool& bucket_start_brace_has_equal, bool& bucket_end_brace_has_equal)
    {
        if (key == "Bucket")
        {
            return _CheckNameValue(value);
        }
        else if (key == "Scale")
        {
            return _CheckScale(value, bucket_type_linear, bucket_scale);
        }
        else if (key == "Bounds")
        {
            return _CheckBounds(value, bucket_lower_bound, bucket_upper_bound,
                bucket_start_brace_has_equal, bucket_end_brace_has_equal);
        }
        else
        {
            throw std::invalid_argument("CheckBucketValue got invalid key");
        }
        return 0;
    }

    static constexpr int32_t
    CheckBucketSyntax(int64_t bucket_scale, int64_t bucket_lower_bound,
        int64_t bucket_upper_bound, bool bucket_type_linear,
        bool bucket_start_brace_has_equal, bool bucket_end_brace_has_equal)
    {
        if (ONE_EXA < bucket_upper_bound || MINUS_ONE_EXA > bucket_upper_bound)
        {
            throw std::invalid_argument(
                "Bucket exponential type upper_bound limit is 1E(exa, 1e18)");
        }
        if (ONE_EXA < bucket_lower_bound || MINUS_ONE_EXA > bucket_lower_bound)
        {
            throw std::invalid_argument(
                "Bucket exponential type lower_bound limit is -1E(exa, 1e18)");
        }

        if (bucket_type_linear)
        {
            return _CheckBucketSyntaxLinearType(bucket_scale, bucket_lower_bound,
                bucket_upper_bound, bucket_start_brace_has_equal,
                bucket_end_brace_has_equal);
        }
        else
        {
            return _CheckBucketSyntaxExponentialType(bucket_scale,
                bucket_lower_bound, bucket_upper_bound,
                bucket_start_brace_has_equal, bucket_end_brace_has_equal);
        }

        return 0;
    }

private:
    static constexpr int64_t
    _GetLongNumber(air::string_view value)
    {
        int64_t result {0};
        uint32_t len {static_cast<uint32_t>(value.size())};
        int64_t pow_of_ten {1};
        int32_t len_last {0};
        bool is_positive {true};
        if ('-' == value[0])
        {
            is_positive = false;
            len_last = 1;
        }
        size_t pow_pos {0};
        if (air::string_view::npos != value.find("^"))
        {
            pow_pos = value.find("^");
            if (air::string_view::npos != value.find("^", pow_pos + 1))
            {
                throw std::invalid_argument(
                    "Bucket range: only one power(^) is acceptable value");
            }
            int64_t base {_GetLongNumber(value.substr(0, pow_pos))};
            int64_t exponent {_GetLongNumber(
                value.substr(pow_pos + 1, value.size() - pow_pos - 1))};
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
                if (0 > exponent)
                {
                    throw std::invalid_argument(
                        "Bucket range: exponent has to be a positive value");
                }

                result = 1;
                while (exponent)
                {
                    result *= base;
                    exponent--;
                }

                if (!is_positive && 0 < result)
                {
                    return -1 * result;
                }
                else
                {
                    return result;
                }
            }
        }

        for (int32_t len_idx = len - 1; len_idx >= len_last; len_idx--)
        {
            if ('.' == value[len_idx])
            {
                throw std::invalid_argument(
                    "Bucket range: decimal point(.) is unaccepatable type");
            }
            uint32_t num {static_cast<uint32_t>(value[len_idx]) - '0'};
            if (9 < num)
            {
                throw std::invalid_argument("Bucket range: invalid value, "
                                            "number(^number) is accepatable value");
            }
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

    static constexpr int32_t
    _CheckScale(
        air::string_view value, bool& bucket_type_linear, int64_t& bucket_scale)
    {
        if (air::string_view::npos == value.find("^"))
        {
            bucket_type_linear = true;
            bucket_scale = _GetLongNumber(value);
        }
        else
        {
            bucket_type_linear = false;
            bucket_scale = _GetLongNumber(value.substr(0, value.size() - 1));
        }

        if (bucket_type_linear)
        {
            if (1 > bucket_scale || ONE_HUNDRED_PETA < bucket_scale)
            {
                throw std::out_of_range(
                    "Bucket linear type's valid bucket range: 1 ~ 1e17");
            }
        }
        else
        {
            if (2 > bucket_scale || 10 < bucket_scale)
            {
                throw std::out_of_range(
                    "Bucket exponential type's valid bucket range: 2^ ~ 10^");
            }
        }
        return 0;
    }

    static constexpr int32_t
    _CheckBounds(air::string_view value, int64_t& bucket_lower_bound,
        int64_t& bucket_upper_bound, bool& bucket_start_brace_has_equal,
        bool& bucket_end_brace_has_equal)
    {
        if ('(' == value[0])
        {
            bucket_start_brace_has_equal = false;
        }
        else if ('[' == value[0])
        {
            bucket_start_brace_has_equal = true;
        }
        else
        {
            throw std::invalid_argument(
                "Bucket DataRange has to start with '[' or '(' brace");
        }

        if (')' == value[value.size() - 1])
        {
            bucket_end_brace_has_equal = false;
        }
        else if (']' == value[value.size() - 1])
        {
            bucket_end_brace_has_equal = true;
        }
        else
        {
            throw std::invalid_argument(
                "Bucket DataRange has to end with ']' or ')' brace");
        }

        std::size_t comma_pos {value.find(",")};
        if (air::string_view::npos == comma_pos)
        {
            throw std::invalid_argument("Bucket DataRange: missing comma(,)");
        }
        air::string_view lower_bound {value.substr(1, comma_pos - 1)};
        lower_bound = Strip(lower_bound);
        air::string_view upper_bound {
            value.substr(comma_pos + 1, value.size() - comma_pos - 2)};
        upper_bound = Strip(upper_bound);

        bucket_lower_bound = _GetLongNumber(lower_bound);
        bucket_upper_bound = _GetLongNumber(upper_bound);

        return 0;
    }

    static constexpr int32_t
    _CheckBucketSyntaxLinearType(int64_t bucket_scale, int64_t bucket_lower_bound,
        int64_t bucket_upper_bound, bool bucket_start_brace_has_equal,
        bool bucket_end_brace_has_equal)
    {
        if (!bucket_start_brace_has_equal)
        {
            throw std::invalid_argument(
                "Bucket linear type has to start widh '[' brace");
        }
        if (bucket_end_brace_has_equal)
        {
            throw std::invalid_argument(
                "Bucket linear type has to end with ')' brace");
        }
        if (0 != bucket_lower_bound % bucket_scale)
        {
            throw std::invalid_argument(
                "Bucket linear type (lower_bound % scale) has to be 0");
        }
        if (0 != bucket_upper_bound % bucket_scale)
        {
            throw std::invalid_argument(
                "Bucket linear type (upper_bound % scale) has to be 0");
        }
        if (bucket_lower_bound >= bucket_upper_bound)
        {
            throw std::invalid_argument(
                "Bucket lower_bound is bigger than or equal upper_bound");
        }
        if (20 < (bucket_upper_bound - bucket_lower_bound) / bucket_scale)
        {
            throw std::invalid_argument("Bucket bucket_size is bigger than 20");
        }
        return 0;
    }

    static constexpr void
    _CheckBucketSyntaxExponentialTypeBrace(int64_t bucket_lower_bound,
        int64_t bucket_upper_bound, bool bucket_start_brace_has_equal,
        bool bucket_end_brace_has_equal)
    {
        if (0 == bucket_lower_bound || 0 == bucket_upper_bound)
        {
            throw std::invalid_argument("Bucket exponential type DataRange doesn't "
                                        "have 0 lower_bound or upper_bound");
        }
        if (bucket_lower_bound >= bucket_upper_bound)
        {
            throw std::invalid_argument(
                "Bucket lower_bound is bigger than or equal upper_bound");
        }
        if (0 < bucket_lower_bound)
        {
            if (!bucket_start_brace_has_equal)
            {
                throw std::invalid_argument(
                    "Bucket exponential type: when lower_bound is positive, "
                    "DataRange starts widh '[' brace");
            }
            if (bucket_end_brace_has_equal)
            {
                throw std::invalid_argument(
                    "Bucket exponential type: when lower_bound is positive, "
                    "DataRange ends with ')' brace");
            }
        }
        if (0 > bucket_upper_bound)
        {
            if (bucket_start_brace_has_equal)
            {
                throw std::invalid_argument(
                    "Bucket exponential type: when upper_bound is negative, "
                    "DataRange starts widh '(' brace");
            }
            if (!bucket_end_brace_has_equal)
            {
                throw std::invalid_argument(
                    "Bucket exponential type: when upper_bound is negative, "
                    "DataRange ends with ']' brace");
            }
        }
        if (0 > bucket_lower_bound && 0 < bucket_upper_bound)
        {
            if (bucket_start_brace_has_equal)
            {
                throw std::invalid_argument(
                    "Bucket exponential type: when lower_bound is negative & "
                    "upper_bound is positive, DataRange starts widh '(' brace");
            }
            if (bucket_end_brace_has_equal)
            {
                throw std::invalid_argument(
                    "Bucket exponential type: when lower_bound is negative & "
                    "upper_bound is positive, DataRange ends widh ')' brace");
            }
        }
    }

    static constexpr int32_t
    _CheckBucketSyntaxExponentialType(int64_t bucket_scale,
        int64_t bucket_lower_bound, int64_t bucket_upper_bound,
        bool bucket_start_brace_has_equal, bool bucket_end_brace_has_equal)
    {
        _CheckBucketSyntaxExponentialTypeBrace(bucket_lower_bound,
            bucket_upper_bound, bucket_start_brace_has_equal,
            bucket_end_brace_has_equal);

        int32_t bucket_start {0};
        if (1 == bucket_lower_bound)
        {
            bucket_start = 1;
        }
        else if (1 < bucket_lower_bound)
        {
            bucket_start = 1;
            int64_t tmp_lower_bound {1};
            while (tmp_lower_bound < bucket_lower_bound)
            {
                tmp_lower_bound *= bucket_scale;
                bucket_start++;
                if (tmp_lower_bound > bucket_lower_bound)
                {
                    throw std::invalid_argument(
                        "Bucket exponential type lower_bound has to be a "
                        "(-/+)scale^n");
                }
            }
        }
        else if (-1 == bucket_lower_bound)
        {
            bucket_start = -1;
        }
        else if (-1 > bucket_lower_bound)
        {
            bucket_start = -1;
            int64_t tmp_lower_bound {-1};
            while (tmp_lower_bound > bucket_lower_bound)
            {
                tmp_lower_bound *= bucket_scale;
                bucket_start--;
                if (tmp_lower_bound < bucket_lower_bound)
                {
                    throw std::invalid_argument(
                        "Bucket exponential type lower_bound has to be a "
                        "(-/+)scale^n");
                }
            }
        }

        int32_t bucket_end {0};
        if (1 == bucket_upper_bound)
        {
            bucket_end = 1;
        }
        else if (1 < bucket_upper_bound)
        {
            bucket_end = 1;
            int64_t tmp_upper_bound {1};
            while (tmp_upper_bound < bucket_upper_bound)
            {
                tmp_upper_bound *= bucket_scale;
                bucket_end++;
                if (tmp_upper_bound > bucket_upper_bound)
                {
                    throw std::invalid_argument(
                        "Bucket exponential type upper_bound has to be a "
                        "(-/+)scale^n");
                }
            }
        }
        else if (-1 == bucket_upper_bound)
        {
            bucket_end = -1;
        }
        else if (-1 > bucket_upper_bound)
        {
            bucket_end = -1;
            int64_t tmp_upper_bound {-1};
            while (tmp_upper_bound > bucket_upper_bound)
            {
                tmp_upper_bound *= bucket_scale;
                bucket_end--;
                if (tmp_upper_bound < bucket_upper_bound)
                {
                    throw std::invalid_argument(
                        "Bucket exponential type upper_bound has to be a "
                        "(-/+)scale^n");
                }
            }
        }

        if (0 > bucket_start && 0 < bucket_end)
        {
            if (20 < bucket_end - bucket_start + 1)
            {
                throw std::invalid_argument("Bucket bucket_size is bigger than 20");
            }
        }
        else
        {
            if (20 < bucket_end - bucket_start)
            {
                throw std::invalid_argument("Bucket bucket_size is bigger than 20");
            }
        }
        return 0;
    }

    static constexpr int32_t
    _CheckNameValue(air::string_view value)
    {
        if (MAX_NAME_LEN < value.size())
        {
            throw std::length_error("Name length limit: 30 characters");
        }
        return 0;
    }

    static constexpr uint32_t MAX_NAME_LEN {30};
    static constexpr int64_t ONE_EXA {1000000000000000000};
    static constexpr int64_t MINUS_ONE_EXA {-1000000000000000000};
    static constexpr int64_t ONE_HUNDRED_PETA {100000000000000000};
};

} // namespace config

#endif // AIR_CONFIG_CHECKER_VALUE_BUCKET_H
