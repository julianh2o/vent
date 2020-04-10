#include <Arduino.h>
#include "hardware_interface.h"
#include "esp32_hardware.h"

// #define LED_BUILTIN 2

Esp32Hardware h;

void setup() {
  Serial.begin(115200);
  Serial.println("Setup begin");

  // pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  Serial.println(h.getSecondsSinceStart());
  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(1000);
  // digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
