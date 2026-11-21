#ifndef SENSOR_MF52X_H
#define SENSOR_MF52X_H

#include <Arduino.h>
#include "SensorAnaloog.h"
#include "Functies/VoltageDivider.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_MF52A_AANTAL > 0 || SENSOR_MF52D_AANTAL > 0

// MF52A: NTC thermistor; spanning, niet omgerekende temperatuur.
// Eén component voor één fysiek apparaat, via de bestaande ADC-Extender.
// De meetcode zit in SensorAnaloog; deze klasse geeft alleen de pin en het bereik door.
// Het fysieke type behoudt zijn eigen componentcode en configuratie.
// De analoge ADC-meetfuncties zijn identiek aan die van MF52A.
struct MF52x : SensorAnaloog {
  MF52x(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, component == GedeeldeBusComponent::MF52D ? SENSOR_MF52D_ADC_BEREIK_MILLIVOLT : SENSOR_MF52A_ADC_BEREIK_MILLIVOLT, Extender) {}
  uint8_t AantalChannels() const override { return 2; }   // meetkanalen voor Data: 1 ruwe ADC-waarde, 2 temperatuur
  // Temperatuur (°C) via de B-formule: 1/T = 1/298,15 + ln(R/R25)/B. Deler: voeding - serieweerstand - meetpunt - NTC - GND.
  // De ADC-waarde wordt herschaald naar de voeding van de deler (het ADC-bereik van de sensor = voedingsspanning).
  // Instellingen: SENSOR_MF52X_NTC_BETA, SENSOR_MF52X_NTC_RESISTANCE_AT_25C_OHM, SENSOR_MF52X_SERIES_RESISTOR_OHM. Mixed: voeding aan voor het meten.
  bool ReadTemperatureCelsius(float& waarde) {
    waarde = NAN;
    int32_t rawValue = 0;
    float sensorResistanceOhm = NAN;
    if (!actief || parent == nullptr || !RawAnalogRead(rawValue)) return false;
    if (!VoltageDivider::CalculateSensorResistanceOhm(static_cast<int32_t>(static_cast<float>(rawValue) * parent->GetExtenderReadAnalogEffectiefBereikMillivolt() / bereikMillivolt),
                                                       SENSOR_MF52X_SERIES_RESISTOR_OHM, sensorResistanceOhm)) return false;
    waarde = 1.0f / (1.0f / 298.15f + logf(sensorResistanceOhm / SENSOR_MF52X_NTC_RESISTANCE_AT_25C_OHM) / SENSOR_MF52X_NTC_BETA) - 273.15f;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    WriteQueueOrStack(2, waarde);   // temperatuur (°C)
#endif
    return true;
  }
#if GEDEELDE_BUS_MIXED_PINS
  // Mixed: het analoge signaal op de ADC-parent, de voeding van de NTC-deler op een tweede Extender (Extender-pinnummer np_VOEDING).
  uint8_t exclusiefMetHulp_[2];
  GedeeldeBusNode* extenderHulp_[2] = {nullptr, nullptr};
  uint8_t np_VOEDING = static_cast<uint8_t>(HardwareResourcePin::NONE);
  bool viaMixed = false;
  MF52x(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin, GedeeldeBusNode* extender, uint8_t np_VOEDING,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, component == GedeeldeBusComponent::MF52D ? SENSOR_MF52D_ADC_BEREIK_MILLIVOLT : SENSOR_MF52A_ADC_BEREIK_MILLIVOLT, Extender), np_VOEDING(np_VOEDING) {
    exclusiefMetHulp_[0] = sensorPin;
    exclusiefMetHulp_[1] = np_VOEDING;
    aangemeldePinnen = {nullptr, 0, exclusiefMetHulp_, 2};
    extenderHulp_[1] = extender;   // index 0 blijft nullptr: het analoge signaal hoort bij de parent
    ownerClaim = extenderHulp_;
    viaMixed = true;
  }
  // Voeding aan of uit. Meten kan alleen met de voeding aan.
  bool WriteVOEDING(bool aan) {
    if (!viaMixed || extenderHulp_[1] == nullptr || !extenderHulp_[1]->actief) return false;
#if EXTENDER_PCA9685_AANTAL > 0
    if (extenderHulp_[1]->component == GedeeldeBusComponent::PWM_PCA9685) return extenderHulp_[1]->WriteExtenderPWM(np_VOEDING, aan ? 4095 : 0);
#endif
    return extenderHulp_[1]->WriteExtenderPin(np_VOEDING, aan ? HIGH : LOW);
  }
  bool Activeren() override {
    if (!SensorAnaloog::Activeren()) return false;
    if (!viaMixed) return true;
    if (extenderHulp_[1] == nullptr || !extenderHulp_[1]->actief) return false;
#if EXTENDER_PCA9685_AANTAL > 0
    // De PCA9685 heeft geen digitale pin: het kanaal volledig aan (4095) als voeding.
    if (extenderHulp_[1]->component == GedeeldeBusComponent::PWM_PCA9685) return extenderHulp_[1]->WriteExtenderPWM(np_VOEDING, 4095);
#endif
    // Voeding aan: eerst HIGH schrijven, dan uitgang (geen korte LOW).
    return extenderHulp_[1]->WriteExtenderPin(np_VOEDING, HIGH) && extenderHulp_[1]->SetExtenderPinMode(np_VOEDING, OUTPUT);
  }
#endif
};

#endif

#endif
