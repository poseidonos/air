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

#include "src/policy/RuleManager.h"

#include "src/config/ConfigInterface.h"

int
policy::Subject::Notify(uint32_t index, uint32_t type1, uint32_t type2,
    uint32_t value1, uint32_t value2, int pid, int cmd_type, int cmd_order)
{
    int result = -1;
    if (index < to_dtype(pi::PolicySubject::COUNT))
    {
        arr_observer[index]->Update(
            type1, type2, value1, value2, pid, cmd_type, cmd_order);
        result = 0;
    }

    return result;
}

void
policy::RuleManager::SetNodeMetaConfig(void* node)
{
    air::NodeMetaData* node_meta;
    node_meta = (air::NodeMetaData*)node;
    uint32_t num_nodes = cfg::GetSentenceCount(config::ParagraphType::NODE);
    for (uint32_t i = 0; i < num_nodes; i++)
    {
        air::string_view node_name =
            cfg::GetSentenceName(config::ParagraphType::NODE, i);
        air::string_view type =
            cfg::GetStrValue(config::ParagraphType::NODE, "Type", node_name);
        bool run = cfg::GetIntValue(config::ParagraphType::NODE, "Run", node_name);
        if (false ==
            cfg::GetIntValue(config::ParagraphType::NODE, "Build", node_name))
        {
            run = false;
        }

        node_meta[i].nid = i;
        if (0 == type.compare("PERFORMANCE") || 0 == type.compare("Performance"))
        {
            node_meta[i].processor_type = air::ProcessorType::PERFORMANCE;
        }
        else if (0 == type.compare("LATENCY") || 0 == type.compare("Latency"))
        {
            node_meta[i].processor_type = air::ProcessorType::LATENCY;
        }
        else if (0 == type.compare("QUEUE") || 0 == type.compare("Queue"))
        {
            node_meta[i].processor_type = air::ProcessorType::QUEUE;
        }
        else if (0 == type.compare("UTILIZATION") ||
            0 == type.compare("Utilization"))
        {
            node_meta[i].processor_type = air::ProcessorType::UTILIZATION;
        }
        else if (0 == type.compare("COUNT") || 0 == type.compare("Count"))
        {
            node_meta[i].processor_type = air::ProcessorType::COUNT;
        }
        else if (0 == type.compare("HISTOGRAM") || 0 == type.compare("Histogram"))
        {
            node_meta[i].processor_type = air::ProcessorType::HISTOGRAM;
        }
        node_meta[i].run = run;

        air::string_view group_name =
            cfg::GetStrValue(config::ParagraphType::NODE, "Group", node_name);
        if (group_name != "")
        {
            uint32_t gid =
                cfg::GetSentenceIndex(config::ParagraphType::GROUP, group_name);
            node_meta[i].group_id = gid;
        }

        node_meta[i].index_size =
            cfg::GetIntValue(config::ParagraphType::NODE, "IndexSize", i, "");

        air::string_view filter_name =
            cfg::GetStrValue(config::ParagraphType::NODE, "Filter", node_name);
        node_meta[i].filter_size = cfg::GetItemSizeWithFilterName(filter_name);
    }
}

void
policy::RuleManager::SetGlobalConfig(void)
{
    ruler->SetStreamingInterval(
        cfg::GetIntValue(config::ParagraphType::DEFAULT, "StreamingInterval"));
    ruler->SetAirBuild(
        cfg::GetIntValue(config::ParagraphType::DEFAULT, "AirBuild"));
    ruler->SetFileWrite(
        cfg::GetIntValue(config::ParagraphType::DEFAULT, "FileWrite"));
    ruler->SetRemainingFileCount(
        cfg::GetIntValue(config::ParagraphType::DEFAULT, "RemainingFileCount"));
}

int
policy::RuleManager::UpdateRule(uint32_t type1, uint32_t type2, uint32_t value1,
    uint32_t value2, int pid, int cmd_type, int cmd_order)
{
    int result = ruler->CheckRule(type1, type2, value1, value2);
    if (0 == result)
    {
        if (ruler->SetRule(type1, type2, value1, value2))
        {
            uint16_t upper_bit = static_cast<uint16_t>(type2 >> 16);

            switch (upper_bit)
            {
                case (to_dtype(pi::Type2_Upper::COLLECTION)):
                {
                    subject->Notify(to_dtype(pi::PolicySubject::TO_COLLECTION),
                        to_dtype(pi::Type1::POLICY_TO_COLLECTION), type2, value1,
                        value2, pid, cmd_type, cmd_order);
                    break;
                }

                case (to_dtype(pi::Type2_Upper::OUTPUT)):
                {
                    subject->Notify(to_dtype(pi::PolicySubject::TO_OUTPUT),
                        to_dtype(pi::Type1::POLICY_TO_OUTPUT), 0, 0, 0, pid,
                        cmd_type, cmd_order);
                    break;
                }
            }
        }
    }
    return result;
}

void
policy::RuleManager::HandleMsg(void)
{
    while (!msg.empty())
    {
        lib::MsgEntry entry = msg.front();
        msg.pop();
        int result = UpdateRule(entry.type1, entry.type2, entry.value1,
            entry.value2, entry.pid, entry.cmd_type, entry.cmd_order);
        if (0 > result)
        {
            subject->Notify(to_dtype(pi::PolicySubject::TO_OUTPUT),
                to_dtype(pi::Type1::POLICY_TO_OUTPUT), 0, result * -1, 0, entry.pid,
                entry.cmd_type, entry.cmd_order);
        }
    }
}
