#include <Arduino.h>
#include "configuration.h"
#include "hardware_interface.h"
#include "esp32_hardware.h"
#include "port_expander.h"

Esp32Hardware h;
PortExpander ports(I2C_1_SDA,I2C_1_SCL);

void setup() {
  Serial.begin(115200);
  Serial.println("Setup begin");

  // IO_RST
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  delay(1000);
  digitalWrite(2, HIGH);

  ports.pinMode(0, OUTPUT);
  ports.pinMode(1, OUTPUT);
  ports.pinMode(2, OUTPUT);

  ports.pinMode(3, OUTPUT);
  ports.pinMode(4, OUTPUT);
}

uint8_t r = 0;
void loop() {
  Serial.println(h.getSecondsSinceStart());
  r += 1;
  if (r > 3) r = 0;

  ports.digitalWrite(0, r == 1);
  ports.digitalWrite(1, r == 2);
  ports.digitalWrite(2, r == 3);

  delay(250);

  ports.digitalWrite(4, HIGH);
  ports.digitalWrite(3, LOW);

  delay(250);

  ports.digitalWrite(4, LOW);
  ports.digitalWrite(3, HIGH);
}
