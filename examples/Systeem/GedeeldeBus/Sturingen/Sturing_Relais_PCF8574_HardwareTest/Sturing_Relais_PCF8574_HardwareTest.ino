// ============================================================================
// Sturing: Sturing_Relais_PCF8574_HardwareTest
// ============================================================================
// Begeleide fysieke AAN/UIT-test voor Relais, met automatische GedeeldeBus-lifecycle.
// Na START schakelt 1 het relais AAN en 0 het relais UIT; STOP schakelt UIT en beëindigt de test.
// NO-contact naar PCF8574 P1, COM naar GND; alleen een spanningsvrij laagspanningscontact.
// De aansluitpinnen volgen de bestaande constructor en boardmapping.
// PCF8574 P0 -> relais IN (actief LOW); modulevoeding en gemeenschappelijke GND volgens de module.
// Terugmelding: spanningsvrij NO -> PCF8574 P1, COM -> GND; geen externe spanning op P1.
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
// Na START: 1 + Enter = relais AAN; 0 + Enter = UIT; STOP schakelt UIT.
// Verwacht een relaisklik EN overgang van het terugmeldsignaal (NO open/gesloten).
// Actieve configuratie: src/Configuratie/Sturingen/ConfigRelais.h,
//   STURING_RELAIS_AANTAL >= 1.
// PCF8574 P0 -> relais IN (actief LOW), P1 -> NO; COM -> GND.
// I2C-extender: ConfigPCF8574.h (EXTENDER_PCF8574_AANTAL >= 1).
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

bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;

// INPUT: NO open = HIGH, NO gesloten = LOW.
// Sluit NO op P1 en COM op GND aan. Geen externe spanning op P1.

void toonFunctieResultaat(bool ok) {
  GA_SERIAL.println(ok ? F("OPDRACHT: OK") : F("OPDRACHT: FOUT"));
}

void toonStartInstructies() {
  GA_SERIAL.println(F("HARDWARETEST: Sturing_Relais_PCF8574_HardwareTest. Typ START na controle van bedrading en voeding."));
  GA_SERIAL.println(F("Typ 1 + Enter voor relais AAN, 0 + Enter voor relais UIT."));
  GA_SERIAL.println(F("STOP schakelt het relais UIT; herstart de Arduino voor een nieuwe test."));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  GA_SERIAL.println(F("TEST: Sturing_Relais_PCF8574_HardwareTest"));
  extenderPCF8574 = GedeeldeBusNewComponent<ExtenderPCF8574>(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574, I2C_ADDRESS_EXTENDER_PCF8574_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8574_1_INT);
  if (extenderPCF8574 != nullptr) relais = GedeeldeBusNewComponent<Relais>(extenderPCF8574, 0, LOW);

  startGelukt = relais != nullptr;
  GA_SERIAL.println(startGelukt ? F("LEVENSCYCLUS: OK") : F("LEVENSCYCLUS: FOUT"));

  if (startGelukt) {
    extenderPCF8574->SetExtenderPinMode(1, INPUT);
    relais->setRelayStatusOnOff(false);

    GA_SERIAL.print(F("P1 RAW (rust): "));
    GA_SERIAL.println(extenderPCF8574->ReadExtenderPin(1));
  }
  toonStartInstructies();
}

void loop() {
  if (!GA_SERIAL.available()) return;

  invoer = GA_SERIAL.readStringUntil('\n');
  invoer.trim();
  invoer.toUpperCase();

  if (invoer == "STOP") {
    if (startGelukt) relais->StopSturing();

    testActief = false;
    testGestopt = true;
    GA_SERIAL.println(F("STOP: relais UIT. Herstart de Arduino om opnieuw te testen."));
    return;
  }

  if (testGestopt) {
    GA_SERIAL.println(F("Test gestopt: herstart de Arduino."));
    return;
  }

  if (!startGelukt) {
    GA_SERIAL.println(F("FOUT: levenscyclus."));
    return;
  }

  if (invoer == "START") {
    if (!testActief) {
      testActief = true;
      GA_SERIAL.println(F("START: bevestigd."));
    }

    return;
  }

  if (!testActief) {
    GA_SERIAL.println(F("Eerst START invoeren."));
    return;
  }

  if (invoer != "1" && invoer != "0") {
    GA_SERIAL.println(F("Gebruik 1 (AAN) of 0 (UIT)."));
    return;
  }

  bool aan = invoer == "1";
  bool gelukt = relais->setRelayStatusOnOff(aan);

  if (!gelukt) {
    toonFunctieResultaat(false);
    return;
  }

  delay(80);
  bool gesloten = extenderPCF8574->ReadExtenderPin(1) == LOW;

  GA_SERIAL.print(F("P1 RAW: "));
  GA_SERIAL.println(gesloten ? LOW : HIGH);
  GA_SERIAL.print(F("NO-contact: "));
  GA_SERIAL.println(gesloten ? F("GESLOTEN") : F("OPEN"));
  GA_SERIAL.println(gesloten == aan ? F("TERUGMELDING: PASS") : F("TERUGMELDING: FAIL (controleer polariteit/bedrading)"));
  GA_SERIAL.println(F("LED visueel controleren (geen automatische LED-feedback)."));
}
