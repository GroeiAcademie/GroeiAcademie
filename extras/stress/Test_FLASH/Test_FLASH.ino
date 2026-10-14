#include <SPI.h>
#include <Wire.h>
#include <PCF8574.h>
#include <Adafruit_SPIFlash.h>
#include <Adafruit_FlashTransport.h>

#if defined(ARDUINO_AVR_UNO)
#error Test_FLASH wordt niet uitgevoerd op Arduino UNO R3.
#endif

const uint8_t FLASH_CS_NATIVE = 10;

const uint8_t PCF8574_ADRES = 0x20;
const uint8_t PCF8574_CS_PIN = 0;

const uint32_t TEST_ADRES = 0;
const uint32_t TEST_LENGTE = 4096;

PCF8574 pcf8574(PCF8574_ADRES);

Adafruit_FlashTransport_SPI flashTransportNative(FLASH_CS_NATIVE, &SPI);
Adafruit_SPIFlash flashNativeCache(&flashTransportNative, true);
Adafruit_SPIFlash flashNativeGeenCache(&flashTransportNative, false);

class : public Adafruit_FlashTransport {
public:
  SPIClass *spi = &SPI;
  uint32_t clockWrite = 4000000;
  uint32_t clockRead = 4000000;

  void begin(void) override {
    _cmd_read = SFLASH_CMD_READ;
    _addr_len = 3;
    pcf8574.write(PCF8574_CS_PIN, HIGH);
    spi->begin();
  }

  void end(void) override {
    spi->end();
    pcf8574.write(PCF8574_CS_PIN, HIGH);
  }

  bool supportQuadMode(void) override {
    return false;
  }

  void setClockSpeed(uint32_t write_hz, uint32_t read_hz) override {
    clockWrite = write_hz;
    clockRead = read_hz;
  }

  bool runCommand(uint8_t command) override {
    spi->beginTransaction(SPISettings(clockWrite, MSBFIRST, SPI_MODE0));
    pcf8574.write(PCF8574_CS_PIN, LOW);
    spi->transfer(command);
    pcf8574.write(PCF8574_CS_PIN, HIGH);
    spi->endTransaction();
    return true;
  }

  bool readCommand(uint8_t command, uint8_t *response, uint32_t len) override {
    spi->beginTransaction(SPISettings(clockRead, MSBFIRST, SPI_MODE0));
    pcf8574.write(PCF8574_CS_PIN, LOW);
    spi->transfer(command);

    while (len--) {
      *response++ = spi->transfer(0xFF);
    }

    pcf8574.write(PCF8574_CS_PIN, HIGH);
    spi->endTransaction();
    return true;
  }

  bool writeCommand(uint8_t command, uint8_t const *data, uint32_t len) override {
    spi->beginTransaction(SPISettings(clockWrite, MSBFIRST, SPI_MODE0));
    pcf8574.write(PCF8574_CS_PIN, LOW);
    spi->transfer(command);

    while (len--) {
      spi->transfer(*data++);
    }

    pcf8574.write(PCF8574_CS_PIN, HIGH);
    spi->endTransaction();
    return true;
  }

  bool eraseCommand(uint8_t command, uint32_t address) override {
    uint8_t cmd[5] = {command, 0, 0, 0, 0};
    uint8_t *p = cmd + 1;

    if (_addr_len == 4) {
      *p++ = (address >> 24) & 0xFF;
    }
    if (_addr_len >= 3) {
      *p++ = (address >> 16) & 0xFF;
    }
    *p++ = (address >> 8) & 0xFF;
    *p++ = address & 0xFF;

    spi->beginTransaction(SPISettings(clockWrite, MSBFIRST, SPI_MODE0));
    pcf8574.write(PCF8574_CS_PIN, LOW);
    spi->transfer(cmd, 1 + _addr_len);
    pcf8574.write(PCF8574_CS_PIN, HIGH);
    spi->endTransaction();
    return true;
  }

