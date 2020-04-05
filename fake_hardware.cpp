#include "fake_hardware.h"


double FakeHardware::getSecondsSinceStart() {
  return seconds_since_start;
}

bool FakeHardware::readSensors(SensorState* state) {
  if (!read_sensors_return_value) return false;
  *state = sensor_state;
  return true;
}

bool FakeHardware::setValves(const ValveState& state) {
  if (!set_valves_return_value) return false;
  valve_state = state;
  return true;
}

bool FakeHardware::readControls(ControlState* state) {
  if (!read_controls_return_value) return false;
  *state = control_state;
  return true;
}

bool FakeHardware::writeIndication(const IndicationState& state) {
  if (!write_indication_return_value) return false;
  indication_state = state;
  return true;
}

bool FakeHardware::updateDisplay(const DisplayState& state) {
  if (!update_display_return_value) return false;
  display_state = state;
  return true;
}

bool FakeHardware::getConfig(ConfigState* state) {
  if (!get_config_return_value) return false;
  *state = config_state;
  return true;
}

FakeHardware::FakeHardware() : HardwareInterface() {
  seconds_since_start = 0.0;
}

FakeHardware::~FakeHardware() {
}
