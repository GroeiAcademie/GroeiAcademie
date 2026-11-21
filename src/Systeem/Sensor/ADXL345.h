#ifndef SENSOR_ADXL345_H
#define SENSOR_ADXL345_H

#include <Arduino.h>
#include <Wire.h>
#include "../GedeeldeBus/GedeeldeBus.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_ADXL345_AANTAL > 0
#include <Adafruit_ADXL345_U.h>

// Sensorinitialisatie en meting via Activeren() en LeesHoofdwaarde().
// parent moet de voor dit apparaat geregistreerde HardwareResourceTypeI2C-node zijn.
struct SensorADXL345 : Sensor {
  SensorADXL345(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, { nullptr, 0, nullptr, 0 }, component, HardwareResourceToegang::GEDEELD) {}
#if GEDEELDE_BUS_MIXED_PINS
  // Mixed: I2C via de gewone parent (bv. achter TCA9548A); INT1, INT2 op een tweede Extender (PCF8574, PCF8575, MCP23017, MCP23S17, SC16IS752 of MAX14830 GPIO).
  // np_INT1, np_INT2: Extender-pinnummer(s) op die tweede Extender, geen Arduino-nummers.
  uint8_t exclusief_[2];
  GedeeldeBusNode* extenderHulp_[2] = {nullptr, nullptr};
  uint8_t np_INT1 = static_cast<uint8_t>(HardwareResourcePin::NONE);
  uint8_t np_INT2 = static_cast<uint8_t>(HardwareResourcePin::NONE);
  bool viaMixed = false;
  SensorADXL345(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component, GedeeldeBusNode* extender, uint8_t np_INT1, uint8_t np_INT2)
    : Sensor(parent, {nullptr, 0, exclusief_, 2}, component, HardwareResourceToegang::GEDEELD), np_INT1(np_INT1), np_INT2(np_INT2) {
    exclusief_[0] = np_INT1;
    extenderHulp_[0] = extender;
    exclusief_[1] = np_INT2;
    extenderHulp_[1] = extender;
    ownerClaim = extenderHulp_;
    viaMixed = true;
  }
#endif
  Adafruit_ADXL345_Unified adxl345;
  uint8_t AantalChannels() const override { return 3; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
#if GEDEELDE_BUS_MIXED_PINS
  // INT1 lezen (true = actief, hoog actief). Activeren() zet DATA_READY op INT1.
  bool ReadINT1(bool& waarde) {
    waarde = false;
    if (!actief || !viaMixed || extenderHulp_[0] == nullptr) return false;
    const uint8_t niveau = extenderHulp_[0]->ReadExtenderPin(np_INT1);
    if (niveau != LOW && niveau != HIGH) return false;
    waarde = niveau == HIGH;
    return true;
  }
  // INT2 lezen (true = beweging boven 1 g, hoog actief). Activeren() zet ACTIVITY op INT2; INT_SOURCE (0x30) lezen wist de gebeurtenis.
  bool ReadINT2(bool& waarde) {
    waarde = false;
    if (!actief || !viaMixed || extenderHulp_[1] == nullptr) return false;
    const uint8_t niveau = extenderHulp_[1]->ReadExtenderPin(np_INT2);
    if (niveau != LOW && niveau != HIGH) return false;
    waarde = niveau == HIGH;
    // ACTIVITY blijft vastgehouden tot INT_SOURCE (0x30) gelezen wordt: na een gelezen gebeurtenis vrijgeven.
    // Eerst de I2C-toegang openen (bv. het TCA9548A-kanaal), zoals de meetfuncties.
    if (waarde && static_cast<HardwareResourceTypeI2C*>(parent)->ComponentBeschikbaarTijdensRuntime()) adxl345.readRegister(0x30);
    return true;
  }
#endif
};

#endif

#endif
