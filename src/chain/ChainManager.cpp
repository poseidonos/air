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

#include "src/chain/ChainManager.h"

#include <stdio.h>

#include "src/lib/Casting.h"

void
chain::TaskChain::_ResetTime(void)
{
    deadline = period;
}

int32_t
chain::TaskChain::_SpendTime(uint32_t time)
{
    deadline -= static_cast<int32_t>(time);
    return deadline;
}

bool
chain::TaskChain::IsRun(meta::GlobalMeta* g_meta, uint32_t delayed_time)
{
    if (0 >= _SpendTime(10 * (delayed_time + 1)))
    {
        _ResetTime();
        return true;
    }

    return false;
}

void
chain::SwitchGearTask::RunChain(lib_design::AbstractCoRHandler** cor_handlers,
    meta::GlobalMeta* g_meta, int32_t option)
{
    if (true == g_meta->AirPlay())
    {
        cor_handlers[to_dtype(pi::ChainHandler::SWITCHGEAR)]->HandleRequest(option);
    }
}

void
chain::PreprocessTask::RunChain(lib_design::AbstractCoRHandler** cor_handlers,
    meta::GlobalMeta* g_meta, int32_t option)
{
    if (true == g_meta->AirPlay())
    {
        cor_handlers[to_dtype(pi::ChainHandler::PREPROCESS)]->HandleRequest(option);
    }
}

void
chain::CLITask::RunChain(lib_design::AbstractCoRHandler** cor_handlers,
    meta::GlobalMeta* g_meta, int32_t option)
{
    cor_handlers[to_dtype(pi::ChainHandler::INPUT)]->HandleRequest(option);
    cor_handlers[to_dtype(pi::ChainHandler::POLICY)]->HandleRequest(option);
    cor_handlers[to_dtype(pi::ChainHandler::COLLECTION)]->HandleRequest(option);
    cor_handlers[to_dtype(pi::ChainHandler::OUTPUT)]->HandleRequest(option);
}

void
chain::AnalysisTask::RunChain(lib_design::AbstractCoRHandler** cor_handlers,
    meta::GlobalMeta* g_meta, int32_t option)
{
    cor_handlers[to_dtype(pi::ChainHandler::PROCESS)]->HandleRequest(option);
    cor_handlers[to_dtype(pi::ChainHandler::TRANSFER)]->HandleRequest(option);
    cor_handlers[to_dtype(pi::ChainHandler::STREAM)]->HandleRequest(option);

    if (true == g_meta->AirPlay())
    {
        cor_handlers[to_dtype(pi::ChainHandler::DETECT)]->HandleRequest(option);
    }

    if (g_meta->StreamingUpdate())
    {
        g_meta->UpdateStreamingInterval();
        SetPeriod(1000 * g_meta->StreamingInterval());
    }
}

void
chain::ChainManager::Init(void)
{
    analysis_task.SetPeriod(1000 * global_meta->StreamingInterval());
}

void
chain::ChainManager::RunChain(uint32_t delayed_time)
{
    bool switch_gear_run, preprocess_run, cli_run, analysis_run;

    switch_gear_run = switch_gear_task.IsRun(global_meta, delayed_time);
    preprocess_run = preprocess_task.IsRun(global_meta, delayed_time);
    cli_run = cli_task.IsRun(global_meta, delayed_time);
    analysis_run = analysis_task.IsRun(global_meta, delayed_time);

    if (switch_gear_run)
    {
        switch_gear_task.RunChain(cor_handlers, global_meta);
    }
    if (preprocess_run)
    {
        if (analysis_run)
        {
            preprocess_task.RunChain(
                cor_handlers, global_meta, to_dtype(pi::PreprocessOption::FORCED));
        }
        else
        {
            preprocess_task.RunChain(
                cor_handlers, global_meta, to_dtype(pi::PreprocessOption::NORMAL));
        }
    }
    if (cli_run)
    {
        cli_task.RunChain(cor_handlers, global_meta);
    }
    if (analysis_run)
    {
        analysis_task.RunChain(cor_handlers, global_meta);
    }
}

void
chain::ChainManager::RunThread(uint32_t run_skip_count)
{
    RunChain(run_skip_count);
}
