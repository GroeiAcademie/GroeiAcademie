// ============================================================================
// Sturing: Relais
// ============================================================================
// Compileer- en levenscyclustest voor Relais. Geen automatische beweging of schakelactie.
// Pinnen zijn afgeleid uit de bestaande constructoren en de boardmapping in src/Configuratie/UserConfig.h en SystemConfig.h.
// PCF8574 P0 -> relaisbesturing (actief LOW in deze test); geschikte relaisvoeding en gemeenschappelijke GND.
// I2C-aansluiting: SDA/SCL en GND op dezelfde bus; modulevoeding volgens het werkelijke Extender-bord.
// UNO R4 Minima/WiFi en UNO Q: SDA/SCL op de gelijknamige boardaansluitingen.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22.
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; ESP32-S3 DEV: SDA=GPIO8, SCL=GPIO9.
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21.
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8.
// ============================================================================

// ============================================================================
// CONCREET AANSLUITEN EN TESTEN
// ============================================================================
// Arduino SDA -> SDA van de I2C-Extender; Arduino SCL -> SCL; GND -> GND.
// Voed de Extender-logica op het voor board en I2C-niveau geschikte VCC/VDD.
// Verifieer fysiek I2C-adres en optionele INT/RESET-pinnen met de actieve Config*.h.
// Bij 3,3V-logica op een 5V-bus: controleer een passende I2C-niveauomzetter.
// Gebruik een LOW-actieve relaisMODULE met geschikte stuurspanning en voeding.
// Relaismodule VCC en GND volgens het bord; signaalbron en relais-GND verbinden.
// Alleen een SPANNINGSVRIJ laagspanningscontact testen: COM -> GND, NO -> meetpin.
// Geen netspanning op een Arduino- of Extender-meetpin aansluiten.
// De .ino schakelt niet fysiek; START/1/0/STOP zitten in de HardwareTests.
// Verwacht een relaisklik EN overgang van het terugmeldsignaal (NO open/gesloten).
// Actieve configuratie: src/Configuratie/Sturingen/ConfigRelais.h,
//   STURING_RELAIS_AANTAL >= 1.
// PCF8574 P0 -> relais IN; deze .ino voert geen schakeltest uit.
// Actieve Extenderconfig: ConfigPCF8574.h (EXTENDER_PCF8574_AANTAL >= 1).
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_RELAIS_AANTAL == 0
  #error Zet STURING_RELAIS_AANTAL op minstens 1 voor deze test.
#endif

#if EXTENDER_PCF8574_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif

ExtenderPCF8574* extenderPCF8574 = nullptr;
Relais* relais = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sturing_Relais"));
  extenderPCF8574 = GedeeldeBusNewComponent<ExtenderPCF8574>(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574, I2C_ADDRESS_EXTENDER_PCF8574_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8574_1_INT);
  if (extenderPCF8574 != nullptr) relais = GedeeldeBusNewComponent<Relais>(extenderPCF8574, 0, LOW);
  bool gelukt = relais != nullptr;
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {}
