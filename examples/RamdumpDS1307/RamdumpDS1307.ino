
// DS1307 register & RAM dump via I2C
#include <Wire.h>
#define DS1307_ADDRESS 0x68

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("=== DS1307 Registerdump ===");

  // Registers 0x00 - 0x07
  const char* names[] = {
    "Seconds", "Minutes", "Hours", "Day", "Date", "Month", "Year", "Control"
  };
  for (uint8_t i = 0; i <= 7; i++) {
    uint8_t val = readRegister(i);
    Serial.print("0x");
    Serial.print(i, HEX);
    Serial.print(" ");
    Serial.print(names[i]);
    Serial.print(": 0x");
    Serial.print(val, HEX);
    Serial.print(" (");
    Serial.print(bcdToDec(val));
    Serial.println(")");
  }

  Serial.println("\n=== DS1307 RAM Dump (0x08–0x3F) ===");
  for (uint8_t i = 0x08; i <= 0x3F; i++) {
    uint8_t val = readRegister(i);
    Serial.print("0x");
    if (i < 0x10) Serial.print("0");
    Serial.print(i, HEX);
    Serial.print(": 0x");
    if (val < 0x10) Serial.print("0");
    Serial.println(val, HEX);
  }
}

void loop() {}

uint8_t readRegister(uint8_t reg) {
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDRESS, 1);
  return Wire.available() ? Wire.read() : 0xFF;
}

uint8_t bcdToDec(uint8_t val) {
  return ((val / 16 * 10) + (val % 16));
}
