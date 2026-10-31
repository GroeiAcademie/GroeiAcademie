#ifndef SENSOR_MAX4466_9814_H
#define SENSOR_MAX4466_9814_H
#include <Arduino.h>
#include "SensorAnaloog.h"
#include "../../Configuratie/SystemConfig.h"
#if SENSOR_MAX4466_AANTAL > 0 || SENSOR_MAX9814_AANTAL > 0
struct MAX4466_9814 : SensorAnaloog {
  MAX4466_9814(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, component == GedeeldeBusComponent::MAX9814 ? SENSOR_MAX9814_ADC_BEREIK_MILLIVOLT : SENSOR_MAX4466_ADC_BEREIK_MILLIVOLT, Extender) {}
};

#endif
#endif
