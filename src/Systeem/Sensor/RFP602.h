#ifndef RFP602_H
#define RFP602_H

#include <Arduino.h>
#include "../GedeeldeBus/GedeeldeBus.h"
#include "../../Configuratie/SystemConfig.h"

// ============================================================================
// RFP602
// ============================================================================
// Eén sensorobject voor de maximaal vier RFP602-kanalen van Stimulus.
// De sensor wordt ingeplugd op de gekozen ADC-extender.
// Voorlopig biedt de sensor alleen de directe leesfunctie RawAnalogRead().
// ============================================================================
struct RFP602 : Sensor {
  uint8_t exclusief_[4];

  const int sensorPin[4] = {
#if ADC_BACKEND == ADC_BACKEND_NATIVE
    NativeArduinoPinVan(ADC_PIN_SENSOR_1),
    NativeArduinoPinVan(ADC_PIN_SENSOR_2),
    NativeArduinoPinVan(ADC_PIN_SENSOR_3),
    NativeArduinoPinVan(ADC_PIN_SENSOR_4)
#else
    static_cast<int>(ADC_PIN_SENSOR_1),
    static_cast<int>(ADC_PIN_SENSOR_2),
    static_cast<int>(ADC_PIN_SENSOR_3),
    static_cast<int>(ADC_PIN_SENSOR_4)
#endif
  };

  RFP602();
  RFP602(GedeeldeBusNode* parent, GedeeldeBusComponent component,
         HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : Sensor(parent, { nullptr, 0, exclusief_, AANTAL_SENSOREN_AANWEZIG }, component, Extender) {
    exclusief_[0] = static_cast<uint8_t>(ADC_PIN_SENSOR_1);
    exclusief_[1] = static_cast<uint8_t>(ADC_PIN_SENSOR_2);
    exclusief_[2] = static_cast<uint8_t>(ADC_PIN_SENSOR_3);
    exclusief_[3] = static_cast<uint8_t>(ADC_PIN_SENSOR_4);
  }

  bool Activeren() override;
  int RawAnalogRead(int sensorPin);
};

extern struct RFP602* sensorRFP602;

#endif
