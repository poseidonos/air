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

#ifndef AIR_JSONPRINT_H
#define AIR_JSONPRINT_H

#include <iostream>
#include <string>

#include "src/lib/json/Jsontype.h"

namespace air
{
static std::ostream&
PrintValue(std::ostream& os, JSONtype type, void* value)
{
    switch (type)
    {
        case JSONtype::LONGLONG:
        {
            os << *((long long*)value);
            break;
        }
        case JSONtype::ULONGLONG:
        {
            os << *((unsigned long long*)value);
            break;
        }
        case JSONtype::INT16:
        {
            os << *((int16_t*)value);
            break;
        }
        case JSONtype::INT32:
        {
            os << *((int32_t*)value);
            break;
        }
        case JSONtype::INT64:
        {
            os << *((int64_t*)value);
            break;
        }
        case JSONtype::UINT16:
        {
            os << *((uint16_t*)value);
            break;
        }
        case JSONtype::UINT32:
        {
            os << *((uint32_t*)value);
            break;
        }
        case JSONtype::UINT64:
        {
            os << *((uint64_t*)value);
            break;
        }
        case JSONtype::FLOAT:
        {
            os << *((float*)value);
            break;
        }
        case JSONtype::DOUBLE:
        {
            os << *((double*)value);
            break;
        }
        case JSONtype::STRING:
        case JSONtype::CHARP:
        {
            os << "\"" << *((std::string*)value) << "\"";
            break;
        }
        case JSONtype::BOOL:
        {
            if (*((bool*)value))
            {
                os << "true";
            }
            else
            {
                os << "false";
            }
            break;
        }
        case JSONtype::NULLVAL:
        {
            os << "null";
            break;
        }
        default:
        {
            throw std::logic_error("[error][JSONdoc::Print] invalid type");
            break;
        }
    }
    return os;
}

} // namespace air

#endif // AIR_JSONPRINT_H
