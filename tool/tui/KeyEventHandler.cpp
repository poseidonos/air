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

#include "tool/tui/KeyEventHandler.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>

void
air::TerminalSetting::SaveDefaultTermios(void)
{
    tcgetattr(STDIN_FILENO, &default_termios);
}

void
air::TerminalSetting::RestoreDefaultTermios(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &default_termios);

    int fd;
    fd = fcntl(STDIN_FILENO, F_SETFL, O_APPEND);
    if (-1 == fd)
    {
        std::cout << "TerminalSetting::RestoreDefaultTermios failed\n";
    }
}

void
air::KeyListener::_SetListenMode(void)
{
    int fd;
    struct termios instant_termios;

    instant_termios = setting.default_termios;
    instant_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &instant_termios);

    fd = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (-1 != fd)
    {
        fd = fcntl(STDIN_FILENO, F_SETFL, fd | O_NONBLOCK);
        if (-1 == fd)
        {
            std::cout << "KeyListener::_SetListenMode failed\n";
        }
    }
}

air::EventData
air::KeyListener::Listening(void)
{
    EventData event;

    int getchar_result;
    getchar_result = getchar();

    if (EOF != getchar_result)
    {
        switch (getchar_result)
        {
            case KeyNormal::SPECIAL:
            {
                getchar_result = getchar();
                if (KeySpecial::KEY_ESC == getchar_result)
                {
                    event.type = air::EventType::TUI_EXIT;
                }
                else if (KeySpecial::KEY_ARROW == getchar_result)
                {
                    getchar_result = getchar();
                    switch (getchar_result)
                    {
                        case KeyArrow::KEY_ARROW_UP:
                        {
                            event.type = air::EventType::MOVE_UP;
                            break;
                        }
                        case KeyArrow::KEY_ARROW_DOWN:
                        {
                            event.type = air::EventType::MOVE_DOWN;
                            break;
                        }
                        case KeyArrow::KEY_ARROW_RIGHT:
                        {
                            event.type = air::EventType::MOVE_RIGHT;
                            break;
                        }
                        case KeyArrow::KEY_ARROW_LEFT:
                        {
                            event.type = air::EventType::MOVE_LEFT;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                }

                break;
            }
            case KeyNormal::KEY_Q:
            case KeyNormal::KEY_q:
            {
                event.type = air::EventType::TUI_EXIT;
                break;
            }
            case KeyNormal::KEY_O:
            case KeyNormal::KEY_o:
            {
                event.type = air::EventType::CLI_RUN;
                break;
            }
            case KeyNormal::KEY_X:
            case KeyNormal::KEY_x:
            {
                event.type = air::EventType::CLI_STOP;
                break;
            }
            case KeyNormal::KEY_I:
            case KeyNormal::KEY_i:
            {
                event.type = air::EventType::CLI_INIT;
                break;
            }
            case KeyNormal::NUM_1... KeyNormal::NUM_9:
            {
                event.type = air::EventType::CLI_INTERVAL;
                event.value = getchar_result - 48;
                break;
            }
            default:
            {
                break;
            }
        }
    }

    return event;
}
