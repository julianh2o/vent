#ifndef ESP32_HARDWARE_H
#define ESP32_HARDWARE_H

#include "hardware_interface.h"
#include "port_expander.h"
#include "multiplexer.h"
#include "configuration.h"

#include "buzzer.h"
#include "button_debouncer.h"
#include "flow_sensor.h"

#include <unistd.h>
#include <sys/time.h>

#define MESSAGE_SIZE 150

#define HISTORY_SIZE 200
#define HISTORY_INTERVAL 200

class Esp32Hardware : public HardwareInterface {

public:
  void restartUptime();
  double getSecondsSinceStart() override;

  // Harware Inputs.
  bool readSensors(SensorState* state);
  const SensorState * getSensorState() override;
  void updateSensorState();

  bool readControls(ControlState* state);
  ControlState * getControlState() override;
  void updateControlState();

  const ConfigState* getConfig() override;

  Statistics * getStatistics() override;

  void setValves(bool v1, bool v2) override;

  void setGreen(bool state);
  void setRed(bool state);

  void displayAlert(const char *) override;
  void displayMessage(const char *) override;

  void standbyMode() override;
  void runningMode() override;

  void begin();
  void runTest();
  void testModeTick();
  void tick() override;

  Esp32Hardware();
  virtual ~Esp32Hardware();

  bool forceRefresh;

  ControlState controlState;
  ControlState lastControlState;

  SensorState sensorState;
  SensorState lastSensorState;

  SensorState sensorHistory[HISTORY_SIZE];

  ConfigState configState;

  Statistics statistics;

  PortExpander ports;
  Multiplexer mux;
  Buzzer buzzer;
  FlowSensor flow1;

  ButtonDebouncer start;
  ButtonDebouncer pause;


private:
  void recordHistory();
  void boxTextTop(uint8_t x, uint8_t y);
  void boxTextBottom(uint8_t x, uint8_t y);
  void drawChart(bool refresh);
  time_t bootTimestamp;

  bool showMessage;
  uint16_t messageColor;
  char message [MESSAGE_SIZE];

  unsigned long lastHistoryRecord;
  uint16_t nextHistoryIndex;
  uint16_t firstHistoryIndex;
  bool newHistoryFrame;
};

#endif  // ESP32_HARDWARE_H
