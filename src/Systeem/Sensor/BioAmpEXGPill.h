#ifndef SENSOR_BIOAMPEXGPILL_H
#define SENSOR_BIOAMPEXGPILL_H

#include <Arduino.h>
#include "SensorAnaloog.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_BIOAMPEXGPILL_AANTAL > 0
struct BioAmpEXGPill : SensorAnaloog {
  BioAmpEXGPill(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, SENSOR_BIOAMPEXGPILL_ADC_BEREIK_MILLIVOLT, Extender) {}
  bool LeesHoofdwaarde(float& waarde);
  unsigned long tijdstipLaatsteSample = 0;
  // true als de ADC-keten de vaste samplefrequentie van de meetmethode niet haalt: dan geen gefilterde waarde.
  bool samplefrequentieNietHaalbaar = false;
  // Aantal samples na elkaar die meer dan 10 % te laat waren; 0 bij een sample op tijd.
  uint8_t aantalTeLateSamplesNaElkaar = 0;
  float filterToestand[4][2] = {};
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EMG
  // Officieel voorbeeld EMGEnvelop: voortschrijdend gemiddelde van |gefilterd EMG| over het venster, maal 2.
  float envelopBuffer[SENSOR_BIOAMPEXGPILL_ENVELOP_VENSTER] = {};
  float envelopSom = 0;
  uint16_t envelopIndex = 0;
  float laatsteEnvelop = NAN;
  bool LeesEMGEnvelop(float& waarde);
#endif

#if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_ECG || SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EOG
  // Officiële Getpeak(): piek als de waarde meer dan piekFactor x de standaardafwijking boven het gemiddelde ligt.
  // Gemiddelde en standaardafwijking per positie in het venster bewaard, zoals in het voorbeeld.
  // ECG (HeartBeatDetection): venster 16, factor 16/2 = 8. EOG (EyeBlinkDetection, "Experimental!"): venster 10, factor 10 x 1,2 = 12.
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_ECG
  static constexpr uint8_t piekVenster = 16;
  static constexpr float piekFactor = 8.0f;
#else
  static constexpr uint8_t piekVenster = 10;
  static constexpr float piekFactor = 12.0f;
#endif
  float piekData[piekVenster] = {};
  float piekGemiddelde[piekVenster] = {};
  float piekAfwijking[piekVenster] = {};
  uint8_t piekIndex = 0;
  bool laatstePiek = false;
  bool piekBekend = false;
#endif

#if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_ECG
  bool LeesHartslagPiek(bool& piek);
#endif

#if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EOG
  bool LeesKnipperen(bool& knipper);
#endif
};

#endif

#endif
