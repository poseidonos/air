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

#include "src/collection/Collector.cpp"
#include "src/collection/Collector.h"
#include "src/collection/writer/CountWriter.h"
#include "src/collection/writer/HistogramWriter.h"
#include "src/collection/writer/LatencyWriter.h"
#include "src/collection/writer/PerformanceWriter.h"
#include "src/collection/writer/QueueWriter.h"
#include "src/collection/writer/UtilizationWriter.h"

class CollectorTest : public ::testing::Test
{
public:
    collection::PerformanceCollector* performance_collector {nullptr};
    collection::LatencyCollector* latency_collector {nullptr};
    collection::QueueCollector* queue_collector {nullptr};
    collection::CountCollector* count_collector {nullptr};
    collection::UtilizationCollector* util_collector {nullptr};
    collection::HistogramCollector* histogram_collector {nullptr};

protected:
    CollectorTest()
    {
        performance_collector = new collection::PerformanceCollector {
            new collection::PerformanceWriter};
        latency_collector =
            new collection::LatencyCollector {new collection::LatencyWriter};
        queue_collector =
            new collection::QueueCollector {new collection::QueueWriter};
        count_collector =
            new collection::CountCollector {new collection::CountWriter};
        util_collector = new collection::UtilizationCollector {
            new collection::UtilizationWriter};
        histogram_collector =
            new collection::HistogramCollector {new collection::HistogramWriter};
    }
    ~CollectorTest()
    {
        if (nullptr != performance_collector)
        {
            delete performance_collector;
            performance_collector = nullptr;
        }
        if (nullptr != latency_collector)
        {
            delete latency_collector;
            latency_collector = nullptr;
        }
        if (nullptr != queue_collector)
        {
            delete queue_collector;
            queue_collector = nullptr;
        }
        if (nullptr != count_collector)
        {
            delete count_collector;
            count_collector = nullptr;
        }
        if (nullptr != util_collector)
        {
            delete util_collector;
            util_collector = nullptr;
        }
        if (nullptr != histogram_collector)
        {
            delete histogram_collector;
            histogram_collector = nullptr;
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
