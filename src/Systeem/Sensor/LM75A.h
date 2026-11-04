#ifndef SENSOR_LM75A_H
#define SENSOR_LM75A_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_LM75A_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"
#include <M2M_LM75A.h>

struct LM75A : Sensor {
  M2M_LM75A lm75a;
  LM75A(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, {nullptr, 0, nullptr, 0}, component, HardwareResourceToegang::GEDEELD), lm75a(parent ? parent->adres : 0) {}   // geen parent: Activeren() faalt later; hier niets lezen via nullptr
  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};

#endif

#endif
