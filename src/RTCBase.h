#ifndef RTCBASE_H
#define RTCBASE_H

#include <Arduino.h>

namespace ReneDeBoer {

struct DateTime {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t day;
  uint8_t month;
  uint16_t year;
};

class RTCBase {
public:
  virtual void begin() = 0;
  virtual void setDateTime(const DateTime& dt) = 0;
  virtual DateTime getDateTime() = 0;
};

}

#endif
