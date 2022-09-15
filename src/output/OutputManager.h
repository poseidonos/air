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

#ifndef AIR_OUTPUT_MANAGER_H
#define AIR_OUTPUT_MANAGER_H

#include <sys/file.h>
#include <unistd.h>

#include <queue>
#include <string>

#include "src/lib/Msg.h"
#include "src/output/Out.h"

namespace output
{
class OutputManager
{
public:
    explicit OutputManager(OutCommand* out_command): out_command(out_command)
    {
    }
    ~OutputManager(void)
    {
    }
    void
    EnqueueMsg(uint32_t type1, uint32_t type2, uint32_t value1, uint32_t value2,
        int pid, int cmd_type, int cmd_order)
    {
        msg.push({type1, type2, value1, value2, pid, cmd_type, cmd_order});
    }
    int HandleMsg(void);

private:
    int _SendCompletionMsg(uint32_t value1, int pid, int cmd_type, int cmd_order);
    std::string _TimeStampToHReadble(int64_t timestamp);
    std::queue<lib::MsgEntry> msg;
    OutCommand* out_command {nullptr};
};

} // namespace output

#endif // AIR_OUTPUT_MANAGER_H
