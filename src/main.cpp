#include <Arduino.h>
#include "configuration.h"
#include "hardware_interface.h"
#include "esp32_hardware.h"
#include "port_expander.h"
#include "multiplexer.h"
#include "utils.h"

Esp32Hardware h;

void setup() {
  Serial.begin(115200);

  Serial.println("Setup begin");

  // IO_RST
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  delay(1000);
  digitalWrite(2, HIGH);

  h.runTest();
}

uint8_t i = 0;
void loop() {
  ControlState controls;
  h.readControls(&controls);

  out("target_switch", controls.target_switch);
  out("tidal_volume", controls.tidal_volume);
  out("inspiratory_pressure", controls.inspiratory_pressure);
  out("rate_assist_switch", controls.rate_assist_switch);
  out("respiratory_rate", controls.respiratory_rate);
  out("inhale_exhale_ratio", controls.inhale_exhale_ratio);
  out("end_inspiratory_pause_button_down", controls.end_inspiratory_pause_button_down);
  out("start_ack_button_down", controls.start_ack_button_down);

  Serial.print("\n");

  IndicationState indicate;
  indicate.status_led_mode = i % 2 == 0 ? IndicationState::SOLID_RED : IndicationState::SOLID_GREEN;
  h.writeIndication(indicate);

  i++;
  delay(1000);
}
