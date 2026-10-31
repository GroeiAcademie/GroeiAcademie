#ifndef SENSOR_WATER_H
#define SENSOR_WATER_H

#include <Arduino.h>
#include "SensorAnaloog.h"
#include "../../Configuratie/SystemConfig.h"
#if SENSOR_WATER_AANTAL > 0

// WATER: analoge watersensormodule; exacte modulevariant te controleren.
// Eén component voor één fysiek apparaat, via de bestaande ADC-Extender.
// De meetcode zit in SensorAnaloog; deze klasse geeft alleen de pin en het bereik door.
struct WATER : SensorAnaloog {
  WATER(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, SENSOR_WATER_ADC_BEREIK_MILLIVOLT, Extender) {}
};

#endif
#endif
