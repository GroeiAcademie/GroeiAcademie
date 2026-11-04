#ifndef SENSOR_TTP223_H
#define SENSOR_TTP223_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_TTP223_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"

// TTP223 digitale OUT, geen I2C-adres en geen seriële configuratie.
struct TTP223 : Sensor {
  const HardwareResourcePin np_OUT;
  uint8_t exclusief_[1];
  TTP223(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_OUT)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, HardwareResourceToegang::EXCLUSIEF), np_OUT(np_OUT) {
    exclusief_[0] = static_cast<uint8_t>(np_OUT);
  }

  bool inpluggen() override;
  uint8_t AantalChannels() const override { return 11; }   // meetkanalen voor Data
  bool Activeren() override;
  // HW-139/TTP223 als Sensor: tijdswaarden zijn uitsluitend betrouwbaar in directe modus
  // en als WerkAanraakstatusBij() voldoende vaak wordt aangeroepen.
  bool statusBekend = false;
  bool aangeraakt = false;
  // aanrakingBegonnen en aanrakingLosgelaten gelden alleen voor de laatste oproep van WerkAanraakstatusBij(); elke oproep wist ze eerst.
  bool aanrakingBegonnen = false;
  bool aanrakingLosgelaten = false;
  uint32_t aantalAanrakingen = 0;
  uint32_t aantalKorteAanrakingen = 0;
  uint32_t aantalLangeAanrakingen = 0;
  uint32_t aantalDubbeleAanrakingen = 0;
  unsigned long tijdstipEersteAanraking = 0;
  unsigned long tijdstipLaatsteAanraking = 0;
  unsigned long tijdstipLaatsteLoslaten = 0;
  unsigned long duurLaatsteAanraking = 0;
  unsigned long totaleVoltooideAanraakduur = 0;
  unsigned long tijdTussenAanrakingen = 0;
  bool heeftEerdereLoslating = false;
  bool heeftAanraakInterval = false;
  bool WerkAanraakstatusBij();
  bool LeesHoofdwaarde(float& waarde);
  bool LeesAantalAanrakingen(uint32_t& waarde);
  bool LeesLaatsteAanraakduur(unsigned long& waarde);
  bool LeesTotaleAanraakduur(unsigned long& waarde);
  bool LeesTijdTussenAanrakingen(unsigned long& waarde);
  bool LeesAanraakfrequentie(float& waarde);
  bool LeesAantalKorteAanrakingen(uint32_t& waarde);
  bool LeesAantalLangeAanrakingen(uint32_t& waarde);
  bool LeesAantalDubbeleAanrakingen(uint32_t& waarde);
  bool LeesTijdstipLaatsteAanraking(unsigned long& waarde);
  bool LeesTijdstipLaatsteLoslaten(unsigned long& waarde);
};

#endif

#endif
