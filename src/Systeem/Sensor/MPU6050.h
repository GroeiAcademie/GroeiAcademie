#ifndef SENSOR_MPU6050_H
#define SENSOR_MPU6050_H

#include <Arduino.h>
#include <Wire.h>
#include "../GedeeldeBus/GedeeldeBus.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_MPU6050_AANTAL > 0
#include <Adafruit_MPU6050.h>

// Sensorinitialisatie en meting via Activeren() en LeesHoofdwaarde().
// parent moet de voor dit apparaat geregistreerde HardwareResourceTypeI2C-node zijn.
struct SensorMPU6050 : Sensor {
  SensorMPU6050(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, { nullptr, 0, nullptr, 0 }, component, HardwareResourceToegang::GEDEELD) {}
  Adafruit_MPU6050 mpu6050;
  uint8_t AantalChannels() const override { return 7; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};

#endif

#endif
