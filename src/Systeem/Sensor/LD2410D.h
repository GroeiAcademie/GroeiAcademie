#ifndef SENSOR_LD2410D_H
#define SENSOR_LD2410D_H
#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"
#if SENSOR_LD2410D_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"
struct LD2410D : Sensor {
  Stream* poort;
  // De sketch verklaart deze TX/RX-pinnen; de claim controleert niet de werkelijke poortconfiguratie.
  uint8_t exclusief_[2];
  LD2410D(GedeeldeBusNode* parent, GedeeldeBusComponent component, Stream& serielePoort, HardwareResourcePin np_TX, HardwareResourcePin np_RX)
    : Sensor(parent, {nullptr, 0, exclusief_, 2}, component, HardwareResourceToegang::EXCLUSIEF), poort(&serielePoort) { exclusief_[0] = static_cast<uint8_t>(np_TX); exclusief_[1] = static_cast<uint8_t>(np_RX); }
  // Via een UART-Extender (SC16IS752, MAX14830): de Sensor haalt de Stream van zijn channel bij zijn parent.
  LD2410D(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t channel)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, HardwareResourceToegang::EXCLUSIEF),
      poort(parent != nullptr ? parent->GetExtenderStream(channel) : nullptr) { exclusief_[0] = channel; }
  bool inpluggen() override;
  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesOntvangenByte(uint8_t& waarde);
  bool LeesHoofdwaarde(float& waarde); // 0 geen doel, 1 bewegend, 2 stilstaand, 3 beide
  uint8_t frame[48] = {0};
  uint8_t framePositie = 0;
  uint8_t verwachteLengte = 0;
};
#endif
#endif
