#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

class Screen {

public:
  Screen(uint8_t sda, uint8_t scl);
  virtual ~Screen();

  void begin();

  void runTest();

  Adafruit_ILI9341 tft;
};

#endif //SCREEN_H
