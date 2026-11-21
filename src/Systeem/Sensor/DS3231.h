#ifndef SENSOR_DS3231_H
#define SENSOR_DS3231_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_DS3231_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"
#include <RTClib.h>

// DS3231: klok met batterijback-up (datum en tijd), plus de eigen temperatuursensor van de chip.
// Data-kanalen: 1 = Unix-tijd (seconden sinds 1 januari 1970, uint32_t), 2 = temperatuur (°C).
// Met microSD krijgt SENSOR.DAT (en elk ander bestand) automatisch de echte bestandsdatum.
struct DS3231 : Sensor {
  RTC_DS3231 ds3231;
  DS3231(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, {nullptr, 0, nullptr, 0}, component, HardwareResourceToegang::GEDEELD) {}
#if GEDEELDE_BUS_MIXED_PINS
  // Mixed: I2C via de gewone parent (bv. achter TCA9548A); INT/SQW op een tweede Extender (PCF8574, PCF8575, MCP23017, MCP23S17, SC16IS752 of MAX14830 GPIO).
  // np_SQW: Extender-pinnummer(s) op die tweede Extender, geen Arduino-nummers.
  uint8_t exclusief_[1];
  GedeeldeBusNode* extenderHulp_[1] = {nullptr};
  uint8_t np_SQW = static_cast<uint8_t>(HardwareResourcePin::NONE);
  bool viaMixed = false;
  DS3231(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component, GedeeldeBusNode* extender, uint8_t np_SQW)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, HardwareResourceToegang::GEDEELD), np_SQW(np_SQW) {
    exclusief_[0] = np_SQW;
    extenderHulp_[0] = extender;
    ownerClaim = extenderHulp_;
    viaMixed = true;
  }
#endif
  ~DS3231() override;   // de bestandsdatum van SdFat niet langer via deze klok laten lopen
  uint8_t AantalChannels() const override { return 2; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesDatumTijd(DateTime& datumTijd);
  bool ZetDatumTijd(const DateTime& datumTijd);
#if GEDEELDE_BUS_MIXED_PINS
  // INT/SQW lezen (true = hoog). Activeren() zet een blokgolf van 1 Hz op SQW.
  bool ReadSQW(bool& waarde) {
    waarde = false;
    if (!actief || !viaMixed || extenderHulp_[0] == nullptr) return false;
    const uint8_t niveau = extenderHulp_[0]->ReadExtenderPin(np_SQW);
    if (niveau != LOW && niveau != HIGH) return false;
    waarde = niveau == HIGH;
    return true;
  }
#endif
};

#endif

#endif
