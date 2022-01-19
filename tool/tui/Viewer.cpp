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

void
air::Viewer::Render(EventData data, AConfig& tree, int pid)
{
    _SetFilename(pid);

    if (_Update(data.type))
    {
        _ClearWindow();
        _Draw(tree);
    }
}

void
air::Viewer::_SetFilename(int new_pid)
{
    pid = new_pid;
    filename = "/tmp/air_";
    time_t curr_time = time(NULL);
    tm* curr_tm = localtime(&curr_time);
    filename += std::to_string(curr_tm->tm_year + 1900);
    if (curr_tm->tm_mon + 1 < 10)
    {
        filename += "0";
    }
    filename += std::to_string(curr_tm->tm_mon + 1);
    if (curr_tm->tm_mday < 10)
    {
        filename += "0";
    }
    filename += std::to_string(curr_tm->tm_mday);
    filename += "_" + std::to_string(pid) + ".json";
}

bool
air::Viewer::_Update(EventType type)
{
    if (_CheckMovement(type))
    {
        return true;
    }

    if (_CheckFilesize())
    {
        return true;
    }

    return false;
}

bool
air::Viewer::_CheckMovement(EventType type)
{
    bool result{false};

    switch (type)
    {
        case air::EventType::MOVE_UP:
        case air::EventType::MOVE_DOWN:
        case air::EventType::MOVE_RIGHT:
        case air::EventType::MOVE_LEFT:
        {
            result = true;
            break;
        }
        default:
        {
            break;
        }
    }

    return result;
}

bool
air::Viewer::_CheckFilesize(void)
{
    bool result{false};
    struct stat file_stat;
    int rc = stat(filename.c_str(), &file_stat);
    if (0 == rc)
    {
        if (file_stat.st_size > file_size)
        {
            if (false == file_update && printed_pos < file_size)
            {
                prev_file_size = file_size;
                file_update = true;
            }
            file_size = file_stat.st_size;
        }
        else if (file_stat.st_size == file_size)
        {
            if (file_update)
            {
                result = true;
                file_update = false;
            }
        }
        else
        {
            result = false;
        }
    }
    else
    {
        result = false;
    }

    return result;
}

void
air::Viewer::_ClearWindow(void)
{
    int result{0};
    result = system("clear");

    if (-1 == result)
    {
        std::cout << "clear window failed" << std::endl;
    }
}

