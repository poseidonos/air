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

#include "tool/tui/FileDetector.h"

#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

filedata
air::FileDetector::Detect(void)
{
    int fd = fcntl(STDIN_FILENO, F_SETFL, O_APPEND);
    if (-1 == fd)
    {
        std::cout << "FileDetector::Detect fcntl failed\n";
    }
    std::cout << "Detecting...\n";
    waiting = true;

    std::thread([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(60000)); // 60 seconds
        this->HandleTimeout();
    }).detach();

    while (!exit)
    {
        _Monitoring();

        if (-1 == pid)
        {
            sleep(1);
        }
    }

    waiting = false;
    if (-1 == pid)
    {
        filedata err {pid, ""};
        return err;
    }

    std::string file {"/tmp/air_"};
    time_t curr_time = time(NULL);
    tm* curr_tm = localtime(&curr_time);
    file += std::to_string(curr_tm->tm_year + 1900);
    if (curr_tm->tm_mon + 1 < 10)
    {
        file += "0";
    }
    file += std::to_string(curr_tm->tm_mon + 1);
    if (curr_tm->tm_mday < 10)
    {
        file += "0";
    }
    file += std::to_string(curr_tm->tm_mday);
    file += "_" + std::to_string(pid) + ".json";
    filedata result {pid, file};
    return result;
}

void
air::FileDetector::HandleTimeout(void)
{
    if (waiting)
    {
        std::cout << "(Timeout) No Running Process...\n";
        exit = true;
    }
}

void
air::FileDetector::_Monitoring(void)
{
    _InitData();
    _UpdatePidMap();
    _UpdatePidStatus();
    if (2 <= candidates)
    {
        _SelectPid();
    }
}

void
air::FileDetector::_InitData(void)
{
    pid_map.clear();
    candidates = 0;
    pid = -1;
}

void
air::FileDetector::_UpdatePidMap(void)
{
    std::string sys_cmd;
    sys_cmd = "find /tmp/ -name \"air_";
    time_t curr_time = time(NULL);
    tm* curr_tm = localtime(&curr_time);
    sys_cmd += std::to_string(curr_tm->tm_year + 1900);
    if (curr_tm->tm_mon + 1 < 10)
    {
        sys_cmd += "0";
    }
    sys_cmd += std::to_string(curr_tm->tm_mon + 1);
    if (curr_tm->tm_mday < 10)
    {
        sys_cmd += "0";
    }
    sys_cmd += std::to_string(curr_tm->tm_mday);
    sys_cmd += "_*.json\"";

    FILE* fp;
    fp = popen(sys_cmd.c_str(), "r");
    if (nullptr == fp)
    {
        return;
    }

    char sys_result[256] {
        0,
    };
    while (NULL != fgets(sys_result, 256, fp))
    {
        char* tok = strtok(sys_result, "_");
        if (nullptr == tok)
        {
            break;
        }
        tok = strtok(NULL, "_");
        if (nullptr == tok)
        {
            break;
        }
        tok = strtok(NULL, "_");
        if (nullptr == tok)
        {
            break;
        }
        tok = strtok(tok, ".");
        if (nullptr == tok)
        {
            break;
        }
        int pid = std::stoi(tok);
        pid_map.insert({pid, false});
    }

    pclose(fp);
}

void
air::FileDetector::_UpdatePidStatus(void)
{
    for (auto& i : pid_map)
    {
        std::string pid_str = std::to_string(i.first);
        std::string sys_cmd {"ps -e | grep " + pid_str};

        FILE* fp;
        fp = popen(sys_cmd.c_str(), "r");
        if (nullptr == fp)
        {
            continue;
        }

        char sys_result[512] {
            0,
        };
        if (NULL != fgets(sys_result, 512, fp))
        {
            if (nullptr != strstr(sys_result, pid_str.c_str()))
            {
                candidates++;
                pid = i.first;
                i.second = true;
                waiting = false;
                exit = true;
            }
        }

        pclose(fp);
    }
}

void
air::FileDetector::_SelectPid(void)
{
    int number {1};

    std::cout << "please select one...\n";
    for (auto& i : pid_map)
    {
        if (i.second)
        {
            std::cout << number << ". " << i.first << std::endl;
            number++;
        }
    }

    std::cout << ">> ";

    int selected {0};
    std::cin >> selected;

    number = 1;
    for (auto& i : pid_map)
    {
        if (i.second)
        {
            if (selected == number)
            {
                pid = i.first;
                break;
            }
            number++;
        }
    }
}
