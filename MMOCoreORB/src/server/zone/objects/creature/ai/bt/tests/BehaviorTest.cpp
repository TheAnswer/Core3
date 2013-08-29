/*
 * BehaviourTest.cpp
 *
 *  Created on: Aug 24, 2013
 *      Author: swgemu
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "MockBehavior.h"
#include "MockSequenceBehavior.h"
#include "../BehaviorTree.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Invoke;
using ::testing::Sequence;
using ::testing::Assign;
using ::testing::DoAll;
using ::testing::Mock;

namespace server {
namespace zone {
namespace objects {
namespace creature {
namespace ai {
namespace bt {
namespace test {

class BehaviorTest : public ::testing::Test {
public:
	Reference<MockAiActor*> actor;
	BehaviorTest() {
		// Perform creation setup here.
	}

	~BehaviorTest() {
		// Clean up.
	}

	void SetUp() {
		actor = new MockAiActor();
	}

	void TearDown() {
		// Perform clean up of common constructs here.
	}
};
TEST_F(BehaviorTest, TaskInitialize) {
	MockBehavior mock;
	BehaviorTree tree;
	ON_CALL(mock,update(_)).WillByDefault(Return(RUNNING));
	EXPECT_CALL(mock,onInitialize(_)).Times(AtLeast(1));
	EXPECT_CALL(mock,update(_)).Times(AtLeast(1));
	EXPECT_CALL(mock,canObserve()).Times(AtLeast(1));
	tree.start(&mock,actor);
	tree.tick(actor);

}
TEST_F(BehaviorTest, TaskUpdate) {
	MockBehavior mock;
	BehaviorTree tree;
	EXPECT_CALL(mock,onTerminate(_,_)).Times(AtLeast(0));
	EXPECT_CALL(mock,onInitialize(_)).Times(AtLeast(1));
	EXPECT_CALL(mock,update(_)).WillOnce(Return(RUNNING));
	EXPECT_CALL(mock,canObserve()).Times(AtLeast(1));
	tree.start(&mock,actor);
	tree.tick(actor);
	EXPECT_CALL(mock,update(_)).WillOnce(Return(RUNNING));
	tree.tick(actor);
}
TEST_F(BehaviorTest, TaskTerminate) {
	MockBehavior mock;
	BehaviorTree tree;
	EXPECT_CALL(mock,onTerminate(_,_)).Times(AtLeast(1));
	EXPECT_CALL(mock,canObserve()).Times(AtLeast(1));
	EXPECT_CALL(mock,onInitialize(_)).Times(AtLeast(1));
	EXPECT_CALL(mock,update(_)).WillOnce(Return(RUNNING));
	tree.start(&mock,actor);
	tree.tick(actor);
	EXPECT_CALL(mock,update(_)).WillOnce(Return(SUCCESS));
	tree.tick(actor);
}
TEST_F(BehaviorTest, TaskAbort) {
	MockBehavior mock;
	BehaviorTree tree;
	EXPECT_CALL(mock,onTerminate(_,_)).Times(AtLeast(0));
	EXPECT_CALL(mock,canObserve()).Times(AtLeast(0));
	EXPECT_CALL(mock,onInitialize(_)).Times(AtLeast(0));
	EXPECT_CALL(mock,update(_)).Times(AtLeast(0));
	tree.start(&mock,actor);
	tree.tick(NULL);

}
TEST_F(BehaviorTest, TestSingleNodeTreePassAndFail) {
	Status status[2] = { SUCCESS,FAILURE };
	for (int i=0; i<2; ++i) {
		BehaviorTree tree;
		MockSequenceBehavior mock(&tree,1);
		EXPECT_CALL(mock,onInitialize(_)).Times(AtLeast(1));
		EXPECT_CALL(mock,update(_)).WillOnce(Return(RUNNING));
		EXPECT_CALL(mock,canObserve()).Times(AtLeast(1));
		tree.start(&mock,actor);
		tree.tick(actor);
		EXPECT_CALL(mock,update(_)).WillOnce(Return(status[i]));
		EXPECT_CALL(mock,onTerminate(_,_)).Times(AtLeast(1));
		EXPECT_CALL(mock,canObserve()).Times(AtLeast(1));
		tree.tick(actor);
		Mock::VerifyAndClearExpectations(&mock);
		actor->resetBehaviorList(&tree);
	}

}
TEST_F(BehaviorTest, TestTwoNodesWithFailingBehavior) {
	BehaviorTree tree;
	MockSequenceBehavior mock(&tree,2);
	EXPECT_CALL(mock,onInitialize(_)).Times(AtLeast(1));
	EXPECT_CALL(mock,update(_)).WillOnce(Return(RUNNING));
	EXPECT_CALL(mock,onTerminate(_,_)).Times(AtLeast(1));
	EXPECT_CALL(mock,canObserve()).Times(AtLeast(1));
	tree.start(&mock,actor);
	tree.tick(actor);
	EXPECT_CALL(mock,canObserve()).Times(AtLeast(1));
	EXPECT_CALL(mock,update(_)).WillOnce(Return(FAILURE));
	tree.tick(actor);

}
TEST_F(BehaviorTest, TestTwoNodeWithRunningBehavior) {
	BehaviorTree tree;
	MockSequenceBehavior mock(&tree,2);
	EXPECT_CALL(mock,onInitialize(_)).Times(AtLeast(1));
	EXPECT_CALL(mock,update(_)).WillOnce(Return(RUNNING));
	EXPECT_CALL(mock,onTerminate(_,_)).Times(AtLeast(0));
	EXPECT_CALL(mock,canObserve()).Times(AtLeast(0));
	tree.start(&mock,actor);
	tree.tick(actor);
	EXPECT_CALL(mock,update(_)).WillOnce(Return(RUNNING));
	tree.tick(actor);
}

}
}
}
}
}
}
}
