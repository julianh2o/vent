#ifndef ESP32_HARDWARE_H
#define ESP32_HARDWARE_H

#include "hardware_interface.h"
#include "port_expander.h"
#include "multiplexer.h"
#include "configuration.h"
#include "screen.h"
#include "buzzer.h"
#include "button_debouncer.h"
#include "flow_sensor.h"

#include <unistd.h>
#include <sys/time.h>

class Esp32Hardware : public HardwareInterface {

public:
  void restartUptime();
  double getSecondsSinceStart() override;

  //Harware Inputs
  bool readSensors(SensorState* state);
  const SensorState * getSensorState() override;
  void updateSensorState();

  bool readControls(ControlState* state);
  ControlState * getControlState() override;
  void updateControlState();

  //Harware outputs
  bool setValves(const ValveState& state) override;
  bool writeIndication(const IndicationState& state) override;
  bool updateDisplay(const DisplayState& state) override;
  bool getConfig(ConfigState* state) override;

  void begin();
  void runTest();
  void testModeTick();
  void tick() override;

  Esp32Hardware();
  virtual ~Esp32Hardware();

  bool controlStateUpdated;
  ControlState controlState;
  SensorState sensorState;

  PortExpander ports;
  Multiplexer mux;
  Screen screen;
  Buzzer buzzer;
  FlowSensor flow1;

  ButtonDebouncer start;
  ButtonDebouncer pause;

  time_t bootTimestamp;
};

#endif  // ESP32_HARDWARE_H
