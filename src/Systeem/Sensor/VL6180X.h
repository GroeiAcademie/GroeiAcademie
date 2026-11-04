#ifndef SENSOR_VL6180X_H
#define SENSOR_VL6180X_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_VL6180X_AANTAL > 0

#include "../GedeeldeBus/GedeeldeBus.h"
#include <Adafruit_VL6180X.h>

// Officiële API: https://github.com/adafruit/Adafruit_VL6180X/blob/master/Adafruit_VL6180X.cpp
struct SensorVL6180X : Sensor {
  Adafruit_VL6180X vl;
  bool metingGestart = false;   // niet-blokkerend: eerste oproep start de meting, een volgende haalt ze op
  SensorVL6180X(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, {nullptr, 0, nullptr, 0}, component, HardwareResourceToegang::GEDEELD),
      vl(parent ? parent->adres : VL6180X_DEFAULT_I2C_ADDR) {}   // het geconfigureerde I2C-adres doorgeven
  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};

#endif

#endif
