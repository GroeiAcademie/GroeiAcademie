#ifndef SENSOR_AHT10_H
#define SENSOR_AHT10_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_AHT10_AANTAL > 0

#include "../GedeeldeBus/GedeeldeBus.h"
#include <Adafruit_AHTX0.h>

// Officiële API: https://github.com/adafruit/Adafruit_AHTX0/blob/master/Adafruit_AHTX0.h
struct AHT10 : Sensor {
  Adafruit_AHTX0 aht;
  AHT10(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, {nullptr, 0, nullptr, 0}, component, HardwareResourceToegang::GEDEELD) {}
  uint8_t AantalChannels() const override { return 2; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};

#endif

#endif
