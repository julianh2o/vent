#include "vent_state.h"


VentState::VentState(int state_id, StatePayload payload) : 
  state_id_(state_id), payload_(payload) {
}

VentState::~VentState() {
}

void VentState::runPayload() {
  payload_();
}

VentState& VentState::addExit(int destination_state_id, ExitCondition condition) {
  exits_.push_back(
    std::pair<int, ExitCondition>(destination_state_id, condition));
  return *this;
}

int VentState::tick() {
  for (auto it = exits_.begin(); it != exits_.end(); ++it) {
    if (it->second()) {
      // If exit condition met then return destination state id.
      return it->first;
    }
  }

  // If no transition conditions have been met then stay in the current state.
  return state_id_;
}
