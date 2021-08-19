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

#include "src/thread/Thread.h"

#include <sched.h>
#include <unistd.h>

void*
thread::OneTimeThread::RunThreadLoop(void* thread)
{
    OneTimeThread* p_thread = (OneTimeThread*)thread;

    if (pthread_setaffinity_np(pthread_self(), sizeof(p_thread->cpu_set),
            &(p_thread->cpu_set)) < 0)
    {
        // perror("pthread_setaffinity_np failed!!!");
    }

    p_thread->_Lock();
    while (p_thread->_IsRun())
    {
        p_thread->RunThread(p_thread->run_skip_count);
        p_thread->run_skip_count = 0;

        pthread_cond_wait(&p_thread->thread_cond, &p_thread->thread_mutex); // unlock when sleep, lock when wake up
    }
    p_thread->_Unlock();

    return nullptr;
}

void
thread::OneTimeThread::StartThread(void)
{
    thread_run = true;
    pthread_create(&thread, nullptr, &thread::OneTimeThread::RunThreadLoop,
        (void*)this);
    pthread_detach(thread);
}

void
thread::OneTimeThread::PullTrigger(void)
{
    if (0 == _TryLock())
    {
        if (thread_run)
        {
            pthread_cond_signal(&thread_cond);
        }
        _Unlock();
    }
    else
    {
        run_skip_count++;
    }
}

void
thread::OneTimeThread::JoinThread(void)
{
    _Lock();
    thread_run = false;
    pthread_cond_signal(&thread_cond);
    _Unlock();
    pthread_join(thread, nullptr);
}

void*
thread::PeriodicThread::RunThreadLoop(void* thread)
{
    PeriodicThread* p_thread = (PeriodicThread*)thread;

    if (pthread_setaffinity_np(pthread_self(), sizeof(p_thread->cpu_set),
            &(p_thread->cpu_set)) < 0)
    {
        // perror("pthread_setaffinity_np failed!!!");
    }

    timespec curr_timestamp, prev_timestamp;
    clock_gettime(CLOCK_MONOTONIC, &curr_timestamp);
    p_thread->SetLastTimestamp(curr_timestamp);

    while (p_thread->_IsRun())
    {
        p_thread->RunThread(0);
        usleep(p_thread->GetSleepTime());

        clock_gettime(CLOCK_MONOTONIC, &curr_timestamp);
        prev_timestamp = p_thread->GetLastTimestamp();
        uint64_t timelag = Timelag::GetDiff(curr_timestamp, prev_timestamp);
        if (UINT_E7 < timelag)
        {
            p_thread->DecreaseSleepTime();
        }
        else
        {
            p_thread->IncreaseSleepTime();
        }
        p_thread->SetLastTimestamp(curr_timestamp);
    }

    return nullptr;
}

void
thread::PeriodicThread::StartThread(void)
{
    thread_run = true;
    pthread_create(&thread, nullptr, &thread::PeriodicThread::RunThreadLoop,
        (void*)this);
}

void
thread::PeriodicThread::JoinThread(void)
{
    thread_run = false;
    pthread_join(thread, nullptr);
}
