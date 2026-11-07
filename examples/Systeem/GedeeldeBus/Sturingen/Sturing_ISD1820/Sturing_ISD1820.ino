// ============================================================================
// Sturing: ISD1820
// ============================================================================
// ISD1820: geluidsfragment (ongeveer 10 s) opnemen en afspelen via twee keer Relais op een MCP23017.
// GPA0 = REC (hoog zolang er opgenomen wordt), GPA1 = P-E / PLAYE (korte puls speelt het fragment volledig af).
// De ISD1820 neemt op met zijn eigen microfoon; er komt geen audiodata naar de Arduino.
// Pinnen zijn afgeleid uit de bestaande constructoren en de boardmapping in src/Configuratie/UserConfig.h en SystemConfig.h.
// MCP23017 GPA0 -> ISD1820 REC; GPA1 -> ISD1820 P-E / PLAYE (beide actief HIGH).
// Rechtstreeks aansluiten: de Relais-klasse stuurt GPIO-signalen, er zijn GEEN fysieke relais nodig.
// MCP23017 GND -> ISD1820 GND; sluit de luidspreker aan op SP+ en SP- van de ISD1820.
// Controleer de voeding en ingangsniveaus van de echte module; gebruik bij voorkeur 3,3 V
// voor de ISD1820 en MCP23017, en passende I2C-niveauomzetting bij een 5V-controller/bus.
// Geen 5V-signaal op een ISD1820-ingang zetten zolang de modulecompatibiliteit niet vaststaat.
// MCP23017 adres, RESET, INTA en INTB: bestaande ConfigMCP23017.h en werkelijke bedrading.
// Deze .ino neemt meteen na initialisatie 5 seconden op en speelt daarna automatisch af.
// RESULTAAT: OK bewijst uitsluitend de componentaanmaak, NIET de hoorbare opname of weergave.
// Extender-pinnen worden gekozen uit de parent van het Sturing-object; MIXED is hiervoor niet nodig.
// I2C-aansluiting: SDA/SCL en GND op dezelfde bus; modulevoeding volgens het werkelijke Extender-bord.
// UNO R4 Minima/WiFi en UNO Q: SDA/SCL op de gelijknamige boardaansluitingen.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22.
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; ESP32-S3 DEV: SDA=GPIO8, SCL=GPIO9.
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21.
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_RELAIS_AANTAL < 2
  #error Zet STURING_RELAIS_AANTAL op minstens 2 voor deze test (REC en PLAYE).
#endif
#if EXTENDER_MCP23017_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif

ExtenderMCP23017* extenderMCP23017 = nullptr;
Relais* opnemen = nullptr;     // REC: actief hoog
Relais* afspelen = nullptr;    // PLAYE: actief hoog, puls
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_ISD1820"));
  extenderMCP23017 = GedeeldeBusNewComponent<ExtenderMCP23017>(&Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23017, I2C_ADDRESS_EXTENDER_MCP23017_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_MCP23017_1_INTA, EXTENDER_MCP23017_1_INTB, EXTENDER_MCP23017_1_RESET);
  if (extenderMCP23017 != nullptr) opnemen = GedeeldeBusNewComponent<Relais>(extenderMCP23017, 0, HIGH);
  if (opnemen != nullptr) afspelen = GedeeldeBusNewComponent<Relais>(extenderMCP23017, 1, HIGH);
  bool gelukt = opnemen != nullptr && afspelen != nullptr;
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
  if (!gelukt) return;
  GA_SERIAL.println(F("Opnemen: 5 seconden"));
  opnemen->setRelayStatusOnOff(true);
  delay(5000);
  opnemen->setRelayStatusOnOff(false);
  GA_SERIAL.println(F("Afspelen"));
  afspelen->setRelayStatusOnOff(true);
  delay(100);
  afspelen->setRelayStatusOnOff(false);
}
void loop() {}
