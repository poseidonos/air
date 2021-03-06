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
enum class TuiMode : int
{
    OFFLINE,
    ONLINE,
};

enum class TuiStatus : int
{
    PLAY,
    PAUSE,
    STOP,
};

enum class NodeType : int
{
    NULLTYPE,
    PERFORMANCE,
    LATENCY,
    QUEUE,
    UTILIZATION,
    COUNT,
    HISTOGRAM,
};

class Viewer
{
public:
    void Render(TuiMode tui_mode, TuiStatus tui_status, AConfig& tree,
        std::string& json_string, int64_t current_page_index,
        int64_t maximum_page_index, int pid = -1);

private:
    void _ClearWindow(void);
    void _Draw(TuiMode tui_mode, TuiStatus tui_status, AConfig& tree,
        std::string& json_string, int64_t current_page_index,
        int64_t maximum_page_index, int pid);
    void _DrawHeadline(TuiMode tui_mode, TuiStatus tui_status,
        int64_t current_page_index, int64_t maximum_page_index, int pid);
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
    void _DrawHistogram(JSONdoc& doc, uint32_t remain_col);
    void _DrawHistogramUnit(JSONdoc& doc, uint32_t remain_col);
    void _DrawHistogramPeriod(JSONdoc& doc, uint32_t remain_col);
    void _DrawHistogramCumulation(JSONdoc& doc, uint32_t remain_col);

    uint64_t sum_perf_period_iops {0};
    uint64_t sum_perf_period_bw {0};
    uint64_t sum_perf_cumulation_iops {0};
    uint64_t sum_perf_cumulation_bw {0};
    uint64_t sum_util_period_usage {0};
    uint64_t sum_util_cumulation_usage {0};

    uint32_t ws_row {0};
    uint32_t ws_col {0};
    uint32_t curr_row {0};
};

} // namespace air

#endif // AIR_TUI_VIEWER_H
