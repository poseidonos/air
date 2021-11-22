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

#ifndef AIR_COLLECTION_QUEUE_WRITER_H
#define AIR_COLLECTION_QUEUE_WRITER_H

#include <cmath>
#include <random>

#include "src/collection/writer/Writer.h"
#include "src/lib/Data.h"

namespace collection
{
class QueueWriter : public Writer
{
public:
    QueueWriter(void)
    {
        _UpdateRand();
    }
    virtual ~QueueWriter(void)
    {
    }
    inline void
    LogData(lib::Data* data, uint64_t q_depth) override
    {
        if (nullptr == data)
        {
            return;
        }
        lib::QueueData* queue_data = static_cast<lib::QueueData*>(data);
        if (false == IsSampling(queue_data))
        {
            return;
        }
        queue_data->access = true;
        queue_data->sum_depth += q_depth;
        queue_data->num_req++;
        if (queue_data->depth_period_max < q_depth)
        {
            queue_data->depth_period_max = q_depth;
        }
    }
    int
    SetSamplingRate(uint32_t rate) override
    {
        if (rate >= MIN_RATIO && rate <= MAX_RATIO)
        {
            sampling_rate = rate;
            _UpdateRand();

            return 0;
        }
        else
        {
            return -2; // boundray error
        }
    }
    inline bool
    IsSampling(lib::QueueData* queue_data)
    {
        bool result {false};
        if (sampling_rate != queue_data->sampling_rate)
        {
            queue_data->sampling_rate = sampling_rate;
            queue_data->logging_point = 0;
            queue_data->num_called = 0;
            queue_data->mersenne = mersenne;
        }
        if (queue_data->logging_point == queue_data->num_called)
        {
            result = true;
        }
        else
        {
            result = false;
        }
        if (queue_data->num_called % sampling_rate == 0)
        {
            queue_data->num_called = 0;
            if (sampling_rate < 10)
            {
                queue_data->logging_point = sampling_rate;
            }
            else
            {
                uint32_t rand_uint = std::uniform_int_distribution<uint32_t> {
                    0, sampling_rate}(queue_data->mersenne);
                float rand_float = rand_uint * 0.8;
                queue_data->logging_point = rand_float + (sampling_rate * 0.1);
            }
        }
        queue_data->num_called++;
        return result;
    }
    uint32_t
    GetLoggingPoint(lib::QueueData* queue_data)
    {
        return queue_data->logging_point;
    }

private:
    void _UpdateRand(void);
    static const uint32_t PADDING {10};
    static const uint32_t DEFAULT_RATIO {1000}; // tmp. 1000
    static const uint32_t MIN_RATIO {1};
    static const uint32_t MAX_RATIO {10000};

    uint32_t sampling_rate {DEFAULT_RATIO};
    std::random_device rand;
    std::mt19937 mersenne;
};

} // namespace collection

#endif // AIR_COLLECTION_QUEUE_WRITER_H
