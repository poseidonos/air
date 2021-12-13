
#include "src/lib/Hash.cpp"

class HashTest : public ::testing::Test
{
public:
    air::HashMap<uint64_t>* hash_map{nullptr};

protected:
    HashTest()
    {
        hash_map = new air::HashMap<uint64_t>{5};
    }
    ~HashTest() override
    {
        if (nullptr != hash_map)
        {
            delete hash_map;
            hash_map = nullptr;
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
