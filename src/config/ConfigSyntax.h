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

#ifndef AIR_CONFIG_SYNTAX_H
#define AIR_CONFIG_SYNTAX_H

#include "src/config/ConfigChecker.h"
#include "src/config/ConfigLib.h"
#include "src/config/ConfigParser.h"
#include "src/config/ConfigReader.h"
#include "src/lib/StringView.h"

namespace config
{
class ConfigSyntax
{
public:
    constexpr int32_t
    CheckParagraphRule(ParagraphType type)
    {
        return ConfigChecker::CheckParagraphRule(type, paragraphs[dtype(type)]);
    }

    constexpr int32_t
    CheckKeyRule(ParagraphType type)
    {
        const uint32_t paragraph_type_index{dtype(type)};
        int32_t result{0};

        for (uint32_t sentence_index = 0; sentence_index < sentences_count[paragraph_type_index]; sentence_index++)
        {
            result = ConfigChecker::CheckKeyRule(type,
                ConfigParser::GetSentenceFromParagraph(type, sentence_index));
            if (result < 0)
            {
                return result;
            }
        }

        return 0;
    }

    constexpr int32_t
    CheckValueRule(ParagraphType type)
    {
        const uint32_t paragraph_type_index{dtype(type)};
        int32_t result{0};

        for (uint32_t sentence_index = 0; sentence_index < sentences_count[paragraph_type_index]; sentence_index++)
        {
            result = ConfigChecker::CheckValueRule(type,
                ConfigParser::GetSentenceFromParagraph(type, sentence_index));
            if (result < 0)
            {
                return result;
            }
        }

        return 0;
    }
};

class ConfigSyntaxVerification
{
public:
    ConfigSyntaxVerification(void)
    {
        ConfigSyntax syntax;
        static_assert(0 == syntax.CheckParagraphRule(ParagraphType::DEFAULT), "[Default] Paragraph Violation!");
        static_assert(0 == syntax.CheckKeyRule(ParagraphType::DEFAULT), "[Default] Key Violation!");
        static_assert(0 == syntax.CheckValueRule(ParagraphType::DEFAULT), "[Default] Value Violation!");

        static_assert(0 == syntax.CheckParagraphRule(ParagraphType::GROUP), "[Group] Paragraph Violation!");
        static_assert(0 == syntax.CheckKeyRule(ParagraphType::GROUP), "[Group] Key Violation!");
        static_assert(0 == syntax.CheckValueRule(ParagraphType::GROUP), "[Group] Value Violation!");

        static_assert(0 == syntax.CheckParagraphRule(ParagraphType::FILTER), "[Filter] Paragraph Violation!");
        static_assert(0 == syntax.CheckKeyRule(ParagraphType::FILTER), "[Filter] Key Violation!");
        static_assert(0 == syntax.CheckValueRule(ParagraphType::FILTER), "[Filter] Value Violation!");

        static_assert(0 == syntax.CheckParagraphRule(ParagraphType::NODE), "[Node] Paragraph Violation!");
        static_assert(0 == syntax.CheckKeyRule(ParagraphType::NODE), "[Node] Key Violation!");
        static_assert(0 == syntax.CheckValueRule(ParagraphType::NODE), "[Node] Value Violation!");
    }
};

} // namespace config

#endif // AIR_CONFIG_SYNTAX_H
