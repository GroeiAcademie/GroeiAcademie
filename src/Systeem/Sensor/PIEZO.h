#ifndef SENSOR_PIEZO_H
#define SENSOR_PIEZO_H

#include <Arduino.h>
#include "SensorAnaloog.h"
#include "../../Configuratie/SystemConfig.h"
#if SENSOR_PIEZO_AANTAL > 0

// PIEZO: piezo-element; alleen beveiligd/conditioneerd analoog signaal.
// Eén component voor één fysiek apparaat, via de bestaande ADC-Extender.
// De meetcode zit in SensorAnaloog; deze klasse geeft alleen de pin en het bereik door.
struct PIEZO : SensorAnaloog {
  PIEZO(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, SENSOR_PIEZO_ADC_BEREIK_MILLIVOLT, Extender) {}
};

#endif
#endif
