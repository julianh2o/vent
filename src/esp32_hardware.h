#ifndef ESP32_HARDWARE_H
#define ESP32_HARDWARE_H

#include "hardware_interface.h"
#include "port_expander.h"
#include "multiplexer.h"
#include "configuration.h"
#include "screen.h"

class Esp32Hardware : public HardwareInterface {

public:
  double getSecondsSinceStart() override;
  bool readSensors(SensorState* state) override;
  bool setValves(const ValveState& state) override;
  bool readControls(ControlState* state) override;
  bool writeIndication(const IndicationState& state) override;
  bool updateDisplay(const DisplayState& state) override;
  bool getConfig(ConfigState* state) override;

  void begin();
  void runTest();
  void tick(uint16_t i);

  Esp32Hardware();
  virtual ~Esp32Hardware();

  PortExpander ports;
  Multiplexer mux;
  Screen screen;
};

#endif  // ESP32_HARDWARE_H
