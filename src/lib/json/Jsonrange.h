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

#ifndef AIR_JSONRANGE_H
#define AIR_JSONRANGE_H

#include <map>
#include <string>

#include "src/lib/json/Jsondoc.h"

namespace air
{
struct RangeValue
{
    RangeValue(std::string key, JSONtype type, void* value)
    : key{key},
      type{type},
      value{value}
    {
    }
    std::string key;
    JSONtype type;
    void* value;
};

class RangeIterator
{
public:
    explicit RangeIterator(std::map<std::string, JSONvalue>::iterator it)
    : it{it}
    {
    }

    RangeIterator&
    operator++(void)
    {
        ++it;
        return *this;
    }

    bool
    operator!=(const RangeIterator& rhs) const
    {
        return it != rhs.it;
    }

    RangeValue operator*(void)const
    {
        void* vp = (*it).second.data;
        JSONdoc& doc = *((JSONdoc*)vp);
        auto doc_it = doc.map.begin();
        return RangeValue{(*it).first, doc_it->second.type, doc_it->second.data};
    }

private:
    std::map<std::string, JSONvalue>::iterator it;
};

class RangeImpl
{
public:
    explicit RangeImpl(JSONdoc& obj)
    : obj{obj}
    {
    }

    RangeIterator
    begin(void) const
    {
        return RangeIterator{obj.map.begin()};
    }

    RangeIterator
    end(void) const
    {
        return RangeIterator{obj.map.end()};
    }

    static RangeImpl
    range(JSONdoc& obj)
    {
        return RangeImpl{obj};
    }

private:
    JSONdoc& obj;
};

constexpr auto range = RangeImpl::range;

} // namespace air

#endif // AIR_JSONRANGE_H
