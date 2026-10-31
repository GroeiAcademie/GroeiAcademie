#ifndef SENSOR_MAX30102_H
#define SENSOR_MAX30102_H
#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"
#if SENSOR_MAX30102_AANTAL > 0

#include "../GedeeldeBus/GedeeldeBus.h"
#include <MAX30105.h>
// Officiële API: https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library/blob/master/src/MAX30105.h
struct MAX30102 : Sensor {
  MAX30105 max30102;
  MAX30102(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, {nullptr, 0, nullptr, 0}, component, HardwareResourceToegang::GEDEELD) {}
  uint8_t AantalChannels() const override { return 2; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};
#endif
#endif
