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

#include "tool/tui/FileData.h"

#include <sys/stat.h>

#include <algorithm>
#include <fstream>
#include <iostream>

air::FileData::FileData(std::string file): title(file)
{
    lines.reserve(1000);
}

bool
air::FileData::Update(void)
{
    std::ifstream file;
    file.open(title);
    if (!file.good())
    {
        std::cout << "file read stopped("
                  << "eof:" << file.eof() << ", fail:" << file.fail()
                  << ", bad:" << file.bad() << ")\n";
        file.close();
        return false;
    }

    struct stat file_stat
    {
    };
    if (0 != stat(title.c_str(), &file_stat))
    {
        std::cout << "file stat failed\n";
        file.close();
        return false;
    }

    int64_t new_data_size {file_stat.st_size};
    if (read_data_size != new_data_size)
    {
        if (false == update_flag || prev_data_size != new_data_size)
        {
            update_flag = true;
            prev_data_size = new_data_size;
            file.close();
            return true;
        }
        update_flag = false;
        file.seekg(read_data_size, file.beg);
        std::string line;
        while (std::getline(file, line))
        {
            lines.push_back(line);
            line_count++;
        }
        read_data_size = new_data_size;
        file.close();
        return true;
    }
    else
    {
        file.close();
        return false;
    }
}