  bool readMemory(uint32_t address, uint8_t *data, uint32_t len) override {
    uint8_t cmd[6] = {_cmd_read, 0, 0, 0, 0, 0};
    uint8_t *p = cmd + 1;

    if (_addr_len == 4) {
      *p++ = (address >> 24) & 0xFF;
    }
    if (_addr_len >= 3) {
      *p++ = (address >> 16) & 0xFF;
    }
    *p++ = (address >> 8) & 0xFF;
    *p++ = address & 0xFF;

    uint8_t cmdLength = 1 + _addr_len + (SFLASH_CMD_FAST_READ == _cmd_read ? 1 : 0);

    spi->beginTransaction(SPISettings(clockRead, MSBFIRST, SPI_MODE0));
    pcf8574.write(PCF8574_CS_PIN, LOW);
    spi->transfer(cmd, cmdLength);

    while (len--) {
      *data++ = spi->transfer(0xFF);
    }

    pcf8574.write(PCF8574_CS_PIN, HIGH);
    spi->endTransaction();
    return true;
  }

  bool writeMemory(uint32_t address, uint8_t const *data, uint32_t len) override {
    uint8_t cmd[5] = {SFLASH_CMD_PAGE_PROGRAM, 0, 0, 0, 0};
    uint8_t *p = cmd + 1;

    if (_addr_len == 4) {
      *p++ = (address >> 24) & 0xFF;
    }
    if (_addr_len >= 3) {
      *p++ = (address >> 16) & 0xFF;
    }
    *p++ = (address >> 8) & 0xFF;
    *p++ = address & 0xFF;

    spi->beginTransaction(SPISettings(clockWrite, MSBFIRST, SPI_MODE0));
    pcf8574.write(PCF8574_CS_PIN, LOW);
    spi->transfer(cmd, 1 + _addr_len);

    while (len--) {
      spi->transfer(*data++);
    }

    pcf8574.write(PCF8574_CS_PIN, HIGH);
    spi->endTransaction();
    return true;
  }
} flashTransportExtender;

Adafruit_SPIFlash flashExtenderCache(&flashTransportExtender, true);
Adafruit_SPIFlash flashExtenderGeenCache(&flashTransportExtender, false);

