#include <Wire.h>
#include <I2C_eeprom.h>

const uint8_t EEPROM_DIRECT_ADRES = 0x51;
const uint8_t EEPROM_TCA_ADRES = 0x50;
const uint32_t EEPROM_GROOTTE = I2C_DEVICESIZE_24LC256;

const uint8_t TCA9548A_ADRES = 0x70;
const uint8_t TCA_KANAAL_1 = 0;
const uint8_t TCA_KANAAL_2 = 1;

const uint16_t TEST_ADRES = 0;
const uint16_t TEST_LENGTE = 64;
const uint16_t AANTAL_HERHALINGEN = 1000;

I2C_eeprom eepromDirect(EEPROM_DIRECT_ADRES, EEPROM_GROOTTE);
I2C_eeprom eepromTCA1(EEPROM_TCA_ADRES, EEPROM_GROOTTE);
I2C_eeprom eepromTCA2(EEPROM_TCA_ADRES, EEPROM_GROOTTE);

uint8_t schrijfBuffer[TEST_LENGTE];
uint8_t leesBuffer[TEST_LENGTE];

void tcaselect(uint8_t channel) {
  if (channel > 7) {
    return;
  }

  Wire.beginTransmission(TCA9548A_ADRES);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Wire.begin();

  for (uint16_t i = 0; i < TEST_LENGTE; i++) {
    schrijfBuffer[i] = static_cast<uint8_t>(i + 1);
  }

  Serial.println();
  Serial.println("=== EEPROM #1: rechtstreeks op I2C ===");

  Wire.beginTransmission(TCA9548A_ADRES);
  Wire.write(0);
  Wire.endTransmission();

  if (!eepromDirect.begin()) {
    Serial.println("INIT: FOUT");
  } else {
    uint32_t start = micros();
    int schrijfStatus = eepromDirect.writeBlock(TEST_ADRES, schrijfBuffer, TEST_LENGTE);
    uint32_t schrijfTijd = micros() - start;

    memset(leesBuffer, 0, sizeof(leesBuffer));

    start = micros();
    uint16_t gelezen = eepromDirect.readBlock(TEST_ADRES, leesBuffer, TEST_LENGTE);
    uint32_t leesTijd = micros() - start;

    bool inhoudOk = true;
    for (uint16_t i = 0; i < TEST_LENGTE; i++) {
      if (leesBuffer[i] != schrijfBuffer[i]) {
        inhoudOk = false;
        break;
      }
    }

    Serial.print("WRITE: ");
    Serial.println(schrijfStatus == 0 ? "OK" : "FOUT");
    Serial.print("READ: ");
    Serial.println(gelezen == TEST_LENGTE ? "OK" : "FOUT");
    Serial.print("VERIFY: ");
    Serial.println(inhoudOk ? "OK" : "FOUT");
    Serial.print("WRITE us: ");
    Serial.println(schrijfTijd);
    Serial.print("READ us: ");
    Serial.println(leesTijd);
  }


  Serial.println();
  Serial.println("=== EEPROM #1 STRESS: rechtstreeks op I2C ===");

  Wire.beginTransmission(TCA9548A_ADRES);
  Wire.write(0);
  Wire.endTransmission();

  if (!eepromDirect.begin()) {
    Serial.println("INIT: FOUT");
  } else {
    uint16_t fouten = 0;
    uint32_t totaalWrite = 0;
    uint32_t totaalRead = 0;

    for (uint16_t herhaling = 0; herhaling < AANTAL_HERHALINGEN; herhaling++) {
      for (uint16_t i = 0; i < TEST_LENGTE; i++) {
        schrijfBuffer[i] = static_cast<uint8_t>(i + herhaling);
      }

      uint32_t start = micros();
      int schrijfStatus = eepromDirect.writeBlock(
        TEST_ADRES, schrijfBuffer, TEST_LENGTE);
      totaalWrite += micros() - start;

      memset(leesBuffer, 0, sizeof(leesBuffer));

      start = micros();
      uint16_t gelezen = eepromDirect.readBlock(
        TEST_ADRES, leesBuffer, TEST_LENGTE);
      totaalRead += micros() - start;

      bool inhoudOk = true;
      for (uint16_t i = 0; i < TEST_LENGTE; i++) {
        if (leesBuffer[i] != schrijfBuffer[i]) {
          inhoudOk = false;
          break;
        }
      }

      if (schrijfStatus != 0 || gelezen != TEST_LENGTE || !inhoudOk) {
        fouten++;
      }
    }

    Serial.print("HERHALINGEN: ");
    Serial.println(AANTAL_HERHALINGEN);
    Serial.print("FOUTEN: ");
    Serial.println(fouten);
    Serial.print("GEM WRITE us: ");
    Serial.println(totaalWrite / AANTAL_HERHALINGEN);
    Serial.print("GEM READ us: ");
    Serial.println(totaalRead / AANTAL_HERHALINGEN);
  }

  Serial.println();
  Serial.println("=== EEPROM #2 STRESS: achter TCA9548A kanaal 0 ===");

  tcaselect(TCA_KANAAL_1);

  const bool eepromTCA1gestart = eepromTCA1.begin();
  if (!eepromTCA1gestart) {
    Serial.println("INIT: FOUT");
  } else {
    uint16_t fouten = 0;
    uint32_t totaalWrite = 0;
    uint32_t totaalRead = 0;

    for (uint16_t herhaling = 0; herhaling < AANTAL_HERHALINGEN; herhaling++) {
      tcaselect(TCA_KANAAL_1);

      schrijfBuffer[0] = static_cast<uint8_t>(herhaling);

      uint32_t start = micros();
      int schrijfStatus = eepromTCA1.writeBlock(TEST_ADRES, schrijfBuffer, TEST_LENGTE);
      totaalWrite += micros() - start;

      memset(leesBuffer, 0, sizeof(leesBuffer));

      tcaselect(TCA_KANAAL_1);
      start = micros();
      uint16_t gelezen = eepromTCA1.readBlock(TEST_ADRES, leesBuffer, TEST_LENGTE);
      totaalRead += micros() - start;

      bool inhoudOk = true;
      for (uint16_t i = 0; i < TEST_LENGTE; i++) {
        if (leesBuffer[i] != schrijfBuffer[i]) {
          inhoudOk = false;
          break;
        }
      }

      if (schrijfStatus != 0 || gelezen != TEST_LENGTE || !inhoudOk) {
        fouten++;
      }
    }

    Serial.print("HERHALINGEN: ");
    Serial.println(AANTAL_HERHALINGEN);
    Serial.print("FOUTEN: ");
    Serial.println(fouten);
    Serial.print("GEM WRITE us: ");
    Serial.println(totaalWrite / AANTAL_HERHALINGEN);
    Serial.print("GEM READ us: ");
    Serial.println(totaalRead / AANTAL_HERHALINGEN);
  }

  Serial.println();
  Serial.println("=== EEPROM #3 STRESS: achter TCA9548A kanaal 1 ===");

  tcaselect(TCA_KANAAL_2);

  const bool eepromTCA2gestart = eepromTCA2.begin();
  if (!eepromTCA2gestart) {
    Serial.println("INIT: FOUT");
  } else {
    uint16_t fouten = 0;
    uint32_t totaalWrite = 0;
    uint32_t totaalRead = 0;

    for (uint16_t herhaling = 0; herhaling < AANTAL_HERHALINGEN; herhaling++) {
      tcaselect(TCA_KANAAL_2);

      schrijfBuffer[0] = static_cast<uint8_t>(herhaling + 1);

      uint32_t start = micros();
      int schrijfStatus = eepromTCA2.writeBlock(TEST_ADRES, schrijfBuffer, TEST_LENGTE);
      totaalWrite += micros() - start;

      memset(leesBuffer, 0, sizeof(leesBuffer));

      tcaselect(TCA_KANAAL_2);
      start = micros();
      uint16_t gelezen = eepromTCA2.readBlock(TEST_ADRES, leesBuffer, TEST_LENGTE);
      totaalRead += micros() - start;

      bool inhoudOk = true;
      for (uint16_t i = 0; i < TEST_LENGTE; i++) {
        if (leesBuffer[i] != schrijfBuffer[i]) {
          inhoudOk = false;
          break;
        }
      }

      if (schrijfStatus != 0 || gelezen != TEST_LENGTE || !inhoudOk) {
        fouten++;
      }
    }

    Serial.print("HERHALINGEN: ");
    Serial.println(AANTAL_HERHALINGEN);
    Serial.print("FOUTEN: ");
    Serial.println(fouten);
    Serial.print("GEM WRITE us: ");
    Serial.println(totaalWrite / AANTAL_HERHALINGEN);
    Serial.print("GEM READ us: ");
    Serial.println(totaalRead / AANTAL_HERHALINGEN);
  }

  Serial.println();
  Serial.println("=== STRESS WISSELTEST TCA KANAAL 0 / KANAAL 1 ===");

  if (!eepromTCA1gestart || !eepromTCA2gestart) {
    Serial.println("WISSELTEST: OVERGESLAGEN, INIT FOUT");
  } else {
    uint16_t fouten = 0;

    for (uint16_t herhaling = 0; herhaling < AANTAL_HERHALINGEN; herhaling++) {
      tcaselect(TCA_KANAAL_1);
      uint8_t waarde1 = eepromTCA1.readByte(TEST_ADRES);

      tcaselect(TCA_KANAAL_2);
      uint8_t waarde2 = eepromTCA2.readByte(TEST_ADRES);

      if (waarde1 != static_cast<uint8_t>(AANTAL_HERHALINGEN - 1) ||
          waarde2 != static_cast<uint8_t>(AANTAL_HERHALINGEN)) {
        fouten++;
      }
    }

    Serial.print("WISSELINGEN: ");
    Serial.println(AANTAL_HERHALINGEN);
    Serial.print("FOUTEN: ");
    Serial.println(fouten);
  }

  Wire.beginTransmission(TCA9548A_ADRES);
  Wire.write(0);
  Wire.endTransmission();

  Serial.println();
  Serial.println("=== EINDE EEPROM TEST ===");
}

void loop() {
}
