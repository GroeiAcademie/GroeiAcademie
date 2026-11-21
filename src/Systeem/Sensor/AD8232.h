#ifndef SENSOR_AD8232_H
#define SENSOR_AD8232_H

#include <Arduino.h>
#include "SensorAnaloog.h"
#include "Functies/BeatPeakDetector.h"
#include "Functies/HeartRateFromBeats.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_AD8232_AANTAL > 0
struct AD8232 : SensorAnaloog {
  // LO+ en LO- (elektrode los), optioneel: native pinnen. Ze worden niet gereserveerd in de GedeeldeBus,
  // want OUT hoort bij de ADC-Extender en gemengde gebieden staan standaard uit: kies vrije pinnen.
  const HardwareResourcePin np_LOPLUS = HardwareResourcePin::NONE;
  const HardwareResourcePin np_LOMIN = HardwareResourcePin::NONE;
  AD8232(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, SENSOR_AD8232_ADC_BEREIK_MILLIVOLT, Extender) { sampleIntervalUS = 1000000UL / SENSOR_AD8232_SAMPLE_RATE_HZ; }
  AD8232(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourcePin np_LOPLUS, HardwareResourcePin np_LOMIN,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, SENSOR_AD8232_ADC_BEREIK_MILLIVOLT, Extender),
      np_LOPLUS(np_LOPLUS), np_LOMIN(np_LOMIN) { sampleIntervalUS = 1000000UL / SENSOR_AD8232_SAMPLE_RATE_HZ; }
  // SDN (shutdown, laag actief). NONE = niet aangesloten (de module houdt SDN dan zelf hoog).
  uint8_t np_SDN = static_cast<uint8_t>(HardwareResourcePin::NONE);
  // true: LO+, LO- en SDN zijn pinnen van de ADC-Extender-parent zelf (bv. EP_GPIO0..7 van de ADS1158), geen native pinnen.
  bool viaExtender = false;
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
  // LO+, LO- en SDN op de eigen GPIO van de ADC-Extender (Extender-pinnummers). Zoals LO+/LO- native: niet gereserveerd.
  AD8232(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      uint8_t pinLOPLUS, uint8_t pinLOMIN, uint8_t np_SDN,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, SENSOR_AD8232_ADC_BEREIK_MILLIVOLT, Extender),
      np_LOPLUS(static_cast<HardwareResourcePin>(pinLOPLUS)), np_LOMIN(static_cast<HardwareResourcePin>(pinLOMIN)), np_SDN(np_SDN), viaExtender(true) { sampleIntervalUS = 1000000UL / SENSOR_AD8232_SAMPLE_RATE_HZ; }
#endif
  bool Activeren() override {
    if (!SensorAnaloog::Activeren()) return false;
    // Nieuwe sessie: geen slagen of hartslag uit een vorige sessie.
    beatPeakDetector = BeatPeakDetector();
    heartRateFromBeats = HeartRateFromBeats();
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    if (viaExtender) {
      // SDN eerst hoog (actief), daarna LO+ en LO- als ingang.
      if (np_SDN != static_cast<uint8_t>(HardwareResourcePin::NONE) &&
          (!parent->WriteExtenderPin(np_SDN, HIGH) || !parent->SetExtenderPinMode(np_SDN, OUTPUT))) return false;
      if (np_LOPLUS != HardwareResourcePin::NONE && !parent->SetExtenderPinMode(static_cast<uint8_t>(np_LOPLUS), INPUT)) return false;
      if (np_LOMIN != HardwareResourcePin::NONE && !parent->SetExtenderPinMode(static_cast<uint8_t>(np_LOMIN), INPUT)) return false;
      return true;
    }
#endif
    if (np_LOPLUS != HardwareResourcePin::NONE) pinMode(NativeArduinoPinVan(np_LOPLUS), INPUT);
    if (np_LOMIN != HardwareResourcePin::NONE) pinMode(NativeArduinoPinVan(np_LOMIN), INPUT);
    return true;
  }
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
  // SDN hoog (actief) of laag (slaap, minder verbruik). Alleen via de Extender en met SDN aangesloten.
  bool WriteSDN(bool hoog) {
    if (!actief || !viaExtender || np_SDN == static_cast<uint8_t>(HardwareResourcePin::NONE)) return false;
    return parent->WriteExtenderPin(np_SDN, hoog ? HIGH : LOW);
  }
#endif
  // Elektrode los (LO+ of LO- hoog): geen meting, zodat een losse elektrode nooit als meetwaarde doorkomt.
  // Zelfde namen als in SensorAnaloog: via een AD8232 worden deze versies gebruikt.
  // Via de Extender: ReadExtenderPin(); een leesfout (0xFF) telt ook als "los".
  bool StartAnalogRead() {
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    if (viaExtender) {
      if ((np_LOPLUS != HardwareResourcePin::NONE && parent->ReadExtenderPin(static_cast<uint8_t>(np_LOPLUS)) != LOW) ||
          (np_LOMIN != HardwareResourcePin::NONE && parent->ReadExtenderPin(static_cast<uint8_t>(np_LOMIN)) != LOW)) return false;
      return SensorAnaloog::StartAnalogRead();
    }
#endif
    if ((np_LOPLUS != HardwareResourcePin::NONE && digitalRead(NativeArduinoPinVan(np_LOPLUS)) == HIGH) ||
        (np_LOMIN != HardwareResourcePin::NONE && digitalRead(NativeArduinoPinVan(np_LOMIN)) == HIGH)) return false;
    return SensorAnaloog::StartAnalogRead();
  }

