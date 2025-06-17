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

  if (rtc.isRunning()) {
    Serial.println("De klok loopt.");
  } else {
    Serial.println("De klok staat stil (CH-bit is gezet).");
  }

  Serial.println("Voer in: YYYY-MM-DD HH:MM:SS");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');

    unsigned int y, mo, d, h, mi, s;
    if (sscanf(input.c_str(), "%u-%u-%u %u:%u:%u", &y, &mo, &d, &h, &mi, &s) == 6) {
      DateTime dt;
      dt.year = y;
      dt.month = mo;
      dt.day = d;
      dt.hour = h;
      dt.minute = mi;
      dt.second = s;
      rtc.setDateTime(dt);
      Serial.println("Tijd ingesteld.");
    } else {
      Serial.println("Ongeldige invoer. Gebruik: YYYY-MM-DD HH:MM:SS");
    }
  }

  DateTime dt = rtc.getDateTime();
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%04u-%02u-%02u %02u:%02u:%02u", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);
  Serial.println(buffer);

  delay(1000);
}

