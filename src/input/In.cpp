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

#include "src/input/In.h"

#include <cstring>
#include <iostream>

#include "src/lib/Protocol.h"

int
input::Subject::Notify(uint32_t index, uint32_t type1, uint32_t type2,
    uint32_t value1, uint32_t value2, int pid, int cmd_type, int cmd_order)
{
    if (index < to_dtype(pi::InSubject::COUNT))
    {
        arr_observer[index]->Update(
            type1, type2, value1, value2, pid, cmd_type, cmd_order);
        return 0;
    }

    return -1;
}

void
input::InCommand::_NotifyToPolicy(uint32_t type2, uint32_t value1, uint32_t value2,
    int pid, int64_t cmd_type, int cmd_order)
{
    subject->Notify(to_dtype(pi::InSubject::TO_POLICY),
        to_dtype(pi::Type1::INPUT_TO_POLICY), type2, value1, value2, pid, cmd_type,
        cmd_order);
}

void
input::InCommand::_SendAirRunCMD(void)
{
    _NotifyToPolicy(to_dtype(pi::Type2::ENABLE_AIR), recv_msg.cmd_int_value1, 0,
        static_cast<int>(recv_msg.pid), recv_msg.cmd_type, recv_msg.cmd_order);
}

void
input::InCommand::_SendAirStreamIntervalCMD(void)
{
    uint32_t type2 = to_dtype(pi::Type2::SET_STREAMING_INTERVAL);
    uint32_t value1 = recv_msg.cmd_int_value1; // interval
    uint32_t value2 = 0;
    _NotifyToPolicy(
        type2, value1, value2, recv_msg.pid, recv_msg.cmd_type, recv_msg.cmd_order);
}

void
input::InCommand::_SendAirFileWriteCMD(void)
{
    uint32_t type2 = to_dtype(pi::Type2::SET_FILE_WRITE);
    uint32_t value1 = recv_msg.cmd_int_value1; // file_write
    _NotifyToPolicy(
        type2, value1, 0, recv_msg.pid, recv_msg.cmd_type, recv_msg.cmd_order);
}

void
input::InCommand::_SendAirRemainFileCountCMD(void)
{
    uint32_t type2 = to_dtype(pi::Type2::SET_REMAIN_FILE_COUNT);
    uint32_t value1 = recv_msg.cmd_int_value1; // remain_file_count
    _NotifyToPolicy(
        type2, value1, 0, recv_msg.pid, recv_msg.cmd_type, recv_msg.cmd_order);
}

void
input::InCommand::_SetNodeRunValue(uint32_t* type2, uint32_t* value2)
{
    if (strcmp(recv_msg.cmd_str_value, "node") == 0)
    {
        (*type2) = to_dtype(pi::Type2::ENABLE_NODE);
        (*value2) = recv_msg.cmd_int_value2;
    }
    else if (strcmp(recv_msg.cmd_str_value, "range") == 0)
    {
        (*type2) = to_dtype(pi::Type2::ENABLE_NODE_WITH_RANGE);
        (*value2) = (recv_msg.cmd_int_value2 << 16) + recv_msg.cmd_int_value3;
    }
    else if (strcmp(recv_msg.cmd_str_value, "group") == 0)
    {
        (*type2) = to_dtype(pi::Type2::ENABLE_NODE_WITH_GROUP);
        (*value2) = recv_msg.cmd_int_value2;
    }
    else if (strcmp(recv_msg.cmd_str_value, "all") == 0)
    {
        (*type2) = to_dtype(pi::Type2::ENABLE_NODE_ALL);
    }
}

void
input::InCommand::_SendNodeRunCMD(void)
{
    uint32_t type2 {0};                        // command
    uint32_t value1 = recv_msg.cmd_int_value1; // bool
    uint32_t value2 = recv_msg.cmd_int_value2; // node_id range
    _SetNodeRunValue(&type2, &value2);
    _NotifyToPolicy(
        type2, value1, value2, recv_msg.pid, recv_msg.cmd_type, recv_msg.cmd_order);
}

