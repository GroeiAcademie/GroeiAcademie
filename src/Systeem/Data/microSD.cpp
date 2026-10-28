#include "Data.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

// ============================================================================
// microSD met CS via een Extender
// SdFat schakelt CS zelf met sdCsInit()/sdCsWrite() en kent alleen een pinnummer. Die functies mag een programma alleen
// vervangen wanneer SD_CHIP_SELECT_MODE in SdFatConfig.h van "SdFat - Adafruit Fork" op 1 staat.
// ============================================================================
#if GEDEELDE_BUS_MIXED_PINS && (defined(DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER) || defined(DATA_LAYER_MICROSD_SPI_2_CS_EXTENDER))
  #define MICROSD_CS_VIA_EXTENDER 1   // alleen in dit bestand
  #if SD_CHIP_SELECT_MODE == 0
    #error microSD met CS via een Extender: zet SD_CHIP_SELECT_MODE op 1 in SdFatConfig.h van de library "SdFat - Adafruit Fork".
  #endif

namespace {
  // vanaf dit nummer betekent het pinnummer voor SdFat "kaart met CS via een Extender", met de plaats in linkCSviaExtender[].
  const SdCsPin_t startValueCSviaExtender = 0xF0;
  struct DataMICROSDviaSPI* linkCSviaExtender[DATA_LAYER_MICROSD_SPI_AANTAL] = {};
}

void sdCsInit(SdCsPin_t pin) {
  if (pin >= startValueCSviaExtender) return;   // via een Extender: al als uitgang en HIGH gezet in HardwareResourceTypeSPI::inpluggen()
  pinMode(pin, OUTPUT);
}

void sdCsWrite(SdCsPin_t pin, bool level) {
  if (pin >= startValueCSviaExtender) {
    struct DataMICROSDviaSPI* card = linkCSviaExtender[pin - startValueCSviaExtender];   // de kaart met dit pinnummer
    if (card != nullptr) card->WriteCS(level ? HIGH : LOW);
    return;
  }
  digitalWrite(pin, level ? HIGH : LOW);
}
#endif

DataMICROSDviaSPI::~DataMICROSDviaSPI() {
  struct Data* data = static_cast<struct Data*>(parent);   // de ouder (Data) van deze chip
  if (data == nullptr) return;
  for (uint8_t teller = 0; teller < DATA_LAYER_MICROSD_SPI_AANTAL; teller++) {
    if (data->MICROSDviaSPI[teller] == this) data->MICROSDviaSPI[teller] = nullptr;
#if MICROSD_CS_VIA_EXTENDER
    if (linkCSviaExtender[teller] == this) linkCSviaExtender[teller] = nullptr;
#endif
  }
}

bool DataMICROSDviaSPI::Activeren() {
  SdCsPin_t pinCS = NativeArduinoPinVan(np_CS);   // het pinnummer dat SdFat krijgt
#if MICROSD_CS_VIA_EXTENDER
  if (extenderCS[0] != nullptr) {
    for (uint8_t teller = 0; teller < DATA_LAYER_MICROSD_SPI_AANTAL; teller++) {
      if (linkCSviaExtender[teller] == nullptr || linkCSviaExtender[teller] == this) {
        linkCSviaExtender[teller] = this;
        pinCS = startValueCSviaExtender + teller;
        break;
      }
    }
  }
#endif
  // SdFat geeft een foutcode terug (sdErrorCode()), 0 = geen fout. Gedeelde SPI-bus, 4 MHz zoals voorheen.
  const bool gelukt = SdFat32::begin(SdSpiConfig(pinCS, SHARED_SPI, SD_SCK_MHZ(4)));
  return FoutDiagnoseSPI(gelukt, SdFat32::sdErrorCode(), gelukt);
}

#endif
