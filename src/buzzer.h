#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class DottedBeeper {
 public:
  DottedBeeper(uint8_t channel, uint64_t beep_ms, uint64_t mute_ms);
  ~DottedBeeper();

  void start();
  void stop();
  void tick();

 private:
  uint8_t channel;
  uint64_t beep_ms_;
  uint64_t mute_ms_;
  uint64_t beep_end_ms_;
  uint64_t mute_end_ms_;
  bool buzzer_on_;
  bool sound_on_;
};


class Buzzer {

public:
  Buzzer(uint8_t pin, uint8_t channel);
  ~Buzzer();

  void begin();
  void tick();

  void off();
  void shortBeeps();
  void longBeeps();

private:
  uint8_t pin;
  uint8_t channel;

  DottedBeeper short_beeps_;
  DottedBeeper long_beeps_;

};

#endif  // BUZZER_H
