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

#ifndef AIR_CONFIG_INTERFACE_H
#define AIR_CONFIG_INTERFACE_H

#include <string>

#include "src/config/ConfigLib.h"
#include "src/config/ConfigParser.h"
#include "src/lib/StringView.h"
#include "src/lib/Type.h"

namespace config
{
class ConfigInterface
{
public:
    ConfigInterface(void)
    {
    }
    virtual ~ConfigInterface(void)
    {
    }

    static constexpr int32_t
    GetSentenceIndex(ParagraphType type, air::string_view name = "")
    {
        return config_parser.GetIndexFromParagraph(type, name);
    }

    static constexpr uint32_t
    GetSentenceCount(ParagraphType type)
    {
        return config_parser.GetSentenceCount(type);
    }

    static constexpr air::string_view
    GetSentenceName(ParagraphType type, uint32_t index)
    {
        air::string_view key {""};
        if (ParagraphType::GROUP == type)
        {
            key = "Group";
        }
        else if (ParagraphType::NODE == type)
        {
            key = "Node";
        }
        else if (ParagraphType::FILTER == type)
        {
            key = "Filter";
        }
        else if (ParagraphType::BUCKET == type)
        {
            key = "Bucket";
        }
        else
        {
            return "";
        }

        air::string_view sentence = config_parser.GetSentenceFromParagraph(type, index);
        return config_parser.GetStrValueFromSentence(sentence, key);
    }

    static constexpr int32_t
    GetIntValue(ParagraphType type, air::string_view key, int index_c, air::string_view item)
    {
        if (ParagraphType::DEFAULT == type)
        {
            if (key != "AirBuild" && key != "StreamingInterval" && key != "NodeBuild" && key != "NodeRun" &&
                key != "NodeSamplingRatio" && key != "NodeIndexSize")
            {
                return -1;
            }
        }
        else if (ParagraphType::GROUP == type)
        {
            if (key != "NodeBuild" && key != "NodeRun" && key != "NodeSamplingRatio" && key != "NodeIndexSize")
            {
                return -1;
            }
        }
        else if (ParagraphType::FILTER == type)
        {
            if (key != "Item")
            {
                return -1;
            }
        }
        else if (ParagraphType::BUCKET == type)
        {
            if (key != "Scale")
            {
                return -1;
            }
        }
        else if (ParagraphType::NODE == type)
        {
            if (key != "Build" && key != "Run" && key != "SamplingRatio" && key != "IndexSize")
            {
                return -1;
            }
        }

        int32_t index {index_c};
        air::string_view sentence {config_parser.GetSentenceFromParagraph(type, index)};
        int32_t ret {config_parser.GetIntValueFromSentence(sentence, key, item)};

        if (0 > ret && (ParagraphType::NODE == type || ParagraphType::GROUP == type))
        {
            ret = _GetIntValueFromUpperLevel(type, key, index, sentence);
        }

        return ret;
    }

    static constexpr int32_t
    GetIntValue(ParagraphType type, air::string_view key, air::string_view name = "", air::string_view item = "")
    {
        int32_t index {config_parser.GetIndexFromParagraph(type, name)};
        return GetIntValue(type, key, index, item);
    }

    static constexpr air::string_view
    GetStrValue(ParagraphType type, air::string_view key, air::string_view name = "")
    {
        int32_t index {config_parser.GetIndexFromParagraph(type, name)};
        air::string_view sentence {config_parser.GetSentenceFromParagraph(type, index)};
        return config_parser.GetStrValueFromSentence(sentence, key);
    }

    static constexpr air::ProcessorType
    GetNodeType(air::string_view node_name)
    {
        int32_t index {config_parser.GetIndexFromParagraph(ParagraphType::NODE, node_name)};
        air::string_view sentence {config_parser.GetSentenceFromParagraph(ParagraphType::NODE, index)};
        air::string_view type {config_parser.GetStrValueFromSentence(sentence, "Type")};
        if (0 == type.compare("PERFORMANCE") || 0 == type.compare("Performance"))
        {
            return air::ProcessorType::PERFORMANCE;
        }
        else if (0 == type.compare("LATENCY") || 0 == type.compare("Latency"))
        {
            return air::ProcessorType::LATENCY;
        }
        else if (0 == type.compare("QUEUE") || 0 == type.compare("Queue"))
        {
            return air::ProcessorType::QUEUE;
        }
        else if (0 == type.compare("UTILIZATION") || 0 == type.compare("Utilization"))
        {
            return air::ProcessorType::UTILIZATION;
        }
        else if (0 == type.compare("COUNT") || 0 == type.compare("Count"))
        {
            return air::ProcessorType::COUNT;
        }
        else if (0 == type.compare("HISTOGRAM") || 0 == type.compare("Histogram"))
        {
            return air::ProcessorType::HISTOGRAM;
        }
        return air::ProcessorType::PROCESSORTYPE_NULL;
    }

