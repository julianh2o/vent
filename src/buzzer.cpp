#include "buzzer.h"

const uint16_t freq = 2000;
const uint8_t ledChannel = 0;
const uint8_t resolution = 8;

void Buzzer::begin() {
  pinMode(pin, OUTPUT);
  ledcAttachPin(pin, channel);
}

void Buzzer::beep(uint16_t duration) {
  ledcSetup(channel, freq, resolution);
  ledcWrite(channel, 125);
  beepUntil = millis() + duration;
}

void Buzzer::shortBeep() {
  beep(200);
}

void Buzzer::longBeep() {
  beep(1000);
}

void Buzzer::tick() {
  if (beepUntil != 0 && millis() > beepUntil) {
    ledcWrite(channel, 0);
  }
}

Buzzer::Buzzer(uint8_t pin, uint8_t channel) : pin(pin), channel(channel) {
  beepUntil = 0;
}

Buzzer::~Buzzer() {
}
