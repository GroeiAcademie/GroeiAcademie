#ifndef SENSOR_SCD41_H
#define SENSOR_SCD41_H
#include "../../Configuratie/SystemConfig.h"
#if SENSOR_SCD41_AANTAL > 0

#include "../GedeeldeBus/GedeeldeBus.h"
#include <SensirionI2cScd4x.h>
struct SCD41 : Sensor {
  SensirionI2cScd4x scd4x;
  SCD41(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, {nullptr, 0, nullptr, 0}, component, HardwareResourceToegang::GEDEELD) {}
  uint8_t AantalChannels() const override { return 3; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};
#endif
#endif
