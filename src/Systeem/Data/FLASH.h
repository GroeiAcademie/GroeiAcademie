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
// CS op een pin van het board, of via een Extender wanneer GEDEELDE_BUS_GEMENGDE_PINNEN actief is (O8).
struct TransportFLASHviaSPI : Adafruit_FlashTransport {
  uint8_t csPin;   // NAAM VOORLOPIG: CS-pin van het board voor deze FLASH-chip
  uint32_t WriteClock = 4000000;   // SPI-klok in Hz voor schrijven, wissen en commando's
  uint32_t ReadClock = 4000000;    // SPI-klok in Hz voor lezen
#if GEDEELDE_BUS_GEMENGDE_PINNEN
  GedeeldeBusNode* csExtender = nullptr;   // NAAM VOORLOPIG: de Extender waarop CS zit; nullptr = CS op het board. csPin is dan de pin op die Extender.
#endif

  TransportFLASHviaSPI(uint8_t csPin) : csPin(csPin) {
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

  // SchrijvenCS (NAAM VOORLOPIG): CS op het board, of via de Extender van CS.
  void SchrijvenCS(uint8_t niveau) {
#if GEDEELDE_BUS_GEMENGDE_PINNEN
    if (csExtender != nullptr) { csExtender->SchrijvenPin(csPin, niveau); return; }
#endif
    digitalWrite(csPin, niveau);
  }
};

struct DataFLASHviaSPI : HardwareResourceTypeSPI, Adafruit_SPIFlashBase {
  TransportFLASHviaSPI transport;   // NAAM VOORLOPIG: het SPI-transport (CS en klok) van deze chip

  DataFLASHviaSPI(GedeeldeBusNode* parent, HardwareResourcePin np_CS, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, GedeeldeBusComponent::DATA_FLASH, np_CS, np_SCK, np_MISO, np_MOSI, Extender), Adafruit_SPIFlashBase(&transport), transport(NativeArduinoPinVan(np_CS)) {}

  ~DataFLASHviaSPI() override;   // wist de eigen plaats in de lijst van Data

  bool Activeren() override;
};

#endif

#endif // FLASH_H
