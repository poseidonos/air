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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdio.h>

#include "air_test.h"
#include "src/config/ConfigInterface.h"

air::InstanceManager* AIR<true, true>::instance_manager = nullptr;
node::NodeManager* AIR<true, true>::node_manager = nullptr;
collection::CollectionManager* AIR<true, true>::collection_manager = nullptr;
thread_local node::NodeDataArray* AIR<true, true>::node_data_array = nullptr;
collection::PerformanceWriter AIR<true, true>::perf_writer;
collection::LatencyWriter AIR<true, true>::lat_writer;
collection::QueueWriter AIR<true, true>::queue_writer;
collection::UtilizationWriter AIR<true, true>::util_writer;
collection::CountWriter AIR<true, true>::count_writer;

FakeCollectionManager* MockAIR::fake_collection_manager = nullptr;
FakeInstanceManager* MockAIR::fake_instance_manager = nullptr;
FakeNodeManager* MockAIR::fake_node_manager = nullptr;

enum IOtype
{
    AIR_READ,
    AIR_WRITE,
};

TEST_F(TestAPI, UT_AIR_Initialize)
{
    mock_air->Initialize();
}

TEST_F(TestAPI, UT_AIR_Activate)
{
    mock_air->Activate();
}

TEST_F(TestAPI, UT_AIR_Deactivate)
{
    mock_air->Deactivate();
}

TEST_F(TestAPI, UT_AIR_Finalize)
{
    mock_air->Finalize();
}

TEST_F(TestAPI, UT_AIR_LogPerf)
{
    mock_air->LogData<cfg::GetSentenceIndex(config::ParagraphType::NODE, "PERF_PSD"),
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item",
            cfg::GetStrValue(config::ParagraphType::NODE, "Filter", "PERF_PSD"), "BI_0"),
        cfg::GetNodeType("PERF_PSD")>(0, 4096);

    mock_air->LogData<cfg::GetSentenceIndex(config::ParagraphType::NODE, "PERF_PSD"),
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item",
            cfg::GetStrValue(config::ParagraphType::NODE, "Filter", "PERF_PSD"), "BI_0"),
        cfg::GetNodeType("PERF_PSD")>(0, 4096);
}

TEST_F(TestAPI, UT_AIR_LogLat)
{
    mock_air->LogData<cfg::GetSentenceIndex(config::ParagraphType::NODE, "LAT_SUBMIT"),
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item",
            cfg::GetStrValue(config::ParagraphType::NODE, "Filter", "LAT_SUBMIT"), "BI_0"),
        cfg::GetNodeType("LAT_SUBMIT")>(0, 123);

    mock_air->LogData<cfg::GetSentenceIndex(config::ParagraphType::NODE, "LAT_SUBMIT"),
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item",
            cfg::GetStrValue(config::ParagraphType::NODE, "Filter", "LAT_SUBMIT"), "BI_0"),
        cfg::GetNodeType("LAT_SUBMIT")>(0, 123);

    mock_air->LogData<cfg::GetSentenceIndex(config::ParagraphType::NODE, "LAT_SUBMIT"),
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item",
            cfg::GetStrValue(config::ParagraphType::NODE, "Filter", "LAT_SUBMIT"), "BI_0"),
        cfg::GetNodeType("LAT_SUBMIT")>(0, 123);
}

TEST_F(TestAPI, UT_AIR_LogQ)
{
    mock_air->LogData<cfg::GetSentenceIndex(config::ParagraphType::NODE, "Q_SUBMIT"),
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item",
            cfg::GetStrValue(config::ParagraphType::NODE, "Filter", "Q_SUBMIT"), "BI_0"),
        cfg::GetNodeType("Q_SUBMIT")>(0, 128);

    mock_air->LogData<cfg::GetSentenceIndex(config::ParagraphType::NODE, "Q_SUBMIT"),
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item",
            cfg::GetStrValue(config::ParagraphType::NODE, "Filter", "Q_SUBMIT"), "BI_0"),
        cfg::GetNodeType("Q_SUBMIT")>(0, 128);

    mock_air->LogData<cfg::GetSentenceIndex(config::ParagraphType::NODE, "Q_SUBMIT"),
        cfg::GetIntValue(config::ParagraphType::FILTER, "Item",
            cfg::GetStrValue(config::ParagraphType::NODE, "Filter", "Q_SUBMIT"), "BI_0"),
        cfg::GetNodeType("Q_SUBMIT")>(0, 128);
}

int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