void
input::InCommand::_SendNodeInitCMD(void)
{
    uint32_t value1 {0};
    uint32_t value2 {0};
    uint32_t type2 {0};

    if (strcmp(recv_msg.cmd_str_value, "node") == 0)
    {
        type2 = to_dtype(pi::Type2::INITIALIZE_NODE);
        value1 = recv_msg.cmd_int_value1;
    }
    else if (strcmp(recv_msg.cmd_str_value, "range") == 0)
    {
        type2 = to_dtype(pi::Type2::INITIALIZE_NODE_WITH_RANGE);
        value1 = (recv_msg.cmd_int_value1 << 16) + recv_msg.cmd_int_value2;
    }
    else if (strcmp(recv_msg.cmd_str_value, "group") == 0)
    {
        type2 = to_dtype(pi::Type2::INITIALIZE_NODE_WITH_GROUP);
        value1 = recv_msg.cmd_int_value1;
    }
    else if (strcmp(recv_msg.cmd_str_value, "all") == 0)
    {
        type2 = to_dtype(pi::Type2::INITIALIZE_NODE_ALL);
    }

    _NotifyToPolicy(
        type2, value1, value2, recv_msg.pid, recv_msg.cmd_type, recv_msg.cmd_order);
}

void
input::InCommand::_SendNodeSampleRatioCMD(void)
{
    uint32_t type2 {0};                        // command
    uint32_t value1 = recv_msg.cmd_int_value1; // ratio
    uint32_t value2 = {0};                     // node_id range

    if (strcmp(recv_msg.cmd_str_value, "node") == 0)
    {
        type2 = to_dtype(pi::Type2::SET_SAMPLING_RATE);
        value2 = recv_msg.cmd_int_value2;
    }
    else if (strcmp(recv_msg.cmd_str_value, "range") == 0)
    {
        type2 = to_dtype(pi::Type2::SET_SAMPLING_RATE_WITH_RANGE);
        value2 = (recv_msg.cmd_int_value2 << 16) + recv_msg.cmd_int_value3;
    }
    else if (strcmp(recv_msg.cmd_str_value, "group") == 0)
    {
        type2 = to_dtype(pi::Type2::SET_SAMPLING_RATE_WITH_GROUP);
        value2 = recv_msg.cmd_int_value2;
    }
    else if (strcmp(recv_msg.cmd_str_value, "all") == 0)
    {
        type2 = to_dtype(pi::Type2::SET_SAMPLING_RATE_ALL);
    }

    _NotifyToPolicy(
        type2, value1, value2, recv_msg.pid, recv_msg.cmd_type, recv_msg.cmd_order);
}

int
input::InCommand::HandleKernelMsg(void)
{
    int num_enq = 0;
    memset(&recv_msg, 0, sizeof(struct RecvMsgSt));
    while (0 < msgrcv(msg_q_key_id, (void*)&recv_msg,
                   sizeof(struct RecvMsgSt) - sizeof(int64_t), 0, IPC_NOWAIT))
    {
        CommandType type = static_cast<CommandType>(recv_msg.cmd_type);
        switch (type)
        {
            case CMD_PID:
                break;
            case CMD_AIR_RUN:
                _SendAirRunCMD();
                num_enq++;
                break;
            case CMD_AIR_STREAM_INTERVAL:
                _SendAirStreamIntervalCMD();
                num_enq++;
                break;
            case CMD_AIR_FILE_WRITE:
                _SendAirFileWriteCMD();
                num_enq++;
                break;
            case CMD_AIR_REMAIN_FILE_COUNT:
                _SendAirRemainFileCountCMD();
                num_enq++;
                break;
            case CMD_NODE_RUN:
                _SendNodeRunCMD();
                num_enq++;
                break;
            case CMD_NODE_INIT:
                _SendNodeInitCMD();
                num_enq++;
                break;
            case CMD_NODE_SAMPLE_RATIO:
                _SendNodeSampleRatioCMD();
                num_enq++;
                break;
        }
        memset(&recv_msg, 0, sizeof(struct RecvMsgSt));
    }

    return num_enq;
}
