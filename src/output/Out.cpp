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

#include "src/output/Out.h"

#include <iostream>

int
output::OutCommand::Send(int pid, int ret_code, int cmd_type, int cmd_order)
{
    int result = 0;
    struct SendMsgSt msg;
    msg.cmd_type = cmd_type;
    msg.ret_code = ret_code;
    msg.pid = pid;
    msg.cmd_order = cmd_order;

    msg_q_key_value = pid;
    msg_q_key_id = msgget(msg_q_key_value, IPC_CREAT | 0666);

    result = msgsnd(msg_q_key_id, (void*)&msg,
        sizeof(struct SendMsgSt) - sizeof(int64_t), IPC_NOWAIT);

    return result;
}
