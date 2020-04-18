#ifndef PORT_EXPANDER_H
#define PORT_EXPANDER_H

#include <Arduino.h>
#include <Wire.h>
#include "wirecompatibility.h"

class PortExpander {

public:
  PortExpander(uint8_t sda, uint8_t scl);
  virtual ~PortExpander();

  void begin();

  void pinMode(uint8_t pin, uint8_t mode);
  void digitalWrite(uint8_t pin, bool value);
  bool digitalRead(uint8_t pin);

private:
  uint8_t sda;
  uint8_t scl;
};

#endif  // PORT_EXPANDER_H
