// DS1302 pin configuration (adjust if needed)
const int DS1302_CE   = 7;  // RST
const int DS1302_IO   = 6;
const int DS1302_SCLK = 5;

void setup() {
  Serial.begin(9600);
  initPins();
  Serial.println("DS1302 Register Dump:\n");

  // Clock & control registers
  for (uint8_t addr = 0x80; addr <= 0x8F; addr += 2) {
    uint8_t val = readRegister(addr);
    printRegister(addr, val, getRegisterName(addr));
  }

  // Trickle charger register
  uint8_t trickle = readRegister(0x90);
  printRegister(0x90, trickle, getRegisterName(0x90));

  // RAM (31 bytes: 0xC0 to 0xFC, step 2)
  Serial.println("\nDS1302 RAM Dump:");
  for (uint8_t addr = 0xC0; addr <= 0xFC; addr += 2) {
    uint8_t val = readRegister(addr);
    printRegister(addr, val, getRegisterName(addr));
  }
}

void loop() {
  uint8_t sec = readRegister(0x80) & 0x7F; // Mask CH bit
  Serial.print("Seconds: ");
  Serial.println(sec);
  delay(1000);
}

void initPins() {
  pinMode(DS1302_CE, OUTPUT);
  pinMode(DS1302_SCLK, OUTPUT);
  digitalWrite(DS1302_CE, LOW);
  digitalWrite(DS1302_SCLK, LOW);
}

// Write 8 bits LSB first
void writeByte(uint8_t data) {
  pinMode(DS1302_IO, OUTPUT);
  for (int i = 0; i < 8; i++) {
    digitalWrite(DS1302_IO, (data >> i) & 0x01);
    digitalWrite(DS1302_SCLK, HIGH);
    digitalWrite(DS1302_SCLK, LOW);
  }
}

// Read 8 bits LSB first
uint8_t readByte() {
  uint8_t data = 0;
  pinMode(DS1302_IO, INPUT);
  for (int i = 0; i < 8; i++) {
    if (digitalRead(DS1302_IO)) {
      data |= (1 << i);
    }
    digitalWrite(DS1302_SCLK, HIGH);
    digitalWrite(DS1302_SCLK, LOW);
  }
  return data;
}

// Read DS1302 register
uint8_t readRegister(uint8_t addr) {
  pinMode(DS1302_IO, OUTPUT);
  digitalWrite(DS1302_CE, HIGH);
  delayMicroseconds(4);
  writeByte(addr | 0x01); // LSB = 1 for read
  uint8_t value = readByte();
  digitalWrite(DS1302_CE, LOW);
  delayMicroseconds(4);
  return value;
}

// Print register content with label
void printRegister(uint8_t addr, uint8_t value, const char* label) {
  Serial.print("Addr 0x");
  if (addr < 0x10) Serial.print("0");
  Serial.print(addr, HEX);
  Serial.print(": 0b");
  for (int i = 7; i >= 0; i--) {
    Serial.print(bitRead(value, i));
  }
  Serial.print("  0x");
  if (value < 0x10) Serial.print("0");
  Serial.print(value, HEX);
  Serial.print("  ");
  Serial.println(label);
}

// Return name of known DS1302 register
const char* getRegisterName(uint8_t addr) {
  switch (addr) {
    case 0x80: return "Seconds";
    case 0x82: return "Minutes";
    case 0x84: return "Hours";
    case 0x86: return "Date (Day of Month)";
    case 0x88: return "Month";
    case 0x8A: return "Day of Week";
    case 0x8C: return "Year";
    case 0x8E: return "Control";
    case 0x90: return "Trickle Charger";
    case 0x92: return "Clock Burst";  // write burst (not typically readable)
    case 0x94: return "RAM Burst";    // write burst (not typically readable)
    default:
      if (addr >= 0xC0 && addr <= 0xFC)
        return "User RAM";
      return "(unknown)";
  }
}
