#ifndef SENSOR_MPU6500_H
#define SENSOR_MPU6500_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_MPU6500_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"
#include <MPU6500_WE.h>

struct MPU6500 : Sensor {
  MPU6500_WE mpu6500;
  MPU6500(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, {nullptr, 0, nullptr, 0}, component, HardwareResourceToegang::GEDEELD), mpu6500(&Wire, parent ? parent->adres : 0x68) {}   // geen parent: Activeren() faalt later; hier niets lezen via nullptr
  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};

#endif

#endif
