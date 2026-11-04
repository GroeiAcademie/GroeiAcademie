#ifndef SENSOR_GSRGrove_H
#define SENSOR_GSRGrove_H

#include <Arduino.h>
#include "SensorAnaloog.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_GSRGROVE_AANTAL > 0

// GSRGrove: Grove GSR analoge uitgang, uitsluitend ADC-waarde.
// Eén component voor één fysiek apparaat, via de bestaande ADC-Extender.
// De meetcode zit in SensorAnaloog; deze klasse geeft alleen de pin en het bereik door.
struct GSRGrove : SensorAnaloog {
  GSRGrove(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT, Extender) {}
};

#endif

#endif
