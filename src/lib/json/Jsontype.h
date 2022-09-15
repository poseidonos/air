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

#ifndef AIR_JSONTYPE_H
#define AIR_JSONTYPE_H

#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace air
{
enum class JSONtype : uint32_t
{
    OBJECT, // 0
    ARRAY,
    SHORT,
    INT,
    LONG,
    LONGLONG, // 5
    USHORT,
    UINT,
    ULONG,
    ULONGLONG,
    INT16, // 10
    INT32,
    INT64,
    UINT16,
    UINT32,
    UINT64, // 15
    FLOAT,
    DOUBLE,
    STRING,
    CHARP,
    BOOL, // 20
    NULLVAL,
    WHITESPACE,
    UNDEFINED,
};

struct JSONvalue
{
    void* data {nullptr};
    JSONtype type {JSONtype::WHITESPACE};
};

class CPPtype
{
public:
    static JSONtype
    Type(std::type_index index)
    {
        return _GetInstance()._GetType(index); // types[index];
    }

    static void*
    Data(JSONtype type, void* v)
    {
        switch (type)
        {
            case JSONtype::SHORT:
                return new short {*((short*)v)};
            case JSONtype::INT:
                return new int {*((int*)v)};
            case JSONtype::LONG:
                return new long {*((long*)v)};
            case JSONtype::LONGLONG:
                return new long long {*((long long*)v)};
            case JSONtype::USHORT:
                return new unsigned short {*((unsigned short*)v)};
            case JSONtype::UINT:
                return new unsigned int {*((unsigned int*)v)};
            case JSONtype::ULONG:
                return new unsigned long {*((unsigned long*)v)};
            case JSONtype::ULONGLONG:
                return new unsigned long long {*((unsigned long long*)v)};
            case JSONtype::INT16:
                return new int16_t {*((int16_t*)v)};
            case JSONtype::INT32:
                return new int32_t {*((int32_t*)v)};
            case JSONtype::INT64:
                return new int64_t {*((int64_t*)v)};
            case JSONtype::UINT16:
                return new uint16_t {*((uint16_t*)v)};
            case JSONtype::UINT32:
                return new uint32_t {*((uint32_t*)v)};
            case JSONtype::UINT64:
                return new uint64_t {*((uint64_t*)v)};
            case JSONtype::FLOAT:
                return new float {*((float*)v)};
            case JSONtype::DOUBLE:
                return new double {*((double*)v)};
            case JSONtype::STRING:
                return new std::string {*((std::string*)v)};
            case JSONtype::CHARP:
                return new std::string {*((std::string*)v)};
            case JSONtype::BOOL:
                return new bool {*((bool*)v)};
            default:
                return nullptr;
        }
        return nullptr;
    }

private:
    CPPtype(void)
    {
        types.clear();
        types[std::type_index(typeid(short))] = JSONtype::SHORT;
        types[std::type_index(typeid(int))] = JSONtype::INT;
        types[std::type_index(typeid(long))] = JSONtype::LONG;
        types[std::type_index(typeid(long long))] = JSONtype::LONGLONG;
        types[std::type_index(typeid(unsigned short))] = JSONtype::USHORT;
        types[std::type_index(typeid(unsigned int))] = JSONtype::UINT;
        types[std::type_index(typeid(unsigned long))] = JSONtype::ULONG;
        types[std::type_index(typeid(unsigned long long))] = JSONtype::ULONGLONG;
        types[std::type_index(typeid(int16_t))] = JSONtype::INT16;
        types[std::type_index(typeid(int32_t))] = JSONtype::INT32;
        types[std::type_index(typeid(int64_t))] = JSONtype::INT64;
        types[std::type_index(typeid(uint16_t))] = JSONtype::UINT16;
        types[std::type_index(typeid(uint32_t))] = JSONtype::UINT32;
        types[std::type_index(typeid(uint64_t))] = JSONtype::UINT64;
        types[std::type_index(typeid(float))] = JSONtype::FLOAT;
        types[std::type_index(typeid(double))] = JSONtype::DOUBLE;
        types[std::type_index(typeid(std::string))] = JSONtype::STRING;
        types[std::type_index(typeid(const char*))] = JSONtype::CHARP;
        types[std::type_index(typeid(bool))] = JSONtype::BOOL;
        types[std::type_index(typeid(nullptr))] = JSONtype::NULLVAL;
    }
    ~CPPtype(void)
    {
        types.clear();
    }
    CPPtype(const CPPtype&) = delete;
    CPPtype& operator=(const CPPtype&) = delete;

    static CPPtype&
    _GetInstance(void)
    {
        static CPPtype instance;
        return instance;
    }

    JSONtype
    _GetType(std::type_index index)
    {
        auto search = types.find(index);
        if (types.end() != search)
        {
            return search->second;
        }
        else
        {
            return JSONtype::UNDEFINED;
        }
    }

    std::unordered_map<std::type_index, JSONtype> types;
};

constexpr auto cpp_type = CPPtype::Type;
constexpr auto cpp_data = CPPtype::Data;

} // namespace air

#endif // AIR_JSONTYPE_H
