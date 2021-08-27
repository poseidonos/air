
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>

#include "task_test.h"
#include "transfer_test.h"

int TaskTest::function_call_count = 0;

TEST_F(TaskTest, Singleton)
{
    auto& instanceA = transfer::Task::Get();
    auto& instanceB = transfer::Task::Get();
    EXPECT_EQ(&instanceA, &instanceB);
}

TEST_F(TaskTest, Register_Lambda_Type1)
{
    transfer::Task::Get().Register({"NodeA", "NodeB"},
        [](const air::JSONdoc& doc) -> int {return 0;}
    );
}

TEST_F(TaskTest, Register_Lambda_Type2)
{
    auto t2 = [](const air::JSONdoc& doc) -> int {return 0;};
    transfer::Task::Get().Register({"NodeA"}, t2);
}

TEST_F(TaskTest, Register_Function)
{
    transfer::Task::Get().Register({"NodeC"}, test_register);
}

TEST_F(TaskTest, NotifyAll)
{
    int count{0};
    transfer::Task::Get().Register({"Node"},
        [&](const air::JSONdoc& doc) -> int
        {
            usleep(count * 100000);
            count++;
            if (count >= 3)
            {
                return -1;
            }
            return 0;
        }
    );
    air::JSONdoc& doc = air::json("doc");
    transfer::Task::Get().NotifyAll(std::move(doc));
    EXPECT_EQ(1, count);
    transfer::Task::Get().NotifyAll(std::move(doc));
    EXPECT_EQ(2, count);
    transfer::Task::Get().NotifyAll(std::move(doc));
    EXPECT_EQ(3, count);
    transfer::Task::Get().NotifyAll(std::move(doc));
    EXPECT_EQ(3, count);
    transfer::Task::Get().NotifyAll(std::move(doc));
    EXPECT_EQ(3, count);
}

TEST_F(TaskTest, Lambda_Capture_Reference)
{
    struct Obj
    {
        int data{100};
    };
    Obj obj;
    EXPECT_EQ(100, obj.data);

    transfer::Task::Get().Register({"Node"},
        [&obj](const air::JSONdoc& doc) -> int
        {
            obj.data++;
            return 0;
        }
    );

    obj.data++;
    air::JSONdoc& doc = air::json("doc");
    transfer::Task::Get().NotifyAll(std::move(doc));

    EXPECT_EQ(102, obj.data);
}

TEST_F(TaskTest, Lambda_Capture_Copy)
{
    struct Obj
    {
        int* data{nullptr};
        Obj(){ data = new int{100}; }
        Obj(const Obj& rhs){ data = new int{*rhs.data}; }
        ~Obj(){ delete data; }
    };
    Obj* obj = new Obj{};
    EXPECT_EQ(100, *obj->data);

    transfer::Task::Get().Register({"Node"},
        [copy = *obj](const air::JSONdoc& doc) mutable -> int
        {
            (*copy.data)++;
            return 0;
        }
    );

    (*obj->data)++;
    air::JSONdoc& doc = air::json("doc");
    transfer::Task::Get().NotifyAll(std::move(doc));

    EXPECT_EQ(101, *obj->data);
    delete obj;
}

TEST_F(TransferTest, Transfer)
{
    transfer::Transfer transfer;
}

TEST_F(TransferTest, SendData)
{
    auto& obj = air::json("air");
    auto& groupTop = air::json("groupTop");
    auto& groupA = air::json("groupA");
    auto& nodeA = air::json("nodeA");
    auto& nodeApple = air::json("nodeApple");
    auto& nodeAnt = air::json("nodeAnt");
    auto& groupB = air::json("groupB");
    auto& nodeB = air::json("nodeB");
    auto& nodeBanana = air::json("nodeBanana");
    auto& nodeBacon = air::json("nodeBacon");
    nodeApple["color"] = {"red"};
    nodeApple["weight(kg)"] = {0.93};
    nodeAnt["color"] = {"black"};
    nodeAnt["type"] = {"queen"};
    nodeA["Apple"] = {nodeApple};
    nodeA["Ant"] = {nodeAnt};
    groupA["node"] = {nodeA};
    groupTop["groupA"] = {groupA};
    nodeBanana["color"] = {"yellow"};
    nodeBanana["weight(kg)"] = {2.34};
    nodeBacon["color"] = {"pink"};
    nodeB["Banana"] = {nodeBanana};
    nodeB["Bacon"] = {nodeBacon};
    groupB["node"] = {nodeB};
    groupTop["groupB"] = {groupB};
    obj["group"] = {groupTop};

    transfer::Task::Get().Register({"Apple", "Banana"},
        [](const air::JSONdoc& doc) -> int
        {
            std::stringstream stream_apple;
            stream_apple << doc["Apple"];
            std::string str_apple = stream_apple.str();
            EXPECT_EQ(0, str_apple.compare("{\"color\": \"red\", \"weight(kg)\": 0.93}"));

            std::stringstream stream_banana;
            stream_banana << doc["Banana"];
            std::string str_banana = stream_banana.str();
            EXPECT_EQ(0, str_banana.compare("{\"color\": \"yellow\", \"weight(kg)\": 2.34}"));
            return 0;
        }
    );

    transfer::Transfer transfer;
    transfer.SendData();
}

int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
