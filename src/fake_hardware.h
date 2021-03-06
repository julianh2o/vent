/*
#ifndef FAKE_HARDWARE_H
#define FAKE_HARDWARE_H

#include "hardware_interface.h"

//
// Fake hardware implementation designated for unit tests.
//
class FakeHardware : public HardwareInterface {

 public:

  double getSecondsSinceStart() override;
  const SensorState * getSensorState() override;
  bool setValves(bool v1, bool v2) override;
  const ControlState * getControlState() override;
  bool writeIndication(const IndicationState& state) override;
  bool updateDisplay(const Statistics& state) override;
  bool getConfig(ConfigState* state);

  void tick();

  FakeHardware();
  virtual ~FakeHardware();

  double seconds_since_start;

  bool read_sensors_return_value;
  SensorState sensor_state;

  bool set_valves_return_value;
  ValveState valve_state;

  bool read_controls_return_value;
  ControlState control_state;

  bool write_indication_return_value;
  IndicationState indication_state;

  bool update_display_return_value;
  Statistics display_state;

  bool get_config_return_value;
  ConfigState config_state;
};

#endif  // FAKE_HARDWARE_H
*/
