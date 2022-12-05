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

#ifndef AIR_COLLECTION_PERFORMANCE_WRITER_H
#define AIR_COLLECTION_PERFORMANCE_WRITER_H

#include "src/collection/writer/Writer.h"
#include "src/lib/Data.h"

namespace collection
{
class PerformanceWriter : public Writer
{
public:
    virtual ~PerformanceWriter(void)
    {
    }
    inline void
    LogData(lib::Data* data, uint64_t io_size) override
    {
        if (nullptr == data)
        {
            return;
        }
        lib::PerformanceData* perf_data = static_cast<lib::PerformanceData*>(data);
        perf_data->access = true;

        perf_data->period_iops++;
        perf_data->period_bandwidth += io_size;

        auto entry = perf_data->period_packet_cnt.find(io_size);
        if (entry != perf_data->period_packet_cnt.end())
        {
            entry->second++;
        }
        else
        {
            if (lib::MAX_PACKET_CNT_SIZE > perf_data->period_packet_cnt.size())
            {
                perf_data->period_packet_cnt.insert({io_size, 1});
            }
        }
    }
};

} // namespace collection

#endif // AIR_COLLECTION_PERFORMANCE_WRITER_H
