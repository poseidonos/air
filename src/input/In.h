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

#ifndef AIR_IN_H
#define AIR_IN_H

#include <sys/msg.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include "src/lib/Design.h"

namespace input
{
enum CommandType
{
    CMD_PID = 0,
    CMD_AIR_RUN,
    CMD_AIR_STREAM_INTERVAL,
    CMD_AIR_FILE_WRITE,
    CMD_AIR_REMAIN_FILE_COUNT,
    CMD_NODE_RUN,
    CMD_NODE_INIT
};

class Subject : public lib_design::Subject
{
public:
    virtual ~Subject(void)
    {
    }
    Subject(void)
    {
    }
    virtual int Notify(uint32_t index, uint32_t type1, uint32_t type2,
        uint32_t value1, uint32_t value2, int pid, int cmd_type, int cmd_order);
};

class InCommand
{
public:
    explicit InCommand(input::Subject* subject): subject(subject)
    {
        struct msqid_ds msq_stat
        {
            0,
        };

        msg_q_key_value = getpid();

        msg_q_key_id = msgget(msg_q_key_value, IPC_CREAT | 0666);

        msq_stat.msg_perm.uid = msg_q_key_value;
        msq_stat.msg_qbytes = 4096; // set max number of bytes allowed in MsgQ
        msgctl(msg_q_key_id, IPC_SET, &msq_stat); // set AIR pid to kernel
    }
    ~InCommand(void)
    {
        msgctl(msg_q_key_id, IPC_RMID, 0);
    }
    int HandleKernelMsg(void);

private:
    void _SendAirRunCMD(void);
    void _SendAirStreamIntervalCMD(void);
    void _SendAirFileWriteCMD(void);
    void _SendAirRemainFileCountCMD(void);
    void _SendNodeRunCMD(void);
    void _SetNodeRunValue(uint32_t* type2, uint32_t* value1);
    void _SendNodeInitCMD(void);

    void _NotifyToPolicy(uint32_t type2, uint32_t value1, uint32_t value2, int pid,
        int64_t cmd_type, int cmd_order);

    input::Subject* subject {nullptr};
    key_t msg_q_key_id {0};
    key_t msg_q_key_value {0};
    struct RecvMsgSt
    {
        int64_t pid;
        int cmd_int_value1;
        int cmd_int_value2;
        int cmd_int_value3;
        char cmd_str_value[10];
        int cmd_type;
        int cmd_order;
    };
    struct RecvMsgSt recv_msg
    {
        0,
    };
};

} // namespace input

#endif // AIR_IN_H
