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

#ifndef AIR_TIMING_DISTRIBUTOR_H
#define AIR_TIMING_DISTRIBUTOR_H

#include <random>

#include "src/config/ConfigInterface.h"
#include "src/data_structure/NodeManager.h"
#include "src/meta/NodeMeta.h"

namespace process
{
class TimingDistributor
{
public:
    TimingDistributor(void)
    {
    }
    ~TimingDistributor(void)
    {
    }
    void SetTiming(meta::NodeMetaGetter* node_meta_getter,
        node::NodeManager* node_manager);

private:
    void _ResetTiming(lib::LatencyData* curr_data,
        lib::LatencyData* next_data, int32_t time_value);
    std::random_device rand_device;
    std::default_random_engine rand_engine {rand_device()};
    std::uniform_int_distribution<int32_t> dist {1, 10};
    const uint32_t MAX_NID_SIZE {cfg::GetSentenceCount(config::ParagraphType::NODE)};
};

} // namespace process

#endif // AIR_TIMING_DISTRIBUTOR_H
