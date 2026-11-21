#include "HSR0x.h"
#if SENSOR_HCSR04_AANTAL > 0 || SENSOR_HCSR04P_AANTAL > 0 || SENSOR_HYSRF05_AANTAL > 0
// HCSR04: TRIG en ECHO zijn gewone digitale signalen zonder registreerbare chipidentiteit.
// HCSR04: https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf
// Signaalniveaus: https://learn.adafruit.com/ultrasonic-sonar-distance-sensors/pinouts
bool HSR0x::inpluggen() {
  if (ingeplugd) return true;
  // Deze driver gebruikt native timing/pinnen: geen GPIO- of 1-Wire-Extenderprotocol,
  // behalve een directe CD74HC4067- of MCP23017/MCP23S17-parent (zie ExtenderFysiekGeschikt()).
  if (firstExtenderNode() != nullptr && !ExtenderFysiekGeschikt(firstExtenderNode())) return false;
  if (this->np_TRIG == HardwareResourcePin::NONE || this->np_TRIG == HardwareResourcePin::CUSTOM) return false;
  if (this->np_ECHO == HardwareResourcePin::NONE || this->np_ECHO == HardwareResourcePin::CUSTOM) return false;
  if (this->np_TRIG == this->np_ECHO) return false; // deze TRIG/ECHO-driver ondersteunt geen éénpinsmodus
  if (firstExtenderNode() != nullptr) {
#if EXTENDER_CD74HC4067_AANTAL > 0
    if (parent->component == GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067) {
      // Z als ingang tot Activeren(); de CD74HC4067 is dan nog uitgeschakeld (TRIG met pull-down aan de sensorkant).
      if (!Sensor::inpluggen()) return false;
      pinMode(NativeArduinoPinVan(static_cast<ExtenderCD74HC4067*>(parent)->np_Z), INPUT);
      return true;
    }
#endif
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    // MCP23017/MCP23S17: de INTA-uitgang van de MCP moet op een native pin zitten (claim door de Extender zelf).
  #if EXTENDER_MCP23017_AANTAL > 0
    if (parent->component == GedeeldeBusComponent::DIGITAL_PINS_MCP23017 &&
        static_cast<ExtenderMCP23017*>(parent)->np_INTA == HardwareResourcePin::NONE) return false;
  #endif
  #if EXTENDER_MCP23S17_AANTAL > 0
    if (parent->component == GedeeldeBusComponent::DIGITAL_PINS_MCP23S17 &&
        static_cast<ExtenderMCP23S17*>(parent)->np_INTA == HardwareResourcePin::NONE) return false;
  #endif
    if (!Sensor::inpluggen()) return false;
    // Veilige rusttoestand: TRIG laag als uitgang, ECHO als ingang. Geen ultrasone puls in deze fase.
    return parent->WriteExtenderPin(static_cast<uint8_t>(np_TRIG), LOW) &&
           parent->SetExtenderPinMode(static_cast<uint8_t>(np_TRIG), OUTPUT) &&
           parent->SetExtenderPinMode(static_cast<uint8_t>(np_ECHO), INPUT);
#else
    return false;
#endif
  }
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
  if (firstExtenderNode() != nullptr) {
    // Via de INTA van de MCP (laag actief, bij elke wissel van ECHO): eerste flank = begin, tweede = einde.
    if (!echoStartEdgeSeenViaINTA) { tijdstipEchoStart = nu; echoStartEdgeSeenViaINTA = true; }
    else { tijdstipEchoEinde = nu; echoKlaar = true; }
    return;
  }
  if (digitalRead(NativeArduinoPinVan(np_ECHO)) == HIGH) tijdstipEchoStart = nu;
  else if (tijdstipEchoStart != 0) { tijdstipEchoEinde = nu; echoKlaar = true; }
}

