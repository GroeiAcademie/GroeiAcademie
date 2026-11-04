#ifndef SENSOR_EGBOPULSESENSOR_H
#define SENSOR_EGBOPULSESENSOR_H

#include <Arduino.h>
#include "SensorAnaloog.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_EGBOPULSESENSOR_AANTAL > 0
struct EGBOPulseSensor : SensorAnaloog {
  EGBOPulseSensor(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, SENSOR_EGBOPULSESENSOR_ADC_BEREIK_MILLIVOLT, Extender) {}
};

#endif

#endif
