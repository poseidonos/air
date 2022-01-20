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

#include "src/lib/Casting.h"

class CastingTest : public ::testing::Test
{
public:
    enum class TypeUINT32_T : uint32_t
    {
        NUM_ZERO = 0,
        NUM_ONE = 1,
        NUM_TWO = 2,
    };

    enum class TypeUINT64_T : uint64_t
    {
        NUM_ZERO = 0,
        NUM_ONE = 1,
        NUM_TWO = 2,
    };

    enum class TypeINT : int
    {
        NUM_NEGATIVE_TWO = -2,
        NUM_NEGATIVE_ONE = -1,
        NUM_ZERO = 0,
        NUM_POSITIVE_ONE = 1,
        NUM_POSITIVE_TWO = 2,
    };

    enum class TypeUINT : unsigned int
    {
        NUM_ZERO = 0,
        NUM_ONE = 1,
        NUM_TWO = 2,
    };

    enum class TypeBOOL : bool
    {
        BOOL_FALSE = false,
        BOOL_TRUE = true,
    };

protected:
    CastingTest()
    {
    }
    ~CastingTest() override
    {
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
