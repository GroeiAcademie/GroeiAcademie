#ifndef NRF24L01_H
#define NRF24L01_H

#include "../GedeeldeBus/GedeeldeBus.h"

// ============================================================================
// NRF24L01 SPI: radiomodule (RF24). Elk record dat naar Data gaat, wordt ook draadloos verstuurd.
// CS kan niet via een Extender: de RF24-library schakelt CS (CSN) en CE zelf.
// Geen ontvangstbevestiging (auto-ack uit): een meting wacht nooit op een ontvanger.
// ============================================================================
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_NRF24L01_SPI)

  #include <RF24.h>

struct DataNRF24L01viaSPI : HardwareResourceTypeSPI, RF24 {
  HardwareResourcePin np_CE;
  uint8_t exclusiefMetCE_[2];
  DataNRF24L01viaSPI(GedeeldeBusNode* parent, HardwareResourcePin np_CS, HardwareResourcePin np_CE, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, GedeeldeBusComponent::DATA_NRF24L01, np_CS, np_SCK, np_MISO, np_MOSI, Extender),
      RF24(NativeArduinoPinVan(np_CE), NativeArduinoPinVan(np_CS)), np_CE(np_CE) {
    exclusiefMetCE_[0] = static_cast<uint8_t>(np_CS);
    exclusiefMetCE_[1] = static_cast<uint8_t>(np_CE);
    aangemeldePinnen = { gedeeld_, 3, exclusiefMetCE_, 2 };
  }

  ~DataNRF24L01viaSPI() override;   // wist de eigen plaats in de lijst van Data

  bool Activeren() override;
};

#endif

#endif // NRF24L01_H
