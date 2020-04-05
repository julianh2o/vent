#include "time_counter.h"

#include <unistd.h>

TimeCounter::TimeCounter() {
  timeval tv;
  gettimeofday(&tv, NULL);
  base_ = tv.tv_sec;
}

TimeCounter::~TimeCounter() {
}

double TimeCounter::getSecondsSinceCreation() {
  //
  // IMPORTANT
  // =========
  //
  // This implementation was on ESP32 with Arduino IDE with
  // ARDUINOJSON_USE_DOUBLE set to 1. It also should work with FLOAT as it
  // debases number of seconds by the timestamp of when the instance of
  // TimeCounter created. Be aware that FLOAT precision is not enough for 
  // storing number of seconds since Unix Epoch. Be careful when working with
  // timestamps on embedded platform.
  //
  timeval tv;
  gettimeofday(&tv, NULL);
  time_t seconds_since_creation_time_t = tv.tv_sec - base_;
  double s = (double)seconds_since_creation_time_t;
  double ms = (double)(tv.tv_usec / 1000LL);
  return s + ms / 1000.0;
}
