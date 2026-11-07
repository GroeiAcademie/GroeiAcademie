// ============================================================================
// Sturing: MG996R
// ============================================================================
// Compileer- en levenscyclustest voor MG996R. Geen automatische beweging of schakelactie.
// Pinnen zijn afgeleid uit de bestaande constructoren en de boardmapping in src/Configuratie/UserConfig.h en SystemConfig.h.
// PCA9685 kanaal 1 -> MG996R servosignaal; servo-voeding geschikt voor het model, GND gemeenschappelijk.
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
// Externe servovoeding (+) -> PCA9685 V+; voeding (-) -> PCA9685 GND.
// Servostekker: GND (meestal bruin/zwart) -> GND, + (meestal rood) -> V+,
//   signaal (meestal oranje/geel/wit) -> signaalcontact van gekozen kanaal.
// Laat de servo onbelast en vrij bewegen; gebruik Arduino 5V niet als motorvoeding.
// Extender-PWM-frequentie wordt alleen geconfigureerd wanneer
//   EXTENDER_PCA9685_1_PWM_FREQUENTIE in ConfigPCA9685.h is gedefinieerd.
//   Voor een servo-test circa 50 Hz instellen/nameten; geen automatische default aannemen.
// 205 en 410 in de constructor zijn 12-bit-ticks, geen graden of microseconden:
//   controleer de daadwerkelijke pulsbreedten en kalibreer zonder eindstopbelasting.
// Een STOP schakelt PWM uit, niet de externe servovoeding of het houdkoppel.
// PCA9685 signaalkanaal 1 -> MG996R-signaaldraad.
// Actieve config: ConfigMG996R.h (STURING_MG996R_AANTAL >= 1).
// Deze .ino voert geen motorbeweging uit; aanmaak is geen fysieke test.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_MG996R_AANTAL == 0
  #error Zet STURING_MG996R_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_PCA9685_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif

ExtenderPCA9685* extenderPCA9685 = nullptr;
MG996R* mg996r = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_MG996R"));
  extenderPCA9685 = GedeeldeBusNewComponent<ExtenderPCA9685>(&Native, GedeeldeBusComponent::PWM_PCA9685, I2C_ADDRESS_EXTENDER_PCA9685_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);
  if (extenderPCA9685 != nullptr) mg996r = GedeeldeBusNewComponent<MG996R>(extenderPCA9685, 1, 205, 410);
  bool gelukt = mg996r != nullptr;
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
