#ifndef SENSOR_BME68X_H
#define SENSOR_BME68X_H

#include <Arduino.h>
#include <Wire.h>
#include "../GedeeldeBus/GedeeldeBus.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_BME680_AANTAL > 0 || SENSOR_BME688_AANTAL > 0
#include <Adafruit_BME680.h>

// Alleen identiteit/lifecycle: nog geen volledige sensorinitialisatie of meting.
// parent moet de voor dit apparaat geregistreerde HardwareResourceTypeI2C-node zijn.
struct BME68x : Sensor {
  BME68x(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, { nullptr, 0, nullptr, 0 }, component, HardwareResourceToegang::GEDEELD) {}
  Adafruit_BME680 bme68x;
  uint8_t AantalChannels() const override { return 4; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};

#endif

#endif
