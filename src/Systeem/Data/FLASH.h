#ifndef FLASH_H
#define FLASH_H

#include "../GedeeldeBus/GedeeldeBus.h"

// ============================================================================
// FLASH SPI: Adafruit_SPIFlashBase (zonder cache en zonder bestandssysteem) met een eigen transport.
// ============================================================================
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)

  #include <SPI.h>
  #include <Adafruit_SPIFlashBase.h>

// Eigen transport: zelfde SPI-commando's als Adafruit_FlashTransport_SPI, maar CS wordt hier geschakeld.
// CS op een pin van het board, of via een Extender wanneer GEDEELDE_BUS_MIXED_PINS actief is (O8).
struct TransportFLASHviaSPI : Adafruit_FlashTransport {
  uint8_t pinCS;   // CS-pin van het board voor deze FLASH-chip
  uint32_t WriteClock = 4000000;   // SPI-klok in Hz voor schrijven, wissen en commando's
  uint32_t ReadClock = 4000000;    // SPI-klok in Hz voor lezen
#if GEDEELDE_BUS_MIXED_PINS
  GedeeldeBusNode* extenderCS = nullptr;   // de Extender waarop CS zit; nullptr = CS op het board. pinCS is dan de pin op die Extender.
#endif

  TransportFLASHviaSPI(uint8_t pinCS) : pinCS(pinCS) {
    _cmd_read = SFLASH_CMD_READ;
    _addr_len = 3;
  }

  void begin(void) override;
  void end(void) override;
  bool supportQuadMode(void) override { return false; }
  void setClockSpeed(uint32_t write_hz, uint32_t read_hz) override { WriteClock = write_hz; ReadClock = read_hz; }
  bool runCommand(uint8_t command) override;
  bool readCommand(uint8_t command, uint8_t* response, uint32_t len) override;
  bool writeCommand(uint8_t command, uint8_t const* data, uint32_t len) override;
  bool eraseCommand(uint8_t command, uint32_t addr) override;
  bool readMemory(uint32_t addr, uint8_t* data, uint32_t len) override;
  bool writeMemory(uint32_t addr, uint8_t const* data, uint32_t len) override;

  // WriteCS: CS op het board, of via de Extender van CS.
  void WriteCS(uint8_t niveau) {
#if GEDEELDE_BUS_MIXED_PINS
    if (extenderCS != nullptr) { extenderCS->WriteExtenderPin(pinCS, niveau); return; }
#endif
    digitalWrite(pinCS, niveau);
  }
};

struct DataFLASHviaSPI : HardwareResourceTypeSPI, Adafruit_SPIFlashBase {
  TransportFLASHviaSPI transport;   // het SPI-transport (CS en klok) van deze chip

  DataFLASHviaSPI(GedeeldeBusNode* parent, HardwareResourcePin np_CS, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, GedeeldeBusComponent::DATA_FLASH, np_CS, np_SCK, np_MISO, np_MOSI, Extender), Adafruit_SPIFlashBase(&transport), transport(NativeArduinoPinVan(np_CS)) {}

  ~DataFLASHviaSPI() override;   // wist de eigen plaats in de lijst van Data

  bool Activeren() override;
};

#endif

#endif // FLASH_H
