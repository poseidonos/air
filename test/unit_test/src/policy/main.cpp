#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdio.h>

#include <vector>

#include "mock_collection_observer.h"
#include "mock_output_observer.h"
#include "policy_cor_handler_test.h"
#include "src/config/ConfigInterface.h"

TEST_F(PolicyTest, RulerCheckRule)
{
    int count = cfg::GetSentenceCount(config::ParagraphType::NODE);

    // enable air
    EXPECT_EQ(0, ruler->CheckRule(0, to_dtype(pi::Type2::ENABLE_AIR), 0, 0));

    // set streaming interval
    EXPECT_EQ(
        0, ruler->CheckRule(0, to_dtype(pi::Type2::SET_STREAMING_INTERVAL), 1, 0));
    EXPECT_EQ(
        0, ruler->CheckRule(0, to_dtype(pi::Type2::SET_STREAMING_INTERVAL), 30, 0));
    EXPECT_EQ(-12,
        ruler->CheckRule(0, to_dtype(pi::Type2::SET_STREAMING_INTERVAL), 0, 0));
    EXPECT_EQ(-12,
        ruler->CheckRule(0, to_dtype(pi::Type2::SET_STREAMING_INTERVAL), 31, 0));

    // enable node
    EXPECT_EQ(
        0, ruler->CheckRule(0, to_dtype(pi::Type2::ENABLE_NODE), 0, count - 1));
    EXPECT_EQ(-11, ruler->CheckRule(0, to_dtype(pi::Type2::ENABLE_NODE), 0, count));
    EXPECT_EQ(0,
        ruler->CheckRule(
            0, to_dtype(pi::Type2::ENABLE_NODE_WITH_RANGE), 0, 0x00000001)); // 0~1
    EXPECT_EQ(-11,
        ruler->CheckRule(0, to_dtype(pi::Type2::ENABLE_NODE_WITH_RANGE), 0,
            0x0000ffff)); // 0~0xffff
    EXPECT_EQ(-11,
        ruler->CheckRule(0, to_dtype(pi::Type2::ENABLE_NODE_WITH_RANGE), 0,
            0xffff0000)); // 0xffff~0
    EXPECT_EQ(-11,
        ruler->CheckRule(
            0, to_dtype(pi::Type2::ENABLE_NODE_WITH_RANGE), 0, 0x00010000)); // 1~0
    EXPECT_EQ(-11,
        ruler->CheckRule(
            0, to_dtype(pi::Type2::ENABLE_NODE_WITH_GROUP), 0, 0x0000000f));
    EXPECT_EQ(0, ruler->CheckRule(0, to_dtype(pi::Type2::ENABLE_NODE_ALL), 0, 0));

    // init node
    EXPECT_EQ(
        0, ruler->CheckRule(0, to_dtype(pi::Type2::INITIALIZE_NODE), count - 1, 0));
    EXPECT_EQ(
        -11, ruler->CheckRule(0, to_dtype(pi::Type2::INITIALIZE_NODE), count, 0));
    EXPECT_EQ(0,
        ruler->CheckRule(0, to_dtype(pi::Type2::INITIALIZE_NODE_WITH_RANGE),
            0x00000001, 0)); // 0~1
    EXPECT_EQ(-11,
        ruler->CheckRule(0, to_dtype(pi::Type2::INITIALIZE_NODE_WITH_RANGE),
            0x0000ffff, 0)); // 0~0xffff
    EXPECT_EQ(-11,
        ruler->CheckRule(0, to_dtype(pi::Type2::INITIALIZE_NODE_WITH_RANGE),
            0xffff0000, 0)); // 0xffff~0
    EXPECT_EQ(-11,
        ruler->CheckRule(0, to_dtype(pi::Type2::INITIALIZE_NODE_WITH_RANGE),
            0x00010000, 0)); // 1~0
    EXPECT_EQ(-11,
        ruler->CheckRule(
            0, to_dtype(pi::Type2::INITIALIZE_NODE_WITH_GROUP), 0x0000000f, 0));
    EXPECT_EQ(
        0, ruler->CheckRule(0, to_dtype(pi::Type2::INITIALIZE_NODE_ALL), 0, 0));

    // invalid command
    EXPECT_EQ(-1, ruler->CheckRule(0, to_dtype(pi::Type2::COUNT), 1, 0));
}

