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

#include "src/api/Air_c.h"

#include "src/api/Air.h"

extern "C" void
AIR_INITIALIZE(uint32_t cpu_num)
{
    air_initialize(cpu_num);
}

extern "C" void
AIR_ACTIVATE(void)
{
    air_activate();
}

extern "C" void
AIR_DEACTIVATE(void)
{
    air_deactivate();
}

extern "C" void
AIR_FINALIZE(void)
{
    air_finalize();
}

extern "C" void
AIRLOG(uint32_t node_id, uint32_t filter_item, uint64_t node_index, uint64_t value)
{
    AIR<cfg::GetIntValue(config::ParagraphType::DEFAULT, "AirBuild"),
        true>::LogData(node_id, filter_item, node_index, value);
}

extern "C" void
AIRLOG_DUMMY(
    uint32_t node_id, uint32_t filter_item, uint64_t node_index, uint64_t value)
{
    AIR<cfg::GetIntValue(config::ParagraphType::DEFAULT, "AirBuild"),
        false>::LogData(node_id, filter_item, node_index, value);
}
