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

#ifndef AIR_TUI_BOOK_H
#define AIR_TUI_BOOK_H

#include <string>

#include "tool/tui/EventType.h"
#include "tool/tui/FileData.h"

namespace air
{
class Book
{
public:
    explicit Book(FileData* file_data);
    std::string& GetPage(EventType type);
    std::string& GetLastPage(void);
    int64_t
    GetCurrentPageIndex(void)
    {
        return current_page_index;
    }
    int64_t
    GetMaximumPageIndex(void)
    {
        return maximum_page_index;
    }
    void UpdatePageIndex(void);

private:
    FileData* file_data {nullptr};
    int64_t current_page_index {0};
    int64_t maximum_page_index {0};
};

} // namespace air

#endif // AIR_TUI_BOOK_H
