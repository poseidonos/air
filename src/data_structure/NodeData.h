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

#ifndef AIR_NODE_DATA_H
#define AIR_NODE_DATA_H

#include "src/lib/Data.h"
#include "src/lib/Hash.h"
#include "src/lib/Type.h"

namespace node
{
class NodeData
{
public:
    NodeData(void) = delete;
    NodeData(air::ProcessorType type, uint32_t index_size, uint32_t filter_size);
    virtual ~NodeData(void);
    virtual inline lib::Data*
    GetAirData(uint32_t hash_idx, uint32_t filter_index)
    {
        if (hash_idx >= index_size || filter_index >= filter_size)
        {
            return nullptr;
        }
        return air_data[hash_idx][filter_index];
    }
    virtual inline lib::Data*
    GetUserDataByNodeIndex(uint64_t node_index, uint32_t filter_index)
    {
        if (filter_index >= filter_size)
        {
            return nullptr;
        }

        uint32_t hash_idx = hash_map->GetHashIndex(node_index);
        if (hash_idx == index_size)
        {
            hash_idx = hash_map->InsertHashNode(node_index);
            if (hash_idx == index_size)
            { // hash map is full
                return nullptr;
            }
        }
        return user_data[hash_idx][filter_index];
    }
    virtual inline lib::Data*
    GetUserDataByHashIndex(uint32_t hash_idx, uint32_t filter_index)
    {
        if (hash_idx >= index_size || filter_index >= filter_size)
        {
            return nullptr;
        }
        return user_data[hash_idx][filter_index];
    }
    virtual inline uint64_t
    GetUserHashValue(uint32_t hash_idx)
    {
        return hash_map->GetHashKey(hash_idx);
    }
    virtual inline lib::AccData*
    GetAccData(uint32_t hash_idx, uint32_t filter_index)
    {
        if (hash_idx >= index_size || filter_index >= filter_size)
        {
            return nullptr;
        }
        return acc_data[hash_idx][filter_index];
    }
    virtual inline void
    SwapBuffer(uint32_t hash_idx)
    {
        if (hash_idx >= index_size)
        {
            return;
        }
        lib::Data** tmp_addr = user_data[hash_idx];
        user_data[hash_idx] = air_data[hash_idx];
        air_data[hash_idx] = tmp_addr;
    }
    virtual inline uint32_t
    GetIndexSize(void)
    {
        return index_size;
    }
    virtual inline uint32_t
    GetFilterSize(void)
    {
        return filter_size;
    }

private:
    air::ProcessorType ptype{air::ProcessorType::PROCESSORTYPE_NULL};
    lib::Data*** user_data{
        nullptr,
    };
    lib::Data*** air_data{
        nullptr,
    };
    lib::AccData*** acc_data{
        nullptr,
    };
    uint32_t index_size{0};
    uint32_t filter_size{0};
    air::HashMap<uint64_t>* hash_map{nullptr};
};

} // namespace node

#endif // AIR_NODE_DATA_H
