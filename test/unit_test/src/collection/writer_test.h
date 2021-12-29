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

#include <gmock/gmock.h>

#include "src/collection/writer/CountWriter.cpp"
#include "src/collection/writer/CountWriter.h"
#include "src/collection/writer/HistogramWriter.cpp"
#include "src/collection/writer/HistogramWriter.h"
#include "src/collection/writer/LatencyWriter.cpp"
#include "src/collection/writer/LatencyWriter.h"
#include "src/collection/writer/PerformanceWriter.cpp"
#include "src/collection/writer/PerformanceWriter.h"
#include "src/collection/writer/QueueWriter.cpp"
#include "src/collection/writer/QueueWriter.h"
#include "src/collection/writer/UtilizationWriter.cpp"
#include "src/collection/writer/UtilizationWriter.h"
#include "src/collection/writer/Writer.cpp"
#include "src/collection/writer/Writer.h"
#include "src/lib/Casting.h"
#include "src/lib/Data.h"
#include "src/lib/Type.h"

class WriterTest : public ::testing::Test
{
public:
    collection::PerformanceWriter* performance_writer {nullptr};
    collection::LatencyWriter* latency_writer {nullptr};
    collection::QueueWriter* queue_writer {nullptr};
    collection::CountWriter* count_writer {nullptr};
    collection::UtilizationWriter* util_writer {nullptr};
    collection::HistogramWriter* histogram_writer {nullptr};

protected:
    WriterTest()
    {
        performance_writer = new collection::PerformanceWriter;
        latency_writer = new collection::LatencyWriter;
        queue_writer = new collection::QueueWriter;
        count_writer = new collection::CountWriter;
        util_writer = new collection::UtilizationWriter;
        histogram_writer = new collection::HistogramWriter;
    }
    ~WriterTest()
    {
        if (nullptr != performance_writer)
        {
            delete performance_writer;
            performance_writer = nullptr;
        }
        if (nullptr != latency_writer)
        {
            delete latency_writer;
            latency_writer = nullptr;
        }
        if (nullptr != queue_writer)
        {
            delete queue_writer;
            queue_writer = nullptr;
        }
        if (nullptr != count_writer)
        {
            delete count_writer;
            count_writer = nullptr;
        }
        if (nullptr != util_writer)
        {
            delete util_writer;
            util_writer = nullptr;
        }
        if (nullptr != histogram_writer)
        {
            delete histogram_writer;
            histogram_writer = nullptr;
        }
    }
    void
    SetUp() override
    {
    }
    void
    TearDown() override
    {
    }
};
