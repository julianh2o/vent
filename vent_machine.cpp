#include "vent_machine.h"

VentState& VentMachine::addState(int state_id, StatePayload payload) {
  return states_.emplace(state_id, VentState(state_id, payload)).first->second;
}

void VentMachine::setCurrentState(int state_id) {
  if (state_id == current_state_id_) return;

  // State not found, no state updated.
  auto it = states_.find(state_id);
  if (it == states_.end()) return;

  current_state_id_ = state_id;
  it->second.runPayload();
}

int VentMachine::tick() {
  // No state found, no transition happen.
  auto it = states_.find(current_state_id_);
  if (it == states_.end()) return current_state_id_;

  VentState& current_state = it->second;

  // Let state to check all its exit conditions.
  int next_state_id = current_state.tick();

  // If no condition met, no transition happen.
  if (next_state_id == current_state_id_) return current_state_id_;

  // State not found, no transition happen.
  it = states_.find(next_state_id);
  if (it == states_.end()) return current_state_id_;

  // Change current state and execute new state payload.
  current_state_id_ = next_state_id;
  it->second.runPayload();

  return current_state_id_;
}

VentMachine::VentMachine() : current_state_id_(-1) {
}

VentMachine::~VentMachine() {
}
