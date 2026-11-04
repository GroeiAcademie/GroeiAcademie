#ifndef SENSOR_MR60BHA2_H
#define SENSOR_MR60BHA2_H

#include <Arduino.h>
#include <string.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_MR60BHA2_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"

// Seeed MR60BHA2: UART-frame 0x01, ID(2), lengte(2), type(2), checksum,
// payload, checksum. De officiele ESP32-only library is niet vereist.
// Stream laat zowel native HardwareSerial als een UART-Extender toe.
struct MR60BHA2 : Sensor {
  Stream* poort;
  HardwareSerial* nativePoort;
  uint8_t exclusief_[2];
  // Aanhoudende parsertoestand per sensorinstantie, niet gedeeld met andere sensoren.
  uint8_t frameKop[8] = {0};
  uint8_t hartslagBytes[4] = {0};
  uint16_t framePositie = 0;
  uint16_t frameLengte = 0;
  uint16_t frameType = 0;
  uint8_t dataXor = 0;

  MR60BHA2(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareSerial* poort,
           HardwareResourcePin np_TX, HardwareResourcePin np_RX)
    : Sensor(parent, {nullptr, 0, exclusief_, 2}, component, HardwareResourceToegang::EXCLUSIEF),
      poort(poort), nativePoort(poort) {
    exclusief_[0] = static_cast<uint8_t>(np_TX);
    exclusief_[1] = static_cast<uint8_t>(np_RX);
  }

  MR60BHA2(GedeeldeBusNode* parent, GedeeldeBusComponent component, Stream* poort,
           HardwareResourcePin np_TX, HardwareResourcePin np_RX)
    : Sensor(parent, {nullptr, 0, exclusief_, 2}, component, HardwareResourceToegang::EXCLUSIEF),
      poort(poort), nativePoort(nullptr) {
    exclusief_[0] = static_cast<uint8_t>(np_TX);
    exclusief_[1] = static_cast<uint8_t>(np_RX);
  }
  // Via een UART-Extender (SC16IS752, MAX14830): de Sensor haalt de Stream van zijn channel bij zijn parent.
  MR60BHA2(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t channel)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, HardwareResourceToegang::EXCLUSIEF),
      poort(parent != nullptr ? parent->GetExtenderStream(channel) : nullptr), nativePoort(nullptr) { exclusief_[0] = channel; }
  bool inpluggen() override;
  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
};

#endif

#endif
