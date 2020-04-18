#include "flow_sensor.h"
#include <Arduino.h>
#include <Wire.h>

#define FLOW_ADDRESS 0x40

/**
 * Reads a given register
 */
// uint8_t readRegister(uint8_t addr){
//   // read the current GPINTEN
//   Wire.beginTransmission(FLOW_ADDRESS);
//   wiresend(addr);
//   Wire.endTransmission();
//   Wire.requestFrom(FLOW_ADDRESS, 1);
//   return wirerecv();
// }
//
//
// /**
//  * Writes a given register
//  */
// void writeRegister(uint8_t regAddr, uint8_t regValue){
//   // Write the register
//   Wire.beginTransmission(FLOW_ADDRESS);
//   wiresend(regAddr);
//   wiresend(regValue);
//   Wire.endTransmission();
// }

//////////////////////////////////////////////

void FlowSensor::begin() {
  Wire.begin(sda, scl);
}

void FlowSensor::read() {
}

FlowSensor::FlowSensor(uint8_t port_sda, uint8_t port_scl) : sda(port_sda), scl(port_scl) {
}

FlowSensor::~FlowSensor() {
}
