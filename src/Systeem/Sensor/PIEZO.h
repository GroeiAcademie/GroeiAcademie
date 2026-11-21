#ifndef SENSOR_PIEZO_H
#define SENSOR_PIEZO_H

#include <Arduino.h>
#include "SensorAnaloog.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_PIEZO_AANTAL > 0

// PIEZO: piezo-element; alleen beveiligd/conditioneerd analoog signaal.
// Eén component voor één fysiek apparaat, via de bestaande ADC-Extender.
// De meetcode zit in SensorAnaloog; deze klasse geeft alleen de pin en het bereik door.
struct PIEZO : SensorAnaloog {
  PIEZO(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, SENSOR_PIEZO_ADC_BEREIK_MILLIVOLT, Extender) {}
  // Hoogste ADC-waarde sinds de vorige oproep: elke geslaagde meting (RawAnalogRead, GetAnalogRead) houdt de piek bij.
  // Deze oproep neemt zelf ook één sample. Daarna begint een nieuwe piek. ReadTapDetected() gebruikt dezelfde piek: kies één van beide.
  int32_t peakSinceLastRead = INT32_MIN;
  bool ReadPeakSinceLastRead(int32_t& peakValue) {
    peakValue = 0;
    if (!actief) return false;
    int32_t waarde = 0;
    (void)RawAnalogRead(waarde);
    if (peakSinceLastRead == INT32_MIN) return false;
    peakValue = peakSinceLastRead;
    peakSinceLastRead = INT32_MIN;
    return true;
  }
  // Tik: de piek sinds de vorige oproep ligt op of boven SENSOR_PIEZO_TAP_THRESHOLD_MILLIVOLT (met het werkelijke meetbereik van de ADC).
  bool ReadTapDetected(bool& tapDetected) {
    tapDetected = false;
    int32_t waarde = 0;
    if (parent == nullptr || !ReadPeakSinceLastRead(waarde)) return false;
    tapDetected = static_cast<float>(waarde) * parent->GetExtenderReadAnalogEffectiefBereikMillivolt() / static_cast<float>(ADC_MAX) >= SENSOR_PIEZO_TAP_THRESHOLD_MILLIVOLT;
    return true;
  }
protected:
  void MeetwaardeVerwerken(int32_t waarde, bool gelukt) override {
    if (gelukt && waarde > peakSinceLastRead) peakSinceLastRead = waarde;
    SensorAnaloog::MeetwaardeVerwerken(waarde, gelukt);
  }
public:
  // Activeren() staat buiten de MIXED-voorwaarde, zodat de oude piek in elke configuratie vervalt.
  bool Activeren() override {
    if (!SensorAnaloog::Activeren()) return false;
    // Nieuwe sessie: geen piek uit een vorige sessie.
    peakSinceLastRead = INT32_MIN;
#if GEDEELDE_BUS_MIXED_PINS
    if (!viaMixed) return true;
    if (extenderHulp_[1] == nullptr || !extenderHulp_[1]->actief) return false;
    return extenderHulp_[1]->SetExtenderPinMode(np_DO, INPUT);   // DO: uitgang van de module
#else
    return true;
#endif
  }
#if GEDEELDE_BUS_MIXED_PINS
  // Mixed: het analoge signaal op de ADC-parent, de digitale comparatoruitgang DO op een tweede Extender (Extender-pinnummer np_DO).
  uint8_t exclusiefMetHulp_[2];
  GedeeldeBusNode* extenderHulp_[2] = {nullptr, nullptr};
  uint8_t np_DO = static_cast<uint8_t>(HardwareResourcePin::NONE);
  bool viaMixed = false;
  PIEZO(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin, GedeeldeBusNode* extender, uint8_t np_DO,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, SENSOR_PIEZO_ADC_BEREIK_MILLIVOLT, Extender), np_DO(np_DO) {
    exclusiefMetHulp_[0] = sensorPin;
    exclusiefMetHulp_[1] = np_DO;
    aangemeldePinnen = {nullptr, 0, exclusiefMetHulp_, 2};
    extenderHulp_[1] = extender;   // index 0 blijft nullptr: het analoge signaal hoort bij de parent
    ownerClaim = extenderHulp_;
    viaMixed = true;
  }
  // DO van de comparator lezen (true = hoog). Welk niveau een tik is, hangt af van de module.
  bool ReadDO(bool& waarde) {
    waarde = false;
    if (!actief || !viaMixed || extenderHulp_[1] == nullptr) return false;
    const uint8_t niveau = extenderHulp_[1]->ReadExtenderPin(np_DO);
    if (niveau != LOW && niveau != HIGH) return false;
    waarde = niveau == HIGH;
    return true;
  }
#endif
};

#endif

#endif
