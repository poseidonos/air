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

#include "src/process/ProcessManager.h"

#include "src/config/ConfigInterface.h"
#include "src/process/processor/CountProcessor.h"
#include "src/process/processor/LatencyProcessor.h"
#include "src/process/processor/PerformanceProcessor.h"
#include "src/process/processor/QueueProcessor.h"
#include "src/process/processor/UtilizationProcessor.h"

process::ProcessManager::~ProcessManager(void)
{
    for (uint32_t i = 0; i < MAX_NID_SIZE; i++)
    {
        if (nullptr != processor[i])
        {
            delete processor[i];
        }
    }
}

int
process::ProcessManager::Init(void)
{
    int num_nodes = 0;
    for (uint32_t i = 0; i < MAX_NID_SIZE; i++)
    {
        switch (node_meta_getter->ProcessorType(i))
        {
            case (air::ProcessorType::PERFORMANCE):
                processor[i] = new PerformanceProcessor;
                break;

            case (air::ProcessorType::LATENCY):
                processor[i] = new LatencyProcessor;
                break;

            case (air::ProcessorType::QUEUE):
                processor[i] = new QueueProcessor;
                break;

            case (air::ProcessorType::UTILIZATION):
                processor[i] = new UtilizationProcessor;
                break;

            case (air::ProcessorType::COUNT):
                processor[i] = new CountProcessor;
                break;

            default:
                break;
        }
        num_nodes++;
    }
    return num_nodes;
}

void
process::ProcessManager::SetTimeSlot(void)
{
    timing_distributor.SetTiming(node_meta_getter, node_manager);
}

void
process::ProcessManager::StreamData(void)
{
    auto& air_json = air::json("air");
    time_t curr_time = time(NULL);

    air_json["timestamp"] = {(int64_t)curr_time};
    air_json["interval"] = {global_meta_getter->StreamingInterval()};
    air_json["play"] = {global_meta_getter->AirPlay()};
    air_json["group"];

    timespec curr_timestamp;
    clock_gettime(CLOCK_MONOTONIC, &curr_timestamp);
    if (0 == last_timestamp.tv_sec && 0 == last_timestamp.tv_nsec)
    {
        time_unit = global_meta_getter->StreamingInterval();
    }
    else
    {
        uint64_t timelag = Timelag::GetDiff(curr_timestamp, last_timestamp);
        time_unit = (double)timelag / UINT_E9;
    }
    last_timestamp = curr_timestamp;

    _AddGroupInfo(air_json);

    for (uint32_t i = 0; i < MAX_NID_SIZE; i++)
    {
        air::ProcessorType type = node_meta_getter->ProcessorType(i);
        if (air::ProcessorType::PROCESSORTYPE_NULL != type)
        {
            air::string_view node_name_view = cfg::GetSentenceName(config::ParagraphType::NODE, i);
            air::string_view group_name_view =
                cfg::GetStrValue(config::ParagraphType::NODE, "Group", node_name_view);

            _AddNodeInfo(group_name_view, node_name_view, i, type);
        }
    }
}

void
process::ProcessManager::_AddGroupInfo(air::JSONdoc& doc)
{
    uint32_t group_size = cfg::GetSentenceCount(config::ParagraphType::GROUP);
    for (uint32_t i = 0; i < group_size; i++)
    {
        int32_t group_name_leng = cfg::GetSentenceName(config::ParagraphType::GROUP, i).size();
        std::string group_name(cfg::GetSentenceName(config::ParagraphType::GROUP, i).data(),
            group_name_leng);

        auto& group_obj = air::json(group_name);
        int32_t group_id = cfg::GetSentenceIndex(
            config::ParagraphType::GROUP, cfg::GetSentenceName(config::ParagraphType::GROUP, i));
        group_obj["group_id"] = {group_id};
        group_obj["node"];
        doc["group"][group_name] = {group_obj};
    }
}

void
process::ProcessManager::_AddNodeInfo(air::string_view& group_name_view,
    air::string_view& node_name_view, uint32_t nid, air::ProcessorType type)
{
    uint32_t index_size = node_meta_getter->IndexSize(nid);
    uint32_t filter_size = node_meta_getter->FilterSize(nid);
    std::string group_name;
    std::string node_name;
    group_name.assign(group_name_view.data(), group_name_view.size());
    node_name.assign(node_name_view.data(), node_name_view.size());

    auto& node_obj = air::json(node_name);
    node_obj["node_id"] = {nid};
    bool node_build =
        (bool)cfg::GetIntValue(config::ParagraphType::NODE, "Build",
            cfg::GetSentenceName(config::ParagraphType::NODE, nid));
    node_obj["build"] = {node_build};
    node_obj["run"] = {node_meta_getter->Run(nid)};
    switch (type)
    {
        case (air::ProcessorType::PERFORMANCE):
            node_obj["type"] = {"performance"};
            break;

        case (air::ProcessorType::LATENCY):
            node_obj["type"] = {"latency"};
            break;

        case (air::ProcessorType::QUEUE):
            node_obj["type"] = {"queue"};
            break;

        case (air::ProcessorType::UTILIZATION):
            node_obj["type"] = {"utilization"};
            break;

        case (air::ProcessorType::COUNT):
            node_obj["type"] = {"count"};
            break;

        default:
            node_obj["type"] = {"undefined"};
            break;
    }

    node_obj["objs"] += {nullptr};
    node_obj["objs"] = {};

    // processing here !!!!
    if (global_meta_getter->AirPlay() && node_build && node_meta_getter->Run(nid))
    {
        if (air::ProcessorType::LATENCY == type)
        {
            for (uint32_t hash_index = 0; hash_index < index_size; hash_index++)
            {
                uint64_t hash_value = hash_index;
                for (const auto& it : node_manager->nda_map)
                {
                    node::NodeDataArray* arr = it.second;
                    node::NodeData* node_data = arr->node[nid];
                    if (nullptr != node_data)
                    {
                        hash_value = node_data->GetUserHashValue(hash_index);
                        if (air::HashMap<uint64_t>::npos != hash_value)
                        {
                            break;
                        }
                    }
                }

                for (uint32_t filter_index = 0; filter_index < filter_size - 1; filter_index++)
                {
                    processor[nid]->StreamData(node_name_view,
                        node_manager->GetAccLatData(nid, hash_index, filter_index),
                        hash_value, filter_index);
                }
            }
        }
        else if (air::ProcessorType::PROCESSORTYPE_NULL != type)
        {
            for (const auto& it : node_manager->nda_map)
            {
                node::NodeDataArray* arr = it.second;
                node::NodeData* node_data = arr->node[nid];
                processor[nid]->StreamData(node_name_view, it.first, arr->tname.c_str(),
                    node_data, type, time_unit, index_size, filter_size);
            }
        }
    }

    auto& group_obj = air::json(group_name);
    group_obj["node"][node_name] = {air::json(node_name)};
}
