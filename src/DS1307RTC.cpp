#include "DS1307RTC.h"
#include <Wire.h>

namespace ReneDeBoer {

#define DS1307_ADDRESS 0x68


uint8_t DS1307RTC::bcdToDec(uint8_t val) { return ((val / 16 * 10) + (val % 16)); }
uint8_t DS1307RTC::decToBcd(uint8_t val) { return ((val / 10 * 16) + (val % 10)); }

void DS1307RTC::begin() {
  Wire.begin();
}







void DS1307RTC::setDateTime(const DateTime& dt) {
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(0);
  Wire.write(decToBcd(dt.second) & 0x7F); // Start klok (CH bit = 0)
  Wire.write(decToBcd(dt.minute));
  Wire.write(decToBcd(dt.hour)); // 24-uursmodus
  Wire.write(decToBcd(0));       // dag van de week (optioneel)
  Wire.write(decToBcd(dt.day));
  Wire.write(decToBcd(dt.month));
  Wire.write(decToBcd(dt.year % 100));
  Wire.endTransmission();
}



DateTime DS1307RTC::getDateTime() {
  DateTime dt = {0, 0, 0, 0, 0, 0};

  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  if (Wire.requestFrom(DS1307_ADDRESS, 7) < 7) {
    return dt; // fout of onvolledige data
  }

  uint8_t rawSecond = Wire.read();
  dt.second = bcdToDec(rawSecond & 0x7F);
  dt.minute = bcdToDec(Wire.read());

  uint8_t rawHour = Wire.read();
  if (rawHour & 0x40) {
    // 12-uursmodus
    bool isPM = rawHour & 0x20;
    dt.hour = bcdToDec(rawHour & 0x1F);
    if (isPM && dt.hour < 12) dt.hour += 12;
    if (!isPM && dt.hour == 12) dt.hour = 0; // 12 AM = 0
  } else {
    dt.hour = bcdToDec(rawHour & 0x3F);
  }

  Wire.read(); // skip dag van de week
  dt.day = bcdToDec(Wire.read());
  dt.month = bcdToDec(Wire.read());
  dt.year = 2000 + bcdToDec(Wire.read());

  return dt;
}


bool DS1307RTC::isRunning() {
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 1);
  if (Wire.available()) {
    uint8_t ch = Wire.read();
    return !(ch & 0x80); // CH-bit (bit 7) moet 0 zijn om te lopen
  }
  return false; // Fout
}


}