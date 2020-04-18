#ifndef ESP32_HARDWARE_H
#define ESP32_HARDWARE_H

#include "hardware_interface.h"
#include "port_expander.h"
#include "multiplexer.h"
#include "configuration.h"
#include "screen.h"
#include "buzzer.h"
#include "button_debouncer.h"

class Esp32Hardware : public HardwareInterface {

public:
  double getSecondsSinceStart() override;
  bool readSensors(SensorState* state) override;
  bool setValves(const ValveState& state) override;

  bool readControls(ControlState* state) override;
  ControlState * getControlState();
  void updateControlState();

  bool writeIndication(const IndicationState& state) override;
  bool updateDisplay(const DisplayState& state) override;
  bool getConfig(ConfigState* state) override;

  void begin();
  void runTest();
  void testModeTick(uint16_t i);
  void tick(uint16_t i);

  Esp32Hardware();
  virtual ~Esp32Hardware();

  bool controlStateUpdated;
  ControlState controlState;

  PortExpander ports;
  Multiplexer mux;
  Screen screen;
  Buzzer buzzer;

  ButtonDebouncer start;
  ButtonDebouncer pause;
};

#endif  // ESP32_HARDWARE_H