  bool RawAnalogRead(int32_t& waarde) {
    waarde = 0;
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    if (viaExtender) {
      if ((np_LOPLUS != HardwareResourcePin::NONE && parent->ReadExtenderPin(static_cast<uint8_t>(np_LOPLUS)) != LOW) ||
          (np_LOMIN != HardwareResourcePin::NONE && parent->ReadExtenderPin(static_cast<uint8_t>(np_LOMIN)) != LOW)) return false;
      return SensorAnaloog::RawAnalogRead(waarde);
    }
#endif
    if ((np_LOPLUS != HardwareResourcePin::NONE && digitalRead(NativeArduinoPinVan(np_LOPLUS)) == HIGH) ||
        (np_LOMIN != HardwareResourcePin::NONE && digitalRead(NativeArduinoPinVan(np_LOMIN)) == HIGH)) return false;
    return SensorAnaloog::RawAnalogRead(waarde);
  }
  // Hartslag uit het ECG. Roep minstens zo vaak aan als SENSOR_AD8232_SAMPLE_RATE_HZ: elke oproep neemt hoogstens één sample op het vaste ritme.
  // Met een losse elektrode wordt geen sample genomen; na 3 s zonder slag is er geen hartslag.
  uint8_t AantalChannels() const override { return 2; }   // meetkanalen voor Data: 1 ruwe ADC-waarde, 2 hartslag
  BeatPeakDetector beatPeakDetector;
  HeartRateFromBeats heartRateFromBeats;
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
  // Elektrode los: LO+ of LO- hoog. Via de Extender telt een leesfout ook als los. false: geen LO-pin aangesloten of niet actief.
  bool ReadElectrodesOff(bool& electrodesOff) {
    electrodesOff = false;
    if (!actief || (np_LOPLUS == HardwareResourcePin::NONE && np_LOMIN == HardwareResourcePin::NONE)) return false;
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    if (viaExtender) {
      electrodesOff = (np_LOPLUS != HardwareResourcePin::NONE && parent->ReadExtenderPin(static_cast<uint8_t>(np_LOPLUS)) != LOW) ||
                      (np_LOMIN != HardwareResourcePin::NONE && parent->ReadExtenderPin(static_cast<uint8_t>(np_LOMIN)) != LOW);
      return true;
    }
#endif
    electrodesOff = (np_LOPLUS != HardwareResourcePin::NONE && digitalRead(NativeArduinoPinVan(np_LOPLUS)) == HIGH) ||
                    (np_LOMIN != HardwareResourcePin::NONE && digitalRead(NativeArduinoPinVan(np_LOMIN)) == HIGH);
    return true;
  }
};

#endif

#endif
