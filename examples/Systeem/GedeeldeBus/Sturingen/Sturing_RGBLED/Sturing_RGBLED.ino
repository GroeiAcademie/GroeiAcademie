// ============================================================================
// Sturing: RGBLED
// ============================================================================
// Compileer- en levenscyclustest voor RGBLED. Geen automatische beweging of schakelactie.
// Pinnen zijn afgeleid uit de bestaande constructoren en de boardmapping in src/Configuratie/UserConfig.h en SystemConfig.h.
// PCA9685 kanaal 0/1/2 -> RGBLED R/G/B; stroombegrenzing en gemeenschappelijke anode/kathode volgens module.
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
// PCA9685 kanaal 0 -> R, kanaal 1 -> G, kanaal 2 -> B.
// Per afzonderlijke LED-kleur een passende SERIEWEERSTAND plaatsen.
// De bestaande RGBLED-constructor gebruikt commonAnode=false (standaard):
//   gemeenschappelijke kathode -> GND; geen gemeenschappelijke anode aansluiten
//   zonder de polariteit en de bestaande constructor te herzien.
// Controleer met R/G/B-commando of de overeenkomstige LED-kleur oplicht.
// Actieve configuratie: src/Configuratie/Sturingen/ConfigRGBLED.h,
//   STURING_RGBLED_AANTAL >= 1.
// Deze .ino controleert alleen componentaanmaak; geen kleurtest.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_RGBLED_AANTAL == 0
  #error Zet STURING_RGBLED_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_PCA9685_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif

ExtenderPCA9685* extenderPCA9685 = nullptr;
RGBLED* rgbled = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sturing_RGBLED"));
  extenderPCA9685 = GedeeldeBusNewComponent<ExtenderPCA9685>(&Native, GedeeldeBusComponent::PWM_PCA9685, I2C_ADDRESS_EXTENDER_PCA9685_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);
  if (extenderPCA9685 != nullptr) rgbled = GedeeldeBusNewComponent<RGBLED>(extenderPCA9685, 0, 1, 2);
  bool gelukt = rgbled != nullptr;
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
