#ifndef FLOW_SENSOR_H
#define FLOW_SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include "wirecompatibility.h"

class FlowSensor {

public:
  FlowSensor(uint8_t sda, uint8_t scl);
  virtual ~FlowSensor();

  void begin();

  void read();

private:
  uint8_t sda;
  uint8_t scl;
};

#endif  // FLOW_SENSOR_H
