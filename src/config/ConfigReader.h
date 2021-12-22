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

#ifndef AIR_CONFIG_READER_H
#define AIR_CONFIG_READER_H

#include "src/config/ConfigLib.h"
#include "src/lib/StringView.h"

#define AIR_CFG_FILE <AIR_CFG>

namespace config
{
struct ConfigReader
{
    ConfigReader(void)
    {
    }
    ~ConfigReader(void)
    {
    }
    static constexpr uint32_t
    GetStrArrSize(air::string_view cfg_str)
    {
        size_t count {0};
        size_t pos {0};

        pos = cfg_str.find("\"");
        while (air::string_view::npos != pos)
        {
            pos = cfg_str.find("\"", pos + 1);
            count++;
        }

        return (count / 2);
    }

    static constexpr air::string_view
    GetStrArrFromRawData(air::string_view start_del, air::string_view end_del)
    {
        size_t start_pos {raw_data.find(start_del) + start_del.size() + 1};
        size_t end_pos {raw_data.find(end_del)};
        return raw_data.substr(start_pos, end_pos - start_pos);
    }

private:
    static constexpr air::string_view raw_data =
#include AIR_CFG_FILE
        ;
};

static constexpr air::string_view paragraphs[dtype(ParagraphType::COUNT)] {
    ConfigReader::GetStrArrFromRawData("[DEFAULT]", "[/DEFAULT]"),
    ConfigReader::GetStrArrFromRawData("[GROUP]", "[/GROUP]"),
    ConfigReader::GetStrArrFromRawData("[FILTER]", "[/FILTER]"),
    ConfigReader::GetStrArrFromRawData("[HISTOGRAM]", "[/HISTOGRAM]"),
    ConfigReader::GetStrArrFromRawData("[NODE]", "[/NODE]")};

static constexpr uint32_t sentences_count[dtype(ParagraphType::COUNT)] {
    ConfigReader::GetStrArrSize(paragraphs[dtype(ParagraphType::DEFAULT)]),
    ConfigReader::GetStrArrSize(paragraphs[dtype(ParagraphType::GROUP)]),
    ConfigReader::GetStrArrSize(paragraphs[dtype(ParagraphType::FILTER)]),
    ConfigReader::GetStrArrSize(paragraphs[dtype(ParagraphType::HISTOGRAM)]),
    ConfigReader::GetStrArrSize(paragraphs[dtype(ParagraphType::NODE)])};

} // namespace config

#endif // AIR_CONFIG_READER_H
