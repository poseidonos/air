
#include "src/lib/json/Json.h"
#include "src/transfer/Task.cpp"
#include "src/transfer/Task.h"

class TaskTest : public ::testing::Test
{
public:
    static int function_call_count;
    static int
    TestFunction(const air::JSONdoc& doc)
    {
        function_call_count++;
        return 0;
    }

protected:
    TaskTest()
    {
    }
    ~TaskTest() override
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

int
test_register(const air::JSONdoc& doc)
{
    return 0;
}
