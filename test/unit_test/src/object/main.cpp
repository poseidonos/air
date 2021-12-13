
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdio.h>

#include "instance_test.h"

TEST_F(InstanceTest, UseCase1)
{
    EXPECT_EQ(0, instance_manager->Initialize(0));
    EXPECT_EQ(0, instance_manager->Finalize());
}

TEST_F(InstanceTest, UseCase2)
{
    EXPECT_EQ(0, instance_manager->Initialize(0));
    EXPECT_EQ(0, instance_manager->Activate());
    EXPECT_EQ(0, instance_manager->Deactivate());
    EXPECT_EQ(0, instance_manager->Finalize());
}

TEST_F(InstanceTest, UseCase3)
{
    EXPECT_EQ(0, instance_manager->Initialize(0));
    EXPECT_EQ(0, instance_manager->Activate());
    EXPECT_EQ(0, instance_manager->Deactivate());
    EXPECT_EQ(0, instance_manager->Activate());
    EXPECT_EQ(0, instance_manager->Deactivate());
    EXPECT_EQ(0, instance_manager->Activate());
    EXPECT_EQ(0, instance_manager->Deactivate());
    EXPECT_EQ(0, instance_manager->Finalize());
}

TEST_F(InstanceTest, GetNodeManager)
{
    EXPECT_EQ(0, instance_manager->Initialize(0));
    EXPECT_NE(nullptr, instance_manager->GetNodeManager());
    EXPECT_EQ(0, instance_manager->Finalize());
}

TEST_F(InstanceTest, GetCollectionManager)
{
    EXPECT_EQ(0, instance_manager->Initialize(0));
    EXPECT_NE(nullptr, instance_manager->GetCollectionManager());
    EXPECT_EQ(0, instance_manager->Finalize());
}

int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
