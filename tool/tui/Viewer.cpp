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

#include "tool/tui/Viewer.h"

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include <fstream>
#include <iostream>

#include "tool/tui/LibToString.h"

#define RESET "\033[0m"
#define RED "\033[1m\033[31m"
#define GREEN "\033[1m\033[32m"
#define YELLOW "\033[1m\033[33m"
#define BLUE "\033[1m\033[34m"
#define PURPLE "\033[1m\033[35m"

void
air::Viewer::Render(ViewMode view_mode, AConfig& tree, std::string& json_string,
    int64_t current_page_index, int64_t maximum_page_index, int pid, bool pause)
{
    _ClearWindow();
    _Draw(view_mode, tree, json_string, current_page_index, maximum_page_index, pid,
        pause);
}

void
air::Viewer::_ClearWindow(void)
{
    int result {0};
    result = system("clear");

    if (-1 == result)
    {
        std::cout << "clear window failed" << std::endl;
    }
}

void
air::Viewer::_Draw(ViewMode view_mode, AConfig& tree, std::string& json_string,
    int64_t current_page_index, int64_t maximum_page_index, int pid, bool pause)
{
    struct winsize ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
    ws_row = ws.ws_row;
    ws_col = ws.ws_col;
    curr_row = 1;

    air::parse("data", json_string);
    try
    {
        _DrawHeadline(
            view_mode, current_page_index, maximum_page_index, pid, pause);
        _DrawGroup(tree);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    air::json_clear();
}

void
air::Viewer::_DrawHeadline(ViewMode view_mode, int64_t current_page_index,
    int64_t maximum_page_index, int pid, bool pause)
{
    auto& head = air::json("data");
    std::stringstream stream;

    std::cout << "AIR TUI status: [";
    if (ViewMode::OFFLINE == view_mode)
    {
        std::cout << BLUE << "offline" << RESET;
    }
    else
    {
        stream << head["play"];
        std::string status {stream.str()};
        if (std::string::npos != status.find("true"))
        {
            if (pause)
            {
                std::cout << PURPLE << "pause" << RESET;
            }
            else
            {
                std::cout << GREEN << "online" << RESET;
            }
        }
        else
        {
            std::cout << RED << "stop" << RESET;
        }
    }

    std::cout << "], page: [";
    std::cout << GREEN << current_page_index + 1 << RESET;
    std::cout << "/";
    if (ViewMode::OFFLINE == view_mode)
    {
        std::cout << BLUE << maximum_page_index << RESET;
    }
    else
    {
        std::cout << GREEN << maximum_page_index << RESET;
    }

    std::cout << "],  interval: [";
    if (ViewMode::OFFLINE == view_mode)
    {
        std::cout << head["interval"];
    }
    else
    {
        std::cout << GREEN << head["interval"] << RESET;
    }
    std::cout << "],  ";

    std::cout << "timestamp: ";
    stream.str("");
    stream << head["timestamp"];
    std::string timestamp {stream.str()};

    time_t time;
    struct tm* tm;
    time = std::stoi(timestamp);
    tm = localtime(&time);
    std::cout << tm->tm_year + 1900 << "-" << tm->tm_mon + 1 << "-" << tm->tm_mday
              << ":" << tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec;

    if (ViewMode::ONLINE == view_mode)
    {
        std::cout << ", pid: " << pid;
    }

    std::cout << std::endl;
    curr_row++;

    if (ViewMode::OFFLINE == view_mode)
    {
        std::cout << "key {\u2191/\u2193}: move, {\u2192/\u2190}: (un)fold, "
                     "{b}: perv page, {n}: next page, {q(esc)}: quit\n";
        curr_row++;
    }
    else
    {
        std::cout << "key {\u2191/\u2193}: move, {\u2192/\u2190}: (un)fold, "
                     "{b}: perv page, {n}: next page, {space}: play/pause\n";
        curr_row++;

        std::cout << "    {1~9}: streaming interval, {i}: init, {o}: run, {x}: "
                     "stop, {q(esc)}: quit\n";
        curr_row++;
    }
}

void
air::Viewer::_DrawGroup(AConfig& tree)
{
    auto& group_json = air::json("data")["group"];
    std::stringstream stream;

    if (tree.pos_top)
    {
        std::cout << YELLOW << "(*)" << RESET;
    }
    else
    {
        std::cout << "( )";
    }
    std::cout << "   Top" << std::endl;
    curr_row++;

    for (auto i : air::range(group_json))
    {
        auto search_g = tree.group.find(i.key);
        if (tree.group.end() == search_g)
        {
            AGroup* NewGroup = new AGroup;
            stream.str("");
            stream << group_json[i.key]["group_id"];
            std::string gid {stream.str()};
            NewGroup->gid = std::stoi(gid);
            tree.group.insert({i.key, NewGroup});
        }
        search_g = tree.group.find(i.key);
        AGroup& group_st = *(search_g->second);
        if (group_st.here)
        {
            std::cout << YELLOW << "(*)" << RESET;
        }
        else
        {
            std::cout << "( )";
        }

        std::cout << "   ";
        if (group_st.fold)
        {
            std::cout << GREEN << "-" << RESET;
        }
        else
        {
            std::cout << GREEN << "+" << RESET;
        }
        std::cout << "Group:" << i.key << std::endl;
        curr_row++;
        if (ws_row <= curr_row)
        {
            return;
        }

        if (group_st.fold)
        {
            continue;
        }

        auto& nodes = group_json[i.key]["node"];
        for (auto node : air::range(nodes))
        {
            auto search_n = group_st.node.find(node.key);
            if (group_st.node.end() == search_n)
            {
                ANode* NewNode = new ANode;
                stream.str("");
                stream << nodes[node.key]["node_id"];
                std::string nid {stream.str()};
                NewNode->nid = std::stoi(nid);
                group_st.node.insert({node.key, NewNode});
            }
            search_n = group_st.node.find(node.key);
            ANode& node_st = *(search_n->second);
            _DrawNode(node_st, node.key, nodes[node.key]);
            if (ws_row <= curr_row)
            {
                return;
            }
        }
    }
}

void
air::Viewer::_DrawNode(ANode& node, std::string name, JSONdoc& doc)
{
    if (node.here)
    {
        std::cout << YELLOW << "(*)" << RESET;
    }
    else
    {
        std::cout << "( )";
    }

    std::stringstream stream;
    stream.str("");
    stream << doc["build"];
    std::string str_build {stream.str()};
    if (std::string::npos != str_build.find("false"))
    {
        std::cout << "[.]..Node:" << name << std::endl;
        curr_row++;
        return;
    }

    stream.str("");
    stream << doc["run"];
    std::string str_run {stream.str()};
    if (std::string::npos != str_run.find("false"))
    {
        std::cout << GREEN << "[X]" << RESET;
    }
    else
    {
        std::cout << GREEN << "[O]" << RESET;
    }

    if (node.fold)
    {
        std::cout << GREEN << "--" << RESET;
    }
    else
    {
        std::cout << GREEN << "++" << RESET;
    }

    std::cout << "Node:" << name << "(";
    std::cout << doc["type"] << ")" << std::endl;
    curr_row++;
    if (ws_row <= curr_row)
    {
        return;
    }

    if (node.fold)
    {
        return;
    }

    stream.str("");
    stream << doc["type"];
    std::string str_node_type {stream.str()};
    NodeType node_type {NodeType::NULLTYPE};
    if (std::string::npos != str_node_type.find("\"performance\""))
    {
        node_type = NodeType::PERFORMANCE;
    }
    else if (std::string::npos != str_node_type.find("\"latency\""))
    {
        node_type = NodeType::LATENCY;
    }
    else if (std::string::npos != str_node_type.find("\"queue\""))
    {
        node_type = NodeType::QUEUE;
    }
    else if (std::string::npos != str_node_type.find("\"utilization\""))
    {
        node_type = NodeType::UTILIZATION;
    }
    else if (std::string::npos != str_node_type.find("\"count\""))
    {
        node_type = NodeType::COUNT;
    }
    else if (std::string::npos != str_node_type.find("\"histogram\""))
    {
        node_type = NodeType::HISTOGRAM;
    }

    sum_perf_period_iops = 0;
    sum_perf_period_bw = 0;
    sum_perf_cumulation_iops = 0;
    sum_perf_cumulation_bw = 0;
    sum_util_period_usage = 0;
    sum_util_cumulation_usage = 0;
    for (auto obj : air::range(doc["objs"]))
    {
        _SumData(doc["objs"][obj.key], node_type);
    }

    if (NodeType::PERFORMANCE == node_type)
    {
        std::cout << "        SUM Period(iops:"
                  << NumberToStrSIFmt(sum_perf_period_iops)
                  << ", bw:" << NumberToStrSIFmt(sum_perf_period_bw, "B") << "), "
                  << "Cumulation(iops:"
                  << NumberToStrSIFmt(sum_perf_cumulation_iops)
                  << ", bw:" << NumberToStrSIFmt(sum_perf_cumulation_bw, "B") << ")"
                  << std::endl;
        curr_row++;
        if (ws_row <= curr_row)
        {
            return;
        }
    }

    for (auto obj : air::range(doc["objs"]))
    {
        _DrawObj(doc["objs"][obj.key], node_type);
        if (ws_row <= curr_row)
        {
            return;
        }
    }
}

void
air::Viewer::_SumData(JSONdoc& doc, NodeType type)
{
    if (NodeType::PERFORMANCE == type)
    {
        std::string str;
        std::stringstream stream;
        uint64_t value;

        stream.str("");
        stream << doc["period"]["iops"];
        value = std::stoull(stream.str());
        sum_perf_period_iops += value;

        stream.str("");
        stream << doc["period"]["bw"];
        value = std::stoull(stream.str());
        sum_perf_period_bw += value;

        stream.str("");
        stream << doc["cumulation"]["iops"];
        value = std::stoull(stream.str());
        sum_perf_cumulation_iops += value;

        stream.str("");
        stream << doc["cumulation"]["bw"];
        value = std::stoull(stream.str());
        sum_perf_cumulation_bw += value;
    }
    else if (NodeType::UTILIZATION == type)
    {
        std::string str;
        std::stringstream stream;
        uint64_t value;

        stream.str("");
        stream << doc["period"]["usage"];
        value = std::stoull(stream.str());
        sum_util_period_usage += value;

        stream.str("");
        stream << doc["cumulation"]["usage"];
        value = std::stoull(stream.str());
        sum_util_cumulation_usage += value;
    }
}

void
air::Viewer::_DrawObj(JSONdoc& doc, NodeType type)
{
    uint32_t remain_col = _DrawDefault(doc);
    if (0 == remain_col)
    {
        std::cout << std::endl;
        curr_row++;
        return;
    }

    switch (type)
    {
        case NodeType::PERFORMANCE:
        {
            _DrawPerf(doc, remain_col);
            break;
        }
        case NodeType::LATENCY:
        {
            _DrawLat(doc, remain_col);
            break;
        }
        case NodeType::QUEUE:
        {
            _DrawQueue(doc, remain_col);
            break;
        }
        case NodeType::UTILIZATION:
        {
            _DrawUtilization(doc, remain_col);
            break;
        }
        case NodeType::COUNT:
        {
            _DrawCount(doc, remain_col);
            break;
        }
        case NodeType::HISTOGRAM:
        {
            _DrawHistogram(doc, remain_col);
            break;
        }
        default:
        {
            break;
        }
    }

    std::cout << std::endl;
    curr_row++;
}

uint32_t
air::Viewer::_DrawDefault(JSONdoc& doc)
{
    std::string str = "      ";
    std::stringstream stream;
    uint64_t number;

    stream.str("");
    stream << doc["target_name"];
    str += stream.str();
    str += "(";

    stream.str("");
    stream << doc["target_id"];
    str += stream.str();
    str += "), index:0x";

    stream.str("");
    stream << doc["index"];
    number = std::stoull(stream.str());
    stream.str("");
    stream << std::hex << number << std::dec << ", filter:";
    str += stream.str();

    stream.str("");
    stream << doc["filter"];
    str += stream.str();

    if (ws_col > str.size())
    {
        std::cout << str;
        return ws_col - str.size();
    }
    else
    {
        std::cout << str.substr(0, str.size());
        return 0;
    }
}

void
air::Viewer::_DrawPerf(JSONdoc& doc, uint32_t remain_col)
{
    std::string str;
    std::stringstream stream;
    double number;
    JSONdoc& doc_period {doc["period"]};
    JSONdoc& doc_cumulation {doc["cumulation"]};
    // JSONdoc& doc_count {doc_period["count"]};

    str = " Period(iops:";

    stream.str("");
    stream << doc_period["iops"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch (...)
    {
        str += "INV    ";
    }
    str += ", bw:";

    stream.str("");
    stream << doc_period["bw"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number, "B");
    }
    catch (...)
    {
        str += "INV     ";
    }

    str += ", count:";
    stream.str("");
    stream << doc_period["count"];
    str += stream.str();

    str += "), Cumulation(iops:";
    stream.str("");
    stream << doc_cumulation["iops"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch (...)
    {
        str += "INV    ";
    }
    str += ", bw:";

    stream.str("");
    stream << doc_cumulation["bw"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number, "B");
    }
    catch (...)
    {
        str += "INV     ";
    }
    str += ")";

    std::cout << str.substr(0, remain_col);
}

void
air::Viewer::_DrawLat(JSONdoc& doc, uint32_t remain_col)
{
    std::string str;
    std::stringstream stream;
    uint64_t number;
    JSONdoc& doc_period {doc["period"]};
    JSONdoc& doc_cumulation {doc["cumulation"]};

    str = " Period(avg:";

    stream.str("");
    stream << doc_period["mean"];
    try
    {
        number = std::stoull(stream.str());
        str += ULLToStrLatFmt(number);
    }
    catch (...)
    {
        str += "INV  ";
    }
    str += ", median:";

    stream.str("");
    stream << doc_period["median"];
    try
    {
        number = std::stoull(stream.str());
        str += ULLToStrLatFmt(number);
    }
    catch (...)
    {
        str += "INV  ";
    }
    str += ", max:";

    stream.str("");
    stream << doc_period["max"];
    try
    {
        number = std::stoull(stream.str());
        str += ULLToStrLatFmt(number);
    }
    catch (...)
    {
        str += "INV  ";
    }
    str += ", sample:";

    stream.str("");
    stream << doc_period["sample_cnt"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch (...)
    {
        str += "INV    ";
    }
    str += "), Cumulation(avg:";

    stream.str("");
    stream << doc_cumulation["mean"];
    try
    {
        number = std::stoull(stream.str());
        str += ULLToStrLatFmt(number);
    }
    catch (...)
    {
        str += "INV  ";
    }
    str += ", median:";

    stream.str("");
    stream << doc_cumulation["median"];
    try
    {
        number = std::stoull(stream.str());
        str += ULLToStrLatFmt(number);
    }
    catch (...)
    {
        str += "INV  ";
    }
    str += ", max:";

    stream.str("");
    stream << doc_cumulation["max"];
    try
    {
        number = std::stoull(stream.str());
        str += ULLToStrLatFmt(number);
    }
    catch (...)
    {
        str += "INV  ";
    }
    str += ", sample:";

    stream.str("");
    stream << doc_cumulation["sample_cnt"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch (...)
    {
        str += "INV    ";
    }
    str += ")";

    std::cout << str.substr(0, remain_col);
}

void
air::Viewer::_DrawQueue(JSONdoc& doc, uint32_t remain_col)
{
    std::string str;
    std::stringstream stream;
    double number;
    JSONdoc& doc_period {doc["period"]};
    JSONdoc& doc_cumulation {doc["cumulation"]};

    str = " Period(avg:";

    stream.str("");
    stream << doc_period["qd_avg"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch (...)
    {
        str += "INV    ";
    }
    str += ", max:";

    stream.str("");
    stream << doc_period["qd_max"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch (...)
    {
        str += "INV    ";
    }
    str += "), Cumulation(avg:";

    stream.str("");
    stream << doc_cumulation["qd_avg"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch (...)
    {
        str += "INV    ";
    }
    str += ", max:";

    stream.str("");
    stream << doc_cumulation["qd_max"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch (...)
    {
        str += "INV    ";
    }
    str += ")";

    std::cout << str.substr(0, remain_col);
}

void
air::Viewer::_DrawUtilization(JSONdoc& doc, uint32_t remain_col)
{
    std::string str;
    std::stringstream stream;
    uint64_t number {0};
    JSONdoc& doc_period {doc["period"]};
    JSONdoc& doc_cumulation {doc["cumulation"]};

    str = " Period(usage:";

    stream.str("");
    stream << doc_period["usage"];
    try
    {
        number = std::stoull(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch (...)
    {
        str += "INV    ";
    }
    str += ", ";

    stream.str("");
    stream << std::fixed << std::setprecision(2);
    if (0 != sum_util_period_usage)
    {
        stream << (((double)number / sum_util_period_usage) * 100.0f);
    }
    str += stream.str();
    str += "%), Cumulation(usage:";

    stream.str("");
    stream << doc_cumulation["usage"];
    try
    {
        number = std::stoull(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch (...)
    {
        str += "INV    ";
    }
    str += ", ";

    stream.str("");
    stream << std::fixed << std::setprecision(2);
    if (0 != sum_util_cumulation_usage)
    {
        stream << (((double)number / sum_util_cumulation_usage) * 100.0f);
    }
    str += stream.str();
    str += "%)";

    std::cout << str.substr(0, remain_col);
}

void
air::Viewer::_DrawCount(JSONdoc& doc, uint32_t remain_col)
{
    std::string str;
    std::stringstream stream;
    int64_t number;
    JSONdoc& doc_period {doc["period"]};
    JSONdoc& doc_cumulation {doc["cumulation"]};

    str = " Period(count:";

    stream.str("");
    stream << doc_period["count"];
    try
    {
        number = std::stoll(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch (...)
    {
        str += "INV    ";
    }
    str += "), Cumulation(count:";

    stream.str("");
    stream << doc_cumulation["count"];
    try
    {
        number = std::stoll(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch (...)
    {
        str += "INV    ";
    }
    str += ")";

    std::cout << str.substr(0, remain_col);
}

void
air::Viewer::_DrawHistogram(JSONdoc& doc, uint32_t remain_col)
{
    curr_row++;
    if (ws_row <= curr_row)
    {
        return;
    }
    std::cout << ", bucket_type:" << doc["bucket_type"] << std::endl;
    _DrawHistogramUnit(doc, ws_col);
    _DrawHistogramPeriod(doc["period"], ws_col);
    _DrawHistogramCumulation(doc["cumulation"], ws_col);
}

void
air::Viewer::_DrawHistogramUnit(JSONdoc& doc, uint32_t remain_col)
{
    curr_row++;
    if (ws_row <= curr_row)
    {
        return;
    }
    std::string str {
        "        Unit    value:(minimum/average/maximum) count:(underflow "};
    std::stringstream stream;
    uint64_t bucket_size;
    int64_t bucket_lower_bound;
    int64_t bucket_upper_bound;
    int64_t bucket_scale;
    int32_t bucket_zero_index;

    stream.str("");
    stream << doc["bucket_size"];
    bucket_size = std::stoull(stream.str());
    stream.str("");
    stream << doc["bucket_lower_bound"];
    bucket_lower_bound = std::stoll(stream.str());
    stream.str("");
    stream << doc["bucket_upper_bound"];
    bucket_upper_bound = std::stoll(stream.str());
    stream.str("");
    stream << doc["bucket_scale"];
    bucket_scale = std::stoll(stream.str());
    stream.str("");
    stream << doc["bucket_zero_index"];
    bucket_zero_index = std::stoi(stream.str());

    stream.str("");
    stream << doc["bucket_type"];
    if (0 == stream.str().compare("\"linear\""))
    {
        for (uint64_t bucket_index {0}; bucket_index <= bucket_size; bucket_index++)
        {
            int64_t bucket_boundary {
                bucket_lower_bound + (int64_t)bucket_index * bucket_scale};
            str += NumberToStrSIFmt(bucket_boundary, " ");
        }
    }
    else
    {
        if (0 < bucket_lower_bound)
        {
            for (uint64_t bucket_index {0}; bucket_index <= bucket_size;
                 bucket_index++)
            {
                int64_t bucket_boundary {1};
                int32_t exponent {(int32_t)bucket_index - bucket_zero_index - 1};
                while (exponent)
                {
                    bucket_boundary *= bucket_scale;
                    exponent--;
                }
                str += NumberToStrSIFmt(bucket_boundary, " ");
            }
        }
        else if (0 > bucket_upper_bound)
        {
            for (uint64_t bucket_index {0}; bucket_index <= bucket_size;
                 bucket_index++)
            {
                int64_t bucket_boundary {1};
                int32_t exponent {bucket_zero_index - (int32_t)bucket_index};
                while (exponent)
                {
                    bucket_boundary *= bucket_scale;
                    exponent--;
                }
                bucket_boundary *= -1;
                str += NumberToStrSIFmt(bucket_boundary, " ");
            }
        }
        else
        {
            for (uint64_t bucket_index {0}; bucket_index < bucket_size;
                 bucket_index++)
            {
                int64_t bucket_boundary {1};
                if (bucket_zero_index > (int32_t)bucket_index)
                {
                    int32_t exponent {bucket_zero_index - (int32_t)bucket_index};
                    while (exponent)
                    {
                        bucket_boundary *= bucket_scale;
                        exponent--;
                    }
                    bucket_boundary *= -1;
                    str += NumberToStrSIFmt(bucket_boundary, " ");
                }
                else if (bucket_zero_index < (int32_t)bucket_index)
                {
                    int32_t exponent {(int32_t)bucket_index - bucket_zero_index};
                    while (exponent)
                    {
                        bucket_boundary *= bucket_scale;
                        exponent--;
                    }
                    str += NumberToStrSIFmt(bucket_boundary, " ");
                }
                else
                {
                    str += "-1  0   1       ";
                }
            }
        }
    }

    str += "overflow)";

    std::cout << str.substr(0, remain_col) << std::endl;
}

void
air::Viewer::_DrawHistogramPeriod(JSONdoc& doc, uint32_t remain_col)
{
    curr_row++;
    if (ws_row <= curr_row)
    {
        return;
    }
    std::string str {"        Period         "};
    std::stringstream stream;
    int64_t number_int64;
    uint64_t number_uint64;

    stream.str("");
    stream << doc["minimum_value"];
    try
    {
        number_int64 = std::stoll(stream.str());
        str += NumberToStrSIFmt(number_int64, " ");
    }
    catch (...)
    {
        str += "     INV";
    }

    stream.str("");
    stream << doc["average_value"];
    try
    {
        number_int64 = std::stoll(stream.str());
        str += NumberToStrSIFmt(number_int64, " ");
    }
    catch (...)
    {
        str += "     INV";
    }

    stream.str("");
    stream << doc["maximum_value"];
    try
    {
        number_int64 = std::stoll(stream.str());
        str += NumberToStrSIFmt(number_int64, " ");
    }
    catch (...)
    {
        str += "     INV";
    }

    str += "        ";
    stream.str("");
    stream << doc["underflow_count"];
    try
    {
        number_uint64 = std::stoull(stream.str());
        str += NumberToStrSIFmt(number_uint64, " ");
    }
    catch (...)
    {
        str += "     INV";
    }

    str += "   |";
    JSONdoc& buckets {doc["buckets"]};
    for (auto bucket : air::range(buckets))
    {
        uint64_t count {*static_cast<uint64_t*>(bucket.value)};
        str += NumberToStrSIFmt(count, "|", 7);
    }

    str += "      ";
    stream.str("");
    stream << doc["overflow_count"];
    try
    {
        number_uint64 = std::stoull(stream.str());
        str += NumberToStrSIFmt(number_uint64, " ");
    }
    catch (...)
    {
        str += "     INV";
    }

    std::cout << str.substr(0, remain_col) << std::endl;
}

void
air::Viewer::_DrawHistogramCumulation(JSONdoc& doc, uint32_t remain_col)
{
    std::string str {"        Cumulation     "};
    std::stringstream stream;
    int64_t number_int64;
    uint64_t number_uint64;

    stream.str("");
    stream << doc["minimum_value"];
    try
    {
        number_int64 = std::stoll(stream.str());
        str += NumberToStrSIFmt(number_int64, " ");
    }
    catch (...)
    {
        str += "     INV";
    }

    stream.str("");
    stream << doc["average_value"];
    try
    {
        number_int64 = std::stoll(stream.str());
        str += NumberToStrSIFmt(number_int64, " ");
    }
    catch (...)
    {
        str += "     INV";
    }

    stream.str("");
    stream << doc["maximum_value"];
    try
    {
        number_int64 = std::stoll(stream.str());
        str += NumberToStrSIFmt(number_int64, " ");
    }
    catch (...)
    {
        str += "     INV";
    }

    str += "        ";
    stream.str("");
    stream << doc["underflow_count"];
    try
    {
        number_uint64 = std::stoull(stream.str());
        str += NumberToStrSIFmt(number_uint64, " ");
    }
    catch (...)
    {
        str += "     INV";
    }

    str += "   |";
    JSONdoc& buckets {doc["buckets"]};
    for (auto bucket : air::range(buckets))
    {
        uint64_t count {*static_cast<uint64_t*>(bucket.value)};
        str += NumberToStrSIFmt(count, "|", 7);
    }

    str += "      ";
    stream.str("");
    stream << doc["overflow_count"];
    try
    {
        number_uint64 = std::stoull(stream.str());
        str += NumberToStrSIFmt(number_uint64, " ");
    }
    catch (...)
    {
        str += "     INV";
    }

    std::cout << str.substr(0, remain_col);
}
