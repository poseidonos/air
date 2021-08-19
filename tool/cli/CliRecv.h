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

#ifndef AIR_CLI_RECV_H
#define AIR_CLI_RECV_H

#include <sys/types.h>

#include "tool/cli/CliResult.h"

namespace air
{
class CliRecv
{
public:
    CliRecv(CliResult* new_result, int new_pid)
    {
        cli_result = new_result;
        pid = new_pid;
        _Initialize();
    }
    ~CliRecv(void)
    {
        _Finalize();
    }
    void Receive(int num_cmd, int target_pid);
    void HandleTimeout(void);

private:
    void _Initialize(void);
    void _Finalize(void);

    key_t msg_q_key_id{0};

    struct msg_q_recv_st
    {
        long pid;
        int result;
        int cmd_type;
        int cmd_order;
    };

    struct msg_q_recv_st recv_msg
    {
        0,
    };
    CliResult* cli_result{nullptr};
    int pid{0};
    int target_pid_value{0};
    bool waiting{false};
};
} // end namespace air

#endif // AIR_CLI_RECV_H
