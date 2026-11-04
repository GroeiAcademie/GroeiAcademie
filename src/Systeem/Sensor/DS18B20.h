#ifndef SENSOR_DS18B20_H
#define SENSOR_DS18B20_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_DS18B20_AANTAL > 0

#include "../GedeeldeBus/GedeeldeBus.h"
#include <OneWire.h>
#include <DallasTemperature.h>

struct DS18B20 : Sensor {
  const HardwareResourcePin np_DQ;
  uint8_t exclusief_[1];
  OneWire oneWire;
  DallasTemperature temperatuur;
  DS18B20(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_DQ,
          const uint8_t* rom = nullptr)
    : Sensor(parent, {nullptr, 0, exclusief_,
#if EXTENDER_DS2482_800_AANTAL > 0
                     static_cast<uint8_t>((parent && parent->component == GedeeldeBusComponent::ONE_WIRE_DS2482v800) ? 0 : 1)
#else
                     1
#endif
                     }, component, HardwareResourceToegang::EXCLUSIEF), np_DQ(np_DQ),
      oneWire(), temperatuur(&oneWire) {
    exclusief_[0] = static_cast<uint8_t>(np_DQ);
    if (rom != nullptr) {
      for (uint8_t i = 0; i < 8; ++i) geselecteerdeRom[i] = rom[i];
      romExpliciet = true;
    }
  }

  bool inpluggen() override;
  // Alleen de DS2482 levert een hardware-1-Wire-bus; een generieke GPIO-Extender is fysiek ongeschikt (GB112).
  bool ExtenderFysiekGeschikt(const GedeeldeBusNode* extender) const override { return extender->component == GedeeldeBusComponent::ONE_WIRE_DS2482v800; }

  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool StartTemperatuurmeting();
  bool IsTemperatuurmetingKlaar();
  bool LeesHoofdwaarde(float& waarde);
  bool temperatuurmetingGestart = false;
  unsigned long tijdstipTemperatuurmetingStart = 0;
  DeviceAddress geselecteerdeRom = {0};
  bool romExpliciet = false;
  bool romGevonden = false;
};

#endif

#endif
