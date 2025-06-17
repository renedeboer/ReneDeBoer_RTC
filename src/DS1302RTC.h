#ifndef DS1302RTC_H
#define DS1302RTC_H

#include "RTCBase.h"

namespace ReneDeBoer {

class DS1302RTC : public RTCBase {
private:
  uint8_t cePin;
  uint8_t ioPin;
  uint8_t sclkPin;

  uint8_t bcdToDec(uint8_t val);
  uint8_t decToBcd(uint8_t val);

  void writeRegister(uint8_t reg, uint8_t val);
  uint8_t readRegister(uint8_t reg);

public:
  DS1302RTC(uint8_t ce, uint8_t io, uint8_t sclk); // Constructor

  void begin() override;
  void setDateTime(const DateTime& dt) override;
  DateTime getDateTime() override;
  bool isRunning();
};

}

#endif
