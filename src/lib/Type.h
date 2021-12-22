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

#ifndef AIR_TYPE_H
#define AIR_TYPE_H

#include <cstdint>

namespace air
{
enum class ProcessorType : uint32_t
{
    PERFORMANCE = 0,
    LATENCY,
    QUEUE,
    UTILIZATION,
    COUNT,
    HISTOGRAM,
    PROCESSORTYPE_NULL
};

struct NodeMetaData
{
    // Mandatory
    uint32_t nid{0};
    ProcessorType processor_type{ProcessorType::PROCESSORTYPE_NULL};
    bool run{false};
    uint32_t group_id{0};
    uint32_t index_size{0};
    uint32_t filter_size{0};

    // Optional
    uint32_t sample_ratio{1000};
};

} // namespace air

#endif // AIR_TYPE_H
