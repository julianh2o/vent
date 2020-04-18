#ifndef WIRECOMPATIBILITY_H
#define WIRECOMPATIBILITY_H

#include <Wire.h>

// minihelper to keep Arduino backward compatibility
static inline void wiresend(uint8_t x) {
#if ARDUINO >= 100
  Wire.write((uint8_t) x);
#else
  Wire.send(x);
#endif
}

static inline uint8_t wirerecv(void) {
#if ARDUINO >= 100
  return Wire.read();
#else
  return Wire.receive();
#endif
}

#endif  // WIRECOMPATIBILITY_H
