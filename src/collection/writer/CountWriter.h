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

#ifndef AIR_COLLECTION_COUNT_WRITER_H
#define AIR_COLLECTION_COUNT_WRITER_H

#include "src/collection/writer/Writer.h"
#include "src/lib/Data.h"

namespace collection
{
class CountWriter : public Writer
{
public:
    CountWriter(void)
    {
    }
    virtual ~CountWriter(void)
    {
    }
    inline void
    LogData(lib::Data* data, uint64_t value) override
    {
        if (nullptr == data)
        {
            return;
        }
        lib::CountData* count_data = static_cast<lib::CountData*>(data);
        count_data->access = true;

        if (MAX_INT64_VALUE < value)
        {
            count_data->count_negative += ((NEGATIVE_ONE - value) + 1);
            count_data->num_req_negative += 1;
        }
        else
        {
            count_data->count_positive += value;
            count_data->num_req_positive += 1;
        }
    }
    int
    SetSamplingRate(uint32_t rate) override
    {
        return 0;
    }

private:
    const uint64_t MAX_INT64_VALUE {9223372036854775807};
    const uint64_t NEGATIVE_ONE {0xffffffffffffffff};
};

} // namespace collection

#endif // AIR_COLLECTION_COUNT_WRITER_H
