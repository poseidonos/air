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

#include "tool/tui/Player.h"

#include <unistd.h>

#include <tuple>

#include "tool/tui/Book.h"
#include "tool/tui/FileData.h"
#include "tool/tui/FileDetector.h"

air::Player::Player(TuiOption* option): option(option)
{
}

void
air::Player::Run(void)
{
    if (option->HasFile())
    {
        _RunBatch();
    }
    else
    {
        _RunRealTime();
    }
}

void
air::Player::_RunBatch(void)
{
    std::string file {option->GetFile()};
    FileData file_data {file};
    Book book {&file_data};
    std::string json_string {book.GetPage(EventType::VIEW_PREV)};
    int64_t current_page_index {book.GetCurrentPageIndex()};
    int64_t maximum_page_index {book.GetMaximumPageIndex()};
    bool update_render {true};

    while (EventType::TUI_EXIT != event.type)
    {
        event = key_listener.Listening();
        switch (event.type)
        {
            case EventType::MOVE_UP... EventType::MOVE_LEFT:
                move_handler.HandleMove(event.type, tree);
                update_render = true;
                break;
            case EventType::VIEW_PREV... EventType::VIEW_NEXT:
                json_string = book.GetPage(event.type);
                current_page_index = book.GetCurrentPageIndex();
                maximum_page_index = book.GetMaximumPageIndex();
                update_render = true;
                break;
            default:
                break;
        }

        if (update_render)
        {
            viewer.Render(ViewMode::OFFLINE, tree, json_string, current_page_index,
                maximum_page_index);
        }

        usleep(1000); // 1ms sleep for cpu efficiencyj
        update_render = false;
    }
}

void
air::Player::_RunRealTime(void)
{
    FileDetector file_detector;
    int pid;
    std::string file;
    std::tie(pid, file) = file_detector.Detect();
    if (0 == file.compare(""))
    {
        std::cout << "File detection failed\n";
    }
    FileData file_data {file};
    Book book {&file_data};
    std::string json_string {book.GetPage(EventType::DEFAULT)};
    int64_t current_page_index {book.GetCurrentPageIndex()};
    int64_t maximum_page_index {book.GetMaximumPageIndex()};
    bool update_render {true};
    bool pause {false};

    while (EventType::TUI_EXIT != event.type)
    {
        event = key_listener.Listening();
        switch (event.type)
        {
            case EventType::MOVE_UP... EventType::MOVE_LEFT:
                move_handler.HandleMove(event.type, tree);
                update_render = true;
                break;
            case EventType::VIEW_PREV... EventType::VIEW_NEXT:
                json_string = book.GetPage(event.type);
                current_page_index = book.GetCurrentPageIndex();
                maximum_page_index = book.GetMaximumPageIndex();
                update_render = true;
                pause = true;
                break;
            case EventType::PLAY_TOGGLE:
                pause = !pause;
                update_render = true;
                break;
            case EventType::CLI_INIT... EventType::CLI_INTERVAL:
                cli_handler.HandleCLI(event, tree, pid);
                break;
            default:
                break;
        }

        if (file_data.Update())
        {
            update_render = true;
            if (pause)
            {
                book.UpdatePageIndex();
            }
            else
            {
                json_string = book.GetLastPage();
            }
            current_page_index = book.GetCurrentPageIndex();
            maximum_page_index = book.GetMaximumPageIndex();
        }

        if (update_render)
        {
            viewer.Render(ViewMode::ONLINE, tree, json_string, current_page_index,
                maximum_page_index, pid, pause);
        }

        usleep(1000); // 1ms sleep for cpu efficiency
        update_render = false;
    }
}
