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
  // OUT via een ADC-Extender zonder digitale pin (ADS1115, ADS7828, ADS7953, CD74HC4067, of een AIN-kanaal van de ADS1158).
  // Meetbereik voor de ADC; 0 betekent: OUT wordt digitaal gelezen (native of via een GPIO-Extender). inpluggen() zet de waarde.
  uint16_t analogMeasurementRangeMillivolt = 0;
  // Vanaf deze spanning is OUT hoog, alleen bij lezen via een ADC-Extender.
  const uint16_t analogThresholdForHighMillivolt;
  SensorDigitaal(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_OUT)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, HardwareResourceToegang::EXCLUSIEF), np_OUT(np_OUT),
      analogThresholdForHighMillivolt(component == GedeeldeBusComponent::HW201 ? SENSOR_HW201_ANALOG_THRESHOLD_FOR_HIGH_MILLIVOLT : SENSOR_HCSR501_ANALOG_THRESHOLD_FOR_HIGH_MILLIVOLT) {
    exclusief_[0] = static_cast<uint8_t>(np_OUT);
  }

  bool inpluggen() override;
  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool IsAnalogChild() const override { return analogMeasurementRangeMillivolt != 0; }   // OUT via een ADC-Extender
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
  // OUT lezen via de ADC-Extender: één meting, hoog vanaf analogThresholdForHighMillivolt (waarde 0 of 1).
  bool ReadOUTviaAnalogThreshold(float& waarde);
};

#endif

#endif
