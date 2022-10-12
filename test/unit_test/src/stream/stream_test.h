
#include "src/meta/GlobalMeta.h"
#include "src/stream/Stream.cpp"
#include "src/stream/Stream.h"

class StreamTest : public ::testing::Test
{
public:
    stream::Stream* stream {nullptr};
    meta::GlobalMetaGetter* global_meta_getter {nullptr};
    meta::GlobalMeta* global_meta {nullptr};

protected:
    StreamTest()
    {
        global_meta = new meta::GlobalMeta {};
        global_meta_getter = new meta::GlobalMetaGetter {global_meta};
        stream = new stream::Stream {global_meta_getter};
    }
    ~StreamTest() override
    {
        if (nullptr != stream)
        {
            delete stream;
        }
        if (nullptr != global_meta_getter)
        {
            delete global_meta_getter;
        }
        if (nullptr != global_meta)
        {
            delete global_meta;
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
