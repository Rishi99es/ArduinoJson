// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#ifndef ARDUINO

#include "../../include/ArduinoJson/Arduino/Print.hpp"

#include <stdio.h>  // for sprintf()

// only for GCC 4.9+
#if defined(__GNUC__) && \
    (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9))
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#ifndef __FAST_MATH__
bool isnan(double x) { return x != x; }
bool isinf(double x) { return !isnan(x) && isnan(x - x); }
#else
#include <math.h>
#endif

size_t Print::print(const char s[]) {
  size_t n = 0;
  while (*s) {
    n += write(*s++);
  }
  return n;
}

size_t Print::print(double value, int digits) {
  // https://github.com/arduino/Arduino/blob/db8cbf24c99dc930b9ccff1a43d018c81f178535/hardware/arduino/sam/cores/arduino/Print.cpp#L218
  if (isnan(value)) return print("nan");
  if (isinf(value)) return print("inf");

  char tmp[32];

  // https://github.com/arduino/Arduino/blob/db8cbf24c99dc930b9ccff1a43d018c81f178535/hardware/arduino/sam/cores/arduino/Print.cpp#L220
  bool isBigDouble = value > 4294967040.0 || value < -4294967040.0;

  if (isBigDouble) {
    // Arduino's implementation prints "ovf"
    // We prefer trying to use scientific notation, since we have sprintf
    sprintf(tmp, "%g", value);
  } else {
    // Here we have the exact same output as Arduino's implementation
    sprintf(tmp, "%.*f", digits, value);
  }

  return print(tmp);
}

template <>
size_t Print::printInteger<int>(int value) {
  char tmp[32];
  sprintf(tmp, "%d", value);
  return print(tmp);
}

template <>
size_t Print::printInteger<long>(long value) {
  char tmp[32];
  sprintf(tmp, "%ld", value);
  return print(tmp);
}

size_t Print::print(int16_t value) { return printInteger(value); }

size_t Print::print(int32_t value) { return printInteger(value); }

size_t Print::print(int64_t value) { return printInteger(value); }

size_t Print::println() { return write('\r') + write('\n'); }

#endif
