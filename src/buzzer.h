#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {

public:
  Buzzer(uint8_t pin, uint8_t channel);
  virtual ~Buzzer();

  void begin();

  void beep(uint16_t length);
  void shortBeep();
  void longBeep();

  void tick();

private:
  uint8_t pin;
  uint8_t channel;

  //0 means we're not beeping, anything else means we should keep beeping until then
  unsigned long beepUntil;
};

#endif  // BUZZER_H
