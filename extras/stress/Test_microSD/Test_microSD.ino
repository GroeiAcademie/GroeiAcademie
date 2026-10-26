/*
Test_microSD.ino bevat nu 7 stresstests:
1. Native #1 alleen
2. Native #2 alleen
3. PCF8574-CS alleen
4. Native #1 + Native #2
5. Native #2 + PCF8574-CS
6. Native #1 + PCF8574-CS
7. Native #1 + Native #2 + PCF8574-CS

microSD #1 Native
SCK  -> D13
MISO -> D12
MOSI -> D11
CS   -> D10

microSD #2 Native
SCK  -> D13
MISO -> D12
MOSI -> D11
CS   -> D9

microSD #3 via PCF8574
SCK  -> D13
MISO -> D12
MOSI -> D11
CS   -> PCF8574 P0

Diagnose volgens de officiële SdFat 2.3.1-opbouw:
1. cardBegin(): kaart / SPI
2. volumeBegin(): bestandssysteem
3. open(): bestand

Bij een fout in laag 1 gebruikt de test printSdError().
Bij een fout in laag 2 gebruikt de test errorPrint().
Na een mislukte cardBegin() of volumeBegin() wordt end() aangeroepen voordat de volgende kaart wordt getest.
*/
#include <SPI.h>
#include <Wire.h>
#include <SdFat.h>
#include <PCF8574.h>

#if defined(ARDUINO_AVR_UNO)
#error Test_microSD wordt niet uitgevoerd op Arduino UNO R3.
#endif

#if SD_CHIP_SELECT_MODE != 1
#error SD_CHIP_SELECT_MODE moet in SdFatConfig.h op 1 staan voor deze test.
#endif

const uint8_t SD_CS_NATIVE_1 = 10;
const uint8_t SD_CS_NATIVE_2 = 9;
const SdCsPin_t SD_CS_EXTENDER = 0xFE;

const uint8_t PCF8574_ADRES = 0x20;
const uint8_t PCF8574_CS_PIN = 0;

const uint32_t AANTAL_BLOKKEN = 128;
const uint16_t BLOK_GROOTTE = 512;

PCF8574 pcf8574(PCF8574_ADRES);

SdFat32 sdNative1;
SdFat32 sdNative2;
SdFat32 sdExtender;

// Gecorrigeerd: Unieke buffers per kaart om contaminatie te voorkomen
uint8_t schrijfBuffer1[BLOK_GROOTTE];
uint8_t schrijfBuffer2[BLOK_GROOTTE];
uint8_t schrijfBuffer3[BLOK_GROOTTE];
uint8_t leesBuffer[BLOK_GROOTTE];

void sdCsInit(SdCsPin_t pin) {
  if (pin == SD_CS_EXTENDER) {
    pcf8574.write(PCF8574_CS_PIN, HIGH);
    return;
  }

  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
}

