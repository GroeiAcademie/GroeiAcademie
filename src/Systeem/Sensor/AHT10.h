#ifndef SENSOR_AHT10_H
#define SENSOR_AHT10_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_AHT10_AANTAL > 0

#include "../GedeeldeBus/GedeeldeBus.h"
#include <Adafruit_AHTX0.h>

// Officiële API: https://github.com/adafruit/Adafruit_AHTX0/blob/master/Adafruit_AHTX0.h
struct AHT10 : Sensor {
  bool metingGestart = false;                // het meetcommando is verzonden en de meting moet nog uitgelezen worden
  unsigned long tijdstipMetingGestart = 0;   // millis() van het meetcommando
  AHT10(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, {nullptr, 0, nullptr, 0}, component, HardwareResourceToegang::GEDEELD) {}
  uint8_t AantalChannels() const override { return 2; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
  uint8_t LeesStatus();   // AHT10-statusbyte, of 0xFF bij een I2C-leesfout
};

#endif

#endif
