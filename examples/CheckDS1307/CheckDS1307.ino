
// DS1307 authenticiteitstest via I2C
#include <Wire.h>
#define DS1307_ADDRESS 0x68

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("=== DS1307 Authenticiteitstest ===");

delay(1000); // wacht 1 seconde
// Controleer CH-bit en start klok indien nodig
uint8_t sec_reg = readRegister(0x00);
if (sec_reg & 0x80) {
  Serial.println("⚠️ CH-bit is gezet. Klok staat stil. Start wordt geprobeerd...");
  writeRegister(0x00, sec_reg & 0x7F); // CH-bit wissen → klok starten
  delay(100); // even wachten op oscillator
} else {
  Serial.println("ℹ️ CH-bit is niet gezet. Klok zou moeten lopen.");
}


  // Lees secondenregister
  uint8_t sec1 = readRegister(0x00) & 0x7F; // Mask CH-bit
  delay(2000);
  uint8_t sec2 = readRegister(0x00) & 0x7F;

  Serial.print("Seconden na 2s: ");
  Serial.print(bcdToDec(sec1));
  Serial.print(" → ");
  Serial.println(bcdToDec(sec2));

  uint8_t diff = (sec2 + 60 - sec1) % 60;

if (diff >= 1 && diff <= 3) {
    Serial.println("✅ Klok loopt.");
  } else {
    Serial.println("❌ Klok loopt niet — mogelijk nep of oscillatorprobleem.");
  }

  // RAM test (adresbereik 0x08 t/m 0x3F)
  Serial.println("\nRAM test:");

  // schrijf testwaarden
  for (uint8_t addr = 0x08; addr <= 0x3F; addr++) {
    writeRegister(addr, addr ^ 0x5A);
  }

  // lees terug en verifieer
  bool ram_ok = true;
  for (uint8_t addr = 0x08; addr <= 0x3F; addr++) {
    uint8_t val = readRegister(addr);
    uint8_t expected = addr ^ 0x5A;
    if (val != expected) {
      Serial.print("❌ RAM fout @ 0x");
      Serial.print(addr, HEX);
      Serial.print(": verwacht ");
      Serial.print(expected, HEX);
      Serial.print(", gelezen ");
      Serial.println(val, HEX);
      ram_ok = false;
    }
  }

  if (ram_ok) {
    Serial.println("✅ RAM test geslaagd.");
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

void writeRegister(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

uint8_t bcdToDec(uint8_t val) {
  return ((val / 16 * 10) + (val % 16));
}
