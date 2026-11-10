// ============================================================================
// Sturing: L293D
// ============================================================================
// Compileer- en levenscyclustest voor L293D. Geen automatische beweging of schakelactie.
// Pinnen zijn afgeleid uit de bestaande constructoren en de boardmapping in src/Configuratie/UserConfig.h en SystemConfig.h.
// PCF8574 P0/P1 -> L293D IN1/IN2; het bord moet ENABLE intern actief houden.
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
// PCF8574 VCC is de logica-/I2C-voeding; motorvoeding blijft apart.
// PCF8574-uitgangen zijn quasi-bidirectioneel; controleer de elektrische HIGH-marge met de motorbrug.
// Actieve Extenderconfig: src/Configuratie/Extenders/ConfigPCF8574.h
//   EXTENDER_PCF8574_AANTAL >= 1, I2C_ADDRESS_EXTENDER_PCF8574_1 = fysiek adres.
// PCF8574-uitgangen zijn uitsluitend LOGICA-signalen voor de H-brug, geen motorvoeding.
// H-brug VCC/VSS-logica en afzonderlijke motorvoeding volgens eigen driver/module.
// H-brug GND, PCF8574 GND, Arduino GND en motorvoeding (-) verbinden.
// DC-motor aan twee H-brug-uitgangen, NOOIT aan de PCF8574 of een GPIO.
// Controleer dat PCF8574-signaal-HIGH de driver-ingangen betrouwbaar activeert.
// Test met vrij draaiende motor en geschikte stroombegrenzing.
// PCF8574: P0 -> IN1, P1 -> IN2. ENABLE (1,2EN) moet op de module actief zijn.
// L293D losse DIP16: ENABLE pin 1 extern HIGH; IN1 -> pin 2; IN2 -> pin 7;
//   motor -> OUT1 pin 3 en OUT2 pin 6; VCC1 pin 16, VCC2 pin 8;
//   GND -> pinnen 4,5,12,13. Bij shield/module pinlabels controleren.
// Actieve configs: ConfigL293D.h (STURING_L293D_AANTAL >= 1)
//   en ConfigPCF8574.h (EXTENDER_PCF8574_AANTAL >= 1).
// Deze .ino compileert/maakt componenten aan; motor draait niet automatisch.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_L293D_AANTAL == 0
  #error Zet STURING_L293D_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_PCF8574_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif

ExtenderPCF8574* extenderPCF8574 = nullptr;
L293D* l293d = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sturing_L293D"));
  extenderPCF8574 = GedeeldeBusNewComponent<ExtenderPCF8574>(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574, I2C_ADDRESS_EXTENDER_PCF8574_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8574_1_INT);
  if (extenderPCF8574 != nullptr) l293d = GedeeldeBusNewComponent<L293D>(extenderPCF8574, 0, 1);
  bool gelukt = l293d != nullptr;
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
