#ifndef SENSOR_LD2450_H
#define SENSOR_LD2450_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_LD2450_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"

struct LD2450 : Sensor {
  Stream* poort;
  // De sketch verklaart deze TX/RX-pinnen; de claim controleert niet de werkelijke poortconfiguratie.
  uint8_t exclusief_[2];
  LD2450(GedeeldeBusNode* parent, GedeeldeBusComponent component, Stream& serielePoort, HardwareResourcePin np_TX, HardwareResourcePin np_RX)
    : Sensor(parent, {nullptr, 0, exclusief_, 2}, component, HardwareResourceToegang::EXCLUSIEF), poort(&serielePoort) { exclusief_[0] = static_cast<uint8_t>(np_TX); exclusief_[1] = static_cast<uint8_t>(np_RX); }
  // Via een UART-Extender (SC16IS752, MAX14830): de Sensor haalt de Stream van zijn channel bij zijn parent.
  LD2450(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t channel)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, HardwareResourceToegang::EXCLUSIEF),
      poort(parent != nullptr ? parent->GetExtenderStream(channel) : nullptr) { exclusief_[0] = channel; }
  bool inpluggen() override;
  uint8_t AantalChannels() const override { return 3; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesOntvangenByte(uint8_t& waarde);
  bool LeesHoofdwaarde(float& waarde); // afstand tot dichtstbijzijnde gerapporteerde doel, mm
  bool LeesPositie(float& x, float& y); // X/Y van dichtstbijzijnde gerapporteerde doel, mm
  float laatsteX = NAN;
  float laatsteY = NAN;
  bool positieGeldig = false;
  uint8_t frame[30] = {0};
  uint8_t framePositie = 0;
  // Alle drie doelen van het laatste geldige frame (3 x 8 bytes: X, Y, snelheid, afstandsresolutie).
  uint8_t lastFrameTargets[24] = {0};
  unsigned long lastFrameMS = 0;  // tijdstip van het laatste geldige frame
  // Doel 0, 1 of 2. targetPresent false: het doel is leeg in het frame. X en Y in mm, snelheid in cm/s (Hi-Link: bit 15 = 1 positief).
  bool ReadTarget(uint8_t targetIndex, int16_t& xMillimeter, int16_t& yMillimeter, int16_t& speedCentimeterPerSecond, bool& targetPresent);
  // true: langer dan SENSOR_LD2450_SILENT_TIMEOUT_MS geen geldig frame (of nog nooit). LeesHoofdwaarde() verwerkt de frames.
  bool IsModuleSilent() { return lastFrameMS == 0 || millis() - lastFrameMS > SENSOR_LD2450_SILENT_TIMEOUT_MS; }
};

#endif

#endif
