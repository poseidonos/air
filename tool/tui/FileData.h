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

#ifndef AIR_TUI_FILE_DATA_H
#define AIR_TUI_FILE_DATA_H

#include <string>
#include <vector>

namespace air
{
class FileData
{
public:
    explicit FileData(std::string file);
    bool Update(void);
    int64_t
    GetLineCount(void)
    {
        return line_count;
    }
    std::string&
    GetLine(int64_t index)
    {
        return lines[index];
    }

private:
    std::string title {""};
    int64_t line_count {0};
    int64_t read_data_size {0};
    int64_t prev_data_size {0};
    bool update_flag {false};
    std::vector<std::string> lines;
};

} // namespace air

#endif // AIR_TUI_FILE_DATA_H
