#ifndef SENSOR_NEUROSKYTGAM_H
#define SENSOR_NEUROSKYTGAM_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_NEUROSKYTGAM_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"

struct NeuroSkyTGAM : Sensor {
  Stream* poort;
  // De sketch verklaart deze TX/RX-pinnen; de claim controleert niet de werkelijke poortconfiguratie.
  uint8_t exclusief_[2];
  NeuroSkyTGAM(GedeeldeBusNode* parent, GedeeldeBusComponent component, Stream& serielePoort, HardwareResourcePin np_TX, HardwareResourcePin np_RX)
    : Sensor(parent, {nullptr, 0, exclusief_, 2}, component, HardwareResourceToegang::EXCLUSIEF), poort(&serielePoort) { exclusief_[0] = static_cast<uint8_t>(np_TX); exclusief_[1] = static_cast<uint8_t>(np_RX); }
  // Via een UART-Extender (SC16IS752, MAX14830): de Sensor haalt de Stream van zijn channel bij zijn parent.
  NeuroSkyTGAM(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t channel)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, HardwareResourceToegang::EXCLUSIEF),
      poort(parent != nullptr ? parent->GetExtenderStream(channel) : nullptr) { exclusief_[0] = channel; }
  bool inpluggen() override;
  uint8_t AantalChannels() const override { return 10; }   // attention, meditation, 8 EEG-banden   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesOntvangenByte(uint8_t& waarde);
  bool LeesHoofdwaarde(float& waarde); // gevalideerde eSense Attention 0..100
  uint8_t pakket[169] = {0};
  uint8_t fasePakket = 0;
  uint8_t pakketLengte = 0;
  uint8_t pakketPositie = 0;
  uint8_t pakketSom = 0;
  // Uit hetzelfde pakket als attention, alleen bij poor signal 0 (zoals de library Brain ze leest).
  float laatsteMeditatie = NAN;              // eSense Meditation 0..100 (code 0x05)
  uint32_t laatsteEEGBanden[8] = {};         // ASIC_EEG_POWER (code 0x83): delta, theta, low alpha, high alpha, low beta, high beta, low gamma, mid gamma
  bool eegBandenGeldig = false;
  bool LeesMeditatie(float& waarde);
  bool LeesEEGBanden(uint32_t banden[8]);
};

#endif

#endif
