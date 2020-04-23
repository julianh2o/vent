#ifndef VENT_MACHINE_H
#define VENT_MACHINE_H

#include <map>

#include "vent_state.h"

// Simple state machine implementation.
// Make sure that all used state identifiers are UNIQUE.
class VentMachine {

 public:
  // Creates new state and registers it within the state machine.
  // No effect on current state.
  VentState& addState(int state_id, StatePayload payload = &dummy);

  // Changes current state of the machine by state id. If corresponding state
  // not found no change happen. If corresponding state found its payload will
  // be executed.
  void setCurrentState(int state_id);

  // Checks the exit conditions of current state and executes transition if
  // certain condition met.
  int tick();

  // Returns current state id.
  int getCurrentStateId() { return current_state_id_; }

  VentMachine();
  ~VentMachine();

 private:
  std::map<int, VentState> states_;
  int current_state_id_;

  static void dummy() {}
};

#endif  // VENT_MACHINE_H