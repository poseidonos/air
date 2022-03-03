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
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#include <iostream>

void
air::KeyListener::_InitKeyMode(void)
{
    tcgetattr(0, &default_termios);
    async_termios = default_termios;
    async_termios.c_lflag &= ~ICANON;
    async_termios.c_lflag &= ~ECHO;
    async_termios.c_cc[VMIN] = 1;
    async_termios.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &async_termios);
}

void
air::KeyListener::_ResetKeyMode(void)
{
    tcsetattr(0, TCSANOW, &default_termios);
}

int
air::KeyListener::_HitKey(void)
{
    unsigned char ch;
    int nread;

    if (-1 != input)
    {
        return 1;
    }
    async_termios.c_cc[VMIN] = 0;
    tcsetattr(0, TCSANOW, &async_termios);
    nread = read(0, &ch, 1);
    async_termios.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &async_termios);
    if (1 == nread)
    {
        input = ch;
    }
    return 0;
}

int
air::KeyListener::_GetCh(void)
{
    char ch;
    if (-1 != input)
    {
        ch = input;
        input = -1;
        return ch;
    }
    if (-1 == read(0, &ch, 1))
    {
        return -1;
    }
    return ch;
}

air::EventData
air::KeyListener::Listening(void)
{
    EventData event;

    if (!_HitKey())
    {
        return event;
    }

    int getchar_result;
    getchar_result = _GetCh();

    switch (getchar_result)
    {
        case KeyNormal::SPECIAL:
        {
            getchar_result = _GetCh();
            if (KeySpecial::KEY_ESC == getchar_result)
            {
                event.type = air::EventType::TUI_EXIT;
            }
            else if (KeySpecial::KEY_ARROW == getchar_result)
            {
                getchar_result = _GetCh();
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
        case KeyNormal::KEY_B:
        case KeyNormal::KEY_b:
        {
            event.type = air::EventType::VIEW_PREV;
            break;
        }
        case KeyNormal::KEY_N:
        case KeyNormal::KEY_n:
        {
            event.type = air::EventType::VIEW_NEXT;
            break;
        }
        case KeyNormal::KEY_SPACE:
        {
            event.type = air::EventType::PLAY_TOGGLE;
            break;
        }
        default:
        {
            break;
        }
    }

    return event;
}
