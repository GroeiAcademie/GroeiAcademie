// ============================================================================
// Sturing: ULN2003
// ============================================================================
// Compileer- en levenscyclustest voor ULN2003. Geen automatische beweging of schakelactie.
// Pinnen zijn afgeleid uit de bestaande constructoren en de boardmapping in src/Configuratie/UserConfig.h en SystemConfig.h.
// PCF8574 P0/P1/P2/P3 -> ULN2003 IN1/IN2/IN3/IN4; externe motorvoeding en GND correct aansluiten.
// Alternatieve GPIO-parents: PCF8575, MCP23017 (I2C) en MCP23S17 (SPI); dit voorbeeld gebruikt PCF8574.
// Extender-pinnen worden gekozen uit de parent van het Sturing-object; MIXED is hiervoor niet nodig.
// I2C-aansluiting: SDA/SCL en GND op dezelfde bus; modulevoeding volgens het werkelijke Extender-bord.
// UNO R4 Minima/WiFi en UNO Q: SDA/SCL op de gelijknamige boardaansluitingen.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22.
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; ESP32-S3 DEV: SDA=GPIO8, SCL=GPIO9.
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21.
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_ULN2003_AANTAL == 0
  #error Zet STURING_ULN2003_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_PCF8574_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif

ExtenderPCF8574* extenderPCF8574 = nullptr;
ULN2003* uln2003 = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_ULN2003"));
  extenderPCF8574 = GedeeldeBusNewComponent<ExtenderPCF8574>(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574, I2C_ADDRESS_EXTENDER_PCF8574_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8574_1_INT);
  if (extenderPCF8574 != nullptr) uln2003 = GedeeldeBusNewComponent<ULN2003>(extenderPCF8574, 0, 1, 2, 3);
  bool gelukt = uln2003 != nullptr;
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
