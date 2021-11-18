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

#ifndef AIR_COLLECTION_LATENCY_WRITER_H
#define AIR_COLLECTION_LATENCY_WRITER_H

#include <time.h>

#include "src/collection/writer/Writer.h"
#include "src/lib/Data.h"

namespace collection
{
class LatencyWriter : public Writer
{
public:
    virtual ~LatencyWriter(void)
    {
    }
    inline void
    LogData(lib::Data* data, uint64_t key) override
    {
        lib::LatencyData* lat_data = static_cast<lib::LatencyData*>(data);

        lat_data->access = true;

        if (lib::TimeLogState::RUN == lat_data->start_state)
        {
            lib::TimeLog time_log;
            time_log.key = key;
            clock_gettime(CLOCK_MONOTONIC, &time_log.timestamp);

            lat_data->start_v.push_back(time_log);
            lat_data->start_token--;
            if (0 >= lat_data->start_token)
            {
                lat_data->start_state = lib::TimeLogState::FULL;
            }
        }

        if (lib::TimeLogState::RUN == lat_data->end_state)
        {
            lib::TimeLog time_log;
            time_log.key = key;
            clock_gettime(CLOCK_MONOTONIC, &time_log.timestamp);

            lat_data->end_v.push_back(time_log);
            lat_data->end_token--;
            if (0 >= lat_data->end_token)
            {
                lat_data->end_state = lib::TimeLogState::FULL;
            }
        }
    }

    inline bool
    AddTimelag(lib::AccLatencyData* lat_data, uint64_t timelag)
    {
        if (nullptr == lat_data || lib::TIMELAG_SIZE <= lat_data->sample_count)
        {
            return false;
        }

        if (timelag > lat_data->max)
        {
            lat_data->max = timelag;
        }

        if ((0 != timelag) && ((0 == lat_data->min) || (timelag < lat_data->min)))
        {
            lat_data->min = timelag;
        }

        lat_data->timelag[lat_data->sample_count] = timelag;
        lat_data->sample_count++;

        return true;
    }

    int
    SetSamplingRate(uint32_t rate) override
    {
        return 0;
    }

private:
};

} // namespace collection

#endif // AIR_COLLECTION_LATENCY_WRITER_H
