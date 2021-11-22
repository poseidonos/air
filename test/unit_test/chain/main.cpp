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

#include "chain_manager_test.h"
#include "mock_cor_handler.h"

TEST_F(ChainManagerTest, Init)
{
    uint32_t value;
    chain::AnalysisTask* anlysis_task = chain_manager->GetAnalysisTask();
    value = 1000;
    EXPECT_EQ(anlysis_task->GetPeriod(), value); // default

    chain_manager->Init(); // mock streaming intverval -> 3
    value = 3000;
    EXPECT_EQ(anlysis_task->GetPeriod(), value);
}

TEST_F(ChainManagerTest, RunThread)
{
    /*
    <Period Default>
    switch gear     10ms
    preprocess     100ms
    cli            100ms
    analysis       1000ms
    */
    chain::SwitchGearTask* switch_gear_task = chain_manager->GetSwitchGearTask();
    chain::PreprocessTask* preprocess_task = chain_manager->GetPreprocessTask();
    chain::CLITask* cli_task = chain_manager->GetCLITask();
    chain::AnalysisTask* anlysis_task = chain_manager->GetAnalysisTask();

    // make fake chains
    MockCoRHandler* mock_cor_handler = new MockCoRHandler();
    chain_manager->AttachChain(mock_cor_handler,
        to_dtype(pi::ChainHandler::INPUT));
    chain_manager->AttachChain(mock_cor_handler,
        to_dtype(pi::ChainHandler::POLICY));
    chain_manager->AttachChain(mock_cor_handler,
        to_dtype(pi::ChainHandler::COLLECTION));
    chain_manager->AttachChain(mock_cor_handler,
        to_dtype(pi::ChainHandler::OUTPUT));
    chain_manager->AttachChain(mock_cor_handler,
        to_dtype(pi::ChainHandler::PROCESS));
    chain_manager->AttachChain(mock_cor_handler,
        to_dtype(pi::ChainHandler::STREAM));
    chain_manager->AttachChain(mock_cor_handler,
        to_dtype(pi::ChainHandler::SWITCHGEAR));
    chain_manager->AttachChain(mock_cor_handler,
        to_dtype(pi::ChainHandler::PREPROCESS));
    chain_manager->AttachChain(mock_cor_handler,
        to_dtype(pi::ChainHandler::DETECT));
    chain_manager->AttachChain(mock_cor_handler,
        to_dtype(pi::ChainHandler::TRANSFER));

    // after 10ms (total: 10ms), trigger switch gear
    chain_manager->RunThread(0);
    EXPECT_EQ(switch_gear_task->GetDeadline(), (uint32_t)40);
    EXPECT_EQ(preprocess_task->GetDeadline(), (uint32_t)90);
    EXPECT_EQ(cli_task->GetDeadline(), (uint32_t)90);
    EXPECT_EQ(anlysis_task->GetDeadline(), (uint32_t)990);

    // after 90ms(10ms(tick)+80ms(delay)) (total: 100ms), trigger switch gear, preprocess, cli
    chain_manager->RunThread(8);
    EXPECT_EQ(switch_gear_task->GetDeadline(), (uint32_t)50);
    EXPECT_EQ(preprocess_task->GetDeadline(), (uint32_t)100);
    EXPECT_EQ(cli_task->GetDeadline(), (uint32_t)100);
    EXPECT_EQ(anlysis_task->GetDeadline(), (uint32_t)900);

    // after 900ms (10ms(tick)+890ms(delay)) (total: 1000ms), trigger all
    chain_manager->RunThread(89);
    EXPECT_EQ(switch_gear_task->GetDeadline(), (uint32_t)50);
    EXPECT_EQ(preprocess_task->GetDeadline(), (uint32_t)100);
    EXPECT_EQ(cli_task->GetDeadline(), (uint32_t)100);
    EXPECT_EQ(anlysis_task->GetDeadline(), (uint32_t)3000); // after trigger, the period is updated
}

int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
