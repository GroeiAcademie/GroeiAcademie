#ifndef SENSOR_INMP441_H
#define SENSOR_INMP441_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_INMP441_AANTAL > 0
#if BOARD_VERSION != BOARD_ESP32S3_ARDI32 && BOARD_VERSION != BOARD_ESP32S3_DEV
#error INMP441 via I2S is in het FrameWork uitsluitend voor ESP32-S3 ARDI32 of DEV voorzien.
#endif
#include <ESP_I2S.h>
#include "../GedeeldeBus/GedeeldeBus.h"

// 32-bit Philips-I2S-slots: bewaar de 24-bit microfoondata in het oorspronkelijke woord.
// Bij stereo staan de I2S-slots links en rechts afwisselend in de uitvoer.
struct INMP441 : Sensor {
  const HardwareResourcePin np_BCLK;
  const HardwareResourcePin np_WS;
  const HardwareResourcePin np_SD;
  uint8_t exclusief_[3];
  I2SClass I2S;

  INMP441(GedeeldeBusNode* parent, GedeeldeBusComponent component,
          HardwareResourcePin np_BCLK, HardwareResourcePin np_WS, HardwareResourcePin np_SD)
    : Sensor(parent, {nullptr, 0, exclusief_, 3}, component, HardwareResourceToegang::EXCLUSIEF),
      np_BCLK(np_BCLK), np_WS(np_WS), np_SD(np_SD) {
    exclusief_[0] = static_cast<uint8_t>(np_BCLK);
    exclusief_[1] = static_cast<uint8_t>(np_WS);
    exclusief_[2] = static_cast<uint8_t>(np_SD);
  }

  bool inpluggen() override;
  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool afmelden() override;
  size_t LeesSamples(int32_t* samples, size_t aantal);
};

#endif

#endif
