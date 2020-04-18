#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

void out(const char text[], int v) {
  Serial.print(text);
  Serial.print(": ");
  Serial.print(v);
  Serial.print("\n");
}

void out(const char text[], double v) {
  Serial.print(text);
  Serial.print(": ");
  Serial.print(v);
  Serial.print("\n");
}

void out(const char text[], bool v) {
  Serial.print(text);
  Serial.print(": ");
  Serial.print(v);
  Serial.print("\n");
}

#endif //UTILS_H
