#ifndef PORT_EXPANDER_H
#define PORT_EXPANDER_H

#include <Arduino.h>
#include <Wire.h>

class PortExpander {

public:
  PortExpander(uint8_t sda, uint8_t scl, uint8_t reset);
  virtual ~PortExpander();

  void begin();

  void pinMode(uint8_t pin, uint8_t mode);
  void digitalWrite(uint8_t pin, bool value);
  bool digitalRead(uint8_t pin);

private:
  uint8_t sda;
  uint8_t scl;
  uint8_t reset;

  uint8_t readRegister(uint8_t addr);
  void writeRegister(uint8_t regAddr, uint8_t regValue);
  uint8_t regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr);
  uint8_t bitForPin(uint8_t pin);
  void updateRegisterBit(uint8_t pin, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr);
};

#endif  // PORT_EXPANDER_H
