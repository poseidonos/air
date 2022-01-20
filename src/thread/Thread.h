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

#ifndef AIR_THREAD_H
#define AIR_THREAD_H

#include <pthread.h>
#include <stdint.h>

#include "src/lib/Timelag.h"

namespace thread
{
class AirThread
{
public:
    AirThread(void)
    {
    }
    virtual ~AirThread(void)
    {
    }
    virtual void RunThread(uint32_t run_skip_count) = 0;
    virtual void StartThread(void) = 0;
    virtual void JoinThread(void) = 0;
    static void*
    RunThreadLoop(void*)
    {
        return nullptr;
    }
    void
    SetCpuSet(uint32_t cpu_num)
    {
        CPU_ZERO(&cpu_set);
        CPU_SET(cpu_num, &cpu_set);
    }
    uint32_t
    GetRunSkipCount(void) const
    {
        return run_skip_count;
    }

protected:
    bool
    _IsRun(void)
    {
        return thread_run;
    }

    pthread_t thread {0};
    cpu_set_t cpu_set {0};
    bool thread_run {false};
    uint32_t run_skip_count {0};
};

class OneTimeThread : public AirThread
{
public:
    static void* RunThreadLoop(void*);
    virtual void StartThread(void);
    virtual void PullTrigger(void);
    virtual void JoinThread(void);
    virtual void
    RunThread(uint32_t run_skip_count)
    {
        return;
    }

private:
    int
    _Lock(void)
    {
        return pthread_mutex_lock(&thread_mutex);
    }
    int
    _TryLock(void)
    {
        return pthread_mutex_trylock(&thread_mutex);
    }
    int
    _Unlock(void)
    {
        return pthread_mutex_unlock(&thread_mutex);
    }
    pthread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t thread_cond = PTHREAD_COND_INITIALIZER;
};

class PeriodicThread : public AirThread
{
public:
    static void* RunThreadLoop(void*);
    virtual void StartThread(void);
    virtual void JoinThread(void);
    virtual void
    RunThread(uint32_t run_skip_count)
    {
        return;
    }
    int32_t
    GetSleepTime(void)
    {
        return sleeptime_us;
    }
    void
    IncreaseSleepTime(void)
    {
        sleeptime_us += 5;
    }
    void
    DecreaseSleepTime(void)
    {
        sleeptime_us -= 5;
        if (0 > sleeptime_us)
        {
            sleeptime_us = INT_E4;
        }
    }
    timespec
    GetLastTimestamp(void)
    {
        return last_timestamp;
    }
    void
    SetLastTimestamp(timespec timestamp)
    {
        last_timestamp = timestamp;
    }

private:
    int32_t sleeptime_us {INT_E4};
    timespec last_timestamp {0, 0};
};

} // namespace thread

#endif // AIR_THREAD_H