void sdCsWrite(SdCsPin_t pin, bool level) {
  if (pin == SD_CS_EXTENDER) {
    pcf8574.write(PCF8574_CS_PIN, level ? HIGH : LOW);
    return;
  }

  digitalWrite(pin, level ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Wire.begin();
  Wire.setClock(400000); // Gecorrigeerd: Voorkom SPI-timeouts door I2C te versnellen naar 400kHz

  if (!pcf8574.begin(0xFF)) {
    Serial.println("PCF8574: NIET GEVONDEN");
    return;
  }

  sdCsInit(SD_CS_NATIVE_1);
  sdCsInit(SD_CS_NATIVE_2);
  sdCsInit(SD_CS_EXTENDER);

  // Initialiseer de buffers met een unieke basiswaarde per kaart
  for (uint16_t i = 0; i < BLOK_GROOTTE; i++) {
    schrijfBuffer1[i] = static_cast<uint8_t>(i);
    schrijfBuffer2[i] = static_cast<uint8_t>(i + 1);
    schrijfBuffer3[i] = static_cast<uint8_t>(i + 2);
  }

  Serial.println();
  Serial.println("=== microSD #1: Native CS via eigen sdCsWrite() ===");

  sdCsWrite(SD_CS_NATIVE_1, HIGH);
  sdCsWrite(SD_CS_NATIVE_2, HIGH);
  sdCsWrite(SD_CS_EXTENDER, HIGH);
  if (!sdNative1.cardBegin(SdSpiConfig(SD_CS_NATIVE_1, SHARED_SPI, SD_SCK_MHZ(4)))) {
    Serial.println("INIT: FOUT IN LAAG 1, KAART/SPI");
    sdNative1.printSdError(&Serial);
    sdNative1.end();
  } else if (!sdNative1.volumeBegin()) {
    Serial.println("INIT: FOUT IN LAAG 2, BESTANDSSYSTEEM");
    sdNative1.errorPrint(&Serial);
    sdNative1.end();
  } else {
    File32 bestand = sdNative1.open("TEST.BIN", O_RDWR | O_CREAT | O_TRUNC);
    if (!bestand) {
      Serial.println("OPEN: FOUT IN LAAG 3, BESTAND");
    } else {
      uint32_t start = micros();
      bool schrijfFout = false;

      for (uint32_t blok = 0; blok < AANTAL_BLOKKEN; blok++) {
        if (bestand.write(schrijfBuffer1, BLOK_GROOTTE) != BLOK_GROOTTE) {
          schrijfFout = true;
          break;
        }
      }

      bestand.flush();
      uint32_t schrijfTijd = micros() - start;

      bestand.rewind();

      start = micros();
      bool leesFout = false;
      bool inhoudFout = false;

      for (uint32_t blok = 0; blok < AANTAL_BLOKKEN; blok++) {
        if (bestand.read(leesBuffer, BLOK_GROOTTE) != BLOK_GROOTTE) {
          leesFout = true;
          break;
        }

        for (uint16_t i = 0; i < BLOK_GROOTTE; i++) {
          if (leesBuffer[i] != schrijfBuffer1[i]) {
            inhoudFout = true;
            break;
          }
        }

        if (inhoudFout) {
          break;
        }
      }

      uint32_t leesTijd = micros() - start;
      bestand.close();

      Serial.print("WRITE: ");
      Serial.println(schrijfFout ? "FOUT" : "OK");
      Serial.print("READ: ");
      Serial.println(leesFout ? "FOUT" : "OK");
      Serial.print("VERIFY: ");
      Serial.println(inhoudFout ? "FOUT" : "OK");
      Serial.print("WRITE us: ");
      Serial.println(schrijfTijd);
      Serial.print("READ us: ");
      Serial.println(leesTijd);
    }
  }

  Serial.println();
  Serial.println("=== microSD #2: tweede Native CS via eigen sdCsWrite() ===");

  sdCsWrite(SD_CS_NATIVE_1, HIGH);
  sdCsWrite(SD_CS_NATIVE_2, HIGH);
  sdCsWrite(SD_CS_EXTENDER, HIGH);
  if (!sdNative2.cardBegin(SdSpiConfig(SD_CS_NATIVE_2, SHARED_SPI, SD_SCK_MHZ(4)))) {
    Serial.println("INIT: FOUT IN LAAG 1, KAART/SPI");
    sdNative2.printSdError(&Serial);
    sdNative2.end();
  } else if (!sdNative2.volumeBegin()) {
    Serial.println("INIT: FOUT IN LAAG 2, BESTANDSSYSTEEM");
    sdNative2.errorPrint(&Serial);
    sdNative2.end();
  } else {
    File32 bestand = sdNative2.open("TEST.BIN", O_RDWR | O_CREAT | O_TRUNC);
    if (!bestand) {
      Serial.println("OPEN: FOUT IN LAAG 3, BESTAND");
    } else {
      uint32_t start = micros();
      bool schrijfFout = false;

      for (uint32_t blok = 0; blok < AANTAL_BLOKKEN; blok++) {
        if (bestand.write(schrijfBuffer2, BLOK_GROOTTE) != BLOK_GROOTTE) {
          schrijfFout = true;
          break;
        }
      }

      bestand.flush();
      uint32_t schrijfTijd = micros() - start;

      bestand.rewind();

      start = micros();
      bool leesFout = false;
      bool inhoudFout = false;

      for (uint32_t blok = 0; blok < AANTAL_BLOKKEN; blok++) {
        if (bestand.read(leesBuffer, BLOK_GROOTTE) != BLOK_GROOTTE) {
          leesFout = true;
          break;
        }

        for (uint16_t i = 0; i < BLOK_GROOTTE; i++) {
          if (leesBuffer[i] != schrijfBuffer2[i]) {
            inhoudFout = true;
            break;
          }
        }

        if (inhoudFout) {
          break;
        }
      }

      uint32_t leesTijd = micros() - start;
      bestand.close();

      Serial.print("WRITE: ");
      Serial.println(schrijfFout ? "FOUT" : "OK");
      Serial.print("READ: ");
      Serial.println(leesFout ? "FOUT" : "OK");
      Serial.print("VERIFY: ");
      Serial.println(inhoudFout ? "FOUT" : "OK");
      Serial.print("WRITE us: ");
      Serial.println(schrijfTijd);
      Serial.print("READ us: ");
      Serial.println(leesTijd);
    }
  }

  Serial.println();
  Serial.println("=== microSD #3: CS via PCF8574 ===");

  sdCsWrite(SD_CS_NATIVE_1, HIGH);
  sdCsWrite(SD_CS_NATIVE_2, HIGH);
  sdCsWrite(SD_CS_EXTENDER, HIGH);
  if (!sdExtender.cardBegin(SdSpiConfig(SD_CS_EXTENDER, SHARED_SPI, SD_SCK_MHZ(4)))) {
    Serial.println("INIT: FOUT IN LAAG 1, KAART/SPI");
    sdExtender.printSdError(&Serial);
    sdExtender.end();
  } else if (!sdExtender.volumeBegin()) {
    Serial.println("INIT: FOUT IN LAAG 2, BESTANDSSYSTEEM");
    sdExtender.errorPrint(&Serial);
    sdExtender.end();
  } else {
    File32 bestand = sdExtender.open("TEST.BIN", O_RDWR | O_CREAT | O_TRUNC);
    if (!bestand) {
      Serial.println("OPEN: FOUT IN LAAG 3, BESTAND");
    } else {
      uint32_t start = micros();
      bool schrijfFout = false;

      for (uint32_t blok = 0; blok < AANTAL_BLOKKEN; blok++) {
        if (bestand.write(schrijfBuffer3, BLOK_GROOTTE) != BLOK_GROOTTE) {
          schrijfFout = true;
          break;
        }
      }

      bestand.flush();
      uint32_t schrijfTijd = micros() - start;

      bestand.rewind();

      start = micros();
      bool leesFout = false;
      bool inhoudFout = false;

      for (uint32_t blok = 0; blok < AANTAL_BLOKKEN; blok++) {
        if (bestand.read(leesBuffer, BLOK_GROOTTE) != BLOK_GROOTTE) {
          leesFout = true;
          break;
        }

        for (uint16_t i = 0; i < BLOK_GROOTTE; i++) {
          if (leesBuffer[i] != schrijfBuffer3[i]) {
            inhoudFout = true;
            break;
          }
        }

        if (inhoudFout) {
          break;
        }
      }

      uint32_t leesTijd = micros() - start;
      bestand.close();

      Serial.print("WRITE: ");
      Serial.println(schrijfFout ? "FOUT" : "OK");
      Serial.print("READ: ");
      Serial.println(leesFout ? "FOUT" : "OK");
      Serial.print("VERIFY: ");
      Serial.println(inhoudFout ? "FOUT" : "OK");
      Serial.print("WRITE us: ");
      Serial.println(schrijfTijd);
      Serial.print("READ us: ");
      Serial.println(leesTijd);
    }
  }


  Serial.println();
  Serial.println("=== microSD STRESS: 100 cycli per kaart, afwisselend 64 en 512 bytes per write ===");
  Serial.println("LET OP: STRESS.BIN wordt telkens overschreven.");

  sdNative1.end();
  sdCsWrite(SD_CS_NATIVE_1, HIGH);
  sdCsWrite(SD_CS_NATIVE_2, HIGH);
  sdCsWrite(SD_CS_EXTENDER, HIGH);
  if (!sdNative1.cardBegin(SdSpiConfig(SD_CS_NATIVE_1, SHARED_SPI, SD_SCK_MHZ(4))) ||
      !sdNative1.volumeBegin()) {
    Serial.println("microSD #1 Native");
    Serial.println("INIT: FOUT, STRESS OVERGESLAGEN");
    sdNative1.end();
  } else {
  {
    uint16_t fouten = 0;
    uint32_t totaalWrite = 0;
    uint32_t totaalRead = 0;

    for (uint16_t herhaling = 0; herhaling < 100; herhaling++) {
      uint16_t bytesOmTeSchrijven = (herhaling & 1) ? 64 : BLOK_GROOTTE;
      for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
        schrijfBuffer1[i] = static_cast<uint8_t>(i + herhaling);
      }

      File32 bestand = sdNative1.open("STRESS.BIN", O_RDWR | O_CREAT | O_TRUNC);
      if (!bestand) {
        fouten++;
        continue;
      }

      bool schrijfFout = false;
      bool leesFout = false;
      bool inhoudFout = false;

      uint32_t start = micros();

      for (uint32_t blok = 0;
           blok < ((herhaling & 1) ? (AANTAL_BLOKKEN * 8UL) : AANTAL_BLOKKEN);
           blok++) {
        if (bestand.write(schrijfBuffer1, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          schrijfFout = true;
          break;
        }
      }

      bestand.flush();
      totaalWrite += micros() - start;
      bestand.rewind();

      start = micros();

      for (uint32_t blok = 0;
           blok < ((herhaling & 1) ? (AANTAL_BLOKKEN * 8UL) : AANTAL_BLOKKEN);
           blok++) {
        uint16_t gelezen = bestand.read(leesBuffer, bytesOmTeSchrijven);

        if (gelezen != bytesOmTeSchrijven) {
          leesFout = true;
          break;
        }

        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          if (leesBuffer[i] != schrijfBuffer1[i]) {
            inhoudFout = true;
            break;
          }
        }

        if (inhoudFout) {
          break;
        }
      }

      totaalRead += micros() - start;
      bestand.close();

      if (schrijfFout || leesFout || inhoudFout) {
        fouten++;
      }
    }

    Serial.println("microSD #1 Native");
    Serial.print("CYCLI: ");
    Serial.println(100);
    Serial.print("FOUTEN: ");
    Serial.println(fouten);
    Serial.print("GEM WRITE us: ");
    Serial.println(totaalWrite / 100UL);
    Serial.print("GEM READ us: ");
    Serial.println(totaalRead / 100UL);
  }
  }

  sdNative2.end();
  sdCsWrite(SD_CS_NATIVE_1, HIGH);
  sdCsWrite(SD_CS_NATIVE_2, HIGH);
  sdCsWrite(SD_CS_EXTENDER, HIGH);
  if (!sdNative2.cardBegin(SdSpiConfig(SD_CS_NATIVE_2, SHARED_SPI, SD_SCK_MHZ(4))) ||
      !sdNative2.volumeBegin()) {
    Serial.println("microSD #2 Native");
    Serial.println("INIT: FOUT, STRESS OVERGESLAGEN");
    sdNative2.end();
  } else {
  {
    uint16_t fouten = 0;
    uint32_t totaalWrite = 0;
    uint32_t totaalRead = 0;

    for (uint16_t herhaling = 0; herhaling < 100; herhaling++) {
      uint16_t bytesOmTeSchrijven = (herhaling & 1) ? 64 : BLOK_GROOTTE;
      for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
        schrijfBuffer2[i] = static_cast<uint8_t>(i + herhaling + 1);
      }

      File32 bestand = sdNative2.open("STRESS.BIN", O_RDWR | O_CREAT | O_TRUNC);
      if (!bestand) {
        fouten++;
        continue;
      }

      bool schrijfFout = false;
      bool leesFout = false;
      bool inhoudFout = false;

      uint32_t start = micros();

      for (uint32_t blok = 0;
           blok < ((herhaling & 1) ? (AANTAL_BLOKKEN * 8UL) : AANTAL_BLOKKEN);
           blok++) {
        if (bestand.write(schrijfBuffer2, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          schrijfFout = true;
          break;
        }
      }

      bestand.flush();
      totaalWrite += micros() - start;
      bestand.rewind();

      start = micros();

      for (uint32_t blok = 0;
           blok < ((herhaling & 1) ? (AANTAL_BLOKKEN * 8UL) : AANTAL_BLOKKEN);
           blok++) {
        uint16_t gelezen = bestand.read(leesBuffer, bytesOmTeSchrijven);

        if (gelezen != bytesOmTeSchrijven) {
          leesFout = true;
          break;
        }

        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          if (leesBuffer[i] != schrijfBuffer2[i]) {
            inhoudFout = true;
            break;
          }
        }

        if (inhoudFout) {
          break;
        }
      }

      totaalRead += micros() - start;
      bestand.close();

      if (schrijfFout || leesFout || inhoudFout) {
        fouten++;
      }
    }

    Serial.println("microSD #2 Native");
    Serial.print("CYCLI: ");
    Serial.println(100);
    Serial.print("FOUTEN: ");
    Serial.println(fouten);
    Serial.print("GEM WRITE us: ");
    Serial.println(totaalWrite / 100UL);
    Serial.print("GEM READ us: ");
    Serial.println(totaalRead / 100UL);
  }
  }

  sdExtender.end();
  sdCsWrite(SD_CS_NATIVE_1, HIGH);
  sdCsWrite(SD_CS_NATIVE_2, HIGH);
  sdCsWrite(SD_CS_EXTENDER, HIGH);
  if (!sdExtender.cardBegin(SdSpiConfig(SD_CS_EXTENDER, SHARED_SPI, SD_SCK_MHZ(4))) ||
      !sdExtender.volumeBegin()) {
    Serial.println("microSD #3 PCF8574 CS");
    Serial.println("INIT: FOUT, STRESS OVERGESLAGEN");
    sdExtender.end();
  } else {
  {
    uint16_t fouten = 0;
    uint32_t totaalWrite = 0;
    uint32_t totaalRead = 0;

    for (uint16_t herhaling = 0; herhaling < 100; herhaling++) {
      uint16_t bytesOmTeSchrijven = (herhaling & 1) ? 64 : BLOK_GROOTTE;
      for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
        schrijfBuffer3[i] = static_cast<uint8_t>(i + herhaling + 2);
      }

      File32 bestand = sdExtender.open("STRESS.BIN", O_RDWR | O_CREAT | O_TRUNC);
      if (!bestand) {
        fouten++;
        continue;
      }

      bool schrijfFout = false;
      bool leesFout = false;
      bool inhoudFout = false;

      uint32_t start = micros();

      for (uint32_t blok = 0;
           blok < ((herhaling & 1) ? (AANTAL_BLOKKEN * 8UL) : AANTAL_BLOKKEN);
           blok++) {
        if (bestand.write(schrijfBuffer3, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          schrijfFout = true;
          break;
        }
      }

      bestand.flush();
      totaalWrite += micros() - start;
      bestand.rewind();

      start = micros();

      for (uint32_t blok = 0;
           blok < ((herhaling & 1) ? (AANTAL_BLOKKEN * 8UL) : AANTAL_BLOKKEN);
           blok++) {
        uint16_t gelezen = bestand.read(leesBuffer, bytesOmTeSchrijven);

        if (gelezen != bytesOmTeSchrijven) {
          leesFout = true;
          break;
        }

        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          if (leesBuffer[i] != schrijfBuffer3[i]) {
            inhoudFout = true;
            break;
          }
        }

        if (inhoudFout) {
          break;
        }
      }

      totaalRead += micros() - start;
      bestand.close();

      if (schrijfFout || leesFout || inhoudFout) {
        fouten++;
      }
    }

    Serial.println("microSD #3 PCF8574 CS");
    Serial.print("CYCLI: ");
    Serial.println(100);
    Serial.print("FOUTEN: ");
    Serial.println(fouten);
    Serial.print("GEM WRITE us: ");
    Serial.println(totaalWrite / 100UL);
    Serial.print("GEM READ us: ");
    Serial.println(totaalRead / 100UL);
  }
  }


  Serial.println();
  Serial.println("=== microSD STRESS: Native #1 + Native #2 tegelijk actief ===");

  sdNative1.end();
  sdNative2.end();
  sdCsWrite(SD_CS_NATIVE_1, HIGH);
  sdCsWrite(SD_CS_NATIVE_2, HIGH);
  sdCsWrite(SD_CS_EXTENDER, HIGH);
  if (sdNative1.cardBegin(SdSpiConfig(SD_CS_NATIVE_1, SHARED_SPI, SD_SCK_MHZ(4))) &&
      sdNative1.volumeBegin() &&
      (sdCsWrite(SD_CS_NATIVE_1, HIGH),
       sdCsWrite(SD_CS_NATIVE_2, HIGH),
       sdCsWrite(SD_CS_EXTENDER, HIGH),
       sdNative2.cardBegin(SdSpiConfig(SD_CS_NATIVE_2, SHARED_SPI, SD_SCK_MHZ(4)))) &&
      sdNative2.volumeBegin()) {
  {
    uint16_t fouten = 0;
    uint32_t totaalWrite = 0;
    uint32_t totaalRead = 0;

    for (uint16_t herhaling = 0; herhaling < 100; herhaling++) {
      uint16_t bytesOmTeSchrijven = (herhaling & 1) ? 64 : BLOK_GROOTTE;
      File32 bestand1, bestand2;
      bestand1 = sdNative1.open("STRESS.BIN", O_RDWR | O_CREAT | O_TRUNC);
      bestand2 = sdNative2.open("STRESS.BIN", O_RDWR | O_CREAT | O_TRUNC);

      if (!bestand1 || !bestand2) {
        fouten++;
        if (bestand1) bestand1.close();
        if (bestand2) bestand2.close();
        continue;
      }

      bool schrijfFout = false;
      bool leesFout = false;
      bool inhoudFout = false;

      uint32_t start = micros();

      for (uint32_t blok = 0;
           blok < ((herhaling & 1) ? (AANTAL_BLOKKEN * 8UL) : AANTAL_BLOKKEN);
           blok++) {
        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          schrijfBuffer1[i] = static_cast<uint8_t>(i + herhaling);
          schrijfBuffer2[i] = static_cast<uint8_t>(i + herhaling);
        }
        schrijfBuffer2[0] = static_cast<uint8_t>(herhaling + 1);

        if (bestand1.write(schrijfBuffer1, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          schrijfFout = true;
          break;
        }

        if (bestand2.write(schrijfBuffer2, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          schrijfFout = true;
          break;
        }
      }

      bestand1.flush();
      bestand2.flush();
      totaalWrite += micros() - start;

      bestand1.rewind();
      bestand2.rewind();
      start = micros();

      for (uint32_t blok = 0;
           blok < ((herhaling & 1) ? (AANTAL_BLOKKEN * 8UL) : AANTAL_BLOKKEN);
           blok++) {
        if (bestand1.read(leesBuffer, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          leesFout = true;
          break;
        }

        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          if (leesBuffer[i] != static_cast<uint8_t>(i + herhaling)) {
            inhoudFout = true;
            break;
          }
        }

        if (inhoudFout) break;

        if (bestand2.read(leesBuffer, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          leesFout = true;
          break;
        }

        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          uint8_t verwachteWaarde = (i == 0) ? static_cast<uint8_t>(herhaling + 1) : static_cast<uint8_t>(i + herhaling);
          if (leesBuffer[i] != verwachteWaarde) {
            inhoudFout = true;
            break;
          }
        }

        if (inhoudFout) break;
      }

      totaalRead += micros() - start;
      bestand1.close();
      bestand2.close();

      if (schrijfFout || leesFout || inhoudFout) fouten++;
    }

    Serial.println("microSD #1 Native + microSD #2 Native");
    Serial.print("CYCLI: ");
    Serial.println(100);
    Serial.print("FOUTEN: ");
    Serial.println(fouten);
    Serial.print("GEM WRITE us: ");
    Serial.println(totaalWrite / 100UL);
    Serial.print("GEM READ us: ");
    Serial.println(totaalRead / 100UL);
  }
  } else {
    Serial.println("microSD #1 Native + microSD #2 Native");
    Serial.println("INIT: FOUT, STRESS OVERGESLAGEN");
  }

  Serial.println();
  Serial.println("=== microSD STRESS: Native #2 + PCF8574 CS tegelijk actief ===");

  sdNative2.end();
  sdExtender.end();
  sdCsWrite(SD_CS_NATIVE_1, HIGH);
  sdCsWrite(SD_CS_NATIVE_2, HIGH);
  sdCsWrite(SD_CS_EXTENDER, HIGH);
  if (sdNative2.cardBegin(SdSpiConfig(SD_CS_NATIVE_2, SHARED_SPI, SD_SCK_MHZ(4))) &&
      sdNative2.volumeBegin() &&
      (sdCsWrite(SD_CS_NATIVE_1, HIGH),
       sdCsWrite(SD_CS_NATIVE_2, HIGH),
       sdCsWrite(SD_CS_EXTENDER, HIGH),
       sdExtender.cardBegin(SdSpiConfig(SD_CS_EXTENDER, SHARED_SPI, SD_SCK_MHZ(4)))) &&
      sdExtender.volumeBegin()) {
  {
    uint16_t fouten = 0;
    uint32_t totaalWrite = 0;
    uint32_t totaalRead = 0;

    for (uint16_t herhaling = 0; herhaling < 100; herhaling++) {
      uint16_t bytesOmTeSchrijven = (herhaling & 1) ? 64 : BLOK_GROOTTE;
      File32 bestand2, bestand3;
      bestand2 = sdNative2.open("STRESS.BIN", O_RDWR | O_CREAT | O_TRUNC);
      bestand3 = sdExtender.open("STRESS.BIN", O_RDWR | O_CREAT | O_TRUNC);

      if (!bestand2 || !bestand3) {
        fouten++;
        if (bestand2) bestand2.close();
        if (bestand3) bestand3.close();
        continue;
      }

      bool schrijfFout = false;
      bool leesFout = false;
      bool inhoudFout = false;

      uint32_t start = micros();

      for (uint32_t blok = 0;
           blok < ((herhaling & 1) ? (AANTAL_BLOKKEN * 8UL) : AANTAL_BLOKKEN);
           blok++) {
        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          schrijfBuffer2[i] = static_cast<uint8_t>(i + herhaling);
          schrijfBuffer3[i] = static_cast<uint8_t>(i + herhaling);
        }
        schrijfBuffer3[0] = static_cast<uint8_t>(herhaling + 2);

        if (bestand2.write(schrijfBuffer2, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          schrijfFout = true;
          break;
        }

        if (bestand3.write(schrijfBuffer3, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          schrijfFout = true;
          break;
        }
      }

      bestand2.flush();
      bestand3.flush();
      totaalWrite += micros() - start;

      bestand2.rewind();
      bestand3.rewind();
      start = micros();

      for (uint32_t blok = 0;
           blok < ((herhaling & 1) ? (AANTAL_BLOKKEN * 8UL) : AANTAL_BLOKKEN);
           blok++) {
        if (bestand2.read(leesBuffer, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          leesFout = true;
          break;
        }

        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          if (leesBuffer[i] != static_cast<uint8_t>(i + herhaling)) {
            inhoudFout = true;
            break;
          }
        }

        if (inhoudFout) break;

        if (bestand3.read(leesBuffer, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          leesFout = true;
          break;
        }

        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          uint8_t verwachteWaarde = (i == 0) ? static_cast<uint8_t>(herhaling + 2) : static_cast<uint8_t>(i + herhaling);
          if (leesBuffer[i] != verwachteWaarde) {
            inhoudFout = true;
            break;
          }
        }

        if (inhoudFout) break;
      }

      totaalRead += micros() - start;
      bestand2.close();
      bestand3.close();

      if (schrijfFout || leesFout || inhoudFout) fouten++;
    }

    Serial.println("microSD #2 Native + microSD #3 PCF8574 CS");
    Serial.print("CYCLI: ");
    Serial.println(100);
    Serial.print("FOUTEN: ");
    Serial.println(fouten);
    Serial.print("GEM WRITE us: ");
    Serial.println(totaalWrite / 100UL);
    Serial.print("GEM READ us: ");
    Serial.println(totaalRead / 100UL);
  }
  } else {
    Serial.println("microSD #2 Native + microSD #3 PCF8574 CS");
    Serial.println("INIT: FOUT, STRESS OVERGESLAGEN");
  }

  Serial.println();
  Serial.println("=== microSD STRESS: Native #1 + PCF8574 CS tegelijk actief ===");

  sdNative1.end();
  sdExtender.end();
  sdCsWrite(SD_CS_NATIVE_1, HIGH);
  sdCsWrite(SD_CS_NATIVE_2, HIGH);
  sdCsWrite(SD_CS_EXTENDER, HIGH);
  if (sdNative1.cardBegin(SdSpiConfig(SD_CS_NATIVE_1, SHARED_SPI, SD_SCK_MHZ(4))) &&
      sdNative1.volumeBegin() &&
      (sdCsWrite(SD_CS_NATIVE_1, HIGH),
       sdCsWrite(SD_CS_NATIVE_2, HIGH),
       sdCsWrite(SD_CS_EXTENDER, HIGH),
       sdExtender.cardBegin(SdSpiConfig(SD_CS_EXTENDER, SHARED_SPI, SD_SCK_MHZ(4)))) &&
      sdExtender.volumeBegin()) {
  {
    uint16_t fouten = 0;
    uint32_t totaalWrite = 0;
    uint32_t totaalRead = 0;

    for (uint16_t herhaling = 0; herhaling < 100; herhaling++) {
      uint16_t bytesOmTeSchrijven = (herhaling & 1) ? 64 : BLOK_GROOTTE;
      File32 bestand1, bestand3;
      bestand1 = sdNative1.open("STRESS.BIN", O_RDWR | O_CREAT | O_TRUNC);
      bestand3 = sdExtender.open("STRESS.BIN", O_RDWR | O_CREAT | O_TRUNC);

      if (!bestand1 || !bestand3) {
        fouten++;
        if (bestand1) bestand1.close();
        if (bestand3) bestand3.close();
        continue;
      }

      bool schrijfFout = false;
      bool leesFout = false;
      bool inhoudFout = false;

      uint32_t start = micros();

      for (uint32_t blok = 0;
           blok < ((herhaling & 1) ? (AANTAL_BLOKKEN * 8UL) : AANTAL_BLOKKEN);
           blok++) {
        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          schrijfBuffer1[i] = static_cast<uint8_t>(i + herhaling);
          schrijfBuffer3[i] = static_cast<uint8_t>(i + herhaling);
        }
        schrijfBuffer3[0] = static_cast<uint8_t>(herhaling + 2);

        if (bestand1.write(schrijfBuffer1, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          schrijfFout = true;
          break;
        }

        if (bestand3.write(schrijfBuffer3, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          schrijfFout = true;
          break;
        }
      }

      bestand1.flush();
      bestand3.flush();
      totaalWrite += micros() - start;

      bestand1.rewind();
      bestand3.rewind();
      start = micros();

      for (uint32_t blok = 0;
           blok < ((herhaling & 1) ? (AANTAL_BLOKKEN * 8UL) : AANTAL_BLOKKEN);
           blok++) {
        if (bestand1.read(leesBuffer, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          leesFout = true;
          break;
        }

        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          if (leesBuffer[i] != static_cast<uint8_t>(i + herhaling)) {
            inhoudFout = true;
            break;
          }
        }

        if (inhoudFout) break;

        if (bestand3.read(leesBuffer, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          leesFout = true;
          break;
        }

        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          uint8_t verwachteWaarde = (i == 0) ? static_cast<uint8_t>(herhaling + 2) : static_cast<uint8_t>(i + herhaling);
          if (leesBuffer[i] != verwachteWaarde) {
            inhoudFout = true;
            break;
          }
        }

        if (inhoudFout) break;
      }

      totaalRead += micros() - start;
      bestand1.close();
      bestand3.close();

      if (schrijfFout || leesFout || inhoudFout) fouten++;
    }

    Serial.println("microSD #1 Native + microSD #3 PCF8574 CS");
    Serial.print("CYCLI: ");
    Serial.println(100);
    Serial.print("FOUTEN: ");
    Serial.println(fouten);
    Serial.print("GEM WRITE us: ");
    Serial.println(totaalWrite / 100UL);
    Serial.print("GEM READ us: ");
    Serial.println(totaalRead / 100UL);
  }
  } else {
    Serial.println("microSD #1 Native + microSD #3 PCF8574 CS");
    Serial.println("INIT: FOUT, STRESS OVERGESLAGEN");
  }

  Serial.println();
  Serial.println("=== microSD STRESS: Native #1 + Native #2 + PCF8574 CS tegelijk actief ===");

  sdNative1.end();
  sdNative2.end();
  sdExtender.end();
  sdCsWrite(SD_CS_NATIVE_1, HIGH);
  sdCsWrite(SD_CS_NATIVE_2, HIGH);
  sdCsWrite(SD_CS_EXTENDER, HIGH);
  if (sdNative1.cardBegin(SdSpiConfig(SD_CS_NATIVE_1, SHARED_SPI, SD_SCK_MHZ(4))) &&
      sdNative1.volumeBegin() &&
      (sdCsWrite(SD_CS_NATIVE_1, HIGH),
       sdCsWrite(SD_CS_NATIVE_2, HIGH),
       sdCsWrite(SD_CS_EXTENDER, HIGH),
       sdNative2.cardBegin(SdSpiConfig(SD_CS_NATIVE_2, SHARED_SPI, SD_SCK_MHZ(4)))) &&
      sdNative2.volumeBegin() &&
      (sdCsWrite(SD_CS_NATIVE_1, HIGH),
       sdCsWrite(SD_CS_NATIVE_2, HIGH),
       sdCsWrite(SD_CS_EXTENDER, HIGH),
       sdExtender.cardBegin(SdSpiConfig(SD_CS_EXTENDER, SHARED_SPI, SD_SCK_MHZ(4)))) &&
      sdExtender.volumeBegin()) {
  {
    uint16_t fouten = 0;
    uint32_t totaalWrite = 0;
    uint32_t totaalRead = 0;

    for (uint16_t herhaling = 0; herhaling < 100; herhaling++) {
      uint16_t bytesOmTeSchrijven = (herhaling & 1) ? 64 : BLOK_GROOTTE;
      File32 bestand1, bestand2, bestand3;
      bestand1 = sdNative1.open("STRESS.BIN", O_RDWR | O_CREAT | O_TRUNC);
      bestand2 = sdNative2.open("STRESS.BIN", O_RDWR | O_CREAT | O_TRUNC);
      bestand3 = sdExtender.open("STRESS.BIN", O_RDWR | O_CREAT | O_TRUNC);

      if (!bestand1 || !bestand2 || !bestand3) {
        fouten++;
        if (bestand1) bestand1.close();
        if (bestand2) bestand2.close();
        if (bestand3) bestand3.close();
        continue;
      }

      bool schrijfFout = false;
      bool leesFout = false;
      bool inhoudFout = false;

      uint32_t start = micros();

      for (uint32_t blok = 0;
           blok < ((herhaling & 1) ? (AANTAL_BLOKKEN * 8UL) : AANTAL_BLOKKEN);
           blok++) {
        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          schrijfBuffer1[i] = static_cast<uint8_t>(i + herhaling);
          schrijfBuffer2[i] = static_cast<uint8_t>(i + herhaling);
          schrijfBuffer3[i] = static_cast<uint8_t>(i + herhaling);
        }
        schrijfBuffer2[0] = static_cast<uint8_t>(herhaling + 1);
        schrijfBuffer3[0] = static_cast<uint8_t>(herhaling + 2);

        if (bestand1.write(schrijfBuffer1, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          schrijfFout = true;
          break;
        }

        if (bestand2.write(schrijfBuffer2, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          schrijfFout = true;
          break;
        }

        if (bestand3.write(schrijfBuffer3, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          schrijfFout = true;
          break;
        }
      }

      bestand1.flush();
      bestand2.flush();
      bestand3.flush();
      totaalWrite += micros() - start;

      bestand1.rewind();
      bestand2.rewind();
      bestand3.rewind();
      start = micros();

      for (uint32_t blok = 0;
           blok < ((herhaling & 1) ? (AANTAL_BLOKKEN * 8UL) : AANTAL_BLOKKEN);
           blok++) {
        if (bestand1.read(leesBuffer, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          leesFout = true;
          break;
        }

        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          if (leesBuffer[i] != static_cast<uint8_t>(i + herhaling)) {
            inhoudFout = true;
            break;
          }
        }

        if (inhoudFout) break;

        if (bestand2.read(leesBuffer, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          leesFout = true;
          break;
        }

        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          uint8_t verwachteWaarde = (i == 0) ? static_cast<uint8_t>(herhaling + 1) : static_cast<uint8_t>(i + herhaling);
          if (leesBuffer[i] != verwachteWaarde) {
            inhoudFout = true;
            break;
          }
        }

        if (inhoudFout) break;

        if (bestand3.read(leesBuffer, bytesOmTeSchrijven) != bytesOmTeSchrijven) {
          leesFout = true;
          break;
        }

        for (uint16_t i = 0; i < bytesOmTeSchrijven; i++) {
          uint8_t verwachteWaarde = (i == 0) ? static_cast<uint8_t>(herhaling + 2) : static_cast<uint8_t>(i + herhaling);
          if (leesBuffer[i] != verwachteWaarde) {
            inhoudFout = true;
            break;
          }
        }

        if (inhoudFout) break;
      }

      totaalRead += micros() - start;
      bestand1.close();
      bestand2.close();
      bestand3.close();

      if (schrijfFout || leesFout || inhoudFout) fouten++;
    }

    Serial.println("microSD #1 Native + microSD #2 Native + microSD #3 PCF8574 CS");
    Serial.print("CYCLI: ");
    Serial.println(100);
    Serial.print("FOUTEN: ");
    Serial.println(fouten);
    Serial.print("GEM WRITE us: ");
    Serial.println(totaalWrite / 100UL);
    Serial.print("GEM READ us: ");
    Serial.println(totaalRead / 100UL);
  }
  } else {
    Serial.println("microSD #1 Native + microSD #2 Native + microSD #3 PCF8574 CS");
    Serial.println("INIT: FOUT, STRESS OVERGESLAGEN");
  }

  Serial.println();
  Serial.println("=== EINDE microSD TEST ===");
}

void loop() {
}
