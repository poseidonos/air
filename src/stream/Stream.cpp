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

#include "src/stream/Stream.h"

#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "src/lib/json/Json.h"

stream::Stream::Stream(meta::GlobalMetaGetter* global_meta_getter)
: global_meta_getter(global_meta_getter)
{
    pid = getpid();
}

void
stream::Stream::SendPacket(void)
{
    if (false == global_meta_getter->FileWrite())
    {
        air::json_clear();
        return;
    }

    std::ofstream export_file;
    // out : output(default)
    // app : append
    // ate : at end, output position starts at the end of the file.
    // export_file.open("tmp.json", std::ofstream::out | std::ofstream::app);

    std::string filename {"/tmp/air_"};
    time_t curr_time = time(NULL);
    struct tm curr_tm;
    localtime_r(&curr_time, &curr_tm);
    filename += std::to_string(curr_tm.tm_year + 1900);
    if (curr_tm.tm_mon + 1 < 10)
    {
        filename += "0";
    }
    filename += std::to_string(curr_tm.tm_mon + 1);
    if (curr_tm.tm_mday < 10)
    {
        filename += "0";
    }
    filename += std::to_string(curr_tm.tm_mday);
    filename += "_";
    filename += std::to_string(pid);
    filename += ".json";

    export_file.open(filename, std::ofstream::out | std::ofstream::app);
    if (!export_file.fail())
    {
        export_file << air::json("air") << std::endl;
    }
    export_file.close();

    air::json_clear();
}

void
stream::Stream::RemainFiles(void)
{
    if (false == global_meta_getter->FileWrite())
    {
        return;
    }

    std::string str_dir {"/tmp/"};
    std::string file_suffix {"_" + std::to_string(pid) + ".json"};
    std::map<unsigned long, std::string> files;
    files.clear();

    DIR* dp {opendir(str_dir.c_str())};
    if (nullptr != dp)
    {
        for (;;)
        {
            struct dirent* item {readdir(dp)};
            if (nullptr == item)
            {
                break;
            }

            std::string str_dir_name {item->d_name};

            if (std::string::npos != str_dir_name.find("air_") &&
                std::string::npos != str_dir_name.find(file_suffix))
            {
                unsigned long date {std::stoul(str_dir_name.substr(4, 8))};
                files.insert({date, "/tmp/" + str_dir_name});
            }
        }
    }
    closedir(dp);

    int32_t delete_file_count {static_cast<int32_t>(files.size()) -
        static_cast<int32_t>(global_meta_getter->RemainingFileCount())};

    if (0 < delete_file_count)
    {
        for (const auto& item : files)
        {
            if (0 != remove(item.second.c_str()))
            {
                std::cout << "air delete file failed: " << item.second << "\n";
            }
            delete_file_count--;

            if (0 >= delete_file_count)
            {
                break;
            }
        }
    }
    files.clear();
}
