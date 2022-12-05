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

#ifndef AIR_PROTOCOL_H
#define AIR_PROTOCOL_H

#include <cstdint>

#include "src/lib/Casting.h"

namespace protocol
{
namespace internal
{
enum class Type1 : uint32_t
{
    INPUT_TO_POLICY = 0x00000000,
    POLICY_TO_COLLECTION = 0x00000001,
    COLLECTION_TO_OUTPUT = 0x00000002,
    POLICY_TO_OUTPUT = 0x00000003,

    COUNT
};

enum class Type2 : uint32_t
{
    LINK_CHAIN = 0x00000000,

    SET_STREAMING_INTERVAL = 0x00000001,

    SET_FILE_WRITE = 0x00000002,

    SET_REMAIN_FILE_COUNT = 0x00000003,

    ENABLE_AIR = 0x00010000,

    ENABLE_NODE = 0x00010001,
    ENABLE_NODE_WITH_RANGE = 0x00010002,
    ENABLE_NODE_WITH_DEPTH = 0x00010003,
    ENABLE_NODE_WITH_CHILD = 0x00010004,
    ENABLE_NODE_WITH_GROUP = 0x00010005,
    ENABLE_NODE_ALL = 0x00010006,

    INITIALIZE_NODE = 0x00010011,
    INITIALIZE_NODE_WITH_RANGE = 0x00010012,
    INITIALIZE_NODE_WITH_DEPTH = 0x00010013,
    INITIALIZE_NODE_WITH_CHILD = 0x00010014,
    INITIALIZE_NODE_WITH_GROUP = 0x00010015,
    INITIALIZE_NODE_ALL = 0x00010016,

    COUNT
};

enum class Type2_Upper : uint16_t
{
    OUTPUT = 0x0000,
    COLLECTION = 0x0001
};

enum class Type2_Lower : uint16_t
{
    LINK_CHAIN = 0x0000,
    ENABLE_MODULE = 0x0001,
    SET_STREAMING_INTERVAL = 0x0001,

    ENABLE_NODE = 0x0001,
    ENABLE_NODE_WITH_RANGE = 0x0002,
    SET_FILE_WRITE = 0x0002,
    ENABLE_NODE_WITH_DEPTH = 0x0003,
    SET_REMAIN_FILE_COUNT = 0x0003,
    ENABLE_NODE_WITH_CHILD = 0x0004,
    ENABLE_NODE_WITH_GROUP = 0x0005,
    ENABLE_NODE_ALL = 0x0006,

    INITIALIZE_NODE = 0x0011,
    INITIALIZE_NODE_WITH_RANGE = 0x0012,
    INITIALIZE_NODE_WITH_DEPTH = 0x0013,
    INITIALIZE_NODE_WITH_CHILD = 0x0014,
    INITIALIZE_NODE_WITH_GROUP = 0x0015,
    INITIALIZE_NODE_ALL = 0x0016,

    GET_CONFIG = 0x0201,
};

enum class InSubject : uint32_t
{
    TO_POLICY = 0,
    TO_OUTPUT,

    COUNT
};

enum class PolicySubject : uint32_t
{
    TO_COLLECTION = 0,
    TO_OUTPUT,

    COUNT
};

enum class CollectionSubject : uint32_t
{
    TO_OUTPUT = 0,

    COUNT
};

enum class ChainHandler : uint32_t
{
    INPUT = 0,
    POLICY,
    COLLECTION,
    OUTPUT,
    PROCESS,
    STREAM,
    SWITCHGEAR,
    PREPROCESS,
    DETECT,
    TRANSFER,

    COUNT
};

enum class PreprocessOption : int32_t
{
    NORMAL = 0,
    FORCED
};

enum class OnOff : uint32_t
{
    OFF = 0,
    ON,

    COUNT
};

static const uint32_t k_max_subject_size {
    (to_dtype(InSubject::COUNT) > to_dtype(PolicySubject::COUNT))
        ? to_dtype(InSubject::COUNT)
        : to_dtype(PolicySubject::COUNT)};

} // namespace internal

namespace external
{
} // namespace external

} // namespace protocol

namespace pi = protocol::internal;
namespace pe = protocol::external;

#endif // AIR_PROTOCOL_H
