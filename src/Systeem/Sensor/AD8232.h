#ifndef SENSOR_AD8232_H
#define SENSOR_AD8232_H
#include <Arduino.h>
#include "SensorAnaloog.h"
#include "../../Configuratie/SystemConfig.h"
#if SENSOR_AD8232_AANTAL > 0
struct AD8232 : SensorAnaloog {
  // LO+ en LO- (elektrode los), optioneel: native pinnen. Ze worden niet gereserveerd in de GedeeldeBus,
  // want OUT hoort bij de ADC-Extender en gemengde gebieden staan standaard uit: kies vrije pinnen.
  const HardwareResourcePin np_LOPLUS = HardwareResourcePin::NONE;
  const HardwareResourcePin np_LOMIN = HardwareResourcePin::NONE;
  AD8232(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, SENSOR_AD8232_ADC_BEREIK_MILLIVOLT, Extender) {}
  AD8232(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourcePin np_LOPLUS, HardwareResourcePin np_LOMIN,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, SENSOR_AD8232_ADC_BEREIK_MILLIVOLT, Extender),
      np_LOPLUS(np_LOPLUS), np_LOMIN(np_LOMIN) {}
  bool Activeren() override {
    if (!SensorAnaloog::Activeren()) return false;
    if (np_LOPLUS != HardwareResourcePin::NONE) pinMode(NativeArduinoPinVan(np_LOPLUS), INPUT);
    if (np_LOMIN != HardwareResourcePin::NONE) pinMode(NativeArduinoPinVan(np_LOMIN), INPUT);
    return true;
  }
  // Elektrode los (LO+ of LO- hoog): geen meting, zodat een losse elektrode nooit als meetwaarde doorkomt.
  // Zelfde namen als in SensorAnaloog: via een AD8232 worden deze versies gebruikt.
  bool StartAnalogRead() {
    if ((np_LOPLUS != HardwareResourcePin::NONE && digitalRead(NativeArduinoPinVan(np_LOPLUS)) == HIGH) ||
        (np_LOMIN != HardwareResourcePin::NONE && digitalRead(NativeArduinoPinVan(np_LOMIN)) == HIGH)) return false;
    return SensorAnaloog::StartAnalogRead();
  }
  bool RawAnalogRead(int32_t& waarde) {
    waarde = 0;
    if ((np_LOPLUS != HardwareResourcePin::NONE && digitalRead(NativeArduinoPinVan(np_LOPLUS)) == HIGH) ||
        (np_LOMIN != HardwareResourcePin::NONE && digitalRead(NativeArduinoPinVan(np_LOMIN)) == HIGH)) return false;
    return SensorAnaloog::RawAnalogRead(waarde);
  }
};

#endif
#endif
