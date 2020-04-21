#include "port_expander.h"
#include <Arduino.h>
#include <Wire.h>
#include "configuration.h"

#define MCP23017_ADDRESS 0x24

// registers
#define MCP23017_IODIRA 0x00
#define MCP23017_IPOLA 0x02
#define MCP23017_GPINTENA 0x04
#define MCP23017_DEFVALA 0x06
#define MCP23017_INTCONA 0x08
#define MCP23017_IOCONA 0x0A
#define MCP23017_GPPUA 0x0C
#define MCP23017_INTFA 0x0E
#define MCP23017_INTCAPA 0x10
#define MCP23017_GPIOA 0x12
#define MCP23017_OLATA 0x14

#define MCP23017_IODIRB 0x01
#define MCP23017_IPOLB 0x03
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALB 0x07
#define MCP23017_INTCONB 0x09
#define MCP23017_IOCONB 0x0B
#define MCP23017_GPPUB 0x0D
#define MCP23017_INTFB 0x0F
#define MCP23017_INTCAPB 0x11
#define MCP23017_GPIOB 0x13
#define MCP23017_OLATB 0x15

#define MCP23017_INT_ERR 255

uint8_t PortExpander::readRegister(uint8_t addr){
  Wire.beginTransmission(MCP23017_ADDRESS);
  Wire.write(addr);
  Wire.endTransmission();
  Wire.requestFrom(MCP23017_ADDRESS, 1);
  return Wire.read();
}


void PortExpander::writeRegister(uint8_t regAddr, uint8_t regValue){
  Wire.beginTransmission(MCP23017_ADDRESS);
  Wire.write(regAddr);
  Wire.write(regValue);
  Wire.endTransmission();
}

//These utility functions abstract port B to pins 8-15
uint8_t PortExpander::regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr){
  return pin < 8 ? portAaddr : portBaddr;
}

uint8_t PortExpander::bitForPin(uint8_t pin) {
  return pin % 8;
}

/**
 * Helper to update a single bit of an A/B register.
 * - Reads the current register value
 * - Writes the new register value
 */
void PortExpander::updateRegisterBit(uint8_t pin, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr) {
  uint8_t regValue;
  uint8_t regAddr=regForPin(pin,portAaddr,portBaddr);
  uint8_t bit=bitForPin(pin);
  regValue = readRegister(regAddr);

  // set the value for the particular bit
  bitWrite(regValue,bit,pValue);

  writeRegister(regAddr,regValue);
}

void PortExpander::begin() {
  ::pinMode(IO_RST, OUTPUT);
  ::digitalWrite(IO_RST, LOW);
  ::digitalWrite(IO_RST, HIGH);

  Wire.begin(sda, scl);
  writeRegister(MCP23017_IODIRA, 0xff);
  writeRegister(MCP23017_IODIRB, 0xff);
}

void PortExpander::pinMode(uint8_t pin, uint8_t mode) {
  updateRegisterBit(pin,(mode==INPUT),MCP23017_IODIRA,MCP23017_IODIRB);
}

void PortExpander::digitalWrite(uint8_t pin, bool value) {
  uint8_t gpio;
  uint8_t bit=bitForPin(pin);


  // read the current GPIO output latches
  uint8_t regAddr=regForPin(pin,MCP23017_OLATA,MCP23017_OLATB);
  gpio = readRegister(regAddr);

  // set the pin and direction
  bitWrite(gpio,bit,value);

  // write the new GPIO
  regAddr=regForPin(pin,MCP23017_GPIOA,MCP23017_GPIOB);
  writeRegister(regAddr,gpio);
}

bool PortExpander::digitalRead(uint8_t pin) {
  uint8_t gpio;
  uint8_t bit=bitForPin(pin);

  uint8_t regAddr=regForPin(pin,MCP23017_GPIOA,MCP23017_GPIOB);
  gpio = readRegister(regAddr);

  return bitRead(gpio,bit);
}

PortExpander::PortExpander(uint8_t sda, uint8_t scl, uint8_t reset) : sda(sda), scl(scl), reset(reset) {
}

PortExpander::~PortExpander() {
}
