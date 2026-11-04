#ifndef SENSOR_SDP810125PA_H
#define SENSOR_SDP810125PA_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_SDP810125PA_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"
#include <SensirionI2CSdp.h>

struct SDP810125PA : Sensor {
  SensirionI2CSdp sdp;
  SDP810125PA(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, {nullptr, 0, nullptr, 0}, component, HardwareResourceToegang::GEDEELD) {}
  uint8_t AantalChannels() const override { return 2; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};

#endif

#endif
