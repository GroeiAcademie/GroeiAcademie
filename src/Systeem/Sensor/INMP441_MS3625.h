#ifndef SENSOR_INMP441_MS3625_H
#define SENSOR_INMP441_MS3625_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_INMP441_AANTAL > 0 || SENSOR_MS3625_AANTAL > 0
// Ondersteund zijn de boards waarvan de Arduino-core een standaard I2S-library heeft.
// ESP32-S3 ARDI32, ESP32-S3 DEV en ESP32 D1 R32: ESP_I2S.h van de ESP32-core.
// RP2040: I2S.h van de arduino-pico-core, via de PIO van de chip.
#if BOARD_VERSION == BOARD_UNO_R4_MINIMA || BOARD_VERSION == BOARD_UNO_R4_WIFI
#error INMP441 en MS3625 vereisen I2S; de RA4M1 van de UNO R4 heeft geen I2S-interface.
#elif BOARD_VERSION == BOARD_UNO_Q
#error INMP441 en MS3625 worden op de UNO Q momenteel niet ondersteund: er is nog geen werkende I2S-implementatie aangetoond voor de gebruikte Arduino-core.
#elif BOARD_VERSION == BOARD_STM32F4_NUCLEO64_F401RE
#error INMP441 en MS3625 worden op de Nucleo F401RE niet ondersteund: de STM32-core heeft geen standaard I2S-library.
#elif BOARD_VERSION == BOARD_RP2040_CYTRON_MAKER_UNO
#include <I2S.h>
#else
#include <ESP_I2S.h>
#endif
#include "../GedeeldeBus/GedeeldeBus.h"
#include "Functies/SoundLevel.h"

// 32-bit Philips-I2S-slots: bewaar de 24-bit microfoondata in het oorspronkelijke woord.
// Bij stereo staan de I2S-slots links en rechts afwisselend in de uitvoer.
// Eén klasse voor de INMP441 en de compatibele MS3625; de componentcode bepaalt welke settings gelden.
struct INMP441_MS3625 : Sensor {
  const HardwareResourcePin np_BCLK;
  const HardwareResourcePin np_WS;
  const HardwareResourcePin np_SD;
  uint8_t exclusief_[3];
#if BOARD_VERSION == BOARD_RP2040_CYTRON_MAKER_UNO
  // arduino-pico noemt zijn klasse I2S; daarom heet het member hier I2Sinput.
  I2S I2Sinput{INPUT};
#else
  I2SClass I2S;
#endif

  INMP441_MS3625(GedeeldeBusNode* parent, GedeeldeBusComponent component,
          HardwareResourcePin np_BCLK, HardwareResourcePin np_WS, HardwareResourcePin np_SD)
    : Sensor(parent, {nullptr, 0, exclusief_, 3}, component, HardwareResourceToegang::EXCLUSIEF),
      np_BCLK(np_BCLK), np_WS(np_WS), np_SD(np_SD) {
    exclusief_[0] = static_cast<uint8_t>(np_BCLK);
    exclusief_[1] = static_cast<uint8_t>(np_WS);
    exclusief_[2] = static_cast<uint8_t>(np_SD);
  }
#if GEDEELDE_BUS_MIXED_PINS && EXTENDER_CD74HC4067_AANTAL > 0
  // Mixed: BCLK en WS native (meerdere microfoons kunnen die klokken delen), SD via een kanaal EP_Yn van een CD74HC4067.
  // De I2S-driver leest SD op de native Z-pin van die CD74HC4067.
  GedeeldeBusNode* extenderHulp_[3] = {nullptr, nullptr, nullptr};
  bool viaMixed = false;
  INMP441_MS3625(GedeeldeBusNode* parent, GedeeldeBusComponent component,
          HardwareResourcePin np_BCLK, HardwareResourcePin np_WS, GedeeldeBusNode* extender, HardwareResourcePin np_SD)
    : Sensor(parent, {nullptr, 0, exclusief_, 3}, component, HardwareResourceToegang::EXCLUSIEF),
      np_BCLK(np_BCLK), np_WS(np_WS), np_SD(np_SD) {
    exclusief_[0] = static_cast<uint8_t>(np_BCLK);
    exclusief_[1] = static_cast<uint8_t>(np_WS);
    exclusief_[2] = static_cast<uint8_t>(np_SD);
    extenderHulp_[2] = extender;
    ownerClaim = extenderHulp_;
    viaMixed = true;
  }
#endif

  bool inpluggen() override;
  uint8_t AantalChannels() const override { return 3; }   // meetkanalen voor Data: 1 ruw I2S-woord, 2 RMS, 3 piek-tot-piek
  bool Activeren() override;
  bool afmelden() override;
  size_t LeesSamples(int32_t* samples, size_t aantal);
  // Geluidsniveau uit de samples van LeesSamples() (linkerslot, L/R-pin aan GND), 24-bit waarde, venster een kwart seconde.
  // Roep LeesSamples() doorlopend aan. Geldig vanaf 16 samples in het venster. Eenheid: 24-bit waarde (geen dB SPL).
  SoundLevel soundLevel;
  bool ReadSoundLevelRootMeanSquare(float& rootMeanSquare) { rootMeanSquare = NAN; return actief && soundLevel.sampleCount >= 16 && soundLevel.ReadRootMeanSquare(rootMeanSquare); }
  bool ReadSoundLevelPeakToPeak(int32_t& peakToPeak) { peakToPeak = 0; return actief && soundLevel.sampleCount >= 16 && soundLevel.ReadPeakToPeak(peakToPeak); }
};

#endif

#endif
