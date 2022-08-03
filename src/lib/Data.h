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

#ifndef AIR_DATA_H
#define AIR_DATA_H

#include <cstdint>
#include <ctime>
#include <map>
#include <random>
#include <string>
#include <vector>

namespace lib
{
const uint32_t IDLE_THRESHOLD {5};
const uint32_t MAX_PACKET_CNT_SIZE {10};
const uint32_t TIMELAG_SIZE {100};

struct Data
{
    virtual ~Data(void)
    {
    }
    uint32_t access {0};
};

struct AccData
{
    virtual ~AccData(void)
    {
    }
    uint32_t need_erase {0};
    uint32_t updated_count {0};
};

struct PerformanceData : public Data
{
    uint64_t period_bandwidth {0};
    uint32_t period_iops {0};
    std::map<uint32_t, uint32_t> period_packet_cnt;
};

struct AccPerformanceData : public AccData
{
    uint64_t cumulation_bandwidth {0};
    uint32_t cumulation_iops {0};
    double duration_second {0};
};

enum class TimeLogState : uint32_t
{
    IDLE, // Processor(TimingDistributor) set
    RUN,  // SwitchGear set, "Only" this state can log
    STOP, // SwitchGear set
    FULL, // Collector set, when token empty
    DONE  // Preprocessor sed
};

struct TimeLog
{
    uint64_t key {0};
    timespec timestamp {
        0,
    };
};

struct LatencyData : public Data
{
    std::vector<struct TimeLog> start_v;
    TimeLogState start_state {TimeLogState::IDLE};
    int32_t start_token {0};
    int32_t start_token_size {static_cast<int32_t>(TIMELAG_SIZE)};
    int32_t start_deadline {-45};

    std::vector<struct TimeLog> end_v;
    TimeLogState end_state {TimeLogState::IDLE};
    int32_t end_token {0};
    int32_t end_token_size {static_cast<int32_t>(TIMELAG_SIZE)};
    int32_t end_deadline {-45};
};

struct AccLatencyData : public AccData
{
    uint32_t period_mean {0};
    uint32_t period_min {0};
    uint32_t period_max {0};
    uint32_t period_median {0};
    uint32_t period_lower_quartile {0};
    uint32_t period_upper_quartile {0};
    uint32_t period_sample_count {0};

    uint32_t cumulation_mean {0};
    uint32_t cumulation_min {0};
    uint32_t cumulation_max {0};
    uint32_t cumulation_median {0};
    uint32_t cumulation_lower_quartile {0};
    uint32_t cumulation_upper_quartile {0};
    uint64_t cumulation_sample_count {0};

    uint32_t overflow_warning {0};

    uint64_t timelag[TIMELAG_SIZE] {
        0,
    };
};

struct QueueData : public Data
{
    double period_qd_avg {0.0};
    uint32_t period_qd_max {0};
    uint32_t period_num_req {0};
    uint64_t period_qd_sum {0};

    uint32_t logging_point {0};
    uint32_t num_called {0};
    uint32_t sampling_rate {0};
    std::mt19937 mersenne {
        0,
    };
};

struct AccQueueData : public AccData
{
    double cumulation_qd_avg {0.0};
    uint32_t cumulation_qd_max {0};
    uint32_t cumulation_num_req {0};
};

struct UtilizationData : public Data
{
    uint64_t period_usage {0};
};

struct AccUtilizationData : public AccData
{
    uint64_t cumulation_usage {0};
};

struct CountData : public Data
{
    uint64_t period_count_positive {0};
    uint64_t period_count_negative {0};
    uint64_t period_num_req_positive {0};
    uint64_t period_num_req_negative {0};
    uint64_t period_count {0};
    uint32_t period_negative {0};
};

struct AccCountData : public AccData
{
    uint64_t cumulation_count {0};
    uint64_t cumulation_num_req_positive {0};
    uint64_t cumulation_num_req_negative {0};
    uint32_t cumulation_negative {0};
};

enum class BucketType : uint32_t
{
    LINEAR,
    EXPONENTIAL
};

// Double Buffer per thread(+filter,+index)
struct HistogramData : public Data
{
    // Periodically & Automatically initialized period_xxx data
    uint64_t period_underflow {0};
    uint64_t period_overflow {0};
    uint64_t period_bucket[20] {
        0,
    };
    int64_t period_min_value {0};
    int64_t period_max_value {0};
    int64_t period_avg_value {0};

    // Common variables to get bucket_index
    // Once set by NodeInitializer, never change
    int64_t bucket_lower_bound {0};
    int64_t bucket_upper_bound {0};
    int64_t bucket_scale {0};
    uint64_t bucket_size {0};
    BucketType bucket_type {BucketType::LINEAR};

    // Exponential type specific variable to get bucket_index
    // Once set by NodeInitializer, never change
    int32_t bucket_zero_index {0};

    // Bucket name is a minimum value of each bucket
    // Once set by NodeInitializer, never change
    std::string bucket_name[20] {
        "",
    };
};

// Single Buffer per thread(+filter,+index), Can be initialized this struct data by
// air_cli
struct AccHistogramData : public AccData
{
    uint64_t cumulation_underflow {0};
    uint64_t cumulation_overflow {0};
    uint64_t cumulation_bucket[20] {
        0,
    };
    int64_t cumulation_min_value {0};
    int64_t cumulation_max_value {0};
    int64_t cumulation_avg_value {0};
    time_t since {0};
    bool is_first {true};
};

} // namespace lib

#endif // AIR_DATA_H
