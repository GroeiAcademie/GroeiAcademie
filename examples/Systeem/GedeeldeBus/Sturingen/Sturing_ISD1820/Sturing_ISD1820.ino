// ============================================================================
// Sturing: ISD1820
// ============================================================================
// ISD1820: geluidsfragment (ongeveer 10 s) opnemen en afspelen via twee keer Relais op een PCF8574.
// P0 = REC (hoog zolang er opgenomen wordt), P1 = PLAYE (korte puls speelt het fragment volledig af).
// De ISD1820 neemt op met zijn eigen microfoon; er komt geen audiodata naar de Arduino.
// Relais vereist GEDEELDE_BUS_MIXED_PINS 1 (in UserConfig.h).
// Pinnen zijn afgeleid uit de bestaande constructoren en de boardmapping in src/Configuratie/UserConfig.h en SystemConfig.h.
// PCF8574 P0 -> ISD1820 REC via relais, P1 -> PLAYE via tweede relais (beide actief HIGH).
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
#if EXTENDER_PCF8574_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif
#if !GEDEELDE_BUS_MIXED_PINS
  #error GEDEELDE_BUS_MIXED_PINS is vereist voor deze Sturing-test.
#endif

ExtenderPCF8574* extender = nullptr;
Relais* opnemen = nullptr;     // REC: actief hoog
Relais* afspelen = nullptr;    // PLAYE: actief hoog, puls
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_ISD1820"));
  extender = GedeeldeBusNewComponent<ExtenderPCF8574>(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574, I2C_ADDRESS_EXTENDER_PCF8574_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8574_1_INT);
  if (extender != nullptr) opnemen = GedeeldeBusNewComponent<Relais>(extender, 0, HIGH);
  if (opnemen != nullptr) afspelen = GedeeldeBusNewComponent<Relais>(extender, 1, HIGH);
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
