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

#ifndef AIR_CONFIG_CHECKER_KEY_H
#define AIR_CONFIG_CHECKER_KEY_H

#include <cstdint>
#include <stdexcept>

#include "src/config/ConfigLib.h"
#include "src/lib/StringView.h"

namespace config
{
static constexpr int32_t
CheckKeyTypo(
    ParagraphType type, const air::string_view* key_list, air::string_view sentence)
{
    size_t cur_pos {sentence.find(":")};
    size_t prev_pos {0};

    while (air::string_view::npos != cur_pos)
    {
        air::string_view key {sentence.substr(prev_pos, cur_pos - prev_pos)};
        key = Strip(key);

        bool find {false};

        if (ParagraphType::DEFAULT == type)
        {
            for (uint32_t i = 0; i < NUM_DEFAULT_KEY; i++)
            {
                if (key == key_list[i])
                {
                    find = true;
                    break;
                }
            }
        }
        else if (ParagraphType::GROUP == type)
        {
            for (uint32_t i = 0; i < NUM_GROUP_KEY; i++)
            {
                if (key == key_list[i])
                {
                    find = true;
                    break;
                }
            }
        }
        else if (ParagraphType::FILTER == type)
        {
            for (uint32_t i = 0; i < NUM_FILTER_KEY; i++)
            {
                if (key == key_list[i])
                {
                    find = true;
                    break;
                }
            }
        }
        else if (ParagraphType::BUCKET == type)
        {
            for (uint32_t i = 0; i < NUM_BUCKET_KEY; i++)
            {
                if (key == key_list[i])
                {
                    find = true;
                    break;
                }
            }
        }
        else
        {
            throw std::invalid_argument("CheckKeyTypo got invalid type");
        }

        if (false == find)
        {
            throw std::logic_error("Invalid key was used");
        }

        cur_pos = sentence.find(":", cur_pos + 1);
        prev_pos = sentence.rfind(",", cur_pos + 1);
        prev_pos += 1;
    }

    return 0;
}

static constexpr int32_t
CheckKeyTypoNode(const air::string_view* key_list, air::string_view sentence)
{
    size_t cur_pos {sentence.find(":")};
    size_t prev_pos {0};
    bool is_histogram_type {false};
    bool has_bucket_key {false};

    while (air::string_view::npos != cur_pos)
    {
        air::string_view key {sentence.substr(prev_pos, cur_pos - prev_pos)};
        key = Strip(key);

        bool find {false};

        if (key == "Bucket")
        {
            has_bucket_key = true;
        }
        if (key == "Type")
        {
            size_t next_comma {sentence.find(",", cur_pos + 1)};
            if (air::string_view::npos == next_comma)
            {
                next_comma = sentence.size();
            }
            air::string_view value {
                sentence.substr(cur_pos + 1, next_comma - cur_pos - 1)};
            value = Strip(value);
            if (value == "HISTOGRAM" || value == "Histogram")
            {
                is_histogram_type = true;
            }
        }
        for (uint32_t i = 0; i < NUM_NODE_KEY; i++)
        {
            if (key == key_list[i])
            {
                find = true;
                break;
            }
        }

        if (false == find)
        {
            throw std::logic_error("Invalid key was used");
        }

        cur_pos = sentence.find(":", cur_pos + 1);
        prev_pos = sentence.rfind(",", cur_pos + 1);
        prev_pos += 1;
    }

    if (!is_histogram_type && has_bucket_key)
    {
        throw std::logic_error("Only histogram type can have Bucket option");
    }

    if (is_histogram_type && !has_bucket_key)
    {
        throw std::logic_error("Histogram type must have Bucket option");
    }

    return 0;
}

static constexpr int32_t
CheckMandatoryKey(const air::string_view* key_list, uint32_t num_mandatory,
    air::string_view sentence)
{
    int key_cnt {0};

    uint32_t num_find {0};
    for (uint32_t index = 0; index < num_mandatory; index++, key_cnt++)
    {
        size_t cur_pos {sentence.find(":")};
        size_t prev_pos {0};
        air::string_view mandatory_key {key_list[key_cnt]};
        bool find {false};

        while (air::string_view::npos != cur_pos)
        {
            air::string_view sentence_key {
                sentence.substr(prev_pos, cur_pos - prev_pos)};
            sentence_key = Strip(sentence_key);

            if (mandatory_key == sentence_key)
            {
                find = true;
                num_find += 1;
                break;
            }

            cur_pos = sentence.find(":", cur_pos + 1);
            prev_pos = sentence.rfind(",", cur_pos + 1);
            prev_pos += 1;
        }

        if (false == find)
        {
            throw std::logic_error("Mandatory key is missing");
        }
    }

    return 0;
}

static constexpr int32_t
CheckKeyDuplication(air::string_view sentence)
{
    size_t colon_pos {sentence.find(":")};
    size_t comma_pos {0};

    while (air::string_view::npos != colon_pos)
    {
        air::string_view target_key {
            sentence.substr(comma_pos, colon_pos - comma_pos)};
        target_key = Strip(target_key);

        size_t comparative_colon {sentence.find(":", colon_pos + 1)};
        size_t comparative_comma {sentence.find(",", comma_pos + 1)};
        comparative_comma += 1;
        while (air::string_view::npos != comparative_colon)
        {
            air::string_view comparative_key {sentence.substr(
                comparative_comma, comparative_colon - comparative_comma)};
            comparative_key = Strip(comparative_key);

            if (target_key == comparative_key)
            {
                throw std::logic_error("Key duplicated");
            }

            comparative_colon = sentence.find(":", comparative_colon + 1);
            comparative_comma = sentence.rfind(",", comparative_colon + 1);
            comparative_comma += 1;
        }

        colon_pos = sentence.find(":", colon_pos + 1);
        comma_pos = sentence.rfind(",", colon_pos + 1);
        comma_pos += 1;
    }
    return 0;
}

} // namespace config

#endif // AIR_CONFIG_CHECKER_KEY_H
