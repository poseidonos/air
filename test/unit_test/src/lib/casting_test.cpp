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

#include "casting_test.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST_F(CastingTest, to_dtype)
{
    uint32_t num_u32{2};
    EXPECT_EQ(num_u32, to_dtype(TypeUINT32_T::NUM_TWO));
    num_u32 = 0;
    EXPECT_EQ(num_u32, to_dtype(TypeUINT32_T::NUM_ZERO));
    EXPECT_NE(num_u32, to_dtype(TypeUINT32_T::NUM_ONE));
    EXPECT_LT(num_u32, to_dtype(TypeUINT32_T::NUM_ONE));

    uint64_t num_u64{2};
    EXPECT_EQ(num_u64, to_dtype(TypeUINT64_T::NUM_TWO));
    num_u64 = 0;
    EXPECT_EQ(num_u64, to_dtype(TypeUINT64_T::NUM_ZERO));
    EXPECT_NE(num_u64, to_dtype(TypeUINT64_T::NUM_ONE));
    EXPECT_LT(num_u64, to_dtype(TypeUINT64_T::NUM_ONE));

    int num_i{2};
    EXPECT_EQ(num_i, to_dtype(TypeINT::NUM_POSITIVE_TWO));
    num_i = -1;
    EXPECT_EQ(num_i, to_dtype(TypeINT::NUM_NEGATIVE_ONE));
    EXPECT_NE(num_i, to_dtype(TypeINT::NUM_POSITIVE_ONE));
    EXPECT_GT(num_i, to_dtype(TypeINT::NUM_NEGATIVE_TWO));

    unsigned int num_ui{2};
    EXPECT_EQ(num_ui, to_dtype(TypeUINT::NUM_TWO));
    num_ui = 0;
    EXPECT_EQ(num_ui, to_dtype(TypeUINT::NUM_ZERO));
    EXPECT_NE(num_ui, to_dtype(TypeUINT::NUM_ONE));
    EXPECT_LT(num_ui, to_dtype(TypeUINT::NUM_ONE));

    bool b{false};
    EXPECT_EQ(b, to_dtype(TypeBOOL::BOOL_FALSE));
    b = true;
    EXPECT_EQ(b, to_dtype(TypeBOOL::BOOL_TRUE));
    EXPECT_NE(b, to_dtype(TypeBOOL::BOOL_FALSE));
}
