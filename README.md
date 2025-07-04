# DS130x RTC Library - Renedeboer

Deze Arduino library ondersteunt zowel de DS1302 (3-draads interface) als de DS1307 (I2C) realtimeklokken zonder gebruik van externe libraries.

Belangrijk: lees de [verkoopvoorwaarden](rtc_kit_verkoopvoorwaarden.md)

## Ondersteunde Boards

- Arduino Uno, Nano, Mega, Leonardo  
- Wemos D1 Mini (ESP8266)  
- Wemos D1 (ESP32)  
- Andere Arduino-compatibele boards

---

## Pinmapping DS1307 (I2C)

| Board                   | VCC  | GND  | SDA           | SCL           | SQW (optioneel) |
|-------------------------|-------|-------|---------------|---------------|-----------------|
| Arduino Uno / Nano      | 5V    | GND   | A4            | A5            | Meestal niet gebruikt |
| Arduino Mega 2560       | 5V    | GND   | Pin 20 (SDA)  | Pin 21 (SCL)  |                 |
| Arduino Leonardo        | 5V    | GND   | Pin 2 (SDA)   | Pin 3 (SCL)   |                 |
| Wemos D1 Mini (ESP8266) | 3.3V  | GND   | D2 (GPIO4)    | D1 (GPIO5)    |                 |
| Wemos D1 (ESP32)        | 3.3V  | GND   | GPIO21        | GPIO22        |                 |

**Tip:** ESP boards werken op 3.3V. Controleer of je RTC module dit ondersteunt.

---

## Pinmapping DS1302 (3-draads interface)

| Board                   | VCC  | GND  | RST (Reset)   | DAT (Data)    | CLK (Clock)    |
|-------------------------|-------|-------|---------------|---------------|----------------|
| Arduino Uno / Nano      | 5V    | GND   | Pin 7         | Pin 6         | Pin 5          |
| Wemos D1 Mini (ESP8266) | 3.3V  | GND   | D7 (GPIO13)   | D6 (GPIO12)   | D5 (GPIO14)    |
| Wemos D1 (ESP32)        | 3.3V  | GND   | GPIO13        | GPIO12        | GPIO14         |

Je kunt elke digitale pin gebruiken, zolang ze maar consistent zijn met je code.

---

## Tip voor het plaatsen van de CR2032-knoopcel

Plaats de CR2032-batterij met de **pluskant (+) naar boven** in de houder. Druk de cel voorzichtig omlaag totdat hij stevig vastklikt. Zorg dat de batterij **pas wordt geplaatst nadat je de module hebt aangesloten** op je microcontroller, om kortsluiting of onbedoelde ontlading te voorkomen.

> Let op: de batterij levert alleen back-upspanning voor de klokfunctie – je moet de module altijd apart voeden tijdens gebruik.

## Aansluitingsadviezen

- Verbind altijd GND van RTC module en board met elkaar.  
- Zorg voor de juiste voedingsspanning: 5V voor Arduino Uno/Nano, 3.3V voor ESP8266/ESP32 (controleer je module).  
- Voor I2C (DS1307) gebruik de standaard SDA en SCL pinnen van je board.  
- Bij twijfel gebruik level shifters om signalen veilig te maken.

---

## Voorbeelden

Download het zip bestand en voeg deze toe aan de Arduino IDE

De volgende demo's zijn beschikbaar:

- RamdumpDS1302
- RamdumpDS1307 tonen de inhoud van het RAM geheugen via de Serial Monitor
- CheckDS1307
- CheckDS1302 testen de werking van de RTC chip en tonen de resultaten via de Serial Monitor
- SetTimeRTC via de Serial Monitor is de datum/tijd in te stellen
- PrintTimeRTC toon de tijd via de Serial Monitor
- DisplayRTCTime_OLED toon de tijd op een Oled display (SSD1306 Adafruit library nodig)

Deze RTC-module is compatibel met de meeste Arduino-libraries die de DS1302 of DS1307 ondersteunen.

---

## License

MIT License

---

Voor vragen of feedback, open gerust een issue of pull request.

*Renedeboer – 2025*