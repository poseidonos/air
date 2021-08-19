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

#include "src/target_detector/Detector.h"

#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

#include <map>
#include <string>

void
detect::Detector::Process(void)
{
    std::string str_dir = "/proc/" + std::to_string(getpid()) + "/task";

    DIR* dp = opendir(str_dir.c_str());
    if (nullptr != dp)
    {
        for (;;)
        {
            struct dirent* item = readdir(dp);
            if (nullptr == item)
            {
                break;
            }

            std::string str_tid = item->d_name;
            if (str_tid.compare(".") == 0 || str_tid.compare("..") == 0)
            {
                continue;
            }
            _CreateNodeDataArray(std::stoi(str_tid));
        }
    }
    closedir(dp);

    _DeleteNodeDataArray();
}

void
detect::Detector::_CreateNodeDataArray(uint32_t tid)
{
    node_manager->CreateNodeDataArray(tid);
}

void
detect::Detector::_DeleteNodeDataArray(void)
{
    std::string str_dir = "/proc/" + std::to_string(getpid()) + "/task/";

    auto iter = node_manager->nda_map.begin();
    while (iter != node_manager->nda_map.end())
    {
        DIR* dp = opendir((str_dir + std::to_string(iter->first)).c_str());
        if (nullptr == dp)
        {
            if (true == node_manager->CanDeleteNodeDataArray(iter->second))
            {
                node_manager->DeleteNodeDataArray(iter->second);
                node_manager->nda_map.erase(iter++);
            }
            else
            {
                iter++;
            }
        }
        else
        {
            iter++;
        }
        closedir(dp);
    }
}
