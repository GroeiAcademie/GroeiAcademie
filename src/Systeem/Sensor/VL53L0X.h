#ifndef SENSOR_VL53L0X_H
#define SENSOR_VL53L0X_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_VL53L0X_AANTAL > 0

#include "../GedeeldeBus/GedeeldeBus.h"
#include <Adafruit_VL53L0X.h>

// Officiële API: https://github.com/adafruit/Adafruit_VL53L0X/blob/master/src/Adafruit_VL53L0X.h
struct SensorVL53L0X : Sensor {
  Adafruit_VL53L0X vl;
  bool metingGestart = false;                // een afstandsmeting gestart, nog niet verwerkt
  unsigned long tijdstipMetingGestart = 0;   // millis() van de meetstart
  unsigned long tijdslimietMeting = 200UL;   // milliseconden; berekend uit het meetbudget, 200 ms als terugval
  SensorVL53L0X(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
    : Sensor(parent, {nullptr, 0, nullptr, 0}, component, HardwareResourceToegang::GEDEELD) {}
#if GEDEELDE_BUS_MIXED_PINS
  // Mixed: I2C via de gewone parent (bv. achter TCA9548A); GPIO1, XSHUT op een tweede Extender (PCF8574, PCF8575, MCP23017, MCP23S17, SC16IS752 of MAX14830 GPIO).
  // np_GPIO1, np_XSHUT: Extender-pinnummer(s) op die tweede Extender, geen Arduino-nummers.
  uint8_t exclusief_[2];
  GedeeldeBusNode* extenderHulp_[2] = {nullptr, nullptr};
  uint8_t np_GPIO1 = static_cast<uint8_t>(HardwareResourcePin::NONE);
  uint8_t np_XSHUT = static_cast<uint8_t>(HardwareResourcePin::NONE);
  bool viaMixed = false;
  SensorVL53L0X(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component, GedeeldeBusNode* extender, uint8_t np_GPIO1, uint8_t np_XSHUT)
    : Sensor(parent, {nullptr, 0, exclusief_, 2}, component, HardwareResourceToegang::GEDEELD), np_GPIO1(np_GPIO1), np_XSHUT(np_XSHUT) {
    exclusief_[0] = np_GPIO1;
    extenderHulp_[0] = extender;
    exclusief_[1] = np_XSHUT;
    extenderHulp_[1] = extender;
    ownerClaim = extenderHulp_;
    viaMixed = true;
  }
#endif
  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
#if GEDEELDE_BUS_MIXED_PINS
  // GPIO1 lezen (true = meting klaar, laag actief). Activeren() stelt GPIO1 zo in (setGpioConfig).
  bool ReadGPIO1(bool& waarde) {
    waarde = false;
    if (!actief || !viaMixed || extenderHulp_[0] == nullptr) return false;
    const uint8_t niveau = extenderHulp_[0]->ReadExtenderPin(np_GPIO1);
    if (niveau != LOW && niveau != HIGH) return false;
    waarde = niveau == LOW;
    return true;
  }
  // XSHUT hoog (sensor aan) of laag (sensor uit). Na weer hoog: opnieuw activeren (de sensor start opnieuw op).
  bool WriteXSHUT(bool hoog) {
    if (!viaMixed || extenderHulp_[1] == nullptr || !extenderHulp_[1]->actief) return false;
    return extenderHulp_[1]->WriteExtenderPin(np_XSHUT, hoog ? HIGH : LOW);
  }
#endif
};

#endif

#endif
