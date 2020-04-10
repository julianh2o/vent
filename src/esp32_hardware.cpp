#include "esp32_hardware.h"
#include "hardware_interface.h"

double Esp32Hardware::getSecondsSinceStart() {
  return seconds_since_start;
}

bool Esp32Hardware::readSensors(SensorState* state) {
  if (!read_sensors_return_value) return false;
  *state = sensor_state;
  return true;
}

bool Esp32Hardware::setValves(const ValveState& state) {
  if (!set_valves_return_value) return false;
  valve_state = state;
  return true;
}

bool Esp32Hardware::readControls(ControlState* state) {
  if (!read_controls_return_value) return false;
  *state = control_state;
  return true;
}

bool Esp32Hardware::writeIndication(const IndicationState& state) {
  if (!write_indication_return_value) return false;
  indication_state = state;
  return true;
}

bool Esp32Hardware::updateDisplay(const DisplayState& state) {
  if (!update_display_return_value) return false;
  display_state = state;
  return true;
}

bool Esp32Hardware::getConfig(ConfigState* state) {
  if (!get_config_return_value) return false;
  *state = config_state;
  return true;
}
Esp32Hardware::Esp32Hardware() : HardwareInterface() {
  seconds_since_start = 0.0;
}
Esp32Hardware::~Esp32Hardware() {
}