    static std::string
    GetItemStrWithNodeName(air::string_view node_name, uint32_t item_index)
    {
        air::string_view filter_name {GetStrValue(ParagraphType::NODE, "Filter", node_name)};
        int32_t index {config_parser.GetIndexFromParagraph(ParagraphType::FILTER, filter_name)};
        air::string_view sentence {config_parser.GetSentenceFromParagraph(ParagraphType::FILTER, index)};
        return config_parser.GetItemStrFromFilterSentence(sentence, item_index);
    }

    static std::string
    GetItemStrWithFilterName(air::string_view filter_name, uint32_t item_index)
    {
        int32_t index {config_parser.GetIndexFromParagraph(ParagraphType::FILTER, filter_name)};
        air::string_view sentence {config_parser.GetSentenceFromParagraph(ParagraphType::FILTER, index)};
        return config_parser.GetItemStrFromFilterSentence(sentence, item_index);
    }

    static constexpr int32_t
    GetItemSizeWithFilterName(air::string_view filter_name)
    {
        int32_t index {config_parser.GetIndexFromParagraph(ParagraphType::FILTER, filter_name)};
        air::string_view sentence {config_parser.GetSentenceFromParagraph(ParagraphType::FILTER, index)};
        return config_parser.GetItemSizeFromFilterSentence(sentence);
    }

    static constexpr int64_t
    GetLowerBoundWithBucketName(air::string_view bucket_name)
    {
        int32_t index {config_parser.GetIndexFromParagraph(ParagraphType::BUCKET, bucket_name)};
        air::string_view sentence {config_parser.GetSentenceFromParagraph(ParagraphType::BUCKET, index)};
        return config_parser.GetLowerBoundFromBucketSentence(sentence);
    }

    static constexpr int64_t
    GetUpperBoundWithBucketName(air::string_view bucket_name)
    {
        int32_t index {config_parser.GetIndexFromParagraph(ParagraphType::BUCKET, bucket_name)};
        air::string_view sentence {config_parser.GetSentenceFromParagraph(ParagraphType::BUCKET, index)};
        return config_parser.GetUpperBoundFromBucketSentence(sentence);
    }

    static constexpr bool
    IsLinearTypeWithBucketName(air::string_view bucket_name)
    {
        int32_t index {config_parser.GetIndexFromParagraph(ParagraphType::BUCKET, bucket_name)};
        air::string_view sentence {config_parser.GetSentenceFromParagraph(ParagraphType::BUCKET, index)};
        return config_parser.IsLinearTypeFromBucketSentence(sentence);
    }

private:
    static constexpr int32_t
    _GetIntValueFromUpperLevel(ParagraphType type, air::string_view key, int32_t index, air::string_view sentence)
    {
        int32_t ret {-1};
        if (ParagraphType::NODE == type)
        {
            air::string_view group_name {config_parser.GetStrValueFromSentence(sentence, "Group")};
            index = config_parser.GetIndexFromParagraph(ParagraphType::GROUP, group_name);
            sentence = config_parser.GetSentenceFromParagraph(ParagraphType::GROUP, index);
            if (key == "Build")
            {
                ret = config_parser.GetIntValueFromSentence(sentence, "NodeBuild");
                if (0 > ret)
                {
                    sentence = config_parser.GetSentenceFromParagraph(ParagraphType::DEFAULT, 0);
                    ret = config_parser.GetIntValueFromSentence(sentence, "NodeBuild");
                }
            }
            else if (key == "Run")
            {
                ret = config_parser.GetIntValueFromSentence(sentence, "NodeRun");
                if (0 > ret)
                {
                    sentence = config_parser.GetSentenceFromParagraph(ParagraphType::DEFAULT, 0);
                    ret = config_parser.GetIntValueFromSentence(sentence, "NodeRun");
                }
            }
            else if (key == "SamplingRatio")
            {
                ret = config_parser.GetIntValueFromSentence(sentence, "NodeSamplingRatio");
                if (0 > ret)
                {
                    sentence = config_parser.GetSentenceFromParagraph(ParagraphType::DEFAULT, 0);
                    ret = config_parser.GetIntValueFromSentence(sentence, "NodeSamplingRatio");
                }
            }
            else if (key == "IndexSize")
            {
                ret = config_parser.GetIntValueFromSentence(sentence, "NodeIndexSize");
                if (0 > ret)
                {
                    sentence = config_parser.GetSentenceFromParagraph(ParagraphType::DEFAULT, 0);
                    ret = config_parser.GetIntValueFromSentence(sentence, "NodeIndexSize");
                }
            }
        }
        else if (ParagraphType::GROUP == type)
        {
            sentence = config_parser.GetSentenceFromParagraph(ParagraphType::DEFAULT, 0);
            ret = config_parser.GetIntValueFromSentence(sentence, key);
        }
        return ret;
    }

    static ConfigParser config_parser;
};

} // namespace config

typedef config::ConfigInterface cfg;

#endif // AIR_CONFIG_INTERFACE_H
