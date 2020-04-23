#include "buzzer.h"

#include "configuration.h"

#define BUZZER_FREQ1 (uint16_t)2000
#define BUZZER_RESOLUTION 8
#define BUZZER_CHANNEL 0

#define SHORT_BEEP_MS 200
#define SHORT_MUTE_MS 200

#define LONG_BEEP_MS 1000
#define LONG_MUTE_MS 2000



DottedBeeper::DottedBeeper(uint64_t beep_ms, uint64_t mute_ms) : 
  beep_ms_(beep_ms), mute_ms_(mute_ms), 
  beep_end_ms_(0), mute_end_ms_(0), buzzer_on_(false) {}

DottedBeeper::~DottedBeeper() {}

void DottedBeeper::start() {
  buzzer_on_ = true;

  sound_on_ = true;
  beep_end_ms_ = millis() + beep_ms_;

  ledcSetup(BUZZER_CHANNEL, BUZZER_FREQ1, BUZZER_RESOLUTION);
  ledcWrite(BUZZER_CHANNEL, 125);
}

void DottedBeeper::stop() {
  buzzer_on_ = false;
  sound_on_ = false;
  ledcWrite(BUZZER_CHANNEL, 0);
}

void DottedBeeper::tick() {
  if (!buzzer_on_) return;
  if (sound_on_) {
    if (millis() > beep_end_ms_) {
      sound_on_ = false;
      ledcWrite(BUZZER_CHANNEL, 0);
      mute_end_ms_ = millis() + mute_ms_;
    }
  } else {
    if (millis() > mute_end_ms_) {
      sound_on_ = true;
      ledcWrite(BUZZER_CHANNEL, 125);
      beep_end_ms_ = millis() + beep_ms_;
    }
  }
}

Buzzer::Buzzer() : 
  state_(IndicationState::OFF),
  short_beeps_(SHORT_BEEP_MS, SHORT_MUTE_MS),
  long_beeps_(LONG_BEEP_MS, LONG_MUTE_MS) {
}

Buzzer::~Buzzer() {}

void Buzzer::begin() {
  pinMode(BUZZER_PIN, OUTPUT);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
}

void Buzzer::tick() {
  if (state_ == IndicationState::SHORT_BEEPS) {
    short_beeps_.tick();
  } else if (state_ == IndicationState::LONG_BEEPS) {
    long_beeps_.tick();
  }
}

void Buzzer::setState(IndicationState::BeeperMode state) {
  if (state_ == state) return;

  if (state_ == IndicationState::SHORT_BEEPS) {
    short_beeps_.stop();
  } else if (state_ == IndicationState::LONG_BEEPS) {
    long_beeps_.stop();
  }

  state_ = state;

  if (state_ == IndicationState::SHORT_BEEPS) {
    short_beeps_.start();
  } else if (state_ == IndicationState::LONG_BEEPS) {
    long_beeps_.start();
  }
}
