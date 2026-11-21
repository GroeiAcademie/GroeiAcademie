#ifndef SENSOR_MAX4466_9814_H
#define SENSOR_MAX4466_9814_H

#include <Arduino.h>
#include "SensorAnaloog.h"
#include "Functies/SoundLevel.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_MAX4466_AANTAL > 0 || SENSOR_MAX9814_AANTAL > 0
struct MAX4466_9814 : SensorAnaloog {
  MAX4466_9814(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, component == GedeeldeBusComponent::MAX9814 ? SENSOR_MAX9814_ADC_BEREIK_MILLIVOLT : SENSOR_MAX4466_ADC_BEREIK_MILLIVOLT, Extender) {
    sampleIntervalUS = 1000000UL / (component == GedeeldeBusComponent::MAX9814 ? SENSOR_MAX9814_SAMPLE_RATE_HZ : SENSOR_MAX4466_SAMPLE_RATE_HZ);
  }
  uint8_t AantalChannels() const override { return 3; }   // meetkanalen voor Data: 1 ruwe ADC-waarde, 2 RMS, 3 piek-tot-piek
  // Geluidsniveau over een venster van een kwart seconde (samplefrequentie / 4 samples), op het vaste ritme.
  // Roep minstens zo vaak aan als de samplefrequentie: elke oproep neemt hoogstens één sample. Geldig vanaf 16 samples in het venster.
  // Eenheid: ADC-waarde (geen dB SPL). Bij de MAX9814 regelt de AGC het niveau zelf bij.
  SoundLevel soundLevel;
  bool ReadSoundLevelRootMeanSquare(float& rootMeanSquare) {
    rootMeanSquare = NAN;
    if (actief && IsSampleDue()) {
      int32_t waarde = 0;
      if (RawAnalogRead(waarde)) {
        if (soundLevel.sampleCount >= (component == GedeeldeBusComponent::MAX9814 ? SENSOR_MAX9814_SAMPLE_RATE_HZ : SENSOR_MAX4466_SAMPLE_RATE_HZ) / 4) soundLevel.ResetWindow();
        // Grens voor een onderbreking: 2 sampleperiodes, in ms.
        soundLevel.AddSample(waarde, 2UL * sampleIntervalUS / 1000UL);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
        // Venster net vol: RMS (kanaal 2) en piek-tot-piek (kanaal 3) van dit venster naar Data, op het moment van afsluiten.
        // Het venster blijft voor de leesfuncties beschikbaar tot de volgende sample het wist.
        if (soundLevel.sampleCount == (component == GedeeldeBusComponent::MAX9814 ? SENSOR_MAX9814_SAMPLE_RATE_HZ : SENSOR_MAX4466_SAMPLE_RATE_HZ) / 4) {
          int32_t peakToPeak = 0;
          if (soundLevel.ReadRootMeanSquare(rootMeanSquare)) WriteQueueOrStack(2, rootMeanSquare);
          if (soundLevel.ReadPeakToPeak(peakToPeak)) WriteQueueOrStack(3, static_cast<int>(peakToPeak));
        }
#endif
      }
    }
    return actief && soundLevel.sampleCount >= 16 && soundLevel.ReadRootMeanSquare(rootMeanSquare);
  }
  bool ReadSoundLevelPeakToPeak(int32_t& peakToPeak) {
    peakToPeak = 0;
    if (actief && IsSampleDue()) {
      int32_t waarde = 0;
      if (RawAnalogRead(waarde)) {
        if (soundLevel.sampleCount >= (component == GedeeldeBusComponent::MAX9814 ? SENSOR_MAX9814_SAMPLE_RATE_HZ : SENSOR_MAX4466_SAMPLE_RATE_HZ) / 4) soundLevel.ResetWindow();
        // Grens voor een onderbreking: 2 sampleperiodes, in ms.
        soundLevel.AddSample(waarde, 2UL * sampleIntervalUS / 1000UL);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
        // Venster net vol: RMS (kanaal 2) en piek-tot-piek (kanaal 3) van dit venster naar Data, op het moment van afsluiten.
        // Het venster blijft voor de leesfuncties beschikbaar tot de volgende sample het wist.
        if (soundLevel.sampleCount == (component == GedeeldeBusComponent::MAX9814 ? SENSOR_MAX9814_SAMPLE_RATE_HZ : SENSOR_MAX4466_SAMPLE_RATE_HZ) / 4) {
          float rootMeanSquare = NAN;
          if (soundLevel.ReadRootMeanSquare(rootMeanSquare)) WriteQueueOrStack(2, rootMeanSquare);
          if (soundLevel.ReadPeakToPeak(peakToPeak)) WriteQueueOrStack(3, static_cast<int>(peakToPeak));
        }
#endif
      }
    }
    return actief && soundLevel.sampleCount >= 16 && soundLevel.ReadPeakToPeak(peakToPeak);
  }
  // GAIN en AR van de MAX9814 (de MAX4466 heeft ze niet). NONE = niet aangesloten.
  uint8_t np_GAIN = static_cast<uint8_t>(HardwareResourcePin::NONE);
  uint8_t np_AR = static_cast<uint8_t>(HardwareResourcePin::NONE);
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
  // MAX9814 met GAIN en AR op de eigen GPIO van de ADC-Extender (Extender-pinnummers, bv. EP_GPIO0..7 van de ADS1158).
  MAX4466_9814(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin, uint8_t np_GAIN, uint8_t np_AR,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, component == GedeeldeBusComponent::MAX9814 ? SENSOR_MAX9814_ADC_BEREIK_MILLIVOLT : SENSOR_MAX4466_ADC_BEREIK_MILLIVOLT, Extender),
      np_GAIN(np_GAIN), np_AR(np_AR) {
    sampleIntervalUS = 1000000UL / (component == GedeeldeBusComponent::MAX9814 ? SENSOR_MAX9814_SAMPLE_RATE_HZ : SENSOR_MAX4466_SAMPLE_RATE_HZ);
  }
  // Versterking van de MAX9814: 40 dB (GAIN hoog), 50 dB (GAIN laag) of 60 dB (GAIN zwevend).
  // Andere waarden: false.
  bool SetGainDB(uint8_t dB) {
    if (!actief || component != GedeeldeBusComponent::MAX9814 || np_GAIN == static_cast<uint8_t>(HardwareResourcePin::NONE)) return false;
    if (dB == 60) return parent->SetExtenderPinMode(np_GAIN, INPUT);
    if (dB == 50) return parent->WriteExtenderPin(np_GAIN, LOW) && parent->SetExtenderPinMode(np_GAIN, OUTPUT);
    if (dB == 40) return parent->WriteExtenderPin(np_GAIN, HIGH) && parent->SetExtenderPinMode(np_GAIN, OUTPUT);
    return false;
  }
#endif
  // Maxim MAX9814: GAIN en AR zwevend = 60 dB en attack/release 1:4000 (de waarden zonder externe sturing).
  // Activeren() zet beide als ingang (zwevend); een andere versterking hoort bij de functies, niet bij Activeren.
  // Activeren() staat buiten de GPIO-voorwaarde, zodat het oude geluidsvenster in elke configuratie vervalt.
  bool Activeren() override {
    if (!SensorAnaloog::Activeren()) return false;
    // Nieuwe sessie: geen geluidsvenster uit een vorige sessie.
    soundLevel.ResetWindow();
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    if (np_GAIN == static_cast<uint8_t>(HardwareResourcePin::NONE) && np_AR == static_cast<uint8_t>(HardwareResourcePin::NONE)) return true;
    if (component != GedeeldeBusComponent::MAX9814) return false;   // de MAX4466 heeft geen GAIN/AR
    if (np_GAIN != static_cast<uint8_t>(HardwareResourcePin::NONE) && !parent->SetExtenderPinMode(np_GAIN, INPUT)) return false;
    if (np_AR != static_cast<uint8_t>(HardwareResourcePin::NONE) && !parent->SetExtenderPinMode(np_AR, INPUT)) return false;
#endif
    return true;
  }
};

#endif

#endif
