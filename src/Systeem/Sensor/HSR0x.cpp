#include "HSR0x.h"
#if SENSOR_HCSR04_AANTAL > 0 || SENSOR_HCSR04P_AANTAL > 0 || SENSOR_HYSRF05_AANTAL > 0
// HCSR04: TRIG en ECHO zijn gewone digitale signalen zonder registreerbare chipidentiteit.
// HCSR04: https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf
// Signaalniveaus: https://learn.adafruit.com/ultrasonic-sonar-distance-sensors/pinouts
bool HSR0x::inpluggen() {
  if (ingeplugd) return true;
  // Deze driver gebruikt native timing/pinnen: geen GPIO- of 1-Wire-Extenderprotocol.
  if (firstExtenderNode() != nullptr) return false;
  if (this->np_TRIG == HardwareResourcePin::NONE || this->np_TRIG == HardwareResourcePin::CUSTOM) return false;
  if (this->np_ECHO == HardwareResourcePin::NONE || this->np_ECHO == HardwareResourcePin::CUSTOM) return false;
  if (this->np_TRIG == this->np_ECHO) return false; // deze TRIG/ECHO-driver ondersteunt geen éénpinsmodus
  if (!Sensor::inpluggen()) return false;
  const uint8_t trigger = NativeArduinoPinVan(this->np_TRIG);
  const uint8_t echo = NativeArduinoPinVan(this->np_ECHO);
  // Veilige rusttoestand; er wordt in deze fase geen ultrasone puls verzonden.
  digitalWrite(trigger, LOW);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  return true;
}
// Maximaal drie gelijktijdige instanties (HCSR04, HCSR04P en HYSRF05, elk 0 of 1).
static HSR0x* actieveHSR0x[3] = { nullptr, nullptr, nullptr };
static void EchoWijziging0() { if (actieveHSR0x[0] != nullptr) actieveHSR0x[0]->EchoWijziging(); }
static void EchoWijziging1() { if (actieveHSR0x[1] != nullptr) actieveHSR0x[1]->EchoWijziging(); }
static void EchoWijziging2() { if (actieveHSR0x[2] != nullptr) actieveHSR0x[2]->EchoWijziging(); }

void HSR0x::EchoWijziging() {
  const unsigned long nu = micros();
  if (digitalRead(NativeArduinoPinVan(np_ECHO)) == HIGH) tijdstipEchoStart = nu;
  else if (tijdstipEchoStart != 0) { tijdstipEchoEinde = nu; echoKlaar = true; }
}

bool HSR0x::Activeren() {
  // Geen busprotocol of chip-ID beschikbaar: bevestigt uitsluitend geldige configuratie
  // en lifecycle van de parent, NIET de fysieke aanwezigheid van deze module.
  if (!(parent && parent->actief && ingeplugd)) return false;
  echoMetingBezig = false;
  echoKlaar = false;
  if (!echoViaInterrupt) {
    const int interrupt = digitalPinToInterrupt(NativeArduinoPinVan(np_ECHO));
#ifdef NOT_AN_INTERRUPT
    const bool interruptMogelijk = interrupt != NOT_AN_INTERRUPT;
#else
    const bool interruptMogelijk = interrupt >= 0;   // bv. UNO R4: geen vooraf controleerbare interruptlijst, zie terugval hieronder
#endif
    for (uint8_t index = 0; index < 3 && interruptMogelijk; ++index) {
      if (actieveHSR0x[index] != nullptr) continue;
      actieveHSR0x[index] = this;
      attachInterrupt(interrupt, index == 0 ? EchoWijziging0 : index == 1 ? EchoWijziging1 : EchoWijziging2, CHANGE);
      echoViaInterrupt = true;
      break;
    }
  }
  return true;
}

bool HSR0x::afmelden() {
  if (echoViaInterrupt) {
    detachInterrupt(digitalPinToInterrupt(NativeArduinoPinVan(np_ECHO)));
    for (uint8_t index = 0; index < 3; ++index) if (actieveHSR0x[index] == this) actieveHSR0x[index] = nullptr;
    echoViaInterrupt = false;
  }
  return Sensor::afmelden();
}

bool HSR0x::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!actief || firstExtenderNode() != nullptr) return false;
  const uint8_t trig = NativeArduinoPinVan(np_TRIG);
  const uint8_t echo = NativeArduinoPinVan(np_ECHO);
  unsigned long duur = 0;
  if (echoViaInterrupt) {
    if (!echoMetingBezig) {
      // Triggerpuls van 10 us; de ECHO-puls wordt verder door de interrupt gemeten.
      noInterrupts(); echoKlaar = false; tijdstipEchoStart = 0; interrupts();
      digitalWrite(trig, LOW);
      delayMicroseconds(2);
      digitalWrite(trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig, LOW);
      tijdstipTrigger = micros();
      echoMetingBezig = true;
      return false;
    }
    if (!echoKlaar) {
      if (micros() - tijdstipTrigger <= 30000UL) return false;
      echoMetingBezig = false;   // geen echo: volgende oproep start opnieuw
      // De HC-SR04 zet ECHO na elke trigger hoog. Werd zelfs het begin niet gezien, dan werkt de interrupt
      // op deze pin niet: terugval naar de blokkerende meting met pulseIn().
      if (tijdstipEchoStart == 0) {
        detachInterrupt(digitalPinToInterrupt(echo));
        for (uint8_t index = 0; index < 3; ++index) if (actieveHSR0x[index] == this) actieveHSR0x[index] = nullptr;
        echoViaInterrupt = false;
      }
      return false;
    }
    noInterrupts(); duur = tijdstipEchoEinde - tijdstipEchoStart; interrupts();
    echoMetingBezig = false;
  } else {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    duur = pulseIn(echo, HIGH, 30000UL);
  }
  if (!duur) return false;
  waarde = static_cast<float>(duur) * 0.343f / 2.0f; // millimeter
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(1, waarde);
#endif
  return true;
}
#endif