uint8_t schrijfBuffer[512];
uint8_t leesBuffer[512];

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Wire.begin();

  if (!pcf8574.begin(0xFF)) {
    Serial.println("PCF8574: NIET GEVONDEN");
    return;
  }

  pcf8574.write(PCF8574_CS_PIN, HIGH);

  for (uint16_t i = 0; i < sizeof(schrijfBuffer); i++) {
    schrijfBuffer[i] = static_cast<uint8_t>(i);
  }

  Serial.println();
  Serial.println("=== FLASH #1: Native transport, cache AAN ===");

  if (!flashNativeCache.begin()) {
    Serial.println("INIT: FOUT");
  } else {
    Serial.print("JEDEC: 0x");
    Serial.println(flashNativeCache.getJEDECID(), HEX);
    Serial.print("SIZE: ");
    Serial.println(flashNativeCache.size());

    uint32_t start = micros();
    bool eraseOk = flashNativeCache.eraseSector(TEST_ADRES / SFLASH_SECTOR_SIZE);
    flashNativeCache.waitUntilReady();
    uint32_t eraseTijd = micros() - start;

    start = micros();
    uint32_t geschreven = flashNativeCache.writeBuffer(TEST_ADRES, schrijfBuffer, sizeof(schrijfBuffer));
    flashNativeCache.waitUntilReady();
    uint32_t schrijfTijd = micros() - start;

    memset(leesBuffer, 0, sizeof(leesBuffer));

    start = micros();
    uint32_t gelezen = flashNativeCache.readBuffer(TEST_ADRES, leesBuffer, sizeof(leesBuffer));
    uint32_t leesTijd = micros() - start;

    bool inhoudOk = true;
    for (uint16_t i = 0; i < sizeof(leesBuffer); i++) {
      if (leesBuffer[i] != schrijfBuffer[i]) {
        inhoudOk = false;
        break;
      }
    }

    Serial.print("CACHE: ");
    Serial.println(flashNativeCache.isCached() ? "AAN" : "UIT");
    Serial.print("ERASE: ");
    Serial.println(eraseOk ? "OK" : "FOUT");
    Serial.print("WRITE: ");
    Serial.println(geschreven == sizeof(schrijfBuffer) ? "OK" : "FOUT");
    Serial.print("READ: ");
    Serial.println(gelezen == sizeof(leesBuffer) ? "OK" : "FOUT");
    Serial.print("VERIFY: ");
    Serial.println(inhoudOk ? "OK" : "FOUT");
    Serial.print("ERASE us: ");
    Serial.println(eraseTijd);
    Serial.print("WRITE us: ");
    Serial.println(schrijfTijd);
    Serial.print("READ us: ");
    Serial.println(leesTijd);

    flashNativeCache.end();
  }

  Serial.println();
  Serial.println("=== FLASH #1: Native transport, cache UIT ===");

  if (!flashNativeGeenCache.begin()) {
    Serial.println("INIT: FOUT");
  } else {
    Serial.print("CACHE: ");
    Serial.println(flashNativeGeenCache.isCached() ? "AAN" : "UIT");

    bool eraseOk = flashNativeGeenCache.eraseSector(TEST_ADRES / SFLASH_SECTOR_SIZE);
    flashNativeGeenCache.waitUntilReady();

    uint32_t start = micros();
    uint32_t geschreven = flashNativeGeenCache.writeBuffer(TEST_ADRES, schrijfBuffer, sizeof(schrijfBuffer));
    flashNativeGeenCache.waitUntilReady();
    uint32_t schrijfTijd = micros() - start;

    memset(leesBuffer, 0, sizeof(leesBuffer));

    start = micros();
    uint32_t gelezen = flashNativeGeenCache.readBuffer(TEST_ADRES, leesBuffer, sizeof(leesBuffer));
    uint32_t leesTijd = micros() - start;

    bool inhoudOk = true;
    for (uint16_t i = 0; i < sizeof(leesBuffer); i++) {
      if (leesBuffer[i] != schrijfBuffer[i]) {
        inhoudOk = false;
        break;
      }
    }

    Serial.print("ERASE: ");
    Serial.println(eraseOk ? "OK" : "FOUT");
    Serial.print("WRITE: ");
    Serial.println(geschreven == sizeof(schrijfBuffer) ? "OK" : "FOUT");
    Serial.print("READ: ");
    Serial.println(gelezen == sizeof(leesBuffer) ? "OK" : "FOUT");
    Serial.print("VERIFY: ");
    Serial.println(inhoudOk ? "OK" : "FOUT");
    Serial.print("WRITE us: ");
    Serial.println(schrijfTijd);
    Serial.print("READ us: ");
    Serial.println(leesTijd);

    flashNativeGeenCache.end();
  }

  Serial.println();
  Serial.println("=== FLASH #2: Extender transport, cache AAN ===");

  if (!flashExtenderCache.begin()) {
    Serial.println("INIT: FOUT");
  } else {
    Serial.print("JEDEC: 0x");
    Serial.println(flashExtenderCache.getJEDECID(), HEX);
    Serial.print("SIZE: ");
    Serial.println(flashExtenderCache.size());

    bool eraseOk = flashExtenderCache.eraseSector(TEST_ADRES / SFLASH_SECTOR_SIZE);
    flashExtenderCache.waitUntilReady();

    uint32_t start = micros();
    uint32_t geschreven = flashExtenderCache.writeBuffer(TEST_ADRES, schrijfBuffer, sizeof(schrijfBuffer));
    flashExtenderCache.waitUntilReady();
    uint32_t schrijfTijd = micros() - start;

    memset(leesBuffer, 0, sizeof(leesBuffer));

    start = micros();
    uint32_t gelezen = flashExtenderCache.readBuffer(TEST_ADRES, leesBuffer, sizeof(leesBuffer));
    uint32_t leesTijd = micros() - start;

    bool inhoudOk = true;
    for (uint16_t i = 0; i < sizeof(leesBuffer); i++) {
      if (leesBuffer[i] != schrijfBuffer[i]) {
        inhoudOk = false;
        break;
      }
    }

    Serial.print("CACHE: ");
    Serial.println(flashExtenderCache.isCached() ? "AAN" : "UIT");
    Serial.print("ERASE: ");
    Serial.println(eraseOk ? "OK" : "FOUT");
    Serial.print("WRITE: ");
    Serial.println(geschreven == sizeof(schrijfBuffer) ? "OK" : "FOUT");
    Serial.print("READ: ");
    Serial.println(gelezen == sizeof(leesBuffer) ? "OK" : "FOUT");
    Serial.print("VERIFY: ");
    Serial.println(inhoudOk ? "OK" : "FOUT");
    Serial.print("WRITE us: ");
    Serial.println(schrijfTijd);
    Serial.print("READ us: ");
    Serial.println(leesTijd);

    flashExtenderCache.end();
  }

  Serial.println();
  Serial.println("=== FLASH #2: Extender transport, cache UIT ===");

  if (!flashExtenderGeenCache.begin()) {
    Serial.println("INIT: FOUT");
  } else {
    Serial.print("CACHE: ");
    Serial.println(flashExtenderGeenCache.isCached() ? "AAN" : "UIT");

    bool eraseOk = flashExtenderGeenCache.eraseSector(TEST_ADRES / SFLASH_SECTOR_SIZE);
    flashExtenderGeenCache.waitUntilReady();

    uint32_t start = micros();
    uint32_t geschreven = flashExtenderGeenCache.writeBuffer(TEST_ADRES, schrijfBuffer, sizeof(schrijfBuffer));
    flashExtenderGeenCache.waitUntilReady();
    uint32_t schrijfTijd = micros() - start;

    memset(leesBuffer, 0, sizeof(leesBuffer));

    start = micros();
    uint32_t gelezen = flashExtenderGeenCache.readBuffer(TEST_ADRES, leesBuffer, sizeof(leesBuffer));
    uint32_t leesTijd = micros() - start;

    bool inhoudOk = true;
    for (uint16_t i = 0; i < sizeof(leesBuffer); i++) {
      if (leesBuffer[i] != schrijfBuffer[i]) {
        inhoudOk = false;
        break;
      }
    }

    Serial.print("ERASE: ");
    Serial.println(eraseOk ? "OK" : "FOUT");
    Serial.print("WRITE: ");
    Serial.println(geschreven == sizeof(schrijfBuffer) ? "OK" : "FOUT");
    Serial.print("READ: ");
    Serial.println(gelezen == sizeof(leesBuffer) ? "OK" : "FOUT");
    Serial.print("VERIFY: ");
    Serial.println(inhoudOk ? "OK" : "FOUT");
    Serial.print("WRITE us: ");
    Serial.println(schrijfTijd);
    Serial.print("READ us: ");
    Serial.println(leesTijd);

    flashExtenderGeenCache.end();
  }


  Serial.println();
  Serial.println("=== FLASH STRESS: 100 erase/write/read/verify-cycli per configuratie ===");
  Serial.println("LET OP: sector 0 wordt herhaald gewist en overschreven.");

  if (!flashNativeCache.begin()) {
    Serial.println("FLASH Native, cache AAN: INIT STRESS FOUT");
  } else {
    uint16_t fouten = 0;
    uint32_t totaalWrite = 0;
    uint32_t totaalRead = 0;

    for (uint16_t herhaling = 0; herhaling < 100; herhaling++) {
      for (uint16_t i = 0; i < sizeof(schrijfBuffer); i++) {
        schrijfBuffer[i] = static_cast<uint8_t>(i + herhaling);
      }

      if (!flashNativeCache.eraseSector(0)) {
        fouten++;
        continue;
      }
      flashNativeCache.waitUntilReady();

      uint32_t start = micros();
      bool schrijfStatus = flashNativeCache.writeSector(0, schrijfBuffer);
      bool syncStatus = flashNativeCache.syncDevice();
      flashNativeCache.waitUntilReady();
      totaalWrite += micros() - start;

      memset(leesBuffer, 0, sizeof(leesBuffer));

      start = micros();
      bool leesStatus = flashNativeCache.readSector(0, leesBuffer);
      totaalRead += micros() - start;

      bool inhoudOk = true;
      for (uint16_t i = 0; i < sizeof(leesBuffer); i++) {
        if (leesBuffer[i] != schrijfBuffer[i]) {
          inhoudOk = false;
          break;
        }
      }

      if (!schrijfStatus || !syncStatus || !leesStatus || !inhoudOk) {
        fouten++;
      }
    }

    Serial.println("FLASH Native, cache AAN");
    Serial.print("CYCLI: ");
    Serial.println(100);
    Serial.print("FOUTEN: ");
    Serial.println(fouten);
    Serial.print("GEM WRITE+SYNC us: ");
    Serial.println(totaalWrite / 100UL);
    Serial.print("GEM READ us: ");
    Serial.println(totaalRead / 100UL);
 

    flashNativeCache.end();
  }

  if (!flashNativeGeenCache.begin()) {
    Serial.println("FLASH Native, cache UIT: INIT STRESS FOUT");
  } else {
    uint16_t fouten = 0;
    uint32_t totaalWrite = 0;
    uint32_t totaalRead = 0;

    for (uint16_t herhaling = 0; herhaling < 100; herhaling++) {
      for (uint16_t i = 0; i < sizeof(schrijfBuffer); i++) {
        schrijfBuffer[i] = static_cast<uint8_t>(i + herhaling + 1);
      }

      if (!flashNativeGeenCache.eraseSector(0)) {
        fouten++;
        continue;
      }
      flashNativeGeenCache.waitUntilReady();

      uint32_t start = micros();
      bool schrijfStatus = flashNativeGeenCache.writeSector(0, schrijfBuffer);
      bool syncStatus = flashNativeGeenCache.syncDevice();
      flashNativeGeenCache.waitUntilReady();
      totaalWrite += micros() - start;

      memset(leesBuffer, 0, sizeof(leesBuffer));

      start = micros();
      bool leesStatus = flashNativeGeenCache.readSector(0, leesBuffer);
      totaalRead += micros() - start;

      bool inhoudOk = true;
      for (uint16_t i = 0; i < sizeof(leesBuffer); i++) {
        if (leesBuffer[i] != schrijfBuffer[i]) {
          inhoudOk = false;
          break;
        }
      }

      if (!schrijfStatus || !syncStatus || !leesStatus || !inhoudOk) {
        fouten++;
      }
    }

    Serial.println("FLASH Native, cache UIT");
    Serial.print("CYCLI: ");
    Serial.println(100);
    Serial.print("FOUTEN: ");
    Serial.println(fouten);
    Serial.print("GEM WRITE+SYNC us: ");
    Serial.println(totaalWrite / 100UL);
    Serial.print("GEM READ us: ");
    Serial.println(totaalRead / 100UL);
 

    flashNativeGeenCache.end();
  }

  if (!flashExtenderCache.begin()) {
    Serial.println("FLASH PCF8574 CS, cache AAN: INIT STRESS FOUT");
  } else {
    uint16_t fouten = 0;
    uint32_t totaalWrite = 0;
    uint32_t totaalRead = 0;

    for (uint16_t herhaling = 0; herhaling < 100; herhaling++) {
      for (uint16_t i = 0; i < sizeof(schrijfBuffer); i++) {
        schrijfBuffer[i] = static_cast<uint8_t>(i + herhaling + 2);
      }

      if (!flashExtenderCache.eraseSector(0)) {
        fouten++;
        continue;
      }
      flashExtenderCache.waitUntilReady();

      uint32_t start = micros();
      bool schrijfStatus = flashExtenderCache.writeSector(0, schrijfBuffer);
      bool syncStatus = flashExtenderCache.syncDevice();
      flashExtenderCache.waitUntilReady();
      totaalWrite += micros() - start;

      memset(leesBuffer, 0, sizeof(leesBuffer));

      start = micros();
      bool leesStatus = flashExtenderCache.readSector(0, leesBuffer);
      totaalRead += micros() - start;

      bool inhoudOk = true;
      for (uint16_t i = 0; i < sizeof(leesBuffer); i++) {
        if (leesBuffer[i] != schrijfBuffer[i]) {
          inhoudOk = false;
          break;
        }
      }

      if (!schrijfStatus || !syncStatus || !leesStatus || !inhoudOk) {
        fouten++;
      }
    }

    Serial.println("FLASH PCF8574 CS, cache AAN");
    Serial.print("CYCLI: ");
    Serial.println(100);
    Serial.print("FOUTEN: ");
    Serial.println(fouten);
    Serial.print("GEM WRITE+SYNC us: ");
    Serial.println(totaalWrite / 100UL);
    Serial.print("GEM READ us: ");
    Serial.println(totaalRead / 100UL);
 

    flashExtenderCache.end();
  }

  if (!flashExtenderGeenCache.begin()) {
    Serial.println("FLASH PCF8574 CS, cache UIT: INIT STRESS FOUT");
  } else {
    uint16_t fouten = 0;
    uint32_t totaalWrite = 0;
    uint32_t totaalRead = 0;

    for (uint16_t herhaling = 0; herhaling < 100; herhaling++) {
      for (uint16_t i = 0; i < sizeof(schrijfBuffer); i++) {
        schrijfBuffer[i] = static_cast<uint8_t>(i + herhaling + 3);
      }

      if (!flashExtenderGeenCache.eraseSector(0)) {
        fouten++;
        continue;
      }
      flashExtenderGeenCache.waitUntilReady();

      uint32_t start = micros();
      bool schrijfStatus = flashExtenderGeenCache.writeSector(0, schrijfBuffer);
      bool syncStatus = flashExtenderGeenCache.syncDevice();
      flashExtenderGeenCache.waitUntilReady();
      totaalWrite += micros() - start;

      memset(leesBuffer, 0, sizeof(leesBuffer));

      start = micros();
      bool leesStatus = flashExtenderGeenCache.readSector(0, leesBuffer);
      totaalRead += micros() - start;

      bool inhoudOk = true;
      for (uint16_t i = 0; i < sizeof(leesBuffer); i++) {
        if (leesBuffer[i] != schrijfBuffer[i]) {
          inhoudOk = false;
          break;
        }
      }

      if (!schrijfStatus || !syncStatus || !leesStatus || !inhoudOk) {
        fouten++;
      }
    }

    Serial.println("FLASH PCF8574 CS, cache UIT");
    Serial.print("CYCLI: ");
    Serial.println(100);
    Serial.print("FOUTEN: ");
    Serial.println(fouten);
    Serial.print("GEM WRITE+SYNC us: ");
    Serial.println(totaalWrite / 100UL);
    Serial.print("GEM READ us: ");
    Serial.println(totaalRead / 100UL);
 

    flashExtenderGeenCache.end();
  }

  Serial.println();
  Serial.println("=== EINDE FLASH TEST ===");
}

void loop() {
}
