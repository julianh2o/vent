#ifndef BUTTON_DEBOUNCER_H
#define BUTTON_DEBOUNCER_H

#include <Arduino.h>

const int debounceDelay = 50;

class ButtonDebouncer {

public:
  ButtonDebouncer();
  virtual ~ButtonDebouncer();

  void begin();

  //True means the button is pressed
  void tick(bool state);

  //Returns true only once for each transition to pushed state
  bool hasPressed();

  //Returns true if the button is pressed down
  bool isPressed();

private:
  bool buttonState;
  bool lastState;
  bool unacknowledgedPress;
  unsigned long lastDebounceTime;
};

#endif  // BUTTON_DEBOUNCER_H
