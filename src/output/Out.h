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

#ifndef AIR_OUT_H
#define AIR_OUT_H

#include <sys/msg.h>
#include <sys/types.h>

#include "src/lib/Msg.h"

namespace output
{
class OutCommand
{
public:
    OutCommand(void)
    {
    }
    virtual ~OutCommand(void)
    {
    }
    virtual int Send(int pid, int ret_code, int cmd_type, int cmd_order);

private:
    struct SendMsgSt
    {
        int64_t pid;
        int ret_code;
        int cmd_type;
        int cmd_order;
    };

    struct SendMsgSt send_msg
    {
        0,
    };

    key_t msg_q_key_id {0};
    key_t msg_q_key_value {0};
};
} // namespace output

#endif // AIR_OUT_H
