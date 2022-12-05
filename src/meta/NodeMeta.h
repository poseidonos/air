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

#ifndef AIR_NODE_META_H
#define AIR_NODE_META_H

#include <stdexcept>

#include "src/config/ConfigInterface.h"
#include "src/lib/Casting.h"
#include "src/lib/Type.h"

namespace meta
{
class NodeMeta
{
public:
    virtual ~NodeMeta(void)
    {
    }
    inline air::ProcessorType
    ProcessorType(uint32_t nid) const
    {
        if (MAX_NID_SIZE <= nid)
        {
            throw std::out_of_range("nid exceeded maximum node size!");
        }

        return node[nid].processor_type;
    }
    virtual inline bool
    Run(uint32_t nid) const
    {
        if (MAX_NID_SIZE <= nid)
        {
            throw std::out_of_range("nid exceeded maximum node size!");
        }

        return node[nid].run;
    }
    virtual inline uint32_t
    GroupId(uint32_t nid) const
    {
        if (MAX_NID_SIZE <= nid)
        {
            throw std::out_of_range("nid exceeded maximum node size!");
        }

        return node[nid].group_id;
    }
    inline void*
    Meta(void)
    {
        return &node;
    }
    inline void
    SetProcessorType(uint32_t nid, air::ProcessorType processor_type)
    {
        if (MAX_NID_SIZE <= nid)
        {
            throw std::out_of_range("nid exceeded maximum node size!");
        }

        node[nid].processor_type = processor_type;
    }
    virtual inline void
    SetRun(uint32_t nid, bool run)
    {
        if (MAX_NID_SIZE <= nid)
        {
            throw std::out_of_range("nid exceeded maximum node size!");
        }

        node[nid].run = run;
    }
    virtual inline void
    SetGroupId(uint32_t nid, uint32_t group_id)
    {
        if (MAX_NID_SIZE <= nid)
        {
            throw std::out_of_range("nid exceeded maximum node size!");
        }

        node[nid].group_id = group_id;
    }
    inline void
    SetIndexSize(uint32_t nid, uint32_t index_size)
    {
        if (MAX_NID_SIZE <= nid)
        {
            throw std::out_of_range("nid exceeded maximum node size!");
        }

        node[nid].index_size = index_size;
    }
    inline uint32_t
    IndexSize(uint32_t nid) const
    {
        if (MAX_NID_SIZE <= nid)
        {
            throw std::out_of_range("nid exceeded maximum node size!");
        }

        return node[nid].index_size;
    }
    inline void
    SetFilterSize(uint32_t nid, uint32_t filter_size)
    {
        if (MAX_NID_SIZE <= nid)
        {
            throw std::out_of_range("nid exceeded maximum node size!");
        }

        node[nid].filter_size = filter_size;
    }
    inline uint32_t
    FilterSize(uint32_t nid) const
    {
        if (MAX_NID_SIZE <= nid)
        {
            throw std::out_of_range("nid exceeded maximum node size!");
        }

        return node[nid].filter_size;
    }

private:
    const uint32_t MAX_NID_SIZE {
        cfg::GetSentenceCount(config::ParagraphType::NODE)};
    air::NodeMetaData node[cfg::GetSentenceCount(config::ParagraphType::NODE)];
};

class NodeMetaGetter
{
public:
    NodeMetaGetter(void)
    {
    }
    explicit NodeMetaGetter(NodeMeta* node_meta): node_meta(node_meta)
    {
    }
    virtual ~NodeMetaGetter(void)
    {
    }
    virtual inline air::ProcessorType
    ProcessorType(uint32_t nid) const
    {
        return node_meta->ProcessorType(nid);
    }
    virtual inline bool
    Run(uint32_t nid) const
    {
        return node_meta->Run(nid);
    }
    virtual inline uint32_t
    GroupId(uint32_t nid) const
    {
        return node_meta->GroupId(nid);
    }
    inline void*
    Meta(void)
    {
        return node_meta->Meta();
    }
    virtual inline uint32_t
    IndexSize(uint32_t nid) const
    {
        return node_meta->IndexSize(nid);
    }
    virtual inline uint32_t
    FilterSize(uint32_t nid) const
    {
        return node_meta->FilterSize(nid);
    }

private:
    NodeMeta* node_meta {nullptr};
};

} // namespace meta

#endif // AIR_NODE_META_H
