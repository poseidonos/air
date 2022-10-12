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

#ifndef AIR_GLOBAL_META_H
#define AIR_GLOBAL_META_H

#include <cstdint>

#include "src/config/ConfigInterface.h"

namespace meta
{
class GlobalMeta
{
public:
    virtual ~GlobalMeta(void)
    {
    }
    inline bool
    AirBuild(void) const
    {
        return air_build;
    }
    inline bool
    AirPlay(void) const
    {
        return air_play;
    }
    virtual inline uint32_t
    StreamingInterval(void) const
    {
        return streaming_interval;
    }
    virtual inline uint32_t
    NextStreamingInterval(void) const
    {
        return streaming_value;
    }
    virtual inline bool
    StreamingUpdate(void) const
    {
        return streaming_update;
    }
    virtual inline void
    SetAirBuild(bool air_build)
    {
        this->air_build = air_build;
    }
    virtual inline void
    SetAirPlay(bool air_play)
    {
        this->air_play = air_play;
    }
    virtual inline void
    SetStreamingInterval(uint32_t streaming_interval)
    {
        this->streaming_value = streaming_interval;
        streaming_update = true;
    }
    virtual inline void
    UpdateStreamingInterval(void)
    {
        streaming_interval = streaming_value;
        streaming_update = false;
    }
    inline void
    SetCpuNum(uint32_t cpu_num)
    {
        this->cpu_num = cpu_num;
    }
    inline uint32_t
    CpuNum(void) const
    {
        return cpu_num;
    }
    inline void
    SetFileWrite(bool file_write)
    {
        this->file_write = file_write;
    }
    inline bool
    FileWrite(void) const
    {
        return file_write;
    }
    inline void
    SetRemainingFileCount(uint32_t file_count)
    {
        this->remaining_file_count = file_count;
    }
    inline uint32_t
    RemainingFileCount(void) const
    {
        return remaining_file_count;
    }

private:
    bool air_build {true};
    bool air_play {true};
    bool streaming_update {false};
    bool file_write {true};
    uint32_t streaming_interval {1};
    uint32_t streaming_value {0};
    uint32_t cpu_num {0};
    uint32_t remaining_file_count {3};
};

class GlobalMetaGetter
{
public:
    GlobalMetaGetter(void)
    {
    }
    explicit GlobalMetaGetter(GlobalMeta* global_meta): global_meta(global_meta)
    {
    }
    virtual ~GlobalMetaGetter(void)
    {
    }
    virtual inline bool
    AirBuild(void) const
    {
        return global_meta->AirBuild();
    }
    virtual inline bool
    AirPlay(void) const
    {
        return global_meta->AirPlay();
    }
    virtual inline uint32_t
    StreamingInterval(void) const
    {
        return global_meta->StreamingInterval();
    }
    virtual inline uint32_t
    NextStreamingInterval(void) const
    {
        return global_meta->NextStreamingInterval();
    }
    inline bool
    StreamingUpdate(void) const
    {
        return global_meta->StreamingUpdate();
    }
    inline uint32_t
    CpuNum(void) const
    {
        return global_meta->CpuNum();
    }
    inline bool
    FileWrite(void) const
    {
        return global_meta->FileWrite();
    }
    inline uint32_t
    RemainingFileCount(void) const
    {
        return global_meta->RemainingFileCount();
    }

private:
    GlobalMeta* global_meta {nullptr};
};

} // namespace meta

#endif // AIR_GLOBAL_META_H
