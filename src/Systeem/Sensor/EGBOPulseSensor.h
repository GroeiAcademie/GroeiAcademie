#ifndef SENSOR_EGBOPULSESENSOR_H
#define SENSOR_EGBOPULSESENSOR_H

#include <Arduino.h>
#include "SensorAnaloog.h"
#include "Functies/BeatPeakDetector.h"
#include "Functies/HeartRateFromBeats.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_EGBOPULSESENSOR_AANTAL > 0
struct EGBOPulseSensor : SensorAnaloog {
  EGBOPulseSensor(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, SENSOR_EGBOPULSESENSOR_ADC_BEREIK_MILLIVOLT, Extender) { sampleIntervalUS = 1000000UL / SENSOR_EGBOPULSESENSOR_SAMPLE_RATE_HZ; }
  // Hartslag uit het PPG-signaal. Roep minstens zo vaak aan als SENSOR_EGBOPULSESENSOR_SAMPLE_RATE_HZ: elke oproep neemt hoogstens één sample op het vaste ritme.
  uint8_t AantalChannels() const override { return 2; }   // meetkanalen voor Data: 1 ruwe ADC-waarde, 2 hartslag
  BeatPeakDetector beatPeakDetector;
  HeartRateFromBeats heartRateFromBeats;
  bool Activeren() override {
    if (!SensorAnaloog::Activeren()) return false;
    // Nieuwe sessie: geen slagen of hartslag uit een vorige sessie.
    beatPeakDetector = BeatPeakDetector();
    heartRateFromBeats = HeartRateFromBeats();
    return true;
  }
  bool ReadHeartRateBeatsPerMinute(float& beatsPerMinute) {
    if (actief && IsSampleDue()) {
      int32_t waarde = 0;
      bool beatDetected = false;
      if (RawAnalogRead(waarde) && beatPeakDetector.AddSample(static_cast<float>(waarde), millis(), beatDetected) && beatDetected) {
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
        // Nieuwe slag aanvaard en geldige hartslag: meteen naar Data (kanaal 2).
        if (heartRateFromBeats.AddBeat(millis()) && heartRateFromBeats.ReadBeatsPerMinute(beatsPerMinute)) WriteQueueOrStack(2, beatsPerMinute);
#else
        heartRateFromBeats.AddBeat(millis());
#endif
      }
    }
    // Niet actief: geen hartslag, ook geen eerder berekende.
    if (!actief) { beatsPerMinute = NAN; return false; }
    return heartRateFromBeats.ReadBeatsPerMinute(beatsPerMinute);
  }
};

#endif

#endif
