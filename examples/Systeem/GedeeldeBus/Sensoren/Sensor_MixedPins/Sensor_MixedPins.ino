// ============================================================================
// Sensor Mixed Pins
// ============================================================================
// Voorbeeld: hoe een sensor zijn pinnen op het board EN op een of meer Extenders claimt.
// - 2 digitale pinnen op het board:     D2 en D3
// - 2 digitale pinnen op een PCF8574:   EP_P0 en EP_P1
// Zo kan je ook pinnen op een tweede of derde Extender zetten: per exclusieve pin één plaats in ownerClaims[].
//
// Vereist: GEDEELDE_BUS_MIXED_PINS 1 en EXTENDER_PCF8574_AANTAL >= 1 (bv. in UserConfig.h).
// GedeeldeBus controleert per pin in het juiste gebied:
// - D2 tegen andere claims op het board;
// - EP_P0 tegen andere claims op dezelfde PCF8574 (en of EP_P0 op een PCF8574 bestaat: anders GB111).
// ============================================================================

#include <GroeiAcademie.h>

#if !GEDEELDE_BUS_MIXED_PINS
  #error Zet GEDEELDE_BUS_MIXED_PINS op 1 voor dit voorbeeld.
#endif

#if EXTENDER_PCF8574_AANTAL < 1
  #error Zet EXTENDER_PCF8574_AANTAL op minstens 1 voor dit voorbeeld.
#endif

// De Extender: zoals altijd een gewoon component onder Native.
ExtenderPCF8574 extenderA(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574, I2C_ADDRESS_EXTENDER_PCF8574_1,
                          HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8574_1_INT);

// De sensor: hangt onder Native (niet onder de Extender), want hij heeft ook pinnen op het board.
struct MixedSensor : Sensor {
  uint8_t exclusief_[4];
  GedeeldeBusNode* ownerClaims[4];   // per exclusieve pin: nullptr = het board, anders de Extender waarop die pin zit

  MixedSensor(GedeeldeBusNode* parent, GedeeldeBusNode* extender)
    : Sensor(parent, { nullptr, 0, exclusief_, 4 }, GedeeldeBusComponent::GC_SENSOR) {
    exclusief_[0] = static_cast<uint8_t>(HardwareResourcePin::NP_D2);                 ownerClaims[0] = nullptr;
    exclusief_[1] = static_cast<uint8_t>(HardwareResourcePin::NP_D3);                 ownerClaims[1] = nullptr;
    exclusief_[2] = static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P0);       ownerClaims[2] = extender;
    exclusief_[3] = static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P1);       ownerClaims[3] = extender;
    ownerClaim = ownerClaims;   // vanaf hier controleert GedeeldeBus elke pin in zijn eigen gebied
  }

  // Eén pin schrijven, waar die ook zit. index = plaats in exclusief_[].
  void WriteSensorPin(uint8_t index, uint8_t niveau) {
    if (ownerClaims[index] != nullptr) ownerClaims[index]->WriteExtenderPin(exclusief_[index], niveau);
    else digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(exclusief_[index])), niveau);
  }

  // Eén pin lezen, waar die ook zit.
  uint8_t ReadSensorPin(uint8_t index) {
    if (ownerClaims[index] != nullptr) return ownerClaims[index]->ReadExtenderPin(exclusief_[index]);
    return digitalRead(NativeArduinoPinVan(static_cast<HardwareResourcePin>(exclusief_[index])));
  }

  // Inpluggen: de Extender moet al werken; daarna de richting van elke pin zetten.
  bool inpluggen() override {
    if (ingeplugd) return true;
    for (uint8_t index = 0; index < 4; index++) {
      if (ownerClaims[index] != nullptr && !ownerClaims[index]->actief) return false;
    }
    if (!Sensor::inpluggen()) return false;

    // D2 en EP_P0 als uitgang, D3 en EP_P1 als ingang.
    pinMode(NativeArduinoPinVan(HardwareResourcePin::NP_D2), OUTPUT);
    pinMode(NativeArduinoPinVan(HardwareResourcePin::NP_D3), INPUT);
    ownerClaims[2]->SetExtenderPinMode(exclusief_[2], OUTPUT);
    ownerClaims[3]->SetExtenderPinMode(exclusief_[3], INPUT);
    return true;
  }
};

MixedSensor mixedSensorPins(&Native, &extenderA);

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
  ToonResultaat(F("Sensor Mixed Pins"), mixedSensorPins.aanmelden() && mixedSensorPins.controleren() && mixedSensorPins.inpluggen() && mixedSensorPins.activeren());
}

void loop() {
  if (!mixedSensorPins.actief) return;

  mixedSensorPins.WriteSensorPin(0, HIGH);   // D2 op het board
  mixedSensorPins.WriteSensorPin(2, HIGH);   // EP_P0 op de PCF8574

  GA_SERIAL.print(F("D3 = "));
  GA_SERIAL.print(mixedSensorPins.ReadSensorPin(1));
  GA_SERIAL.print(F("   EP_P1 = "));
  GA_SERIAL.println(mixedSensorPins.ReadSensorPin(3));

  delay(500);
  mixedSensorPins.WriteSensorPin(0, LOW);
  mixedSensorPins.WriteSensorPin(2, LOW);
  delay(500);
}