TEST_F(PolicyTest, RulerSetRule)
{
    // enable air
    EXPECT_EQ(true, ruler->SetRule(0, to_dtype(pi::Type2::ENABLE_AIR), true, 0));
    EXPECT_EQ(true, ruler->SetRule(0, to_dtype(pi::Type2::ENABLE_AIR), false, 0));

    // set streaming interval
    EXPECT_EQ(
        true, ruler->SetRule(0, to_dtype(pi::Type2::SET_STREAMING_INTERVAL), 1, 0));
    EXPECT_EQ(
        true, ruler->SetRule(0, to_dtype(pi::Type2::SET_STREAMING_INTERVAL), 2, 0));

    mock_node_meta->SetGroupId(1, 0);
    mock_node_meta->SetGroupId(5, 0);
    mock_node_meta->SetGroupId(3, 1);
    mock_node_meta->SetGroupId(6, 1);

    // set enable node
    EXPECT_EQ(true, ruler->SetRule(0, to_dtype(pi::Type2::ENABLE_NODE), false, 0));
    EXPECT_EQ(true,
        ruler->SetRule(
            0, to_dtype(pi::Type2::ENABLE_NODE_WITH_RANGE), false, 0x00000001));
    EXPECT_EQ(true,
        ruler->SetRule(
            0, to_dtype(pi::Type2::ENABLE_NODE_WITH_GROUP), false, 0x00000000));
    EXPECT_EQ(
        true, ruler->SetRule(0, to_dtype(pi::Type2::ENABLE_NODE_ALL), false, 0));

    // set init node
    EXPECT_EQ(true, ruler->SetRule(0, to_dtype(pi::Type2::INITIALIZE_NODE), 0, 0));
    EXPECT_EQ(true,
        ruler->SetRule(0, to_dtype(pi::Type2::INITIALIZE_NODE_WITH_RANGE), 0, 0));
    EXPECT_EQ(true,
        ruler->SetRule(0, to_dtype(pi::Type2::INITIALIZE_NODE_WITH_GROUP), 0, 0));
    EXPECT_EQ(
        true, ruler->SetRule(0, to_dtype(pi::Type2::INITIALIZE_NODE_ALL), 0, 0));
}

TEST_F(PolicyTest, RuleManagerConfig)
{
    // set node meta config
    air::NodeMetaData node[cfg::GetSentenceCount(config::ParagraphType::NODE)];
    rule_manager->SetNodeMetaConfig(node);

    EXPECT_EQ(air::ProcessorType::PERFORMANCE,
        node[cfg::GetSentenceIndex(config::ParagraphType::NODE, "PERF_PSD")]
            .processor_type);
    EXPECT_EQ(air::ProcessorType::LATENCY,
        node[cfg::GetSentenceIndex(config::ParagraphType::NODE, "LAT_PSD")]
            .processor_type);
    EXPECT_EQ(air::ProcessorType::QUEUE,
        node[cfg::GetSentenceIndex(config::ParagraphType::NODE, "Q_EVENT")]
            .processor_type);

    // set global config
    rule_manager->SetGlobalConfig();
}

TEST_F(PolicyTest, CollectionCoRHandler)
{
    MockCollectionObserver* mock_collection_observer =
        new MockCollectionObserver {};
    MockOutputObserver* mock_output_observer = new MockOutputObserver {};
    policy_subject->Attach(
        mock_collection_observer, to_dtype(pi::PolicySubject::TO_COLLECTION));
    policy_subject->Attach(
        mock_output_observer, to_dtype(pi::PolicySubject::TO_OUTPUT));

    EXPECT_EQ((unsigned int)1, mock_global_meta->StreamingInterval());
    EXPECT_EQ(false, mock_node_meta->Run(2));

    // send msg
    policy_observer->Update(
        0, to_dtype(pi::Type2::SET_STREAMING_INTERVAL), 2, 0, 0, 0, 0);
    policy_observer->Update(0, to_dtype(pi::Type2::ENABLE_NODE), 2, 2, 0, 0, 0);

    // handle msg
    policy_cor_handler->HandleRequest();

    EXPECT_EQ((unsigned int)2, mock_global_meta->StreamingInterval());
    EXPECT_EQ(true, mock_node_meta->Run(2));
}

int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
