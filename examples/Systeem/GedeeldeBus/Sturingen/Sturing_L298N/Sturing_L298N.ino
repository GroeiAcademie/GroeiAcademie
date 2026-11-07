// ============================================================================
// Sturing: L298N
// ============================================================================
// Compileer- en levenscyclustest voor L298N. Geen automatische beweging of schakelactie.
// Pinnen zijn afgeleid uit de bestaande constructoren en de boardmapping in src/Configuratie/UserConfig.h en SystemConfig.h.
// PCA9685 kanaal 3/4/5 -> L298N IN1/IN2/ENABLE; motorvoeding extern, GND gemeenschappelijk.
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
// PCA9685 VCC is de logica-/I2C-voeding; V+ is een APARTE servo-/lastvoeding.
// PCA9685 OE moet een gedefinieerd niveau hebben; controleer de concrete breakout.
// Actieve Extenderconfig: src/Configuratie/Extenders/ConfigPCA9685.h
//   EXTENDER_PCA9685_AANTAL >= 1, I2C_ADDRESS_EXTENDER_PCA9685_1 = fysiek adres.
// PCA9685 PWM-uitgangen zijn uitsluitend LOGICA-signalen voor de H-brug.
// H-brug VCC/VSS-logica en afzonderlijke motorvoeding volgens eigen driver/module.
// H-brug GND, PCA9685 GND, Arduino GND en motorvoeding (-) verbinden.
// DC-motor aan twee H-brug-uitgangen, NOOIT aan de PCA9685 of een GPIO.
// Controleer dat PCA9685-signaal-HIGH de driver-ingangen betrouwbaar activeert.
// Test met vrij draaiende motor en geschikte stroombegrenzing.
// PCA9685: kanaal 3 -> IN1, 4 -> IN2, 5 -> ENA (enable-jumper verwijderen)
// L298N MODULE: PCA-EN -> ENA, PCA-IN1 -> IN1, PCA-IN2 -> IN2;
//   DC-motor -> OUT1/OUT2, externe motorvoeding -> VS/12V-klem en GND.
//   ENA-jumper verwijderen om PCA9685-PWM op ENA toe te laten;
//   5V-regelaarjumper/logicavoeding uitsluitend volgens de concrete module.
// Actieve configs: ConfigL298N.h (STURING_L298N_AANTAL >= 1)
//   en ConfigPCA9685.h (EXTENDER_PCA9685_AANTAL >= 1).
// Deze .ino compileert/maakt componenten aan; motor draait niet automatisch.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_L298N_AANTAL == 0
  #error Zet STURING_L298N_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_PCA9685_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif

ExtenderPCA9685* extenderPCA9685 = nullptr;
L298N* l298n = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_L298N"));
  extenderPCA9685 = GedeeldeBusNewComponent<ExtenderPCA9685>(&Native, GedeeldeBusComponent::PWM_PCA9685, I2C_ADDRESS_EXTENDER_PCA9685_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);
  if (extenderPCA9685 != nullptr) l298n = GedeeldeBusNewComponent<L298N>(extenderPCA9685, 3, 4, 5);
  bool gelukt = l298n != nullptr;
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
