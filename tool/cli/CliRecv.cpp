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

#include "tool/cli/CliRecv.h"

#include <sys/ipc.h>
#include <sys/msg.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

void
air::CliRecv::_Initialize(void)
{
    msg_q_key_id = msgget(pid, IPC_CREAT | 0666);
    if (-1 == msg_q_key_id)
    {
        std::cout << "msgget() failed!\n\n\n";
        exit(1);
    }
}

void
air::CliRecv::_Finalize(void)
{
    if (-1 != msg_q_key_id)
    {
        msgctl(msg_q_key_id, IPC_RMID, 0);
    }
}

void
air::CliRecv::Receive(int num_cmd, int target_pid)
{
    waiting = true;
    target_pid_value = target_pid;

    std::thread([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        this->HandleTimeout();
    }).detach();

    for (int loop = 0; loop < num_cmd; loop++)
    {
        memset(&recv_msg, 0, sizeof(struct msg_q_recv_st));
        int recv_bytes = msgrcv(msg_q_key_id, (void*)&recv_msg,
            sizeof(struct msg_q_recv_st) - sizeof(long), pid, 0);
        if (0 < recv_bytes)
        {
            cli_result->SetReturn(ReturnCode::SUCCESS);
        }
        else
        {
            cli_result->SetReturn(ReturnCode::ERR_KERNEL_MSGQ_FAIL, "CliRecv::Receive");
        }
    }

    waiting = false;
}

void
air::CliRecv::HandleTimeout(void)
{
    if (waiting)
    {
        cli_result->SetReturn(ReturnCode::ERR_TIMEOUT, "may use wrong pid");
        msgctl(msg_q_key_id, IPC_RMID, 0);
        key_t msg_q_key_target = msgget(target_pid_value, IPC_CREAT | 0666);
        msgctl(msg_q_key_target, IPC_RMID, 0);
    }
}