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

#include <air/Air.h>
#include <benchmark/benchmark.h>
#include <pthread.h>

struct AirFixture
{
    AirFixture()
    {
        air_initialize(0);
        air_activate();
    }
    ~AirFixture()
    {
        air_deactivate();
        air_finalize();
    }
};
static AirFixture air_fixture;


class AirAPI : public benchmark::Fixture
{
public:
    void SetUp(const ::benchmark::State& state)
    {
        cpu_set_t cpu_set;
        CPU_ZERO(&cpu_set);
        CPU_SET(state.thread_index + 1, &cpu_set);
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set), &cpu_set);
    }
    void TearDown(const ::benchmark::State& state)
    {
    }
};


BENCHMARK_DEFINE_F(AirAPI, Type_Performance)(benchmark::State& state)
{
    for (auto _ : state)
    {
        airlog("PERF_BENCHMARK", "AIR_READ", 0, 4096);
    }
}
BENCHMARK_REGISTER_F(AirAPI, Type_Performance)->MinTime(10)->ThreadRange(1, 8)->Repetitions(3);


BENCHMARK_DEFINE_F(AirAPI, Type_Latency)(benchmark::State& state)
{
    uint64_t key {0};
    for (auto _ : state)
    {
        airlog("LAT_SUBMIT", "AIR_0", 0, key);
        airlog("LAT_SUBMIT", "AIR_1", 0, key);
        key++;
    }
}
BENCHMARK_REGISTER_F(AirAPI, Type_Latency)->MinTime(10)->ThreadRange(1, 8)->Repetitions(3);


BENCHMARK_DEFINE_F(AirAPI, Type_Queue)(benchmark::State& state)
{
    for (auto _ : state)
    {
        airlog("Q_SUBMISSION", "AIR_BASE", 0, 24);
    }
}
BENCHMARK_REGISTER_F(AirAPI, Type_Queue)->MinTime(10)->ThreadRange(1, 8)->Repetitions(3);


BENCHMARK_DEFINE_F(AirAPI, Type_Utilization)(benchmark::State& state)
{
    for (auto _ : state)
    {
        airlog("UTIL_SUBMIT_THR", "AIR_SUBMIT", 0, 3);
    }
}
BENCHMARK_REGISTER_F(AirAPI, Type_Utilization)->MinTime(10)->ThreadRange(1, 8)->Repetitions(3);


BENCHMARK_DEFINE_F(AirAPI, Type_Count)(benchmark::State& state)
{
    for (auto _ : state)
    {
        airlog("CNT_TEST_EVENT", "AIR_SUBMIT", 0, 2);
    }
}
BENCHMARK_REGISTER_F(AirAPI, Type_Count)->MinTime(10)->ThreadRange(1, 8)->Repetitions(3);


BENCHMARK_DEFINE_F(AirAPI, Type_Histogram_Best)(benchmark::State& state)
{
    for (auto _ : state)
    {
        airlog("HIST_SAMPLE_3", "AIR_0", 0, 10);
    }
}
BENCHMARK_REGISTER_F(AirAPI, Type_Histogram_Best)->MinTime(10)->ThreadRange(1, 8)->Repetitions(3);


BENCHMARK_DEFINE_F(AirAPI, Type_Histogram_Worst)(benchmark::State& state)
{
    for (auto _ : state)
    {
        airlog("HIST_SAMPLE_6", "AIR_0", 0, 99999);
    }
}
BENCHMARK_REGISTER_F(AirAPI, Type_Histogram_Worst)->MinTime(10)->ThreadRange(1, 8)->Repetitions(3);


BENCHMARK_MAIN();
