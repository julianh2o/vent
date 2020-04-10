#ifndef TIME_COUNTER_H
#define TIME_COUNTER_H

#include <sys/time.h>

//
// Timer implementation that returns fractional number of seconds since this
// class has created.
//
// NOTE: Why it is implemented this way:
// =====================================
//
// This implementation was on ESP32 with Arduino IDE with
// ARDUINOJSON_USE_DOUBLE set to 1. It also should work with FLOAT as it
// debases number of seconds by the timestamp of when the instance of
// TimeCounter created. Be aware that FLOAT precision is not enough for
// storing number of seconds since Unix Epoch. Be careful when working with
// timestamps on embedded platform.
//
class TimeCounter {

 public:
  TimeCounter();
  ~TimeCounter();

  double getSecondsSinceCreation();

 private:
  time_t base_;
};

#endif  // TIME_COUNTER_H
