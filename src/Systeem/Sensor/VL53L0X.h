#ifndef SENSOR_VL53L0X_H
#define SENSOR_VL53L0X_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_VL53L0X_AANTAL > 0

#include "../GedeeldeBus/GedeeldeBus.h"
#include <Adafruit_VL53L0X.h>

// Officiële API: https://github.com/adafruit/Adafruit_VL53L0X/blob/master/src/Adafruit_VL53L0X.h
struct SensorVL53L0X : Sensor {
  Adafruit_VL53L0X vl;
  bool metingGestart = false;                // een afstandsmeting gestart, nog niet verwerkt
  unsigned long tijdstipMetingGestart = 0;   // millis() van de meetstart
  unsigned long tijdslimietMeting = 200UL;   // milliseconden; berekend uit het meetbudget, 200 ms als terugval
  SensorVL53L0X(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, {nullptr, 0, nullptr, 0}, component, HardwareResourceToegang::GEDEELD) {}
  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};

#endif

#endif
