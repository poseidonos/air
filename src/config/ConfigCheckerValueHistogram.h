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

#ifndef AIR_CONFIG_CHECKER_VALUE_HISTOGRAM_H
#define AIR_CONFIG_CHECKER_VALUE_HISTOGRAM_H

#include <stdexcept>
#include <iostream>

#include "src/config/ConfigLib.h"
#include "src/config/ConfigParser.h"
#include "src/lib/StringView.h"

namespace config
{
class ConfigCheckerValueHistogram
{
public:
    ConfigCheckerValueHistogram(void)
    {
    }

    virtual ~ConfigCheckerValueHistogram(void)
    {
    }

    static constexpr int32_t
    CheckHistogramValue(air::string_view key, air::string_view value,
        int64_t& histogram_bucket_range, int64_t& histogram_min_value,
        int64_t& histogram_max_value, bool& histogram_type_linear,
        bool& histogram_start_brace_has_equal, bool& histogram_end_brace_has_equal)
    {
        if (key == "Histogram")
        {
            return _CheckNameValue(value);
        }
        else if (key == "BucketRange")
        {
            return _CheckBucketRange(value, histogram_type_linear, histogram_bucket_range);
        }
        else if (key == "DataRange")
        {
            return _CheckDataRange(value, histogram_min_value, histogram_max_value,
                histogram_start_brace_has_equal, histogram_end_brace_has_equal);
        }
        else
        {
            throw std::invalid_argument("_CheckHistogramValue got invalid key");
        }
        return 0;
    }

