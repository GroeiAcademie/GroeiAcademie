#ifndef SENSOR_HW201_H
#define SENSOR_HW201_H

#include <Arduino.h>
#include "SensorDigitaal.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_HW201_AANTAL > 0
// Digitale comparator heeft geen chip-ID. Alleen de invoer is voorbereid;
// aanwezigheid en detectie moeten tijdens de hardwaretest bewezen worden.
struct HW201 : SensorDigitaal {
  HW201(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_OUT)
    : SensorDigitaal(parent, component, np_OUT) {}
#if GEDEELDE_BUS_MIXED_PINS
  // Mixed: OUT (DO) op de GPIO-Extender-parent, AO op een kanaal van een ADC-Extender (Extender-pinnummer np_AO).
  uint8_t exclusiefMetHulp_[2];
  GedeeldeBusNode* extenderHulp_[2] = {nullptr, nullptr};
  uint8_t np_AO = static_cast<uint8_t>(HardwareResourcePin::NONE);
  bool viaMixed = false;
  HW201(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_OUT, GedeeldeBusNode* extender, uint8_t np_AO)
    : SensorDigitaal(parent, component, np_OUT), np_AO(np_AO) {
    exclusiefMetHulp_[0] = static_cast<uint8_t>(np_OUT);
    exclusiefMetHulp_[1] = np_AO;
    aangemeldePinnen = {nullptr, 0, exclusiefMetHulp_, 2};
    extenderHulp_[1] = extender;   // index 0 blijft nullptr: OUT hoort bij de parent
    ownerClaim = extenderHulp_;
    viaMixed = true;
  }
  // AO (analoge uitgang) ruw lezen via de ADC-Extender van AO; meetbereik SENSOR_HW201_ADC_MEASUREMENT_RANGE_MILLIVOLT.
  // Zelfde blokkerende wachtpatroon als SensorAnaloog::RawAnalogRead(). Werkt ook met twee keer dezelfde Extender (HW201 op één Extender).
  bool RawAnalogReadAnalogOutputAO(int32_t& waarde) {
    waarde = 0;
    if (!actief || !viaMixed || extenderHulp_[1] == nullptr) return false;
    GedeeldeBusNode* const analogExtenderOfAO = extenderHulp_[1];
    const unsigned long startTime = millis();
    int8_t startResult = analogExtenderOfAO->StartExtenderReadAnalog(np_AO, SENSOR_HW201_ADC_MEASUREMENT_RANGE_MILLIVOLT);
    while (startResult == 0 && millis() - startTime <= 150) {
      yield();
      startResult = analogExtenderOfAO->StartExtenderReadAnalog(np_AO, SENSOR_HW201_ADC_MEASUREMENT_RANGE_MILLIVOLT);
    }
    if (startResult != 1) return false;
    while (!analogExtenderOfAO->IsExtenderReadAnalogReady() && millis() - startTime <= 150) yield();
    // Na een gestarte meting altijd Get: die geeft de ADC vrij, ook na een time-out (ADC-contract in GedeeldeBus.h).
    // Ready beslist alleen of de waarde geldig is.
    const bool klaar = analogExtenderOfAO->IsExtenderReadAnalogReady();
    const bool gelukt = analogExtenderOfAO->GetExtenderReadAnalogValue(waarde) && klaar;
    if (analogExtenderOfAO->gedeeldeIdentiteiten != nullptr && analogExtenderOfAO->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
      static_cast<HardwareResourceTypeI2C*>(analogExtenderOfAO)->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
    }
    if (!gelukt) waarde = 0;
    return gelukt;
  }
  bool Activeren() override {
    if (!SensorDigitaal::Activeren()) return false;
    // AO: proef met bereik 0 (alleen kanaalondersteuning van de ADC-Extender, geen busactie).
    return !viaMixed || (extenderHulp_[1] != nullptr && extenderHulp_[1]->actief && extenderHulp_[1]->StartExtenderReadAnalog(np_AO, 0) == 1);
  }
#endif
};

#endif

#endif
