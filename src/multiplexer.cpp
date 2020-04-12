#include "multiplexer.h"
#include <Arduino.h>

//:: means look in the global scope.. because we're overloading these function names
void Multiplexer::begin() {
  ::pinMode(mux0, OUTPUT);
  ::pinMode(mux1, OUTPUT);
  ::pinMode(mux2, OUTPUT);
  ::pinMode(muxio, INPUT);
}

//We're just going to set the pin mode universally.... we could enhance this to keep track of which pins need which.. but i dont think we care right now
void Multiplexer::pinMode(uint8_t mode) {
  ::pinMode(muxio, mode);
}

int Multiplexer::analogRead(uint8_t pin) {
  setMux(pin);
  return ::analogRead(muxio);
}

//Do we need to write these pins ever..?
// void Multiplexer::digitalWrite(uint8_t pin)
//   this.setMux(pin);
//   digitalWrite(muxio);
//   pinMode(muxio, mode);
// }

Multiplexer::Multiplexer(uint8_t mux0, uint8_t mux1, uint8_t mux2, uint8_t muxio) : mux0(mux0), mux1(mux1), mux2(mux2), muxio(muxio) {
}

Multiplexer::~Multiplexer() {
}

void Multiplexer::setMux(uint8_t pin) {
  digitalWrite(mux0,1 & (pin >> 2)); //low bit
  digitalWrite(mux1,1 & (pin >> 2)); //mid bit
  digitalWrite(mux2,1 & (pin >> 2)); //High bit
}