bool HSR0x::Activeren() {
  // Geen busprotocol of chip-ID beschikbaar: bevestigt uitsluitend geldige configuratie
  // en lifecycle van de parent, NIET de fysieke aanwezigheid van deze module.
  if (!(parent && parent->actief && ingeplugd)) return false;
  echoMetingBezig = false;
  echoKlaar = false;
  if (firstExtenderNode() != nullptr) {
#if EXTENDER_CD74HC4067_AANTAL > 0
    if (parent->component == GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067) {
      // De CD74HC4067 heeft het TRIG-kanaal gekozen (eerste claim): Z laag als uitgang = TRIG in rust.
      const uint8_t z = NativeArduinoPinVan(static_cast<ExtenderCD74HC4067*>(parent)->np_Z);
      digitalWrite(z, LOW);
      pinMode(z, OUTPUT);
    }
#endif
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF && (EXTENDER_MCP23017_AANTAL > 0 || EXTENDER_MCP23S17_AANTAL > 0)
    // MCP23017/MCP23S17: interrupt-on-change op ECHO; de INTA-uitgang (laag actief) op een native interrupt.
    if (!echoViaInterrupt && parent->component != GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067) {
      HardwareResourcePin nativePinINTAofMCP = HardwareResourcePin::NONE;
      bool gelukt = false;
  #if EXTENDER_MCP23017_AANTAL > 0
      if (parent->component == GedeeldeBusComponent::DIGITAL_PINS_MCP23017) {
        ExtenderMCP23017* mcp = static_cast<ExtenderMCP23017*>(parent);
        nativePinINTAofMCP = mcp->np_INTA;
        // Datasheet: bank B geeft INTB; met IOCON.MIRROR geven INTA en INTB samen elke interrupt, zodat ECHO ook op GPB0..GPB7 werkt.
        gelukt = mcp->enableInterrupt(static_cast<uint8_t>(np_ECHO), CHANGE) && mcp->mirrorInterrupts(true);
      }
  #endif
  #if EXTENDER_MCP23S17_AANTAL > 0
      if (parent->component == GedeeldeBusComponent::DIGITAL_PINS_MCP23S17) {
        ExtenderMCP23S17* mcp = static_cast<ExtenderMCP23S17*>(parent);
        nativePinINTAofMCP = mcp->np_INTA;
        // Datasheet: bank B geeft INTB; met IOCON.MIRROR geven INTA en INTB samen elke interrupt, zodat ECHO ook op GPB0..GPB7 werkt.
        gelukt = mcp->enableInterrupt(static_cast<uint8_t>(np_ECHO), CHANGE) && mcp->mirrorInterrupts(true);
      }
  #endif
      if (nativePinINTAofMCP == HardwareResourcePin::NONE || !gelukt) return false;
      const int interrupt = digitalPinToInterrupt(NativeArduinoPinVan(nativePinINTAofMCP));
      if (interrupt < 0) return false;
      for (uint8_t index = 0; index < 3; ++index) {
        if (actieveHSR0x[index] != nullptr) continue;
        actieveHSR0x[index] = this;
        attachInterrupt(interrupt, index == 0 ? EchoWijziging0 : index == 1 ? EchoWijziging1 : EchoWijziging2, FALLING);
        echoViaInterrupt = true;
        break;
      }
      if (!echoViaInterrupt) return false;
    }
#endif
    return true;
  }
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
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF && EXTENDER_MCP23017_AANTAL > 0
    if (firstExtenderNode() != nullptr && parent->component == GedeeldeBusComponent::DIGITAL_PINS_MCP23017)
      detachInterrupt(digitalPinToInterrupt(NativeArduinoPinVan(static_cast<ExtenderMCP23017*>(parent)->np_INTA)));
    else
#endif
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF && EXTENDER_MCP23S17_AANTAL > 0
    if (firstExtenderNode() != nullptr && parent->component == GedeeldeBusComponent::DIGITAL_PINS_MCP23S17)
      detachInterrupt(digitalPinToInterrupt(NativeArduinoPinVan(static_cast<ExtenderMCP23S17*>(parent)->np_INTA)));
    else
#endif
    detachInterrupt(digitalPinToInterrupt(NativeArduinoPinVan(np_ECHO)));
    for (uint8_t index = 0; index < 3; ++index) if (actieveHSR0x[index] == this) actieveHSR0x[index] = nullptr;
    echoViaInterrupt = false;
  }
  return Sensor::afmelden();
}

