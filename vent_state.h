#ifndef VENT_STATE_H
#define VENT_STATE_H

#include <functional>
#include <vector>
#include <utility>

typedef std::function<void(void)> StatePayload;
typedef std::function<bool(void)> ExitCondition;

// Implements the vent state machine state.
class VentState {

 public:
  VentState(int state_id, StatePayload payload);
  ~VentState();

  // Executes payload function privided in constructor.
  void runPayload();

  // Adds exit to the state with lambda exit conditions.
  VentState& addExit(int destination_state_id, ExitCondition condition);

  // Returns current state id.
  int getStateId() { return state_id_; }

  // Checks whether any of exit conditions met and return new destination state
  // id. If no conditions met then it returns this state id. 
  int tick();

 private:
  int state_id_;
  StatePayload payload_;
  std::vector<std::pair<int, ExitCondition> > exits_;
};

#endif /* VENT_STATE_H */