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

#ifndef AIR_JSONDOC_H
#define AIR_JSONDOC_H

#include <deque>
#include <initializer_list>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <utility>

#include "src/lib/json/Jsonprint.h"
#include "src/lib/json/Jsontype.h"

namespace air
{
class JSONdoc
{
public:
    JSONdoc(JSONtype new_type = JSONtype::WHITESPACE, bool new_ownership = false,
        bool new_key = false)
    : type(new_type),
      ownership(new_ownership),
      key(new_key)
    {
    }

    template<typename T>
    void
    operator=(std::initializer_list<T> t)
    {
        if (false == key)
        {
            throw std::logic_error(
                "[error][JSONdoc::operator=<T>] invalid syntax: no key");
        }
        Clear();
        is_cleared = true;
        operator+=(t);
    }

    void
    operator=(void* empty)
    {
        Clear();
    }

    template<typename T>
    void
    operator+=(std::initializer_list<T> t)
    {
        if (false == key)
        {
            throw std::logic_error(
                "[error][JSONdoc::operator+=<T>] invalid syntax: no key");
        }
        auto it = t.begin();
        size_t map_key = map.size();
        while (it != t.end())
        {
            std::string value_type(typeid(*it).name());

            T* dt = new T {*it};

            type = cpp_type(std::type_index(typeid(*dt)));

            if (JSONtype::NULLVAL == type)
            {
                map.insert({std::to_string(map_key), {nullptr, type}});
                delete dt;
            }
            else if (JSONtype::UNDEFINED == type)
            {
                throw std::logic_error(
                    "[error][JSONdoc::operator+=<T>] deduction failed");
            }
            else
            {
                map.insert({std::to_string(map_key), {dt, type}});
            }

            ++it;
            map_key++;
        }
        if (2 <= map.size() || (false == is_cleared))
        {
            type = JSONtype::ARRAY;
        }
        is_cleared = false;
    }

    void
    operator+=(std::initializer_list<const char*> t)
    {
        if (false == key)
        {
            throw std::logic_error(
                "[error][JSONdoc::operator+=<const char*>] invalid syntax: no key");
        }
        auto it = t.begin();
        size_t map_key = map.size();
        while (it != t.end())
        {
            std::string value_type(typeid(*it).name());

            if (0 == value_type.compare("PKc"))
            {
                type = JSONtype::CHARP;
                std::string* str_value = new std::string {*it};
                map.insert({std::to_string(map_key), {str_value, JSONtype::CHARP}});
            }
            ++it;
            map_key++;
        }
        if (2 <= map.size() || (false == is_cleared))
        {
            type = JSONtype::ARRAY;
        }
        is_cleared = false;
    }

    void
    operator+=(std::initializer_list<JSONdoc> t)
    {
        if (false == key)
        {
            throw std::logic_error(
                "[error][JSONdoc::operator+=<JSONdoc>] invalid syntax: no key");
        }
        if (1 >= map.size() + t.size() && is_cleared)
        {
            type = (*(t.begin())).type;
            map = (*(t.begin())).map;
            ownership = false;
        }
        else
        {
            type = JSONtype::ARRAY;
            auto it = t.begin();
            size_t map_key = map.size();
            while (it != t.end())
            {
                JSONdoc* doc = new JSONdoc {};
                doc->type = (*it).type; // JSONtype::OBJECT;
                doc->map = (*it).map;
                doc->ownership = false;
                map.insert({std::to_string(map_key), {doc, JSONtype::OBJECT}});
                ++it;
                map_key++;
            }
        }
        is_cleared = false;
    }

    JSONdoc& operator[](std::string key)
    {
        auto it = map.find(key);
        if (it != map.end())
        { // found
            void* v = (it->second).data;
            JSONdoc* doc = reinterpret_cast<JSONdoc*>(v);
            return *doc;
        }
        else
        { // not found
            JSONdoc* doc = new JSONdoc {JSONtype::OBJECT, true, true};
            map.insert({key, {doc, JSONtype::OBJECT}});
            return *doc;
        }
    }

    const JSONdoc& operator[](std::string key) const
    {
        auto it = map.find(key);
        if (it != map.end())
        {
            void* v = (it->second).data;
            return *(reinterpret_cast<JSONdoc*>(v));
        }
        else
        {
            throw std::logic_error("[error][JSONdoc::operator[] invalid key");
        }
    }

    friend std::ostream&
    operator<<(std::ostream& os, const JSONdoc& doc)
    {
        switch (doc.type)
        {
            case JSONtype::OBJECT:
            {
                os << "{";
                size_t obj_count = 0;
                for (auto const& obj : doc.map)
                {
                    void* v = obj.second.data;
                    JSONdoc* child_doc = reinterpret_cast<JSONdoc*>(v);
                    os << "\"" << obj.first << "\": " << *child_doc;
                    obj_count++;
                    if (obj_count == doc.map.size())
                    {
                        break;
                    }
                    os << ", ";
                }
                os << "}";
                break;
            }
            case JSONtype::ARRAY:
            {
                os << "[";
                size_t value_count = 0;
                for (auto const& obj : doc.map)
                {
                    if (JSONtype::OBJECT == obj.second.type)
                    {
                        void* v = obj.second.data;
                        JSONdoc* child_doc = reinterpret_cast<JSONdoc*>(v);
                        os << *child_doc;
                    }
                    else
                    {
                        PrintValue(os, obj.second.type, obj.second.data);
                    }
                    value_count++;
                    if (doc.map.size() == value_count)
                    {
                        break;
                    }
                    os << ", ";
                }
                os << "]";
                break;
            }
            case JSONtype::WHITESPACE:
            {
                os << " ";
                break;
            }
            case JSONtype::UNDEFINED:
            {
                throw std::logic_error("[error][JSONdoc::operator<<] Invalid JSONtype");
                break;
            }
            default:
            {
                auto value = doc.map.begin();
                PrintValue(os, (*value).second.type, (*value).second.data);
                break;
            }
        }
        return os;
    }

