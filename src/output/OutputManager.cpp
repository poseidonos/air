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

#include "src/output/OutputManager.h"

#include <string.h>
#include <time.h>

int
output::OutputManager::HandleMsg(void)
{
    int num_send = 0;
    bool retry = false;
    int num_try = 0;
    while (!msg.empty())
    {
        num_try++;
        lib::MsgEntry entry = msg.front();

        if (0 == _SendCompletionMsg(entry.value1, entry.pid, entry.cmd_type, entry.cmd_order))
        {
            msg.pop();
            num_send++;

            if (true == retry)
            {
                retry = false;
            }
        }
        else
        {
            if (false == retry)
            {
                retry = true;
            }
            else
            {
                retry = false;
                msg.pop();
            }
        }
    }

    return num_send;
}

int
output::OutputManager::_SendCompletionMsg(uint32_t value1, int pid,
    int cmd_type, int cmd_order)
{
    return out_command->Send(pid, static_cast<int>(value1) * -1, cmd_type, cmd_order);
}

std::string
output::OutputManager::_TimeStampToHReadble(int64_t timestamp)
{
    const time_t rawtime = (const time_t)timestamp;

    struct tm dt;
    char timestr[30];
    char buffer[30];

    if (NULL == localtime_r(&rawtime, &dt))
    {
        return "";
    }

    strftime(timestr, sizeof(timestr), "%y-%m-%d-%H:%M:%S", &dt);
    snprintf(buffer, sizeof(buffer), "%s", timestr);
    std::string stdBuffer(buffer);

    return stdBuffer;
}
