#include "Data.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

// ============================================================================
// microSD met CS via een Extender
// SdFat schakelt CS zelf met sdCsInit()/sdCsWrite() en kent alleen een pinnummer. Die functies mag een programma alleen
// vervangen wanneer SD_CHIP_SELECT_MODE in SdFatConfig.h van "SdFat - Adafruit Fork" op 1 of 2 staat.
// ============================================================================
#if GEDEELDE_BUS_GEMENGDE_PINNEN && (defined(DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER) || defined(DATA_LAYER_MICROSD_SPI_2_CS_EXTENDER))
  #define MICROSD_CS_VIA_EXTENDER 1   // NAAM VOORLOPIG: alleen in dit bestand
  #if SD_CHIP_SELECT_MODE == 0
    #error microSD met CS via een Extender: zet SD_CHIP_SELECT_MODE op 1 of 2 in SdFatConfig.h van de library "SdFat - Adafruit Fork".
  #endif

namespace {
  // NAAM VOORLOPIG: vanaf dit nummer betekent het pinnummer voor SdFat "kaart met CS via een Extender", met de plaats in csViaExtender[].
  const SdCsPin_t csViaExtenderBasis = 0xF0;
  struct DataMICROSDviaSPI* csViaExtender[DATA_LAYER_MICROSD_SPI_AANTAL] = {};   // NAAM VOORLOPIG
}

void sdCsInit(SdCsPin_t pin) {
  if (pin >= csViaExtenderBasis) return;   // via een Extender: al als uitgang en HIGH gezet in HardwareResourceTypeSPI::inpluggen()
  pinMode(pin, OUTPUT);
}

void sdCsWrite(SdCsPin_t pin, bool level) {
  if (pin >= csViaExtenderBasis) {
    struct DataMICROSDviaSPI* kaart = csViaExtender[pin - csViaExtenderBasis];   // NAAM VOORLOPIG: de kaart met dit pinnummer
    if (kaart != nullptr) kaart->SchrijvenCS(level ? HIGH : LOW);
    return;
  }
  digitalWrite(pin, level ? HIGH : LOW);
}
#endif

DataMICROSDviaSPI::~DataMICROSDviaSPI() {
  struct Data* data = static_cast<struct Data*>(parent);   // NAAM VOORLOPIG: de ouder (Data) van deze chip
  if (data == nullptr) return;
  for (uint8_t teller = 0; teller < DATA_LAYER_MICROSD_SPI_AANTAL; teller++) {
    if (data->MICROSDviaSPI[teller] == this) data->MICROSDviaSPI[teller] = nullptr;
#if MICROSD_CS_VIA_EXTENDER
    if (csViaExtender[teller] == this) csViaExtender[teller] = nullptr;
#endif
  }
}

bool DataMICROSDviaSPI::Activeren() {
  SdCsPin_t cs = NativeArduinoPinVan(np_CS);   // NAAM VOORLOPIG: het pinnummer dat SdFat krijgt
#if MICROSD_CS_VIA_EXTENDER
  if (csGebied_[0] != nullptr) {
    for (uint8_t teller = 0; teller < DATA_LAYER_MICROSD_SPI_AANTAL; teller++) {
      if (csViaExtender[teller] == nullptr || csViaExtender[teller] == this) {
        csViaExtender[teller] = this;
        cs = csViaExtenderBasis + teller;
        break;
      }
    }
  }
#endif
  // SdFat geeft een foutcode terug (sdErrorCode()), 0 = geen fout. Gedeelde SPI-bus, 4 MHz zoals voorheen.
  const bool gelukt = SdFat32::begin(SdSpiConfig(cs, SHARED_SPI, SD_SCK_MHZ(4)));
  return FoutDiagnoseSPI(gelukt, SdFat32::sdErrorCode(), gelukt);
}

#endif
