#include <Wire.h>
#include <FRAM.h>

const uint8_t FRAM_I2C_ADDRESS = 0x50;
const uint16_t TEST_ADRES = 0;
const uint16_t TEST_LENGTE = 64;
const uint16_t AANTAL_HERHALINGEN = 1000;

FRAM FRAM_I2C(&Wire);

uint8_t schrijfBuffer[TEST_LENGTE];
uint8_t leesBuffer[TEST_LENGTE];

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Wire.begin();

  Serial.println();
  Serial.println("=== FRAM MB85RC256V STRESS ===");

  const int beginStatus = FRAM_I2C.begin(FRAM_I2C_ADDRESS);
  if (beginStatus != FRAM_OK) {
    Serial.print("INIT: FOUT ");
    Serial.println(beginStatus);
    return;
  }

  Serial.print("SIZE: ");
  Serial.println(FRAM_I2C.getSizeBytes());

  if (FRAM_I2C.getSizeBytes() != FRAM_MB85RC256V) {
    Serial.println("SIZE: FOUT");
    return;
  }

  uint16_t fouten = 0;
  uint32_t totaalWrite = 0;
  uint32_t totaalRead = 0;

  for (uint16_t herhaling = 0; herhaling < AANTAL_HERHALINGEN; herhaling++) {
    for (uint16_t i = 0; i < TEST_LENGTE; i++) {
      schrijfBuffer[i] = static_cast<uint8_t>(i + herhaling);
    }

    uint32_t start = micros();
    FRAM_I2C.write(TEST_ADRES, schrijfBuffer, TEST_LENGTE);
    totaalWrite += micros() - start;
    const int schrijfStatus = FRAM_I2C.lastError();

    memset(leesBuffer, 0, sizeof(leesBuffer));

    start = micros();
    FRAM_I2C.read(TEST_ADRES, leesBuffer, TEST_LENGTE);
    totaalRead += micros() - start;
    const int leesStatus = FRAM_I2C.lastError();

    bool inhoudOk = true;
    for (uint16_t i = 0; i < TEST_LENGTE; i++) {
      if (leesBuffer[i] != schrijfBuffer[i]) {
        inhoudOk = false;
        break;
      }
    }

    if (schrijfStatus != FRAM_OK || leesStatus != FRAM_OK || !inhoudOk) {
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
  Serial.print("RESULTAAT: ");
  Serial.println(fouten == 0 ? "OK" : "FOUT");
}

void loop() {}
