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
  bool IsAnalogChild() const override { return true; }    // leest via StartExtenderReadAnalog() van de parent
  bool Activeren() override;
  bool StartAnalogRead();
  bool IsAnalogReadKlaar();
  int GetAnalogRead();
  bool afmelden() override;
  bool RawAnalogRead(int32_t& waarde);
  // Vast ritme: true als het volgende geplande sample aan de beurt is (sampleIntervalUS na het vorige geplande sample).
  // Te laat (meer dan één periode achter): het ritme begint opnieuw vanaf nu, zonder in te halen. sampleIntervalUS 0 = altijd.
  bool IsSampleDue();
  unsigned long lastSampleUS = 0;
  unsigned long sampleIntervalUS = 0;   // ingesteld door de afgeleide Sensor uit SENSOR_<NAAM>_SAMPLE_RATE_HZ

protected:
  // Resultaat van één meting verwerken. Standaard: een geslaagde ADC-waarde naar Data-kanaal 1.
  virtual void MeetwaardeVerwerken(int32_t waarde, bool gelukt);
};

#endif
