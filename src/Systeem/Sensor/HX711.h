#ifndef SENSOR_HX711_H
#define SENSOR_HX711_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_HX711_AANTAL > 0

#include "../GedeeldeBus/GedeeldeBus.h"
#include <HX711.h>

struct SensorHX711 : Sensor {
  const HardwareResourcePin np_DOUT;
  const HardwareResourcePin np_PD_SCK;
  uint8_t exclusief_[2];
  ::HX711 adc;
  SensorHX711(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_DOUT, HardwareResourcePin np_PD_SCK)
    : Sensor(parent, {nullptr, 0, exclusief_, 2}, component, HardwareResourceToegang::EXCLUSIEF), np_DOUT(np_DOUT), np_PD_SCK(np_PD_SCK) {
    exclusief_[0] = static_cast<uint8_t>(np_DOUT);
    exclusief_[1] = static_cast<uint8_t>(np_PD_SCK);
  }

  bool inpluggen() override;
  // Timinggevoelig native protocol: niet via een generieke GPIO-Extender (GB112).
  bool ExtenderFysiekGeschikt(const GedeeldeBusNode* extender) const override { (void)extender; return false; }

  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};

#endif

#endif