    static constexpr int32_t
    CheckHistogramRangeRule(int64_t histogram_bucket_range, int64_t histogram_min_value,
        int64_t histogram_max_value, bool histogram_type_linear,
        bool histogram_start_brace_has_equal, bool histogram_end_brace_has_equal)
    {
        if (ONE_EXA < histogram_max_value || MINUS_ONE_EXA > histogram_max_value)
        {
            throw std::invalid_argument("Histogram exponential type max_value limit is 1E(exa, 1e18)");
        }
        if (ONE_EXA < histogram_min_value || MINUS_ONE_EXA > histogram_min_value)
        {
            throw std::invalid_argument("Histogram exponential type min_value limit is -1E(exa, 1e18)");
        }

        if (histogram_type_linear)
        {
            return _CheckHistogramRangeRuleLinearType(histogram_bucket_range, histogram_min_value,
                histogram_max_value, histogram_start_brace_has_equal, histogram_end_brace_has_equal);
        }
        else
        {
            return _CheckHistogramRangeRuleExponentialType(histogram_bucket_range, histogram_min_value,
                histogram_max_value, histogram_start_brace_has_equal, histogram_end_brace_has_equal);
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
                throw std::invalid_argument("Histogram range: only one power(^) is acceptable value");
            }
            int64_t base {_GetLongNumber(value.substr(0, pow_pos))};
            int64_t exponent {_GetLongNumber(value.substr(pow_pos + 1, value.size() - pow_pos - 1))};
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
                    throw std::invalid_argument("Histogram range: exponent has to be a positive value");
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
                throw std::invalid_argument("Histogram range: decimal point(.) is unaccepatable type");
            }
            uint32_t num {static_cast<uint32_t>(value[len_idx]) - '0'};
            if (9 < num)
            {
                throw std::invalid_argument("Histogram range: invalid value, number(^number) is accepatable value");
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
    _CheckBucketRange(air::string_view value, bool& histogram_type_linear, int64_t& histogram_bucket_range)
    {
        if (air::string_view::npos == value.find("^"))
        {
            histogram_type_linear = true;
            histogram_bucket_range = _GetLongNumber(value);
        }
        else
        {
            histogram_type_linear = false;
            histogram_bucket_range = _GetLongNumber(value.substr(0, value.size() - 1));
        }

        if (histogram_type_linear)
        {
            if (1 > histogram_bucket_range || ONE_HUNDRED_PETA < histogram_bucket_range)
            {
                throw std::out_of_range("Histogram linear type's valid bucket range: 1 ~ 1e17");
            }
        }
        else
        {
            if (2 > histogram_bucket_range || 10 < histogram_bucket_range)
            {
                throw std::out_of_range("Histogram exponential type's valid bucket range: 2^ ~ 10^");
            }
        }
        return 0;
    }

    static constexpr int32_t
    _CheckDataRange(air::string_view value, int64_t& histogram_min_value, int64_t& histogram_max_value,
        bool& histogram_start_brace_has_equal, bool& histogram_end_brace_has_equal)
    {
        if ('(' == value[0])
        {
            histogram_start_brace_has_equal = false;
        }
        else if ('[' == value[0])
        {
            histogram_start_brace_has_equal = true;
        }
        else
        {
            throw std::invalid_argument("Histogram DataRange has to start with '[' or '(' brace");
        }

        if (')' == value[value.size() - 1])
        {
            histogram_end_brace_has_equal = false;
        }
        else if (']' == value[value.size() - 1])
        {
            histogram_end_brace_has_equal = true;
        }
        else
        {
            throw std::invalid_argument("Histogram DataRange has to end with ']' or ')' brace");
        }

        std::size_t comma_pos {value.find(",")};
        if (air::string_view::npos == comma_pos)
        {
            throw std::invalid_argument("Histogram DataRange: missing comma(,)");
        }
        air::string_view min_value {value.substr(1, comma_pos - 1)};
        min_value = Strip(min_value);
        air::string_view max_value {value.substr(comma_pos + 1, value.size() - comma_pos - 2)};
        max_value = Strip(max_value);

        histogram_min_value = _GetLongNumber(min_value);
        histogram_max_value = _GetLongNumber(max_value);

        return 0;
    }

    static constexpr int32_t
    _CheckHistogramRangeRuleLinearType(int64_t histogram_bucket_range, int64_t histogram_min_value,
        int64_t histogram_max_value, bool histogram_start_brace_has_equal, bool histogram_end_brace_has_equal)
    {
        if (!histogram_start_brace_has_equal)
        {
            throw std::invalid_argument("Histogram linear type has to start widh '[' brace");
        }
        if (histogram_end_brace_has_equal)
        {
            throw std::invalid_argument("Histogram linear type has to end with ')' brace");
        }
        if (0 != histogram_min_value % histogram_bucket_range)
        {
            throw std::invalid_argument("Histogram linear type (min_value % bucket_range) has to be 0");
        }
        if (0 != histogram_max_value % histogram_bucket_range)
        {
            throw std::invalid_argument("Histogram linear type (max_value % bucket_range) has to be 0");
        }
        if (histogram_min_value >= histogram_max_value)
        {
            throw std::invalid_argument("Histogram min_value is bigger than or equal max_value");
        }
        if (20 < (histogram_max_value - histogram_min_value) / histogram_bucket_range)
        {
            throw std::invalid_argument("Histogram bucket_size is bigger than 20");
        }
        return 0;
    }

    static constexpr void
    _CheckHistogramRangeRuleExponentialTypeSyntax(int64_t histogram_min_value,
        int64_t histogram_max_value, bool histogram_start_brace_has_equal, bool histogram_end_brace_has_equal)
    {
        if (0 == histogram_min_value || 0 == histogram_max_value)
        {
            throw std::invalid_argument("Histogram exponential type DataRange doesn't have 0 min_value or max_value");
        }
        if (histogram_min_value >= histogram_max_value)
        {
            throw std::invalid_argument("Histogram min_value is bigger than or equal max_value");
        }
        if (0 < histogram_min_value)
        {
            if (!histogram_start_brace_has_equal)
            {
                throw std::invalid_argument("Histogram exponential type: when min_value is positive, DataRange starts widh '[' brace");
            }
            if (histogram_end_brace_has_equal)
            {
                throw std::invalid_argument("Histogram exponential type: when min_value is positive, DataRange ends with ')' brace");
            }
        }
        if (0 > histogram_max_value)
        {
            if (histogram_start_brace_has_equal)
            {
                throw std::invalid_argument("Histogram exponential type: when max_value is negative, DataRange starts widh '(' brace");
            }
            if (!histogram_end_brace_has_equal)
            {
                throw std::invalid_argument("Histogram exponential type: when max_value is negative, DataRange ends with ']' brace");
            }
        }
        if (0 > histogram_min_value && 0 < histogram_max_value)
        {
            if (histogram_start_brace_has_equal)
            {
                throw std::invalid_argument("Histogram exponential type: when min_value is negative & max_value is positive, DataRange starts widh '(' brace");
            }
            if (histogram_end_brace_has_equal)
            {
                throw std::invalid_argument("Histogram exponential type: when min_value is negative & max_value is positive, DataRange ends widh ')' brace");
            }
        }
    }

    static constexpr int32_t
    _CheckHistogramRangeRuleExponentialType(int64_t histogram_bucket_range, int64_t histogram_min_value,
        int64_t histogram_max_value, bool histogram_start_brace_has_equal, bool histogram_end_brace_has_equal)
    {
        _CheckHistogramRangeRuleExponentialTypeSyntax(histogram_min_value, histogram_max_value,
            histogram_start_brace_has_equal, histogram_end_brace_has_equal);

        int32_t bucket_start {0};
        if (1 == histogram_min_value)
        {
            bucket_start = 1;
        }
        else if (1 < histogram_min_value)
        {
            bucket_start = 1;
            int64_t tmp_min_value {1};
            while (tmp_min_value < histogram_min_value)
            {
                tmp_min_value *= histogram_bucket_range;
                bucket_start++;
                if (tmp_min_value > histogram_min_value)
                {
                    throw std::invalid_argument("Histogram exponential type min_value has to be a (-/+)bucket_range^n");
                }
            }
        }
        else if (-1 == histogram_min_value)
        {
            bucket_start = -1;
        }
        else if (-1 > histogram_min_value)
        {
            bucket_start = -1;
            int64_t tmp_min_value {-1};
            while (tmp_min_value > histogram_min_value)
            {
                tmp_min_value *= histogram_bucket_range;
                bucket_start--;
                if (tmp_min_value < histogram_min_value)
                {
                    throw std::invalid_argument("Histogram exponential type min_value has to be a (-/+)bucket_range^n");
                }
            }
        }

        int32_t bucket_end {0};
        if (1 == histogram_max_value)
        {
            bucket_end = 1;
        }
        else if (1 < histogram_max_value)
        {
            bucket_end = 1;
            int64_t tmp_max_value {1};
            while (tmp_max_value < histogram_max_value)
            {
                tmp_max_value *= histogram_bucket_range;
                bucket_end++;
                if (tmp_max_value > histogram_max_value)
                {
                    throw std::invalid_argument("Histogram exponential type max_value has to be a (-/+)bucket_range^n");
                }
            }
        }
        else if (-1 == histogram_max_value)
        {
            bucket_end = -1;
        }
        else if (-1 > histogram_max_value)
        {
            bucket_end = -1;
            int64_t tmp_max_value {-1};
            while (tmp_max_value > histogram_max_value)
            {
                tmp_max_value *= histogram_bucket_range;
                bucket_end--;
                if (tmp_max_value < histogram_max_value)
                {
                    throw std::invalid_argument("Histogram exponential type max_value has to be a (-/+)bucket_range^n");
                }
            }
        }

        if (0 > bucket_start && 0 < bucket_end)
        {
            if (20 < bucket_end - bucket_start + 1)
            {
                throw std::invalid_argument("Histogram bucket_size is bigger than 20");
            }
        }
        else
        {
            if (20 < bucket_end - bucket_start)
            {
                throw std::invalid_argument("Histogram bucket_size is bigger than 20");
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

#endif // AIR_CONFIG_CHECKER_VALUE_HISTOGRAM_H
