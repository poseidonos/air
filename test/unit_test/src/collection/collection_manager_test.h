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
#include "mock_node_manager.h"
#include "mock_node_meta.h"
#include "mock_output_observer.h"
#include "src/collection/CollectionCorHandler.h"
#include "src/collection/CollectionManager.cpp"
#include "src/collection/CollectionManager.h"
#include "src/collection/CollectionObserver.cpp"
#include "src/collection/CollectionObserver.h"
#include "src/lib/Design.cpp"
#include "src/lib/Design.h"
#include "src/lib/Msg.h"

class CollectionManagerTest : public ::testing::Test
{
public:
    MockGlobalMetaGetter* mock_global_meta_getter {nullptr};
    MockNodeMetaGetter* mock_node_meta_getter {nullptr};
    MockNodeManager* mock_node_manager {nullptr};

    collection::Subject* collection_subject {nullptr};
    collection::CollectionManager* collection_manager {nullptr};

protected:
    CollectionManagerTest()
    {
        mock_global_meta_getter = new MockGlobalMetaGetter;
        mock_node_meta_getter = new MockNodeMetaGetter;
        mock_node_manager =
            new MockNodeManager {mock_global_meta_getter, mock_node_meta_getter};
        collection_subject = new collection::Subject;
        collection_manager =
            new collection::CollectionManager {mock_global_meta_getter,
                mock_node_meta_getter, mock_node_manager, collection_subject};
    }
    ~CollectionManagerTest() override
    {
        if (nullptr != mock_global_meta_getter)
        {
            delete mock_global_meta_getter;
            mock_global_meta_getter = nullptr;
        }
        if (nullptr != mock_node_meta_getter)
        {
            delete mock_node_meta_getter;
            mock_node_meta_getter = nullptr;
        }
        if (nullptr != collection_subject)
        {
            delete collection_subject;
            collection_subject = nullptr;
        }
        if (nullptr != collection_manager)
        {
            delete collection_manager;
            collection_manager = nullptr;
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
