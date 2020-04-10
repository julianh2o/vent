#include "test_suite.h"

#include "../src/vent_machine.h"

#define STATE1_ID 1
#define STATE2_ID 2
#define STATE3_ID 3


TEST(VentMachine, NoCurrentStateFound) {
  VentMachine machine;
  EXPECT_EQ(machine.tick(), -1);
  EXPECT_EQ(machine.getCurrentStateId(), -1);

  machine.setCurrentState(STATE1_ID);
  EXPECT_EQ(machine.tick(), -1);
  EXPECT_EQ(machine.getCurrentStateId(), -1);
}

TEST(VentMachine, NoExitConditionMet) {
  VentMachine machine;

  int control_value = 0;

  machine.addState(STATE1_ID, [&]() {
    control_value = 1;
  }).addExit(STATE2_ID, []() {
    return false;
  });

  EXPECT_EQ(control_value, 0);

  machine.setCurrentState(STATE1_ID); // Executes payload.
  EXPECT_EQ(control_value, 1);
  EXPECT_EQ(machine.tick(), STATE1_ID);
}

TEST(VentMachine, NoDestinationStateFound) {
  VentMachine machine;

  machine.addState(STATE1_ID, []() {
  }).addExit(STATE2_ID, []() {
    return true;
  });

  machine.setCurrentState(STATE1_ID);

  // Even though STATE2 exit condition met, no transition found because STATE2
  // was not found.
  EXPECT_EQ(machine.tick(), STATE1_ID);
}

TEST(VentMachine, ThreeStateMachine) {
  VentMachine machine;

  int control_value = 0;

  machine.addState(STATE1_ID, [&]() {
    control_value = 1;
  }).addExit(STATE2_ID, [&]() {
    return true;
  });

  machine.addState(STATE2_ID, [&]() {
    control_value = 2;
  }).addExit(STATE3_ID, [&]() {
    return true;
  });

  machine.addState(STATE3_ID, [&]() {
    control_value = 3;
  });

  EXPECT_EQ(machine.getCurrentStateId(), -1);
  EXPECT_EQ(control_value, 0);

  machine.setCurrentState(STATE1_ID);
  EXPECT_EQ(machine.getCurrentStateId(), STATE1_ID);
  EXPECT_EQ(control_value, 1);

  machine.tick();
  EXPECT_EQ(machine.getCurrentStateId(), STATE2_ID);
  EXPECT_EQ(control_value, 2);

  machine.tick();
  EXPECT_EQ(machine.getCurrentStateId(), STATE3_ID);
  EXPECT_EQ(control_value, 3);

  machine.tick();
  EXPECT_EQ(machine.getCurrentStateId(), STATE3_ID);
  EXPECT_EQ(control_value, 3);
}

TEST(VentMachine, StateSelfLoop) {
  VentMachine machine;

  int control_value = 0;

  machine.addState(STATE1_ID, [&]() {
    control_value++;
  }).addExit(STATE1_ID, [&]() {
    // The state exits to itself.
    return true;
  });

  machine.setCurrentState(STATE1_ID);

  // No transition should happen below.
  machine.tick();
  machine.tick();
  machine.tick();

  EXPECT_EQ(machine.getCurrentStateId(), STATE1_ID);
  EXPECT_EQ(control_value, 1);
}

TEST(VentMachine, TwoStateLoop) {
  VentMachine machine;

  int control_value = 0;

  machine.addState(STATE1_ID, [&]() {
    control_value++;
  }).addExit(STATE2_ID, [&]() {
    return true;
  });

  machine.addState(STATE2_ID, [&]() {
    control_value++;
  }).addExit(STATE1_ID, [&]() {
    return true;
  });

  machine.setCurrentState(STATE1_ID);
  EXPECT_EQ(control_value, 1);

  // Each tick below increments control_value by 1.
  machine.tick();
  EXPECT_EQ(control_value, 2);

  machine.tick();
  EXPECT_EQ(control_value, 3);

  machine.tick();
  EXPECT_EQ(control_value, 4);
  EXPECT_EQ(machine.getCurrentStateId(), STATE2_ID);
}
