#ifndef SENSOR_BMP280_H
#define SENSOR_BMP280_H

#include <Arduino.h>
#include <Wire.h>
#include "../GedeeldeBus/GedeeldeBus.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_BMP280_AANTAL > 0
#include <Adafruit_BMP280.h>

// Alleen identiteit/lifecycle: nog geen volledige sensorinitialisatie of meting.
// parent moet de voor dit apparaat geregistreerde HardwareResourceTypeI2C-node zijn.
struct BMP280 : Sensor {
  BMP280(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, { nullptr, 0, nullptr, 0 }, component, HardwareResourceToegang::GEDEELD) {}
  Adafruit_BMP280 bmp280;
  uint8_t AantalChannels() const override { return 2; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};

#endif

#endif
