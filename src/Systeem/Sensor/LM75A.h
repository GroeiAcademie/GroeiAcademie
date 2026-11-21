#ifndef SENSOR_LM75A_H
#define SENSOR_LM75A_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_LM75A_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"
#include <M2M_LM75A.h>

struct LM75A : Sensor {
  M2M_LM75A lm75a;
  LM75A(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, {nullptr, 0, nullptr, 0}, component, HardwareResourceToegang::GEDEELD), lm75a(parent ? parent->adres : 0) {}   // geen parent: Activeren() faalt later; hier niets lezen via nullptr
#if GEDEELDE_BUS_MIXED_PINS
  // Mixed: I2C via de gewone parent (bv. achter TCA9548A); OS op een tweede Extender (PCF8574, PCF8575, MCP23017, MCP23S17, SC16IS752 of MAX14830 GPIO).
  // np_OS: Extender-pinnummer(s) op die tweede Extender, geen Arduino-nummers.
  uint8_t exclusief_[1];
  GedeeldeBusNode* extenderHulp_[1] = {nullptr};
  uint8_t np_OS = static_cast<uint8_t>(HardwareResourcePin::NONE);
  bool viaMixed = false;
  LM75A(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component, GedeeldeBusNode* extender, uint8_t np_OS)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, HardwareResourceToegang::GEDEELD), lm75a(parent ? parent->adres : 0), np_OS(np_OS) {
    exclusief_[0] = np_OS;
    extenderHulp_[0] = extender;
    ownerClaim = extenderHulp_;
    viaMixed = true;
  }
#endif
  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
#if GEDEELDE_BUS_MIXED_PINS
  // OS lezen (true = overtemperatuur; standaard laag actief, boven Tos 80 °C).
  bool ReadOS(bool& waarde) {
    waarde = false;
    if (!actief || !viaMixed || extenderHulp_[0] == nullptr) return false;
    const uint8_t niveau = extenderHulp_[0]->ReadExtenderPin(np_OS);
    if (niveau != LOW && niveau != HIGH) return false;
    waarde = niveau == LOW;
    return true;
  }
#endif
};

#endif

#endif
