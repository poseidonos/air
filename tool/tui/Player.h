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

#ifndef AIR_TUI_PLAYER_H
#define AIR_TUI_PLAYER_H

#include <string>

#include "tool/tui/CliHandler.h"
#include "tool/tui/ConfigTree.h"
#include "tool/tui/EventType.h"
#include "tool/tui/KeyEventHandler.h"
#include "tool/tui/MoveHandler.h"
#include "tool/tui/Option.h"
#include "tool/tui/Viewer.h"

namespace air
{
class Player
{
public:
    explicit Player(TuiOption* option);
    void Run(void);

private:
    void _RunBatch(void);
    void _RunRealTime(void);

    TuiOption* option {nullptr};
    EventData event;
    KeyListener key_listener;
    AConfig tree;
    MoveHandler move_handler;
    CLIHandler cli_handler;
    Viewer viewer;
};

} // namespace air

#endif // AIR_TUI_PLAYER_H
