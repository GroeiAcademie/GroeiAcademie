#ifndef SENSOR_HCSR501_H
#define SENSOR_HCSR501_H

#include <Arduino.h>
#include "SensorDigitaal.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_HCSR501_AANTAL > 0
// HC-SR501 heeft geen ID of busprotocol. Dit controleert alleen de geconfigureerde ingang.
struct HCSR501 : SensorDigitaal {
  HCSR501(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_OUT)
    : SensorDigitaal(parent, component, np_OUT) {}
};

#endif

#endif
