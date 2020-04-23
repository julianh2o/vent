#include "screen.h"

#include "configuration.h"

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"


Screen* Screen::getInstance() {
  static Screen instance;
  return &instance;
}


void Screen::begin() {
  tft.begin(20/*MHZ*/*1000000);
  tft.setRotation(1);

  tft.fillScreen(ILI9341_BLACK);
}

void Screen::runTest(void) {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
}

void Screen::clear() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
}

void Screen::padprint(const char * fmt, double n, uint8_t width) {
  char s[width+1];
  uint8_t len = sprintf(s,fmt,n);
  for (int i=len; i<width; i++) s[i] = ' ';
  s[width] = 0;
  tft.print(s);
}

Screen::Screen() : tft(LCD_CS, LCD_DC, LCD_RST) {
}

Screen::Screen(Screen&) : tft(LCD_CS, LCD_DC, LCD_RST) {
}

Screen::~Screen() {
}
