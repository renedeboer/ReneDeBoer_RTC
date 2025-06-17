#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Uncomment your RTC library and init accordingly:

// For DS1302:

// #include <DS1302RTC.h>
// using ReneDeBoer::DateTime;
// using ReneDeBoer::DS1302RTC;
// DS1302RTC rtc(7, 6, 5); // RST, DAT, CLK

// For DS1307 (I2C RTC):

// #include <DS1307RTC.h>
// using ReneDeBoer::DateTime;
// using ReneDeBoer::DS1307RTC;
// DS1307RTC rtc;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Declaration for SSD1306 I2C display
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);

  // Init RTC
  rtc.begin();

  // Init OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C is typical I2C address for 128x64 OLEDs
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("RTC OLED Clock"));
  display.display();

  delay(2000);
  display.clearDisplay();
}

void loop() {
  // Get current time from RTC
  DateTime dt = rtc.getDateTime();

  char line1[32];
  char line2[32];

  // Format date YYYY-MM-DD
  snprintf(line1, sizeof(line1), "%04u-%02u-%02u", dt.year, dt.month, dt.day);
  // Format time HH:MM:SS
  snprintf(line2, sizeof(line2), "%02u:%02u:%02u", dt.hour, dt.minute, dt.second);

  Serial.println(line1);
  Serial.println(line2);

  // Show on OLED
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(0, 10);
  display.println(line1);

  display.setTextSize(2);
  display.setCursor(0, 35);
  display.println(line2);

  display.display();

  delay(1000);
}
