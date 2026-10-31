#ifndef SENSOR_ANALOOG_H
#define SENSOR_ANALOOG_H

#include <Arduino.h>
#include "../GedeeldeBus/GedeeldeBus.h"
#include "../../Configuratie/SystemConfig.h"

// SensorAnaloog: gemeenschappelijke basis voor een analoge Sensor met één ingang op een ADC-Extender.
// StartAnalogRead(), IsAnalogReadKlaar(), GetAnalogRead() en RawAnalogRead() staan hier één keer.
// Een afgeleide Sensor geeft alleen zijn ADC-pin en het maximale signaal van zijn uitgang (mV) door.
struct SensorAnaloog : Sensor {
  uint8_t exclusief_[1];
  const uint8_t sensorPin;
  const uint16_t bereikMillivolt;   // maximale spanning van de sensoruitgang; de ADC-Extender kiest zelf zijn bereik
  bool metingGestart = false;
  int8_t laatsteStartresultaat = -1;

  SensorAnaloog(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin, uint16_t bereikMillivolt,
                HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : Sensor(parent, { nullptr, 0, exclusief_, 1 }, component, Extender), sensorPin(sensorPin), bereikMillivolt(bereikMillivolt) {
    exclusief_[0] = sensorPin;
  }

  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool StartAnalogRead();
  bool IsAnalogReadKlaar();
  int GetAnalogRead();
  bool afmelden() override;
  bool RawAnalogRead(int32_t& waarde);

protected:
  // Resultaat van één meting verwerken. Standaard: een geslaagde ADC-waarde naar Data-kanaal 1.
  virtual void MeetwaardeVerwerken(int32_t waarde, bool gelukt);
};

#endif
