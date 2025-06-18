// DS1302 testscript voor authenticiteit
const int DS1302_CE   = 7;  // RST
const int DS1302_IO   = 6;
const int DS1302_SCLK = 5;

void setup() {
  Serial.begin(9600);
  initPins();

  Serial.println("=== DS1302 Authenticiteitstest ===");

  // Zet write-protect UIT
  writeRegister(0x8E, 0x00);

  // Zet de klok aan (CH-bit = 0)
  uint8_t sec = readRegister(0x80);
  sec &= 0x7F;  // CH-bit uit
  sec |= 0x00;
  writeRegister(0x80, sec);

  delay(1000); // wacht 1 seconde

  uint8_t sec1 = bcdToDec(readRegister(0x80) & 0x7F);
  delay(2000);
  uint8_t sec2 = bcdToDec(readRegister(0x80) & 0x7F);

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

  // Test write protect
  writeRegister(0x8E, 0x80); // schrijfbeveiliging AAN
  writeRegister(0x80, decToBcd(55)); // poging tot wijzigen
  uint8_t sec3 = bcdToDec(readRegister(0x80) & 0x7F);

  Serial.print("Seconden na write protect poging: ");
  Serial.println(sec3);

  if (sec3 == 55) {
    Serial.println("❌ Write Protect FAALT — chip respecteert register 0x8E niet.");
  } else {
    Serial.println("✅ Write Protect werkt.");
  }
writeRegister(0x8E, 0x00);
  // RAM test
  Serial.println("\nRAM schrijf/lees test:");
  bool ramOk = true;
  for (uint8_t addr = 0xC0; addr <= 0xFC; addr += 2) {
    writeRegister(addr, addr ^ 0x5A);  // herkenbaar patroon
    uint8_t val = readRegister(addr);
    if (val != (addr ^ 0x5A)) {
      Serial.print("❌ RAM-fout op adres 0x");
      Serial.print(addr, HEX);
      Serial.print(": gelezen ");
      Serial.print(val, HEX);
      Serial.print(" verwacht ");
      Serial.println((addr ^ 0x5A), HEX);
      ramOk = false;
    }
  }
  if (ramOk) Serial.println("✅ RAM werkt correct.");
  else Serial.println("❌ RAM vertoont fouten.");

}

void loop() {
  // niets
}

// -------------------- Hulpfuncties -------------------------

void initPins() {
  pinMode(DS1302_CE, OUTPUT);
  pinMode(DS1302_SCLK, OUTPUT);
  digitalWrite(DS1302_CE, LOW);
  digitalWrite(DS1302_SCLK, LOW);
}

void writeByte(uint8_t data) {
  pinMode(DS1302_IO, OUTPUT);
  for (int i = 0; i < 8; i++) {
    digitalWrite(DS1302_IO, (data >> i) & 1);
    digitalWrite(DS1302_SCLK, HIGH);
    digitalWrite(DS1302_SCLK, LOW);
  }
}

uint8_t readByte() {
  uint8_t val = 0;
  pinMode(DS1302_IO, INPUT);
  for (int i = 0; i < 8; i++) {
    if (digitalRead(DS1302_IO)) {
      val |= (1 << i);
    }
    digitalWrite(DS1302_SCLK, HIGH);
    digitalWrite(DS1302_SCLK, LOW);
  }
  return val;
}

void writeRegister(uint8_t addr, uint8_t data) {
  digitalWrite(DS1302_CE, HIGH);
  delayMicroseconds(4);
  writeByte(addr & 0xFE); // schrijf, LSB=0
  writeByte(data);
  digitalWrite(DS1302_CE, LOW);
  delayMicroseconds(4);
}

uint8_t readRegister(uint8_t addr) {
  uint8_t data;
  digitalWrite(DS1302_CE, HIGH);
  delayMicroseconds(4);
  writeByte(addr | 0x01); // lees, LSB=1
  data = readByte();
  digitalWrite(DS1302_CE, LOW);
  delayMicroseconds(4);
  return data;
}

uint8_t decToBcd(uint8_t val) {
  return ((val / 10 * 16) + (val % 10));
}

uint8_t bcdToDec(uint8_t val) {
  return ((val / 16 * 10) + (val % 16));
}
