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
#if EXTENDER_CD74HC4067_AANTAL > 0
      // Via de CD74HC4067: np_DATA is het kanaal EP_Yn; de DHT-driver werkt op de native Z-pin van die CD74HC4067.
      dht(NativeArduinoPinVan(parent != nullptr && parent->component == GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067 ? static_cast<ExtenderCD74HC4067*>(parent)->np_Z : np_DATA),
          component == GedeeldeBusComponent::DHT22 ? DHT22 : DHT11) {
#else
      dht(NativeArduinoPinVan(np_DATA), component == GedeeldeBusComponent::DHT22 ? DHT22 : DHT11) {
#endif
    exclusief_[0] = static_cast<uint8_t>(np_DATA);
  }

  bool inpluggen() override;
  // Timinggevoelig native protocol: niet via een generieke GPIO-Extender (GB112).
  // Wel via de CD74HC4067: die schakelt de DATA-lijn analoog door naar de native Z-pin.
#if EXTENDER_CD74HC4067_AANTAL > 0
  bool ExtenderFysiekGeschikt(const GedeeldeBusNode* extender) const override { return extender->component == GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067 && extender == parent; }
#else
  bool ExtenderFysiekGeschikt(const GedeeldeBusNode* extender) const override { (void)extender; return false; }
#endif

  uint8_t AantalChannels() const override { return 2; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};

#endif

#endif
