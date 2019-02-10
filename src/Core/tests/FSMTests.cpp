#include "FSM.h"

#include <gtest/gtest.h>

using namespace VendingMachine::Core;
using namespace ::testing;

TEST(FSMTests, test_state_is_preserved)
{
    FSM fsm("My FSM", 0);
    EXPECT_EQ(0, fsm.getState());
    EXPECT_STREQ("My FSM", fsm.getName());  
}

TEST(FSMTests, test_state_changed)
{
    FSM fsm("My FSM", 0);

    fsm.setState(1);
    EXPECT_EQ(1, fsm.getState()); 
}
