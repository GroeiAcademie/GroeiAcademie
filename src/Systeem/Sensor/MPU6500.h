#ifndef SENSOR_MPU6500_H
#define SENSOR_MPU6500_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_MPU6500_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"
#include <MPU6500_WE.h>

struct MPU6500 : Sensor {
  MPU6500_WE mpu6500;
  MPU6500(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, {nullptr, 0, nullptr, 0}, component, HardwareResourceToegang::GEDEELD), mpu6500(&Wire, parent ? parent->adres : 0x68) {}   // geen parent: Activeren() faalt later; hier niets lezen via nullptr
#if GEDEELDE_BUS_MIXED_PINS
  // Mixed: I2C via de gewone parent (bv. achter TCA9548A); INT op een tweede Extender (PCF8574, PCF8575, MCP23017, MCP23S17, SC16IS752 of MAX14830 GPIO).
  // np_INT: Extender-pinnummer(s) op die tweede Extender, geen Arduino-nummers.
  uint8_t exclusief_[1];
  GedeeldeBusNode* extenderHulp_[1] = {nullptr};
  uint8_t np_INT = static_cast<uint8_t>(HardwareResourcePin::NONE);
  bool viaMixed = false;
  MPU6500(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component, GedeeldeBusNode* extender, uint8_t np_INT)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, HardwareResourceToegang::GEDEELD), mpu6500(&Wire, parent ? parent->adres : 0x68), np_INT(np_INT) {
    exclusief_[0] = np_INT;
    extenderHulp_[0] = extender;
    ownerClaim = extenderHulp_;
    viaMixed = true;
  }
#endif
  uint8_t AantalChannels() const override { return 7; }   // versnelling X/Y/Z (m/s²), hoeksnelheid X/Y/Z (rad/s), temperatuur (°C), zoals de MPU6050
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
#if GEDEELDE_BUS_MIXED_PINS
  // INT lezen (true = actief, hoog actief). Activeren() zet "data klaar" aan, vergrendeld tot een lezing.
  bool ReadINT(bool& waarde) {
    waarde = false;
    if (!actief || !viaMixed || extenderHulp_[0] == nullptr) return false;
    const uint8_t niveau = extenderHulp_[0]->ReadExtenderPin(np_INT);
    if (niveau != LOW && niveau != HIGH) return false;
    waarde = niveau == HIGH;
    return true;
  }
#endif
};

#endif

#endif
