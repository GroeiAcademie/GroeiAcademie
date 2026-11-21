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
  // Totaal aantal samples dat sinds het activeren verloren ging, omdat de library er tussen twee oproepen hoogstens 3 bijhoudt.
  uint32_t aantalVerlorenSamples = 0;
  MAX30102(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, {nullptr, 0, nullptr, 0}, component, HardwareResourceToegang::GEDEELD) {}
#if GEDEELDE_BUS_MIXED_PINS
  // Mixed: I2C via de gewone parent (bv. achter TCA9548A); INT op een tweede Extender (PCF8574, PCF8575, MCP23017, MCP23S17, SC16IS752 of MAX14830 GPIO).
  // np_INT: Extender-pinnummer(s) op die tweede Extender, geen Arduino-nummers.
  uint8_t exclusief_[1];
  GedeeldeBusNode* extenderHulp_[1] = {nullptr};
  uint8_t np_INT = static_cast<uint8_t>(HardwareResourcePin::NONE);
  bool viaMixed = false;
  MAX30102(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component, GedeeldeBusNode* extender, uint8_t np_INT)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, HardwareResourceToegang::GEDEELD), np_INT(np_INT) {
    exclusief_[0] = np_INT;
    extenderHulp_[0] = extender;
    ownerClaim = extenderHulp_;
    viaMixed = true;
  }
#endif
  uint8_t AantalChannels() const override { return 4; }   // meetkanalen voor Data: 1 IR, 2 rood, 3 hartslag, 4 SpO₂
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
  // Hartslag en SpO₂ uit de samples van LeesHoofdwaarde() (25 samples per seconde): roep LeesHoofdwaarde() minstens 10 keer per seconde aan.
  // Hartslag en SpO₂: officieel algoritme van de SparkFun-library (spo2_algorithm), elke seconde over de laatste 4 s.
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  uint16_t irBuffer[100] = {};
  uint16_t redBuffer[100] = {};
#else
  uint32_t irBuffer[100] = {};
  uint32_t redBuffer[100] = {};
#endif
  uint8_t bufferSampleCount = 0;
  int32_t oxygenSaturationPercent = 0;
  int8_t oxygenSaturationValid = 0;
  int32_t heartRateFromAlgorithm = 0;
  int8_t heartRateFromAlgorithmValid = 0;
  // Tijdstip van de laatste berekening: beide leesfuncties geven false als die ouder is dan 3 s.
  uint32_t lastAlgorithmMS = 0;
  bool ReadHeartRateBeatsPerMinute(float& waarde);
  bool ReadOxygenSaturationPercent(float& waarde);
#if GEDEELDE_BUS_MIXED_PINS
  // INT lezen (true = actief, laag actief). Activeren() zet de interrupt "nieuwe sample" aan; lezen van de status wist hem.
  bool ReadINT(bool& waarde) {
    waarde = false;
    if (!actief || !viaMixed || extenderHulp_[0] == nullptr) return false;
    const uint8_t niveau = extenderHulp_[0]->ReadExtenderPin(np_INT);
    if (niveau != LOW && niveau != HIGH) return false;
    waarde = niveau == LOW;
    return true;
  }
#endif
};

#endif

#endif
