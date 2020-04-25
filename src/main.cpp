#include <Arduino.h>
#include "configuration.h"
#include "hardware_interface.h"
#include "esp32_hardware.h"
#include "port_expander.h"
#include "multiplexer.h"
#include "utils.h"
#include "button_debouncer.h"
#include "buzzer.h"
#include "flow_sensor.h"
#include "vent.h"

//If true, we dont run the state machine, we just test the hardware
#define HARDWARE_TEST false


Esp32Hardware hardware;
Vent vent(&hardware);

void setup() {
  Serial.begin(115200);
  hardware.begin();
}

void loop() {
  if (HARDWARE_TEST) {
    hardware.tick();
    hardware.testModeTick();
  } else {
    vent.tick();
  }
  delay(20);
}