void
air::Viewer::_Draw(AConfig& tree)
{
    std::ifstream file;
    std::string str_json;
    file.open(filename, std::ifstream::in | std::ifstream::ate);
    file.seekg(prev_file_size, file.beg);
    std::getline(file, str_json);
    file.close();
    printed_pos = prev_file_size + str_json.size() - 1;

    struct winsize ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
    ws_row = ws.ws_row;
    ws_col = ws.ws_col;
    curr_row = 1;

    air::parse("data", str_json);
    try
    {
        _DrawHeadline();
        _DrawGroup(tree);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    air::json_clear();
}

void
air::Viewer::_DrawHeadline(void)
{
    auto& head = air::json("data");
    std::stringstream stream;

    std::cout << "AIR TUI status: [";
    stream << head["play"];
    std::string status{stream.str()};
    if (std::string::npos != status.find("true"))
    {
        std::cout << GREEN << "play" << RESET;
    }
    else
    {
        std::cout << RED << "pause" << RESET;
    }

    std::cout << "],  interval: [";
    std::cout << GREEN << head["interval"] << RESET;
    std::cout << "],  ";

    std::cout << "timestamp: ";
    stream.str("");
    stream << head["timestamp"];
    std::string timestamp{stream.str()};

    time_t time;
    struct tm* tm;
    time = std::stoi(timestamp);
    tm = localtime(&time);
    std::cout << tm->tm_year + 1900 << "-" << tm->tm_mon + 1 << "-" << tm->tm_mday
              << ":" << tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec;

    std::cout << ", pid: " << pid << std::endl;
    curr_row++;

    std::string tip = "arrow up/down: movement, arrow right/left: unfold/fold, q(esc): quit, 1~9: interval, i: init, o: run, x: stop";

    std::cout << tip.substr(0, ws_col) << std::endl;
    curr_row++;
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
            std::string gid{stream.str()};
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
                std::string nid{stream.str()};
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
    std::string str_build{stream.str()};
    if (std::string::npos != str_build.find("false"))
    {
        std::cout << "[.]..Node:" << name << std::endl;
        curr_row++;
        return;
    }

    stream.str("");
    stream << doc["run"];
    std::string str_run{stream.str()};
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
    std::string str_node_type{stream.str()};
    NodeType node_type{NodeType::NULLTYPE};
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

    sum_iops = 0;
    sum_bw = 0;
    sum_util_usage = 0;
    sum_util_total_usage = 0;
    for (auto obj : air::range(doc["objs"]))
    {
        _SumData(doc["objs"][obj.key], node_type);
    }

    if (NodeType::PERFORMANCE == node_type)
    {
        std::cout << "        SUM_Period(iops:" << NumberToStrSIFmt(sum_iops)
                  << ", bw:" << NumberToStrSIFmt(sum_bw, "B") << ")" << std::endl;
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
        stream << doc["iops"];
        value = std::stoull(stream.str());
        sum_iops += value;

        stream.str("");
        stream << doc["bw"];
        value = std::stoull(stream.str());
        sum_bw += value;
    }
    else if (NodeType::UTILIZATION == type)
    {
        std::string str;
        std::stringstream stream;
        uint64_t value;

        stream.str("");
        stream << doc["usage"];
        value = std::stoull(stream.str());
        sum_util_usage += value;

        stream.str("");
        stream << doc["total_usage"];
        value = std::stoull(stream.str());
        sum_util_total_usage += value;
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

    str = " Period(iops:";

    stream.str("");
    stream << doc["iops"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch(...)
    {
        str += "INV    ";
    }
    str += ", bw:";

    stream.str("");
    stream << doc["bw"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number, "B");
    }
    catch(...)
    {
        str += "INV     ";
    }
    str += ", ";

    for (uint32_t index = 1; index <= 10; index++)
    {
        if (doc.HasKey("cnt_" + std::to_string(index)))
        {
            if (1 != index)
            {
                str += ", ";
            }
            stream.str("");
            stream << doc["cnt_" + std::to_string(index)];
            str += stream.str();
        }
    }
    str += "), Total(iops_avg:";

    stream.str("");
    stream << doc["iops_avg"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch(...)
    {
        str += "INV    ";
    }
    str += ", bw_avg:";

    stream.str("");
    stream << doc["bw_avg"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number, "B");
    }
    catch(...)
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

    str = " Period(avg:";

    stream.str("");
    stream << doc["mean"];
    try
    {
        number = std::stoull(stream.str());
        str += ULLToStrLatFmt(number);
    }
    catch(...)
    {
        str += "INV  ";
    }
    str += ", median:";

    stream.str("");
    stream << doc["median"];
    try
    {
        number = std::stoull(stream.str());
        str += ULLToStrLatFmt(number);
    }
    catch(...)
    {
        str += "INV  ";
    }
    str += ", max:";

    stream.str("");
    stream << doc["max"];
    try
    {
        number = std::stoull(stream.str());
        str += ULLToStrLatFmt(number);
    }
    catch(...)
    {
        str += "INV  ";
    }
    str += ", sample:";

    stream.str("");
    stream << doc["sample_cnt"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch(...)
    {
        str += "INV    ";
    }
    str += "), Total(avg:";

    stream.str("");
    stream << doc["total_mean"];
    try
    {
        number = std::stoull(stream.str());
        str += ULLToStrLatFmt(number);
    }
    catch(...)
    {
        str += "INV  ";
    }
    str += ", median:";

    stream.str("");
    stream << doc["total_median"];
    try
    {
        number = std::stoull(stream.str());
        str += ULLToStrLatFmt(number);
    }
    catch(...)
    {
        str += "INV  ";
    }
    str += ", max:";

    stream.str("");
    stream << doc["total_max"];
    try
    {
        number = std::stoull(stream.str());
        str += ULLToStrLatFmt(number);
    }
    catch(...)
    {
        str += "INV  ";
    }
    str += ", sample:";

    stream.str("");
    stream << doc["total_sample_cnt"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch(...)
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

    str = " Period(avg:";

    stream.str("");
    stream << doc["depth_period_avg"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch(...)
    {
        str += "INV    ";
    }
    str += ", max:";

    stream.str("");
    stream << doc["depth_period_max"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch(...)
    {
        str += "INV    ";
    }
    str += "), Total(avg:";

    stream.str("");
    stream << doc["depth_total_avg"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch(...)
    {
        str += "INV    ";
    }
    str += ", max:";

    stream.str("");
    stream << doc["depth_total_max"];
    try
    {
        number = std::stod(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch(...)
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

    str = " Period(usage:";

    stream.str("");
    stream << doc["usage"];
    try
    {
        number = std::stoull(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch(...)
    {
        str += "INV    ";
    }
    str += ", ";

    stream.str("");
    stream << std::fixed << std::setprecision(2);
    if (0 != sum_util_usage)
    {
        stream << (((double)number / sum_util_usage) * 100.0f);
    }
    str += stream.str();
    str += "%), Total(usage:";

    stream.str("");
    stream << doc["total_usage"];
    try
    {
        number = std::stoull(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch(...)
    {
        str += "INV    ";
    }
    str += ", ";

    stream.str("");
    stream << std::fixed << std::setprecision(2);
    if (0 != sum_util_total_usage)
    {
        stream << (((double)number / sum_util_total_usage) * 100.0f);
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

    str = " Period(count:";

    stream.str("");
    stream << doc["count"];
    try
    {
        number = std::stoll(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch(...)
    {
        str += "INV    ";
    }
    str += "), Total(count:";

    stream.str("");
    stream << doc["total_count"];
    try
    {
        number = std::stoll(stream.str());
        str += NumberToStrSIFmt(number);
    }
    catch(...)
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
    std::string str {"        Unit    value:(minimum/average/maximum) count:(underflow "};
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
    if(0 == stream.str().compare("\"linear\""))
    {
        for (uint64_t bucket_index {0}; bucket_index <= bucket_size; bucket_index++)
        {
            int64_t bucket_boundary {bucket_lower_bound + (int64_t)bucket_index * bucket_scale};
            str += NumberToStrSIFmt(bucket_boundary, " ");
        }
    }
    else
    {
        if (0 < bucket_lower_bound)
        {
            for (uint64_t bucket_index {0}; bucket_index <= bucket_size; bucket_index++)
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
            for (uint64_t bucket_index {0}; bucket_index <= bucket_size; bucket_index++)
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
            for (uint64_t bucket_index {0}; bucket_index < bucket_size; bucket_index++)
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
    catch(...)
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
    catch(...)
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
    catch(...)
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
    catch(...)
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
    catch(...)
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
    catch(...)
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
    catch(...)
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
    catch(...)
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
    catch(...)
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
    catch(...)
    {
        str += "     INV";
    }

    std::cout << str.substr(0, remain_col);
}
