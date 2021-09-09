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

#ifndef AIR_LATENCY_PROCESSOR_H
#define AIR_LATENCY_PROCESSOR_H

#include "src/lib/Data.h"
#include "src/lib/StringView.h"
#include "src/process/processor/Processor.h"

namespace process
{
class LatencyProcessor : public Processor
{
public:
    virtual ~LatencyProcessor(void)
    {
    }

private:
    void _InitData(lib::Data* air_data, lib::AccData* acc_data) override;
    void _ProcessData(lib::Data* air_data, lib::AccData* acc_data) override;
    void _Calculate(lib::AccLatencyData* data);
    void _JsonifyData(lib::Data* air_data, lib::AccData* acc_data,
        air::string_view& node_name_view, uint32_t tid, const char* tname,
        uint64_t hash_value, uint32_t filter_index) override;

    static const uint64_t OVERFLOW_THRESHOLD{0x7FFFFFFFFFFFFFFF};
};

} // namespace process

#endif // AIR_LATENCY_PROCESSOR_H