bool HSR0x::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!actief) return false;
  if (firstExtenderNode() != nullptr) {
    unsigned long duur = 0;
#if EXTENDER_CD74HC4067_AANTAL > 0
    if (parent->component == GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067) {
      // Eén CD74HC4067: TRIG-kanaal kiezen, puls op Z, dan het ECHO-kanaal kiezen en de puls meten op Z.
      // De HC-SR04 zet ECHO pas na de ultrasone burst hoog (rond 0,2 ms): tijd genoeg om van kanaal te wisselen.
      ExtenderCD74HC4067* mux = static_cast<ExtenderCD74HC4067*>(parent);
      if (mux->metingBezig) return false;   // een analoge meting op dezelfde CD74HC4067 loopt
      const uint8_t z = NativeArduinoPinVan(mux->np_Z);
      mux->selectChannel(static_cast<ExtenderCD74HC4067::ExtenderPins>(static_cast<uint8_t>(np_TRIG)));
      mux->enable();
      digitalWrite(z, LOW);
      pinMode(z, OUTPUT);
      delayMicroseconds(2);
      digitalWrite(z, HIGH);
      delayMicroseconds(10);
      digitalWrite(z, LOW);
      pinMode(z, INPUT);
      mux->selectChannel(static_cast<ExtenderCD74HC4067::ExtenderPins>(static_cast<uint8_t>(np_ECHO)));
      duur = pulseIn(z, HIGH, 30000UL);
      // Terug naar TRIG in rust (laag).
      mux->selectChannel(static_cast<ExtenderCD74HC4067::ExtenderPins>(static_cast<uint8_t>(np_TRIG)));
      pinMode(z, OUTPUT);
    } else
#endif
    {
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
      // MCP23017/MCP23S17: TRIG via de Extender, ECHO via de INTA-interrupt.
      if (!echoViaInterrupt) return false;
      noInterrupts(); echoKlaar = false; echoStartEdgeSeenViaINTA = false; tijdstipEchoStart = 0; interrupts();
      parent->ReadExtenderPin(static_cast<uint8_t>(np_ECHO));   // GPIO lezen wist een openstaande MCP-interrupt
      // De HC-SR04 vraagt minstens 10 µs HIGH; een snelle SPI-bus (MCP23S17) haalt dat niet altijd met twee busopdrachten.
      if (!parent->WriteExtenderPin(static_cast<uint8_t>(np_TRIG), HIGH)) return false;
      delayMicroseconds(10);
      if (!parent->WriteExtenderPin(static_cast<uint8_t>(np_TRIG), LOW)) return false;
      const unsigned long begin = micros();
      bool intaReleasedAfterEchoStart = false;
      while (!echoKlaar && micros() - begin <= 30000UL) {
        // Na de eerste flank INTA vrijgeven (GPIO lezen), zodat de dalende flank van ECHO een nieuwe interrupt geeft.
        if (echoStartEdgeSeenViaINTA && !intaReleasedAfterEchoStart) {
          parent->ReadExtenderPin(static_cast<uint8_t>(np_ECHO));
          intaReleasedAfterEchoStart = true;
        }
      }
      if (!echoKlaar) return false;
      noInterrupts(); duur = tijdstipEchoEinde - tijdstipEchoStart; interrupts();
#else
      return false;
#endif
    }
    if (!duur) return false;
    waarde = static_cast<float>(duur) * 0.343f / 2.0f; // millimeter
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    WriteQueueOrStack(1, waarde);
#endif
    return true;
  }
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
