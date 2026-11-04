#ifndef SENSOR_HW201_H
#define SENSOR_HW201_H

#include <Arduino.h>
#include "SensorDigitaal.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_HW201_AANTAL > 0
// Digitale comparator heeft geen chip-ID. Alleen de invoer is voorbereid;
// aanwezigheid en detectie moeten tijdens de hardwaretest bewezen worden.
struct HW201 : SensorDigitaal {
  HW201(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_OUT)
    : SensorDigitaal(parent, component, np_OUT) {}
};

#endif

#endif
