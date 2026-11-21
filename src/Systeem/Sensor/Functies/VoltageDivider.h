#ifndef VOLTAGEDIVIDER_H
#define VOLTAGEDIVIDER_H

#include <Arduino.h>
#include "../../../Configuratie/SystemConfig.h"

// Weerstand van een sensor in een spanningsdeler: voeding - serieweerstand - meetpunt - sensor - GND.
// rawValue: ADC-waarde herschaald zodat ADC_MAX overeenkomt met de voedingsspanning van de deler (zie MF52x).
// Sensor aan de voedingskant in plaats van aan GND: de formule is dan omgekeerd; dat hangt van de module af.
struct VoltageDivider {
  static bool CalculateSensorResistanceOhm(int32_t rawValue, uint32_t seriesResistorOhm, float& sensorResistanceOhm);
};

#endif
