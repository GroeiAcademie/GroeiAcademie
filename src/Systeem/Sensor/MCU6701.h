#ifndef SENSOR_MCU6701_H
#define SENSOR_MCU6701_H
#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"
#if SENSOR_MCU6701_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"
#include <SPI.h>
// CJMCU-6701: analog OUT of MCP3201 SPI. SPI heeft geen MOSI-dataoverdracht.
// Modus wordt door de bestaande constructor-pinstructuur gekozen.
struct MCU6701 : Sensor {
  enum class UitleesMethode : uint8_t { ANALOOG, SPI };
  UitleesMethode methode;
  const uint8_t sensorPin;
  const HardwareResourcePin np_CS;
  uint8_t gedeeld_[3];
  uint8_t exclusief_[1];
  bool metingGestart = false;
  int8_t laatsteStartresultaat = -1;
  // OUT via bestaande ADC-Extender: sensorPin is zijn channel.
  MCU6701(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
          HardwareResourceToegang extenderToegang = HardwareResourceToegang::GEDEELD)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, extenderToegang),
      methode(UitleesMethode::ANALOOG), sensorPin(sensorPin), np_CS(HardwareResourcePin::NONE) {
    exclusief_[0] = sensorPin;
  }
  // MCP3201 SPI: CS exclusief en SCK/MISO/MOSI gedeelde SPI-busclaims.
  MCU6701(GedeeldeBusNode* parent, GedeeldeBusComponent component,
          HardwareResourcePin np_CS, HardwareResourcePin np_SCK,
          HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI)
    : Sensor(parent, {gedeeld_, 3, exclusief_, 1}, component, HardwareResourceToegang::GEDEELD),
      methode(UitleesMethode::SPI), sensorPin(0), np_CS(np_CS) {
    gedeeldeIdentiteiten = GedeeldeBusIdentiteitSPI;
    gedeeld_[0] = static_cast<uint8_t>(np_SCK);
    gedeeld_[1] = static_cast<uint8_t>(np_MISO);
    gedeeld_[2] = static_cast<uint8_t>(np_MOSI);
    exclusief_[0] = static_cast<uint8_t>(np_CS);
  }
  bool inpluggen() override;
  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool StartAnalogRead();
  bool IsAnalogReadKlaar();
  int GetAnalogRead();
  bool RawAnalogRead(int32_t& waarde);
  bool LeesHoofdwaarde(float& waarde);
  bool afmelden() override;
};
#endif
#endif
