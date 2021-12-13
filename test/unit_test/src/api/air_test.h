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

#include "fake_collection_manager.h"
#include "fake_instance_manager.h"
#include "fake_node_manager.h"
#include "fake_node_data_array.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/config/ConfigInterface.h"

class MockAIR : public AIR<true, true>
{
public:
    MockAIR()
    : AIR()
    {
    }
    void
    MockInit()
    {
        fake_collection_manager = new FakeCollectionManager {nullptr, nullptr, nullptr, nullptr};
        AIR::collection_manager = fake_collection_manager;

        fake_instance_manager = new FakeInstanceManager {};
        AIR::instance_manager = fake_instance_manager;

        fake_node_manager = new FakeNodeManager {nullptr, nullptr};
        AIR::node_manager = fake_node_manager;

        AIR::node_data_array = &fake_node_data_array;
    }
    static FakeCollectionManager* fake_collection_manager;
    static FakeInstanceManager* fake_instance_manager;
    static FakeNodeManager* fake_node_manager;
    FakeNodeDataArray fake_node_data_array;
};

class TestAPI : public ::testing::Test
{
public:
    MockAIR* mock_air {nullptr};

protected:
    TestAPI()
    {
        mock_air = new MockAIR {};
    }
    ~TestAPI() override
    {
        if (nullptr != mock_air)
        {
            delete mock_air;
            mock_air = nullptr;
        }
    }
    void
    SetUp() override
    {
        mock_air->MockInit();
    }
    void
    TearDown() override
    {
    }
};
