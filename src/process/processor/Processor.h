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

#ifndef AIR_PROCESSOR_H
#define AIR_PROCESSOR_H

#include <string>

#include "src/data_structure/NodeData.h"
#include "src/lib/Data.h"
#include "src/lib/StringView.h"

namespace process
{
class Processor
{
public:
    virtual ~Processor(void)
    {
    }
    void StreamData(air::string_view& node_name_view, uint32_t tid,
        const char* tname, node::NodeData* node_data, air::ProcessorType ptype,
        double time, uint32_t index_size, uint32_t filter_size);
    void StreamData(air::string_view& node_name_view, lib::AccLatencyData* data,
        uint64_t hash_value, uint32_t filter_index);

protected:
    double lap_time {1.0f};
    struct JsonifyData
    {
        lib::Data* air_data;
        lib::AccData* acc_data;
        air::string_view node_name_view;
        uint32_t tid;
        const char* tname;
        uint64_t hash_value;
        uint32_t filter_index;
    };

private:
    virtual void _ProcessData(lib::Data* air_data, lib::AccData* acc_data) = 0;
    virtual void _JsonifyData(struct JsonifyData data) = 0;
    virtual void _InitData(lib::Data* air_data, lib::AccData* acc_data) = 0;
};

} // namespace process

#endif // AIR_PROCESSOR_H
