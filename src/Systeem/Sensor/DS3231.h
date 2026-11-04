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
  ~DS3231() override;   // de bestandsdatum van SdFat niet langer via deze klok laten lopen
  uint8_t AantalChannels() const override { return 2; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesDatumTijd(DateTime& datumTijd);
  bool ZetDatumTijd(const DateTime& datumTijd);
};

#endif

#endif
