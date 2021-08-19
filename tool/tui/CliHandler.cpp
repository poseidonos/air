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

#include "tool/tui/CliHandler.h"

#include <stdlib.h>

#include <iostream>
#include <thread>

void
air::CLIHandler::HandleCLI(EventData data, AConfig& tree, int new_pid)
{
    pid = new_pid;

    switch (data.type)
    {
        case air::EventType::CLI_INIT:
        {
            _SendInit(tree);
            break;
        }
        case air::EventType::CLI_RUN:
        {
            _SendRun(tree);
            break;
        }
        case air::EventType::CLI_STOP:
        {
            _SendStop(tree);
            break;
        }
        case air::EventType::CLI_INTERVAL:
        {
            _SendInterval(data.value);
            break;
        }
        default:
        {
            break;
        }
    }
}

void
air::CLIHandler::_SendInit(AConfig& tree)
{
    std::string cmd_str;
    cmd_str = "air_cli --pid=" + std::to_string(pid);
    cmd_str += " --node-init=";
    _SendCommandWithOption(tree, cmd_str);
}

void
air::CLIHandler::_SendRun(AConfig& tree)
{
    std::string cmd_str;
    cmd_str = "air_cli --pid=" + std::to_string(pid);

    if (tree.pos_top)
    {
        cmd_str += " --air-run=true";
        _AsyncSystemCall(cmd_str);
    }
    else
    {
        cmd_str += " --node-run=true_";
        _SendCommandWithOption(tree, cmd_str);
    }
}

void
air::CLIHandler::_SendStop(AConfig& tree)
{
    std::string cmd_str;
    cmd_str = "air_cli --pid=" + std::to_string(pid);

    if (tree.pos_top)
    {
        cmd_str += " --air-run=false";
        _AsyncSystemCall(cmd_str);
    }
    else
    {
        cmd_str += " --node-run=false_";
        _SendCommandWithOption(tree, cmd_str);
    }
}

void
air::CLIHandler::_SendInterval(int interval)
{
    std::string cmd_str;
    cmd_str = "air_cli --pid=" + std::to_string(pid);
    cmd_str += " --air-stream-interval=";
    cmd_str += std::to_string(interval);

    _AsyncSystemCall(cmd_str);
}

void
air::CLIHandler::_SendCommandWithOption(AConfig& tree, std::string& cmd_str)
{
    if (tree.pos_top)
    {
        cmd_str += "all";
    }
    else if (tree.pos_group)
    {
        cmd_str += "group:";
        cmd_str += std::to_string(tree.pos_id);
    }
    else if (tree.pos_node)
    {
        cmd_str += "node:";
        cmd_str += std::to_string(tree.pos_id);
    }

    _AsyncSystemCall(cmd_str);
}

void
air::CLIHandler::_AsyncSystemCall(std::string& cmd_str)
{
    std::thread([this, cmd_str]() {
        int result = system(cmd_str.c_str());
        if (-1 == result)
        {
            std::cout << cmd_str << " failed\n";
        }
    }).detach();
}
