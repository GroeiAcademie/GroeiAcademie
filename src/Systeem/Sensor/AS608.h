#ifndef SENSOR_AS608_H
#define SENSOR_AS608_H
#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"
#if SENSOR_AS608_AANTAL > 0

#include "../GedeeldeBus/GedeeldeBus.h"
#include <Adafruit_Fingerprint.h>
struct AS608 : Sensor {
  Adafruit_Fingerprint vingerafdruk;
  // De sketch verklaart deze TX/RX-pinnen; de claim controleert niet de werkelijke poortconfiguratie.
  uint8_t exclusief_[2];
  AS608(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareSerial* poort, HardwareResourcePin np_TX, HardwareResourcePin np_RX)
    : Sensor(parent, {nullptr, 0, exclusief_, 2}, component, HardwareResourceToegang::EXCLUSIEF), vingerafdruk(poort) { exclusief_[0] = static_cast<uint8_t>(np_TX); exclusief_[1] = static_cast<uint8_t>(np_RX); }
  // Adafruit_Fingerprint ondersteunt officieel ook een Stream*: hiermee is
  // een UART-channel van de SC16IS752 bruikbaar zonder nieuwe driver.
  AS608(GedeeldeBusNode* parent, GedeeldeBusComponent component, Stream* poort,
        HardwareResourcePin np_TX, HardwareResourcePin np_RX)
    : Sensor(parent, {nullptr, 0, exclusief_, 2}, component, HardwareResourceToegang::EXCLUSIEF), vingerafdruk(poort) {
    exclusief_[0] = static_cast<uint8_t>(np_TX);
    exclusief_[1] = static_cast<uint8_t>(np_RX);
  }
  // Via een UART-Extender (SC16IS752, MAX14830): de Sensor haalt de Stream van zijn channel bij zijn parent.
  AS608(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t channel)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, HardwareResourceToegang::EXCLUSIEF),
      vingerafdruk(parent != nullptr ? parent->GetExtenderStream(channel) : nullptr) { exclusief_[0] = channel; }
  bool inpluggen() override;
  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};
#endif
#endif
