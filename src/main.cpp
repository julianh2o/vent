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

Esp32Hardware hardware;
Vent vent(&hardware);

void setup() {
  Serial.begin(115200);
  hardware.begin();
}

void loop() {
  vent.tick();
  delay(20);
}
