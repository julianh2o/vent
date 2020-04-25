#include "buzzer.h"

#define BUZZER_FREQ1 (uint16_t)2000
#define BUZZER_RESOLUTION 8

#define SHORT_BEEP_MS 200
#define SHORT_MUTE_MS 200

#define LONG_BEEP_MS 1000
#define LONG_MUTE_MS 2000

DottedBeeper::DottedBeeper(uint8_t channel, uint64_t beep_ms, uint64_t mute_ms) :
  channel(channel),
  beep_ms_(beep_ms), mute_ms_(mute_ms),
  beep_end_ms_(0), mute_end_ms_(0), buzzer_on_(false), sound_on_(false) {}

DottedBeeper::~DottedBeeper() {}

void DottedBeeper::start() {
  buzzer_on_ = true;
  sound_on_ = true;

  beep_end_ms_ = millis() + beep_ms_;

  ledcSetup(channel, BUZZER_FREQ1, BUZZER_RESOLUTION);
  ledcWrite(channel, 125);
}

void DottedBeeper::stop() {
  if (!buzzer_on_) return;
  buzzer_on_ = false;
  sound_on_ = false;
  ledcWrite(channel, 0);
}

void DottedBeeper::tick() {
  if (!buzzer_on_) return;
  if (sound_on_) {
    if (millis() > beep_end_ms_) {
      sound_on_ = false;
      ledcWrite(channel, 0);
      mute_end_ms_ = millis() + mute_ms_;
    }
  } else {
    if (millis() > mute_end_ms_) {
      sound_on_ = true;
      ledcWrite(channel, 125);
      beep_end_ms_ = millis() + beep_ms_;
    }
  }
}

Buzzer::Buzzer(uint8_t pin, uint8_t channel) :
  pin(pin), channel(channel),
  short_beeps_(channel,SHORT_BEEP_MS, SHORT_MUTE_MS),
  long_beeps_(channel,LONG_BEEP_MS, LONG_MUTE_MS) {
}

Buzzer::~Buzzer() {}

void Buzzer::begin() {
  pinMode(pin, OUTPUT);
  ledcAttachPin(pin, channel);
}

void Buzzer::tick() {
  short_beeps_.tick();
  long_beeps_.tick();
}

void Buzzer::off() {
  short_beeps_.stop();
  long_beeps_.stop();
}

void Buzzer::shortBeeps() {
  short_beeps_.start();
  long_beeps_.stop();
}

void Buzzer::longBeeps() {
  short_beeps_.stop();
  long_beeps_.start();
}
