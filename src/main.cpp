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

  h.begin();
}

void tick() {
  h.tick();
  h.testModeTick(); //Sets the indicator based on switch, beeps when buttons are pushed for demoing
}

void loop() {
  tick();
  delay(20);
}
