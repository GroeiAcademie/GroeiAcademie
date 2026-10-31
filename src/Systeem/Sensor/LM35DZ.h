#ifndef SENSOR_LM35DZ_H
#define SENSOR_LM35DZ_H

#include <Arduino.h>
#include "SensorAnaloog.h"
#include "../../Configuratie/SystemConfig.h"
#if SENSOR_LM35DZ_AANTAL > 0

// LM35DZ: temperatuursensor; ruwe ADC-waarde plus temperatuur via expliciet ADC-meetbereik.
// Eén component voor één fysiek apparaat, via de bestaande ADC-Extender.
// De meetcode zit in SensorAnaloog; deze klasse rekent in MeetwaardeVerwerken() om naar temperatuur.
struct LM35DZ : SensorAnaloog {
  int laatsteMeetwaarde = 0;
  uint16_t laatsteEffectiefBereikMillivolt = 0;
  bool laatsteMetingGeldig = false;

  LM35DZ(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
         HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, SENSOR_LM35DZ_ADC_BEREIK_MILLIVOLT, Extender) {}

  uint8_t AantalChannels() const override { return 2; }   // meetkanalen voor Data
  // De omrekening heeft het werkelijk geconfigureerde ADC-meetbereik nodig.
  float TemperatuurC();

protected:
  // Kanaal 1: ADC-waarde, kanaal 2: afgeleide temperatuur (°C).
  void MeetwaardeVerwerken(int32_t waarde, bool gelukt) override;
};

#endif
#endif
