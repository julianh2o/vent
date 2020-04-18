#include <Arduino.h>
#include "configuration.h"
#include "hardware_interface.h"
#include "esp32_hardware.h"
#include "port_expander.h"
#include "multiplexer.h"
#include "utils.h"
#include "button_debouncer.h"
#include "buzzer.h"

Esp32Hardware h;

void setup() {
  Serial.begin(115200);

  Serial.println("Setup begin");

  pinMode(IO_RST, OUTPUT);
  digitalWrite(IO_RST, LOW);
  delay(1000);
  digitalWrite(IO_RST, HIGH);

  h.begin();
}

void tick(uint16_t i) {
  // if (i % 10 == 0) {
  //   ControlState controls;
  //   h.readControls(&controls);
  //
  //   out("target_switch", controls.target_switch);
  //   out("tidal_volume", controls.tidal_volume);
  //   out("inspiratory_pressure", controls.inspiratory_pressure);
  //   out("rate_assist_switch", controls.rate_assist_switch);
  //   out("respiratory_rate", controls.respiratory_rate);
  //   out("inhale_exhale_ratio", controls.inhale_exhale_ratio);
  //   out("end_inspiratory_pause_button_down", controls.end_inspiratory_pause_button_down);
  //   out("start_ack_button_down", controls.start_ack_button_down);
  //
  //   Serial.print("\n");
  //
  // }

  h.tick(i);
}

uint8_t tickCounter = 0;
void loop() {
  tick(tickCounter++);
  delay(20);
}
