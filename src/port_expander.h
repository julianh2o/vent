#ifndef PORT_EXPANDER_H
#define PORT_EXPANDER_H

#include <Wire.h>

class PortExpander {

public:
  PortExpander(uint8_t sda, uint8_t scl);
  virtual ~PortExpander();

  void begin();

  //TODO these should probaby not be ints.. u_int8s maybe?
  void pinMode(uint8_t pin, uint8_t mode);
  void digitalWrite(uint8_t pin, uint8_t value);

private:
  uint8_t sda;
  uint8_t scl;
};

#endif  // PORT_EXPANDER_H
