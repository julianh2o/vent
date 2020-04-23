#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

class Screen {

 public:
  static Screen* getInstance();

  void begin();

  void runTest();

  void clear();
  void padprint(const char * fmt, double n, uint8_t width);

  Adafruit_ILI9341 tft;

 private:
  Screen();
  Screen(Screen&);
  ~Screen();
};

#endif //SCREEN_H
