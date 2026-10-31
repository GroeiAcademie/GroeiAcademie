#ifndef SENSOR_DHTXX_H
#define SENSOR_DHTXX_H
#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"
#if SENSOR_DHT11_AANTAL > 0 || SENSOR_DHT22_AANTAL > 0

#include "../GedeeldeBus/GedeeldeBus.h"
#include <DHT.h>
// DHT11 en DHT22: dezelfde klasse; de componentcode kiest het fysieke type.
struct DHTxx : Sensor {
  const HardwareResourcePin np_DATA;
  uint8_t exclusief_[1];
  DHT dht;
  DHTxx(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_DATA)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, HardwareResourceToegang::EXCLUSIEF), np_DATA(np_DATA),
      dht(NativeArduinoPinVan(np_DATA), component == GedeeldeBusComponent::DHT22 ? DHT22 : DHT11) {
    exclusief_[0] = static_cast<uint8_t>(np_DATA);
  }
  bool inpluggen() override;
  // Timinggevoelig native protocol: niet via een generieke GPIO-Extender (GB112).
  bool ExtenderFysiekGeschikt(const GedeeldeBusNode* extender) const override { (void)extender; return false; }
  uint8_t AantalChannels() const override { return 2; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};
#endif
#endif