    bool
    HasKey(std::string key) const
    {
        auto it = map.find(key);
        if (it != map.end())
        {
            return true;
        }
        return false;
    }

    void
    Clear(void)
    {
        for (const auto& value : map)
        {
            DeleteValue(value.second.type, value.second.data);
        }
        map.clear();
    }

    void
    DeleteValue(JSONtype type, void* v)
    {
        switch (type)
        {
            case JSONtype::OBJECT:
            {
                JSONdoc* doc = reinterpret_cast<JSONdoc*>(v);
                if (doc->ownership)
                {
                    doc->Clear();
                }
                delete doc;
                break;
            }
            case JSONtype::LONGLONG:
            {
                long long* value = reinterpret_cast<long long*>(v);
                delete value;
                break;
            }
            case JSONtype::ULONGLONG:
            {
                unsigned long long* value = reinterpret_cast<unsigned long long*>(v);
                delete value;
                break;
            }
            case JSONtype::INT16:
            {
                int16_t* value = reinterpret_cast<int16_t*>(v);
                delete value;
                break;
            }
            case JSONtype::INT32:
            {
                int32_t* value = reinterpret_cast<int32_t*>(v);
                delete value;
                break;
            }
            case JSONtype::INT64:
            {
                int64_t* value = reinterpret_cast<int64_t*>(v);
                delete value;
                break;
            }
            case JSONtype::UINT16:
            {
                uint16_t* value = reinterpret_cast<uint16_t*>(v);
                delete value;
                break;
            }
            case JSONtype::UINT32:
            {
                uint32_t* value = reinterpret_cast<uint32_t*>(v);
                delete value;
                break;
            }
            case JSONtype::UINT64:
            {
                uint64_t* value = reinterpret_cast<uint64_t*>(v);
                delete value;
                break;
            }
            case JSONtype::FLOAT:
            {
                float* value = reinterpret_cast<float*>(v);
                delete value;
                break;
            }
            case JSONtype::DOUBLE:
            {
                double* value = reinterpret_cast<double*>(v);
                delete value;
                break;
            }
            case JSONtype::STRING:
            {
                std::string* value = reinterpret_cast<std::string*>(v);
                delete value;
                break;
            }
            case JSONtype::CHARP:
            {
                std::string* value = reinterpret_cast<std::string*>(v);
                delete value;
                break;
            }
            case JSONtype::BOOL:
            {
                bool* value = reinterpret_cast<bool*>(v);
                delete value;
                break;
            }
            case JSONtype::NULLVAL:
            {
                break;
            }
            default:
            {
                throw std::logic_error("[error][JSONdoc::clear] Invalid JSONtype");
                break;
            }
        }
    }

    bool
    Ownership(void)
    {
        return ownership;
    }
    void
    SetType(JSONtype new_type)
    {
        type = new_type;
    }

    void
    Copy(JSONdoc* dst)
    {
        for (const auto& v : map)
        {
            if (JSONtype::OBJECT == v.second.type)
            {
                JSONdoc* doc = reinterpret_cast<JSONdoc*>(v.second.data);
                JSONdoc* sub_dst = new JSONdoc {doc->type, true, true};
                dst->map.insert({v.first, {sub_dst, JSONtype::OBJECT}});
                doc->Copy(sub_dst);
            }
            else if (JSONtype::ARRAY == v.second.type)
            {
            }
            else
            {
                void* data = new_data(v.second.type, v.second.data);
                dst->map.insert({v.first, {data, v.second.type}});
            }
        }
    }

    auto
    begin(void) const
    {
        return map.begin();
    }

    auto
    end(void) const
    {
        return map.end();
    }

    JSONdoc&&
    Compound(std::deque<std::string> node_queue)
    {
        JSONdoc* doc = new JSONdoc {JSONtype::OBJECT, false, false};

        JSONdoc& groups = (*this)["group"];
        for (auto& group : groups)
        {
            JSONdoc& nodes = groups[group.first]["node"];
            for (auto& node : nodes)
            {
                for (const auto& node_name : node_queue)
                {
                    if (node.first == node_name)
                    {
                        JSONdoc& item = nodes[node_name];
                        doc->map.insert({node_name, {(void*)&item, JSONtype::OBJECT}});
                        break;
                    }
                }
            }
        }
        return std::move(*doc);
    }

    friend class RangeImpl;
    friend class RangeIterator;

private:
    JSONdoc& operator=(const JSONdoc&) = delete;

    JSONtype type{JSONtype::WHITESPACE};
    std::map<std::string, JSONvalue> map;
    bool ownership{false};
    bool key{false};
    bool is_cleared{false};
};

} // namespace air

#endif // AIR_JSONDOC_H
