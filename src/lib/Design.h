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

#ifndef AIR_DESIGN_PATTERN_H
#define AIR_DESIGN_PATTERN_H

#include <cstdint>

#include "src/lib/Protocol.h"

namespace lib_design
{
class AbstractCoRHandler
{
public:
    virtual ~AbstractCoRHandler(void)
    {
    }
    virtual void HandleRequest(int option = 0) = 0;

protected:
    AbstractCoRHandler* next_handler {nullptr};
};

class Observer
{
public:
    virtual ~Observer(void)
    {
    }
    virtual void Update(uint32_t type1, uint32_t type2, uint32_t value1,
        uint32_t value2, int pid, int cmd_type, int cmd_order) = 0;
    virtual void Handle(void) = 0;
};

class Subject
{
public:
    virtual ~Subject(void)
    {
    }
    int Attach(Observer* observer, uint32_t index);
    virtual int Notify(uint32_t index, uint32_t type1, uint32_t type2,
        uint32_t value1, uint32_t value2, int pid, int cmd_type, int cmd_order) = 0;

protected:
    static const uint32_t ARR_SIZE {pi::k_max_subject_size};
    Observer* arr_observer[ARR_SIZE] {
        nullptr,
    };
};
} // namespace lib_design

#endif // AIR_DESIGN_PATTERN_H
