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

#include "src/transfer/Task.h"

#include <unistd.h>

#include <stdexcept>
#include <utility>

std::mutex transfer::Task::mutex_outbox;
std::list<transfer::Task::TaskInfo> transfer::Task::task_outbox;
std::list<transfer::Task::TaskInfo> transfer::Task::task_list;

transfer::Task::~Task(void)
{
    for (auto& task_info : task_list)
    {
        task_info.nodes.clear();
    }
    task_list.clear();
    for (auto& task_info : task_outbox)
    {
        task_info.nodes.clear();
    }
    task_outbox.clear();
}

void
transfer::Task::Register(node_list nodes, task_unit function)
{
    const std::lock_guard<std::mutex> lock(mutex_outbox);
    TaskInfo info;
    info.function = std::move(function);
    for (auto node : nodes)
    {
        info.nodes.push_back(node);
    }
    task_outbox.push_back(std::move(info));
}

void
transfer::Task::_Outbox2List(void)
{
    const std::lock_guard<std::mutex> lock(mutex_outbox);
    while (false == task_outbox.empty())
    {
        task_list.push_back(std::move(task_outbox.front()));
        task_outbox.pop_front();
    }
}

void
transfer::Task::_WaitJobDone(void)
{
    size_t task_done_count = 0;
    while (task_done_count != task_list.size())
    {
        usleep(100000); // 100ms

        task_done_count = 0;
        for (auto& task_info : task_list)
        {
            if (task_info.async_task.valid())
            {
                task_done_count++;
            }
        }
    }
}

void
transfer::Task::_EraseFinishedJob(void)
{
    auto task_it = task_list.begin();
    while (task_it != task_list.end())
    {
        auto& task_info = *task_it;
        if (0 == task_info.async_task.get())
        {
            task_it++;
        }
        else
        {
            task_it = task_list.erase(task_it);
        }
    }
}

int
transfer::Task::NotifyAll(air::JSONdoc* json_data)
{
    int rc {0};
    _Outbox2List();

    std::deque<air::JSONdoc*> task_data_q;
    for (auto& task_info : task_list)
    {
        auto task_data = json_data->Compound(task_info.nodes);
        task_data_q.push_back(task_data);
        task_info.async_task = std::async(std::launch::async,
            [&] { return task_info.function(std::move(*task_data)); });
    }

    _WaitJobDone();
    _EraseFinishedJob();

    while (!task_data_q.empty())
    {
        auto task_data = task_data_q.back();
        task_data_q.pop_back();
        delete task_data;
        task_data = nullptr;
    }

    try
    {
        json_data->Clear();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        rc = -1;
    }

    delete json_data;
    json_data = nullptr;

    return rc;
}
