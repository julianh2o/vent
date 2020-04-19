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

Esp32Hardware h;

void setup() {
  Serial.begin(115200);

  Serial.println("Setup begin");

  h.begin();
}

void tick() {
  h.tick();
}

void loop() {
  tick();
  delay(20);
}
