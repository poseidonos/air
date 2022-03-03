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

#include "tool/tui/Book.h"

#include <unistd.h>

air::Book::Book(FileData* file_data): file_data(file_data)
{
    while (file_data->Update())
    {
        usleep(1000);
    }
    maximum_page_index = file_data->GetLineCount();
}

std::string&
air::Book::GetPage(EventType type)
{
    maximum_page_index = file_data->GetLineCount();

    if (EventType::VIEW_PREV == type)
    {
        if (0 < current_page_index)
        {
            current_page_index--;
        }
    }
    else if (EventType::VIEW_NEXT == type)
    {
        if (maximum_page_index - 1 > current_page_index)
        {
            current_page_index++;
        }
    }

    return file_data->GetLine(current_page_index);
}

std::string&
air::Book::GetLastPage(void)
{
    maximum_page_index = file_data->GetLineCount();
    current_page_index = maximum_page_index - 1;
    return file_data->GetLine(current_page_index);
}

void
air::Book::UpdatePageIndex(void)
{
    maximum_page_index = file_data->GetLineCount();
}
