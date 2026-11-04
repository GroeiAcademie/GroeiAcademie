#ifndef SENSOR_DIGITAAL_H
#define SENSOR_DIGITAAL_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_HCSR501_AANTAL > 0 || SENSOR_HW201_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"

// SensorDigitaal: gemeenschappelijke basis voor een digitale Sensor met één OUT-ingang,
// native of via een GPIO-Extender. Een afgeleide Sensor geeft alleen zijn OUT-pin door.
struct SensorDigitaal : Sensor {
  const HardwareResourcePin np_OUT;
  uint8_t exclusief_[1];
  SensorDigitaal(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_OUT)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, HardwareResourceToegang::EXCLUSIEF), np_OUT(np_OUT) {
    exclusief_[0] = static_cast<uint8_t>(np_OUT);
  }

  bool inpluggen() override;
  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};

#endif

#endif
