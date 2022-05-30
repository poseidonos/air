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

#ifndef AIR_TRANSFER_TASK_H
#define AIR_TRANSFER_TASK_H

#include <deque>
#include <functional>
#include <future>
#include <list>
#include <mutex>
#include <string>

#include "src/lib/json/Json.h"

namespace transfer
{
using task_unit = std::function<int(const air::JSONdoc&&)>;
using node_list = std::initializer_list<std::string>;

class Task
{
public:
    static Task&
    Get(void)
    {
        static Task* task = new Task {};
        return *task;
    }
    Task(Task const&) = delete;
    Task(Task&&) = delete;
    Task& operator=(Task const&) = delete;
    Task& operator=(Task&&) = delete;
    ~Task(void);

    void Register(node_list nodes, task_unit function);
    int NotifyAll(air::JSONdoc* json_data);

private:
    void _Outbox2List(void);
    void _WaitJobDone(void);
    void _EraseFinishedJob(void);

    struct TaskInfo
    {
        task_unit function;
        std::deque<std::string> nodes;
        std::future<int> async_task;
    };

    static std::mutex mutex_outbox;
    static std::list<TaskInfo> task_outbox;
    static std::list<TaskInfo> task_list;
};

} // namespace transfer

#endif // AIR_TRANSFER_TASK_H
