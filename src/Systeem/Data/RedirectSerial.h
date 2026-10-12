#ifndef REDIRECT_SERIAL_H
#define REDIRECT_SERIAL_H

#include <Arduino.h>

struct RedirectSerial : Print {
  void begin(unsigned long);
  operator bool();
  size_t write(uint8_t byte) override;
  using Print::write;
};

#endif // REDIRECT_SERIAL_H
