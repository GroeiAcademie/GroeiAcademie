// ============================================================================
// Sensor: XYSTICK
// ============================================================================
// XY-STICK (PS2-joystick): twee analoge assen en een drukknop, zonder eigen klasse.
// VRx -> AIN0 en VRy -> AIN1 van een ADS1115 (via het bestaande ADC-contract van de Extender), SW -> D4 (native, INPUT_PULLUP).
// +5V en GND van de joystick op de voeding van de ADS1115 (dezelfde referentie).
// Elke joystick staat in rust net iets anders dan in het midden: de middenstand wordt bij de start gemeten,
// en een dode zone rond het midden telt als "niet bewogen".
// ============================================================================

#include <GroeiAcademie.h>
#if EXTENDER_ADS1115_AANTAL < 1
#error Zet EXTENDER_ADS1115_AANTAL op minstens 1.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE.
#endif
ExtenderADS1115 extenderADS1115(&Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::NP_SDA,
  HardwareResourcePin::NP_SCL, EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD);
const uint8_t pinKnop = ARDUINO_UNO_SHIELD_PIN_D4;
const uint16_t bereikMillivolt = 5000;   // joystick op 5 V
const int32_t dodeZone = ADC_MAX / 20;    // 5 % rond het midden
int32_t middenX = ADC_MAX / 2;
int32_t middenY = ADC_MAX / 2;
bool extenderOK = false;

// Eén as meten via het ADC-contract: Start, wachten tot klaar, Get (ook na een fout, zie het contract).
bool LeesAs(uint8_t pin, int32_t& waarde) {
  if (extenderADS1115.StartExtenderReadAnalog(pin, bereikMillivolt) != 1) return false;
  const unsigned long start = millis();
  while (!extenderADS1115.IsExtenderReadAnalogReady() && millis() - start < 50) { yield(); }
  return extenderADS1115.GetExtenderReadAnalogValue(waarde);
}

// Afwijking van het midden, met de dode zone als 0.
int32_t Richting(int32_t waarde, int32_t midden) {
  const int32_t verschil = waarde - midden;
  return (verschil > dodeZone || verschil < -dodeZone) ? verschil : 0;
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_XYSTICK"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  extenderOK = extenderADS1115.aanmelden() && extenderADS1115.controleren() && extenderADS1115.inpluggen() && extenderADS1115.activeren();
  GA_SERIAL.println(extenderOK ? F("ADS1115: OK") : F("ADS1115: FOUT"));
  pinMode(pinKnop, INPUT_PULLUP);
  if (!extenderOK) return;
  // Middenstand meten: joystick niet aanraken tijdens de start.
  int32_t waarde;
  if (LeesAs(static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0), waarde)) middenX = waarde;
  if (LeesAs(static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN1), waarde)) middenY = waarde;
}

void loop() {
  if (!extenderOK) return;
  int32_t x, y;
  const bool gelukt = LeesAs(static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0), x) &&
                      LeesAs(static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN1), y);
  if (!gelukt) {
    GA_SERIAL.println(F("Meting: FOUT"));
  } else {
    GA_SERIAL.print(F("X: "));
    GA_SERIAL.print(Richting(x, middenX));
    GA_SERIAL.print(F("  Y: "));
    GA_SERIAL.print(Richting(y, middenY));
    GA_SERIAL.print(F("  Knop: "));
    GA_SERIAL.println(digitalRead(pinKnop) == LOW ? F("ingedrukt") : F("los"));
  }
  delay(100);
}
