#ifndef SENSOR_MF52X_H
#define SENSOR_MF52X_H

#include <Arduino.h>
#include "SensorAnaloog.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_MF52A_AANTAL > 0 || SENSOR_MF52D_AANTAL > 0

// MF52A: NTC thermistor; spanning, niet omgerekende temperatuur.
// Eén component voor één fysiek apparaat, via de bestaande ADC-Extender.
// De meetcode zit in SensorAnaloog; deze klasse geeft alleen de pin en het bereik door.
// Het fysieke type behoudt zijn eigen componentcode en configuratie.
// De analoge ADC-meetfuncties zijn identiek aan die van MF52A.
struct MF52x : SensorAnaloog {
  MF52x(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, component == GedeeldeBusComponent::MF52D ? SENSOR_MF52D_ADC_BEREIK_MILLIVOLT : SENSOR_MF52A_ADC_BEREIK_MILLIVOLT, Extender) {}
};

#endif

#endif
