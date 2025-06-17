#ifndef DS1307RTC_H
#define DS1307RTC_H

#include <Wire.h>
#include "RTCBase.h"

namespace ReneDeBoer {



class DS1307RTC : public RTCBase {
private:
  uint8_t bcdToDec(uint8_t val);
  uint8_t decToBcd(uint8_t val);




public:
  void begin() override;
  void setDateTime(const DateTime& dt) override;
  DateTime getDateTime() override;
  bool isRunning();
};

}

#endif
