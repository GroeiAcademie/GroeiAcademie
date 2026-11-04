#ifndef SENSOR_OT3686_H
#define SENSOR_OT3686_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_OT3686_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"

// OT3686: DO digitaal; AO is volgens Otronic niet functioneel.
// Eén native interrupt per sensor, pulse count = software teller; geen Input-code.
struct OT3686 : Sensor {
  const HardwareResourcePin np_OUT;
  uint8_t exclusief_[1];
  volatile uint32_t aantalPulsen = 0;
  volatile unsigned long tijdstipLaatstePulsUs = 0;
  volatile unsigned long periodeLaatstePulsUs = 0;
  volatile bool eerstePulsOntvangen = false;
  bool interruptActief = false;
  uint32_t aantalPulsenVorigVenster = 0;
  unsigned long tijdstipVorigVensterMs = 0;
  float laatsteVensterRPM = NAN;
  static OT3686* actieveInterruptSensor;
  static void OntvangPuls();
  OT3686(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_OUT)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, HardwareResourceToegang::EXCLUSIEF), np_OUT(np_OUT) {
    exclusief_[0] = static_cast<uint8_t>(np_OUT);
  }

  bool inpluggen() override;
  uint8_t AantalChannels() const override { return 4; }   // meetkanalen voor Data
  bool Activeren() override;
  bool afmelden() override;
  bool LeesDigitaleToestand(bool& waarde);
  bool LeesAantalPulsen(uint32_t& waarde);
  // Gemiddelde over het laatste afgesloten venster (SENSOR_OT3686_MEETINTERVAL_MS); tot het volgende venster blijft die waarde staan.
  // Voor een momentane waarde: LeesRPMViaPeriode().
  bool LeesRPMViaTijdvenster(float& waarde);
  bool LeesRPMViaPeriode(float& waarde);
  bool LeesHoofdwaarde(float& waarde); // RPM via vaste tijdinterval
};

#endif

#endif
