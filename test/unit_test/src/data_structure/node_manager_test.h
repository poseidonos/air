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

#include "mock_global_meta.h"
#include "mock_node_meta.h"
#include "src/data_structure/NodeInitializer.cpp"
#include "src/data_structure/NodeInitializer.h"
#include "src/data_structure/NodeManager.cpp"
#include "src/data_structure/NodeManager.h"

class NodeManagerTest : public ::testing::Test
{
public:
    node::NodeManager* node_manager {nullptr};
    MockGlobalMetaGetter* mock_gloal_meta_getter {nullptr};
    MockNodeMetaGetter* mock_node_meta_getter {nullptr};

protected:
    NodeManagerTest()
    {
        mock_gloal_meta_getter = new MockGlobalMetaGetter {};
        mock_node_meta_getter = new MockNodeMetaGetter {};
        node_manager =
            new node::NodeManager {mock_gloal_meta_getter, mock_node_meta_getter};
        node_manager->Init();
    }
    ~NodeManagerTest() override
    {
        if (nullptr != node_manager)
        {
            delete node_manager;
            node_manager = nullptr;
        }
        if (nullptr != mock_gloal_meta_getter)
        {
            delete mock_gloal_meta_getter;
            mock_gloal_meta_getter = nullptr;
        }
        if (nullptr != mock_node_meta_getter)
        {
            delete mock_node_meta_getter;
            mock_node_meta_getter = nullptr;
        }
    }
    void
    SetUp() override
    {
    }
    void
    TearDown() override
    {
    }
};
