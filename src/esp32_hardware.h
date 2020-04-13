#ifndef ESP32_HARDWARE_H
#define ESP32_HARDWARE_H

#include "hardware_interface.h"
#include "port_expander.h"
#include "multiplexer.h"
#include "configuration.h"

class Esp32Hardware : public HardwareInterface {

public:

  double getSecondsSinceStart() override;
  bool readSensors(SensorState* state) override;
  bool setValves(const ValveState& state) override;
  bool readControls(ControlState* state) override;
  bool writeIndication(const IndicationState& state) override;
  bool updateDisplay(const DisplayState& state) override;
  bool getConfig(ConfigState* state) override;

  Esp32Hardware();
  virtual ~Esp32Hardware();

  PortExpander ports;
  Multiplexer mux;
};

#endif  // ESP32_HARDWARE_H
