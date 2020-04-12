#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include <Arduino.h>

class Multiplexer {

public:
  Multiplexer(uint8_t mux0, uint8_t mux1, uint8_t mux2, uint8_t muxio);
  virtual ~Multiplexer();

  void begin();

  void pinMode(uint8_t mode);
  // void digitalWrite(uint8_t pin, uint8_t value);
  int analogRead(uint8_t pin);

private:
  uint8_t mux0;
  uint8_t mux1;
  uint8_t mux2;
  uint8_t muxio;
  void setMux(uint8_t pin);
};

#endif  // MULTIPLEXER_H
