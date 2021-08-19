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

#include <stdio.h>
#include <unistd.h>

#include "tool/cli/CliRecv.h"
#include "tool/cli/CliSend.h"

int
main(int argc, char* argv[])
{
    int pid = getpid();
    air::CliResult* cli_result = new air::CliResult{};
    air::CliSend* cli_send = new air::CliSend{cli_result, pid};
    air::CliRecv* cli_recv = new air::CliRecv{cli_result, pid};

    int num_cmd{0};
    int target_pid{-1};

    num_cmd = cli_send->Send(argc, argv, target_pid);

    if (0 < num_cmd)
    {
        cli_recv->Receive(num_cmd, target_pid);
    }

    cli_result->PrintCliResult();

    delete cli_send;
    delete cli_recv;
    delete cli_result;
}
