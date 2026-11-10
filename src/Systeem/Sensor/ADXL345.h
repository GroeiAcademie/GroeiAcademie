#ifndef SENSOR_ADXL345_H
#define SENSOR_ADXL345_H

#include <Arduino.h>
#include <Wire.h>
#include "../GedeeldeBus/GedeeldeBus.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_ADXL345_AANTAL > 0
#include <Adafruit_ADXL345_U.h>

// Sensorinitialisatie en meting via Activeren() en LeesHoofdwaarde().
// parent moet de voor dit apparaat geregistreerde HardwareResourceTypeI2C-node zijn.
struct SensorADXL345 : Sensor {
  SensorADXL345(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, { nullptr, 0, nullptr, 0 }, component, HardwareResourceToegang::GEDEELD) {}
  Adafruit_ADXL345_Unified adxl345;
  uint8_t AantalChannels() const override { return 3; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};

#endif

#endif
