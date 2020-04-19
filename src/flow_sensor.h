#ifndef FLOW_SENSOR_H
#define FLOW_SENSOR_H

#include <Arduino.h>
#include <Wire.h>

class FlowSensor {

public:
  FlowSensor(uint8_t sda, uint8_t scl);
  virtual ~FlowSensor();


  void begin();

  void startContinuousMeasurement();

  float read();


private:
  uint8_t sda;
  uint8_t scl;

  uint8_t crc8(const uint8_t data, uint8_t crc);
};

#endif  // FLOW_SENSOR_H
