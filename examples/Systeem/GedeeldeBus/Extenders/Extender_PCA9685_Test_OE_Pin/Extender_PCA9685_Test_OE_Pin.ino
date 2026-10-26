// ============================================================================
// Extender: PCA9685, test OE-pin bij uitval (T2-10)
// ============================================================================
// De OE-pin (EXTENDER_PCA9685_1_OE_PIN) zet alle 16 uitgangen in één keer uit, ook als de I2C-bus hangt.
// Aansluiten: de OE-pin van het board naar OE van de PCA9685; een LED (met weerstand) op PWM-uitgang 0.
// Werkwijze: laat de sketch lopen (LED brandt half), maak de PCA9685 los (SDA of voeding van de logica)
// tot hij uitgeschakeld is, en sluit hem daarna terug aan.
// Verwacht: OE-pin LOW zolang alles werkt; HIGH zodra de PCA9685 uitgeschakeld is; na herstel weer LOW.
// Let op: de OE-pin wordt niet gereserveerd in de GedeeldeBus; kies een vrije pin.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_PCA9685_AANTAL != 1
  #error Zet EXTENDER_PCA9685_AANTAL op 1 voor deze test.
#endif
#if !defined(EXTENDER_PCA9685_1_OE_PIN)
  #error Zet EXTENDER_PCA9685_1_OE_PIN op een vrije pin van het board voor deze test.
#endif

ExtenderPCA9685 extender(&Native, GedeeldeBusComponent::PWM_PCA9685, I2C_ADDRESS_EXTENDER_PCA9685_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  GA_SERIAL.println(F("=== PCA9685 test OE-pin bij uitval (T2-10) ==="));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extender);
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
  const bool gelukt = extender.WriteExtenderPWM(0, 2048);   // met runtime-herstel (D042)
  GA_SERIAL.print(millis());
  GA_SERIAL.print(F(" ms | PWM 0 gezet: "));
  GA_SERIAL.print(gelukt ? F("ja") : F("nee"));
  GA_SERIAL.print(F(" | PCA9685 actief: "));
  GA_SERIAL.print(extender.actief ? F("ja") : F("nee"));
  GA_SERIAL.print(F(" | opeenvolgende fouten: "));
  GA_SERIAL.print(static_cast<int>(extender.toestandTijdensRuntime.aantalOpeenvolgendeFouten));
  GA_SERIAL.print(F(" | OE-pin: "));
  GA_SERIAL.println(digitalRead(EXTENDER_PCA9685_1_OE_PIN) == HIGH ? F("HIGH (uitgangen uit)") : F("LOW (uitgangen aan)"));
  delay(200);
}
