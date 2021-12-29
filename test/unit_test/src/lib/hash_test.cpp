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

#include "hash_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST_F(HashTest, hash)
{
    uint32_t size = 5;
    // Insert
    uint32_t index_0 = hash_map->InsertHashNode(0x00A3FC00);
    uint32_t index_1 = hash_map->InsertHashNode(1234);
    uint32_t index_2 = hash_map->InsertHashNode(981);
    uint32_t index_3 = hash_map->InsertHashNode(451);
    uint32_t index_4 = hash_map->InsertHashNode(4);
    uint32_t full = hash_map->InsertHashNode(2);
    EXPECT_EQ(size, hash_map->GetHashSize());
    EXPECT_EQ(full, size);

    EXPECT_LT(index_0, size);
    EXPECT_LT(index_1, size);
    EXPECT_LT(index_2, size);
    EXPECT_LT(index_3, size);
    EXPECT_LT(index_4, size);

    // Get hash key
    uint64_t invalid_result = air::HashMap<uint64_t>::npos;
    EXPECT_EQ(0x00A3FC00, hash_map->GetHashKey(index_0));
    EXPECT_EQ(1234, hash_map->GetHashKey(index_1));
    EXPECT_EQ(981, hash_map->GetHashKey(index_2));
    EXPECT_EQ(451, hash_map->GetHashKey(index_3));
    EXPECT_EQ(4, hash_map->GetHashKey(index_4));
    EXPECT_EQ(invalid_result, hash_map->GetHashKey(size));

    // Delete
    EXPECT_EQ(true, hash_map->DeleteHashNode(1234));
    EXPECT_EQ(true, hash_map->DeleteHashNode(0x00A3FC00));
    EXPECT_EQ(false, hash_map->DeleteHashNode(0x00A3FC0F));
    EXPECT_EQ(size - 2, hash_map->GetHashSize());
    EXPECT_EQ(invalid_result, hash_map->GetHashKey(index_0));
    EXPECT_EQ(invalid_result, hash_map->GetHashKey(index_1));

    // Get hash index
    EXPECT_EQ(size, hash_map->GetHashIndex(0x00A3FC00));
    EXPECT_EQ(size, hash_map->GetHashIndex(1234));
    index_0 = hash_map->InsertHashNode(0x12341234);
    index_1 = hash_map->InsertHashNode(77777);
    EXPECT_EQ(size, hash_map->GetHashSize());
    EXPECT_EQ(index_0, hash_map->GetHashIndex(0x12341234));
    EXPECT_EQ(index_1, hash_map->GetHashIndex(77777));
    EXPECT_EQ(index_2, hash_map->GetHashIndex(981));
    EXPECT_EQ(index_3, hash_map->GetHashIndex(451));
    EXPECT_EQ(index_4, hash_map->GetHashIndex(4));
}
