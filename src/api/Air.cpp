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

#include "src/api/Air.h"

#include <utility>

air::InstanceManager* AIR<true, true>::instance_manager = nullptr;
node::NodeManager* AIR<true, true>::node_manager = nullptr;
collection::CollectionManager* AIR<true, true>::collection_manager = nullptr;
thread_local node::NodeDataArray* AIR<true, true>::node_data_array = nullptr;
collection::PerformanceWriter AIR<true, true>::perf_writer;
collection::LatencyWriter AIR<true, true>::lat_writer;
collection::QueueWriter AIR<true, true>::queue_writer;
collection::UtilizationWriter AIR<true, true>::util_writer;
collection::CountWriter AIR<true, true>::count_writer;
collection::HistogramWriter AIR<true, true>::histogram_writer;

void
air_request_data(transfer::node_list nodes, transfer::task_unit&& function)
{
    transfer::Task::Get().Register(nodes, std::move(function));
}
