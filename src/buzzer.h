#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "hardware_interface.h"


class DottedBeeper {
 public:
  DottedBeeper(uint64_t beep_ms, uint64_t mute_ms);
  ~DottedBeeper();

  void start();
  void stop();
  void tick();

 private:
  uint64_t beep_ms_;
  uint64_t mute_ms_;
  bool buzzer_on_;
  bool sound_on_;
  uint64_t beep_end_ms_;
  uint64_t mute_end_ms_;
};


class Buzzer {

public:
  Buzzer();
  ~Buzzer();

  void begin();
  void setState(IndicationState::BeeperMode state);
  void tick();

private:
  IndicationState::BeeperMode state_;

  DottedBeeper short_beeps_;
  DottedBeeper long_beeps_;

};

#endif  // BUZZER_H
