#include "test_suite.h"

#include "../vent_state.h"

#define STATE1_ID 1
#define STATE2_ID 2
#define STATE3_ID 3


TEST(VentState, Payload) {
  int control_value = 0;
  VentState state(STATE1_ID, [&]() {
    control_value = 123;
  });

  EXPECT_EQ(control_value, 0);
  state.runPayload();
  EXPECT_EQ(control_value, 123);
}

TEST(VentState, ExitConditions) {
  VentState state(STATE1_ID, []() { /* no payload */});

  int some = 0;

  state.addExit(STATE2_ID, [&]() {
    return some == 234;
  });

  state.addExit(STATE3_ID, [&]() {
    return some == 123;
  });
  
  // No state transition.
  EXPECT_EQ(state.tick(), state.getStateId());
  EXPECT_EQ(state.getStateId(), STATE1_ID);

  some = 123;
  EXPECT_EQ(state.tick(), STATE3_ID);

  some = 234;
  EXPECT_EQ(state.tick(), STATE2_ID);
}