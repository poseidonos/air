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

#ifndef AIR_TUI_VIEWER_H
#define AIR_TUI_VIEWER_H

#include <string>

#include "src/lib/json/Json.h"
#include "tool/tui/ConfigTree.h"
#include "tool/tui/EventType.h"

namespace air
{
enum class NodeType : int
{
    NULLTYPE,
    PERFORMANCE,
    LATENCY,
    QUEUE,
    UTILIZATION,
    COUNT,
};

class Viewer
{
public:
    void Render(EventData data, AConfig& tree, int pid);

private:
    void _SetFilename(int pid);
    bool _Update(EventType type);
    bool _CheckMovement(EventType type);
    bool _CheckFilesize(void);
    void _ClearWindow(void);
    void _Draw(AConfig& tree);
    void _DrawHeadline(void);
    void _DrawGroup(AConfig& tree);
    void _DrawNode(ANode& tree, std::string name, JSONdoc& doc);
    void _SumData(JSONdoc& doc, NodeType type);
    void _DrawObj(JSONdoc& doc, NodeType type);
    uint32_t _DrawDefault(JSONdoc& doc);
    void _DrawPerf(JSONdoc& doc, uint32_t remain_col);
    void _DrawLat(JSONdoc& doc, uint32_t remain_col);
    void _DrawQueue(JSONdoc& doc, uint32_t remain_col);
    void _DrawUtilization(JSONdoc& doc, uint32_t remain_col);
    void _DrawCount(JSONdoc& doc, uint32_t remain_col);

    std::string filename{""};
    int file_size{0};
    int prev_file_size{0};
    int printed_pos{0};
    bool file_update{false};
    int pid{-1};

    uint64_t sum_iops;
    uint64_t sum_bw;
    uint64_t sum_util_usage;
    uint64_t sum_util_total_usage;

    uint32_t ws_row;
    uint32_t ws_col;
    uint32_t curr_row;
};

} // namespace air

#endif // AIR_TUI_VIEWER_H
