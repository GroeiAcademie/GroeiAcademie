// ============================================================================
// Sensor met gemengde pinnen (NAAM VOORLOPIG)
// ============================================================================
// Voorbeeld: hoe een sensor zijn pinnen op het board EN op een of meer Extenders claimt.
// - 2 digitale pinnen op het board:     D2 en D3
// - 2 digitale pinnen op een PCF8574:   EP_P0 en EP_P1
// Zo kan je ook pinnen op een tweede of derde Extender zetten: per exclusieve pin één plaats in gebied_[].
//
// Vereist: GEDEELDE_BUS_GEMENGDE_PINNEN 1 en EXTENDER_PCF8574_AANTAL >= 1 (bv. in UserConfig.h).
// GedeeldeBus controleert per pin in het juiste gebied:
// - D2 tegen andere claims op het board;
// - EP_P0 tegen andere claims op dezelfde PCF8574 (en of EP_P0 op een PCF8574 bestaat: anders GB111).
// ============================================================================

#include <GroeiAcademie.h>

#if !GEDEELDE_BUS_GEMENGDE_PINNEN
  #error Zet GEDEELDE_BUS_GEMENGDE_PINNEN op 1 voor dit voorbeeld.
#endif

#if EXTENDER_PCF8574_AANTAL < 1
  #error Zet EXTENDER_PCF8574_AANTAL op minstens 1 voor dit voorbeeld.
#endif

// De Extender: zoals altijd een gewoon component onder Native.
ExtenderPCF8574 extenderA(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574, I2C_ADDRESS_EXTENDER_PCF8574_1,
                          HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8574_1_INT);   // NAAM VOORLOPIG

// De sensor: hangt onder Native (niet onder de Extender), want hij heeft ook pinnen op het board.
struct SensorGemengd : Sensor {   // NAAM VOORLOPIG
  uint8_t exclusief_[4];
  GedeeldeBusNode* gebied_[4];   // per exclusieve pin: nullptr = het board, anders de Extender waarop die pin zit

  SensorGemengd(GedeeldeBusNode* parent, GedeeldeBusNode* extender)
    : Sensor(parent, { nullptr, 0, exclusief_, 4 }, GedeeldeBusComponent::GC_SENSOR) {
    exclusief_[0] = static_cast<uint8_t>(HardwareResourcePin::NP_D2);                 gebied_[0] = nullptr;
    exclusief_[1] = static_cast<uint8_t>(HardwareResourcePin::NP_D3);                 gebied_[1] = nullptr;
    exclusief_[2] = static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P0);       gebied_[2] = extender;
    exclusief_[3] = static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P1);       gebied_[3] = extender;
    gebiedPerClaim = gebied_;   // vanaf hier controleert GedeeldeBus elke pin in zijn eigen gebied
  }

  // Eén pin schrijven, waar die ook zit. index = plaats in exclusief_[].
  void Schrijven(uint8_t index, uint8_t niveau) {   // NAAM VOORLOPIG
    if (gebied_[index] != nullptr) gebied_[index]->SchrijvenPin(exclusief_[index], niveau);
    else digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(exclusief_[index])), niveau);
  }

  // Eén pin lezen, waar die ook zit.
  uint8_t Lezen(uint8_t index) {   // NAAM VOORLOPIG
    if (gebied_[index] != nullptr) return gebied_[index]->LezenPin(exclusief_[index]);
    return digitalRead(NativeArduinoPinVan(static_cast<HardwareResourcePin>(exclusief_[index])));
  }

  // Inpluggen: de Extender moet al werken; daarna de richting van elke pin zetten.
  bool inpluggen() override {
    if (ingeplugd) return true;
    for (uint8_t index = 0; index < 4; index++) {
      if (gebied_[index] != nullptr && !gebied_[index]->actief) return false;
    }
    if (!Sensor::inpluggen()) return false;

    // D2 en EP_P0 als uitgang, D3 en EP_P1 als ingang.
    pinMode(NativeArduinoPinVan(HardwareResourcePin::NP_D2), OUTPUT);
    pinMode(NativeArduinoPinVan(HardwareResourcePin::NP_D3), INPUT);
    gebied_[2]->ModusPin(exclusief_[2], OUTPUT);
    gebied_[3]->ModusPin(exclusief_[3], INPUT);
    return true;
  }
};

SensorGemengd sensor(&Native, &extenderA);   // NAAM VOORLOPIG

void ToonResultaat(const __FlashStringHelper* stap, bool gelukt) {
  GA_SERIAL.print(stap);
  GA_SERIAL.print(F(": "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  // Eerst de Extender, dan de sensor: de sensor gebruikt pinnen van die Extender.
  ToonResultaat(F("Extender A"), extenderA.aanmelden() && extenderA.controleren() && extenderA.inpluggen() && extenderA.activeren());
  ToonResultaat(F("Sensor met gemengde pinnen"), sensor.aanmelden() && sensor.controleren() && sensor.inpluggen() && sensor.activeren());
}

void loop() {
  if (!sensor.actief) return;

  sensor.Schrijven(0, HIGH);   // D2 op het board
  sensor.Schrijven(2, HIGH);   // EP_P0 op de PCF8574

  GA_SERIAL.print(F("D3 = "));
  GA_SERIAL.print(sensor.Lezen(1));
  GA_SERIAL.print(F("   EP_P1 = "));
  GA_SERIAL.println(sensor.Lezen(3));

  delay(500);
  sensor.Schrijven(0, LOW);
  sensor.Schrijven(2, LOW);
  delay(500);
}
