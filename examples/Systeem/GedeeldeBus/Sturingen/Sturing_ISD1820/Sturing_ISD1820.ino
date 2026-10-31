// ISD1820: geluidsfragment (ongeveer 10 s) opnemen en afspelen via twee keer Relais op een PCF8574.
// P0 = REC (hoog zolang er opgenomen wordt), P1 = PLAYE (korte puls speelt het fragment volledig af).
// De ISD1820 neemt op met zijn eigen microfoon; er komt geen audiodata naar de Arduino.
// Relais vereist GEDEELDE_BUS_MIXED_PINS 1 (in UserConfig.h).
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

ExtenderPCF8574 extender(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574, I2C_ADDRESS_EXTENDER_PCF8574_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8574_1_INT);
Relais opnemen(&extender, 0, HIGH);     // REC: actief hoog
Relais afspelen(&extender, 1, HIGH);    // PLAYE: actief hoog, puls
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren()
    && opnemen.aanmelden() && opnemen.controleren() && opnemen.inpluggen() && opnemen.activeren()
    && afspelen.aanmelden() && afspelen.controleren() && afspelen.inpluggen() && afspelen.activeren();
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
  if (!gelukt) return;
  GA_SERIAL.println(F("Opnemen: 5 seconden"));
  opnemen.setRelayStatusOnOff(true);
  delay(5000);
  opnemen.setRelayStatusOnOff(false);
  GA_SERIAL.println(F("Afspelen"));
  afspelen.setRelayStatusOnOff(true);
  delay(100);
  afspelen.setRelayStatusOnOff(false);
}
void loop() {}
