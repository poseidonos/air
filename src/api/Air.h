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

#ifndef AIR_H
#define AIR_H

#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "src/collection/CollectionManager.h"
#include "src/collection/writer/CountWriter.h"
#include "src/collection/writer/LatencyWriter.h"
#include "src/collection/writer/PerformanceWriter.h"
#include "src/collection/writer/QueueWriter.h"
#include "src/collection/writer/UtilizationWriter.h"
#include "src/config/ConfigInterface.h"
#include "src/data_structure/NodeManager.h"
#include "src/lib/Casting.h"
#include "src/lib/Type.h"
#include "src/lib/json/Json.h"
#include "src/object/Instance.h"
#include "src/transfer/Task.h"

#define air_initialize(...) \
    AIR<cfg::GetIntValue(config::ParagraphType::DEFAULT, "AirBuild"), true>::Initialize(__VA_ARGS__)
#define air_activate() \
    AIR<cfg::GetIntValue(config::ParagraphType::DEFAULT, "AirBuild"), true>::Activate()
#define air_deactivate() \
    AIR<cfg::GetIntValue(config::ParagraphType::DEFAULT, "AirBuild"), true>::Deactivate()
#define air_finalize() \
    AIR<cfg::GetIntValue(config::ParagraphType::DEFAULT, "AirBuild"), true>::Finalize()

#define airlog(node_name, filter_item, node_index, data)                                        \
    AIR<cfg::GetIntValue(config::ParagraphType::DEFAULT, "AirBuild"),                           \
        cfg::GetIntValue(config::ParagraphType::NODE, "Build", node_name)>                      \
        ::LogData<cfg::GetSentenceIndex(config::ParagraphType::NODE, node_name),                \
            cfg::GetIntValue(config::ParagraphType::FILTER, "Item",                             \
            cfg::GetStrValue(config::ParagraphType::NODE, "Filter", node_name), filter_item),   \
            cfg::GetNodeType(node_name)>(node_index, data)

// Primary template
template<bool AirBuild, bool NodeBuild>
class AIR
{
public:
    static void Initialize(uint32_t cpu_num = 0);
    static void Activate(void);
    static void Deactivate(void);
    static void Finalize(void);

    template<int32_t node_id, int32_t filter_index, air::ProcessorType node_type,
        air::ProcessorType enable = air::ProcessorType::PROCESSORTYPE_NULL>
    static void LogData(uint64_t node_index, uint64_t value);
    static void LogData(uint32_t node_id, uint32_t filter_index, uint64_t node_index, uint64_t value);
};

// AIR build : true && Node build : true
template<>
class AIR<true, true>
{
public:
    static void
    Activate(void)
    {
        if (nullptr != instance_manager)
        {
            instance_manager->Activate();
        }
    }
    static void
    Finalize(void)
    {
        if (nullptr != instance_manager)
        {
            instance_manager->Finalize();
            delete instance_manager;
            instance_manager = nullptr;
        }
    }
    static void
    Deactivate(void)
    {
        if (nullptr != instance_manager)
        {
            instance_manager->Deactivate();
        }
    }
    static void
    Initialize(uint32_t cpu_num = 0)
    {
        instance_manager = new air::InstanceManager();
        instance_manager->Initialize(cpu_num);
        node_manager = instance_manager->GetNodeManager();
        collection_manager = instance_manager->GetCollectionManager();
    }

    template<int32_t node_id, int32_t filter_index, air::ProcessorType node_type,
        typename std::enable_if<air::ProcessorType::PERFORMANCE == node_type, air::ProcessorType>::type = node_type>
    static void
    LogData(uint64_t node_index, uint64_t value)
    {
        static_assert(-1 != node_id, "Invalid Node");
        static_assert(-1 != filter_index, "Invalid Filter Item");

        perf_writer.LogData(_GetData(node_id, filter_index, node_index), value);
    }

    template<int32_t node_id, int32_t filter_index, air::ProcessorType node_type,
        typename std::enable_if<air::ProcessorType::LATENCY == node_type, air::ProcessorType>::type = node_type>
    static void
    LogData(uint64_t node_index, uint64_t value)
    {
        static_assert(-1 != node_id, "Invalid Node");
        static_assert(-1 != filter_index, "Invalid Filter Item");

        lat_writer.LogData(_GetData(node_id, filter_index, node_index), value);
    }

