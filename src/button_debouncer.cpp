#include "button_debouncer.h"

void ButtonDebouncer::begin() {
}

void ButtonDebouncer::tick(bool state) {
  bool currentState = state;
  unsigned long currentMillis = millis();

  if (currentState != lastState) {
    if (lastDebounceTime == 0) {
      lastDebounceTime = currentMillis;
    } else if (lastDebounceTime - currentMillis > debounceDelay) {
      lastState = currentState;
      lastDebounceTime = 0;
      if (isPressed()) {
        unacknowledgedPress = true;
      }
    }
  }
}

boolean ButtonDebouncer::isPressed() {
  return lastState == true;
}

boolean ButtonDebouncer::hasPressed() {
  if (unacknowledgedPress) {
    unacknowledgedPress = false;
    return true;
  }
  return false;
}

ButtonDebouncer::ButtonDebouncer() {
    lastDebounceTime = 0;
}

ButtonDebouncer::~ButtonDebouncer() {
}
