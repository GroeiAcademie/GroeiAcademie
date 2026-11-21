#ifndef SENSOR_HSR0X_H
#define SENSOR_HSR0X_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_HCSR04_AANTAL > 0 || SENSOR_HCSR04P_AANTAL > 0 || SENSOR_HYSRF05_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"

// HC-SR04 en HC-SR04P: zelfde TRIG/ECHO-klasse.
// De componentcode identificeert de fysieke variant zonder een tweede klasse.
struct HSR0x : Sensor {
  const HardwareResourcePin np_TRIG;
  const HardwareResourcePin np_ECHO;
  uint8_t exclusief_[2];
  HSR0x(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_TRIG, HardwareResourcePin np_ECHO)
    : Sensor(parent, {nullptr, 0, exclusief_, 2}, component, HardwareResourceToegang::EXCLUSIEF), np_TRIG(np_TRIG), np_ECHO(np_ECHO) {
    exclusief_[0] = static_cast<uint8_t>(np_TRIG);
    exclusief_[1] = static_cast<uint8_t>(np_ECHO);
  }

  bool inpluggen() override;
  // Timinggevoelig native protocol: niet via een generieke GPIO-Extender (GB112). Wel als directe parent:
  // - CD74HC4067: TRIG en ECHO zijn twee kanalen EP_Yn, na elkaar op de native Z-pin (één CD74HC4067);
  // - MCP23017/MCP23S17: TRIG en ECHO op de MCP, de ECHO-timing via de INTA-uitgang op een native pin (np_INTA van de Extender).
  bool ExtenderFysiekGeschikt(const GedeeldeBusNode* extender) const override {
    return extender == parent && (
#if EXTENDER_CD74HC4067_AANTAL > 0
      extender->component == GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067 ||
#endif
#if EXTENDER_MCP23017_AANTAL > 0
      extender->component == GedeeldeBusComponent::DIGITAL_PINS_MCP23017 ||
#endif
#if EXTENDER_MCP23S17_AANTAL > 0
      extender->component == GedeeldeBusComponent::DIGITAL_PINS_MCP23S17 ||
#endif
      false);
  }

  uint8_t AantalChannels() const override { return 1; }   // meetkanalen voor Data
  bool Activeren() override;
  bool afmelden() override;
  // Niet-blokkerend: de eerste oproep verstuurt de triggerpuls, een volgende oproep geeft de afstand (mm)
  // zodra de ECHO-puls via een interrupt gemeten is. Zonder interrupt op de ECHO-pin: blokkerend met pulseIn().
  bool LeesHoofdwaarde(float& waarde);

  volatile unsigned long tijdstipEchoStart = 0;
  volatile unsigned long tijdstipEchoEinde = 0;
  volatile bool echoKlaar = false;
  unsigned long tijdstipTrigger = 0;
  bool echoMetingBezig = false;
  bool echoViaInterrupt = false;
  // Via de INTA van een MCP23017/MCP23S17: elke flank van INTA is een wissel van ECHO; true na de eerste (ECHO hoog).
  volatile bool echoStartEdgeSeenViaINTA = false;
  void EchoWijziging();   // aangeroepen vanuit de interrupt van de ECHO-pin
};

#endif

#endif