    template<int32_t node_id, int32_t filter_index, air::ProcessorType node_type,
        typename std::enable_if<air::ProcessorType::QUEUE == node_type, air::ProcessorType>::type = node_type>
    static void
    LogData(uint64_t node_index, uint64_t value)
    {
        static_assert(-1 != node_id, "Invalid Node");
        static_assert(-1 != filter_index, "Invalid Filter Item");

        queue_writer.LogData(_GetData(node_id, filter_index, node_index), value);
    }

    template<int32_t node_id, int32_t filter_index, air::ProcessorType node_type,
        typename std::enable_if<air::ProcessorType::UTILIZATION == node_type, air::ProcessorType>::type = node_type>
    static void
    LogData(uint64_t node_index, uint64_t value)
    {
        static_assert(-1 != node_id, "Invalid Node");
        static_assert(-1 != filter_index, "Invalid Filter Item");

        util_writer.LogData(_GetData(node_id, filter_index, node_index), value);
    }

    template<int32_t node_id, int32_t filter_index, air::ProcessorType node_type,
        typename std::enable_if<air::ProcessorType::COUNT == node_type, air::ProcessorType>::type = node_type>
    static void
    LogData(uint64_t node_index, uint64_t value)
    {
        static_assert(-1 != node_id, "Invalid Node");
        static_assert(-1 != filter_index, "Invalid Filter Item");

        count_writer.LogData(_GetData(node_id, filter_index, node_index), value);
    }

    static void
    LogData(uint32_t node_id, uint32_t filter_index, uint64_t node_index, uint64_t value)
    {
        if ((nullptr == collection_manager) ||
            (false == collection_manager->IsLog(node_id)))
        {
            return;
        }

        if (nullptr == node_data_array && nullptr != node_manager)
        {
            uint32_t tid = syscall(SYS_gettid);
            node_data_array = node_manager->GetNodeDataArray(tid);
            node_manager->SetNodeDataArrayName(tid);
        }
        if (nullptr != node_data_array)
        {
            collection_manager->LogData(node_id, filter_index, node_data_array, node_index, value);
        }
    }

protected:
    static lib::Data*
    _GetData(int32_t node_id, int32_t filter_index, uint64_t node_index)
    {
        if ((nullptr == collection_manager) ||
            (false == collection_manager->IsLog(node_id)))
        {
            return nullptr;
        }
        if (nullptr != node_data_array)
        {
            node::NodeData* node_data = node_data_array->node[node_id];
            if (nullptr == node_data)
            {
                return nullptr;
            }
            return node_data->GetUserDataByNodeIndex(node_index, filter_index);
        }
        else if (nullptr != node_manager)
        {
            uint32_t tid = syscall(SYS_gettid);
            node_data_array = node_manager->GetNodeDataArray(tid);
            node_manager->SetNodeDataArrayName(tid);
        }
        return nullptr;
    }
    static air::InstanceManager* instance_manager;
    static node::NodeManager* node_manager;
    static collection::CollectionManager* collection_manager;
    static thread_local node::NodeDataArray* node_data_array;
    static collection::PerformanceWriter perf_writer;
    static collection::LatencyWriter lat_writer;
    static collection::QueueWriter queue_writer;
    static collection::UtilizationWriter util_writer;
    static collection::CountWriter count_writer;
};

// AIR build : true && Node build : false
template<>
class AIR<true, false>
{
public:
    template<int32_t node_id, int32_t filter_index, air::ProcessorType type>
    static void
    LogData(uint64_t node_index, uint64_t value)
    {
    }
    static void
    LogData(uint32_t node_id, uint32_t filter_index, uint64_t node_index, uint64_t value)
    {
    }
};

// AIR build : false
template<bool NodeBuild>
class AIR<false, NodeBuild>
{
public:
    static void
    Initialize(uint32_t cpu_num = 0)
    {
    }
    static void
    Activate(void)
    {
    }
    static void
    Deactivate(void)
    {
    }
    static void
    Finalize(void)
    {
    }
    template<int32_t node_id, int32_t filter_index, air::ProcessorType type>
    static void
    LogData(uint64_t node_index, uint64_t value)
    {
    }
    static void
    LogData(uint32_t node_id, uint32_t filter_index, uint64_t node_index, uint64_t value)
    {
    }
};

void
air_request_data(transfer::node_list nodes, transfer::task_unit&& function);

#endif // AIR_H
