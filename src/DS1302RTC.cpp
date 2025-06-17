#include "DS1302RTC.h"
#include <Arduino.h>

#define DS1302_CLOCK_BURST_READ  0xBF
#define DS1302_CLOCK_BURST_WRITE 0xBE

namespace ReneDeBoer {

// Constructor
DS1302RTC::DS1302RTC(uint8_t ce, uint8_t io, uint8_t sclk)
  : cePin(ce), ioPin(io), sclkPin(sclk) {}

uint8_t DS1302RTC::bcdToDec(uint8_t val) { return ((val / 16 * 10) + (val % 16)); }
uint8_t DS1302RTC::decToBcd(uint8_t val) { return ((val / 10 * 16) + (val % 10)); }

void DS1302RTC::begin() {
  pinMode(cePin, OUTPUT);
  pinMode(sclkPin, OUTPUT);
  pinMode(ioPin, OUTPUT);

  digitalWrite(cePin, LOW);
  digitalWrite(sclkPin, LOW);
  digitalWrite(ioPin, LOW);
}

void DS1302RTC::setDateTime(const DateTime& dt) {
  uint8_t sec = decToBcd(dt.second) & 0x7F; // CH = 0
  writeRegister(0x80, sec);                 // Seconds

  writeRegister(0x82, decToBcd(dt.minute));  // Minutes
  writeRegister(0x84, decToBcd(dt.hour));    // Hours (24-uurs)
  writeRegister(0x8A, 1);                    // Day of week (optioneel: 1 = maandag)
  writeRegister(0x86, decToBcd(dt.day));     // Day (date)
  writeRegister(0x88, decToBcd(dt.month));   // Month
  writeRegister(0x8C, decToBcd(dt.year % 100)); // Year (2 digits)
}

DateTime DS1302RTC::getDateTime() {
  DateTime dt = {0, 0, 0, 0, 0, 0};

  uint8_t secRaw = readRegister(0x81);
  dt.second = bcdToDec(secRaw & 0x7F); // CH-bit maskeren

  dt.minute = bcdToDec(readRegister(0x83));

  uint8_t rawHour = readRegister(0x85);
  if (rawHour & 0x40) {
    // 12-uurs modus
    bool isPM = rawHour & 0x20;
    dt.hour = bcdToDec(rawHour & 0x1F);
    if (isPM && dt.hour < 12) dt.hour += 12;
    if (!isPM && dt.hour == 12) dt.hour = 0;
  } else {
    dt.hour = bcdToDec(rawHour & 0x3F);
  }

  // Day of week (0x8B) wordt overgeslagen
  dt.day = bcdToDec(readRegister(0x87));
  dt.month = bcdToDec(readRegister(0x89));
  dt.year = 2000 + bcdToDec(readRegister(0x8D));

  return dt;
}

bool DS1302RTC::isRunning() {
  uint8_t secReg = readRegister(0x00);
  return !(secReg & 0x80); // CH bit moet 0 zijn
}

//================ PRIVATE METHODS

void DS1302RTC::writeRegister(uint8_t reg, uint8_t val) {
  digitalWrite(cePin, HIGH);
  delayMicroseconds(4);

  reg &= 0xFE; // LSB=0 schrijfbit
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(ioPin, (reg >> i) & 1);
    digitalWrite(sclkPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(sclkPin, LOW);
    delayMicroseconds(1);
  }

  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(ioPin, (val >> i) & 1);
    digitalWrite(sclkPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(sclkPin, LOW);
    delayMicroseconds(1);
  }

  digitalWrite(cePin, LOW);
}

uint8_t DS1302RTC::readRegister(uint8_t reg) {
  uint8_t val = 0;

  digitalWrite(cePin, HIGH);
  delayMicroseconds(4);

  reg |= 0x01; // LSB=1 leesbit
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(ioPin, (reg >> i) & 1);
    digitalWrite(sclkPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(sclkPin, LOW);
    delayMicroseconds(1);
  }

  pinMode(ioPin, INPUT);

  for (uint8_t i = 0; i < 8; i++) {
    if (digitalRead(ioPin)) {
      val |= (1 << i);
    }
    digitalWrite(sclkPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(sclkPin, LOW);
    delayMicroseconds(1);
  }

  pinMode(ioPin, OUTPUT);
  digitalWrite(ioPin, LOW);
  digitalWrite(cePin, LOW);

  return val;
}


}
