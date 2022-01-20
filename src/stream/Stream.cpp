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

#include <time.h>
#include <unistd.h>

#include <fstream>
#include <string>

#include "src/lib/json/Json.h"

stream::Stream::Stream(void)
{
    pid = getpid();
}

void
stream::Stream::SendPacket(void)
{
    // new json
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
