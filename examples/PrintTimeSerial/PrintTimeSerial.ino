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

void setup() {
  Serial.begin(9600);
  rtc.begin();
}

void loop() {
DateTime dt = rtc.getDateTime();
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%04u-%02u-%02u %02u:%02u:%02u", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);
  Serial.println(buffer);

  delay(1000);
}
