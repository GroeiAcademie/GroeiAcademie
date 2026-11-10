// ============================================================================
// Sturing: Sturing_Relais_Native_HardwareTest
// ============================================================================
// Begeleide fysieke aan/uit-test van een Relais, met automatische GedeeldeBus-lifecycle.
// Na START: 1 + Enter schakelt AAN, 0 + Enter schakelt UIT.
// STOP + Enter schakelt UIT en beëindigt de test; herstart de Arduino voor een nieuwe reeks.
// Test uitsluitend met een geschikte relaismodule en een geïsoleerd laagspanningscontact.
// Rechtstreeks relais via Native: aansluiting D6 (relais IN), GND gemeenschappelijk.
// Voeding van de relaismodule volgens haar specificaties, niet vanaf een Arduino-GPIO.
// Terugmelding: spanningsvrij NO-contact naar D2 en COM naar GND. Geen netspanning op meetpin D2.
// De Native pin D6 wordt via NativeArduinoPinVan() naar de effectieve boardpin vertaald.
// Concrete boardmapping uit UserConfig.h (relais IN / NO-contactmeting):
// UNO R4 Minima/WiFi en UNO Q: D6 / D2.
// ESP32 D1 UNO R32: GPIO27 (D6) / GPIO26 (D2).
// ESP32-S3 ARDI-32: GPIO9 (D6) / GPIO1 (D2); ESP32-S3 DEV: GPIO3 (D6) / GPIO18 (D2).
// RP2040 Cytron Maker UNO: GPIO6 (D6) / GPIO2 (D2).
// STM32 F401RE Nucleo: PB10 (D6) / PA10 (D2).
// Configuratie: src/Configuratie/Sturingen/ConfigRelais.h met STURING_RELAIS_AANTAL >= 1.
// Dit voorbeeld gebruikt een LOW-actieve relaismodule; geen ander niveau veronderstellen.
// ============================================================================

// ============================================================================
// CONCREET AANSLUITEN EN TESTEN
// ============================================================================
// Gebruik een LOW-actieve relaisMODULE met geschikte stuurspanning en voeding.
// Relaismodule VCC en GND volgens het bord; signaalbron en relais-GND verbinden.
// Alleen een SPANNINGSVRIJ laagspanningscontact testen: COM -> GND, NO -> meetpin.
// Geen netspanning op een Arduino- of Extender-meetpin aansluiten.
// Na START: 1 + Enter = relais AAN; 0 + Enter = UIT; STOP schakelt UIT.
// Verwacht een relaisklik EN overgang van het terugmeldsignaal (NO open/gesloten).
// Actieve configuratie: src/Configuratie/Sturingen/ConfigRelais.h,
//   STURING_RELAIS_AANTAL >= 1.
// Arduino D6 -> relais IN (actief LOW), D2 (INPUT_PULLUP) -> NO;
//   COM -> GND; rust NO open: HIGH, AAN NO gesloten: LOW.
// ============================================================================

#include <GroeiAcademie.h>

#if STURING_RELAIS_AANTAL == 0
  #error Zet STURING_RELAIS_AANTAL op minstens 1 voor deze test.
#endif

Relais* relais = nullptr;

bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;

// INPUT_PULLUP: NO open = HIGH, NO gesloten = LOW.
// Sluit NO op D2 en COM op GND aan. Geen externe spanning op D2.
const uint8_t meetPin = NativeArduinoPinVan(HardwareResourcePin::NP_D2);

void toonFunctieResultaat(bool ok) {
  GA_SERIAL.println(ok ? F("OPDRACHT: OK") : F("OPDRACHT: FOUT"));
}

void toonStartInstructies() {
  GA_SERIAL.println(F("HARDWARETEST: Sturing_Relais_Native_HardwareTest. Typ START na controle van bedrading en voeding."));
  GA_SERIAL.println(F("Typ 1 + Enter voor relais AAN, 0 + Enter voor relais UIT."));
  GA_SERIAL.println(F("STOP schakelt het relais UIT; herstart de Arduino voor een nieuwe test."));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif

  GA_SERIAL.println(F("TEST: Sturing_Relais_Native_HardwareTest"));
  relais = GedeeldeBusNewComponent<Relais>(&Native, static_cast<uint8_t>(HardwareResourcePin::NP_D6), LOW);

  startGelukt = relais != nullptr;
  GA_SERIAL.println(startGelukt ? F("LEVENSCYCLUS: OK") : F("LEVENSCYCLUS: FOUT"));

  pinMode(meetPin, INPUT_PULLUP);

  if (startGelukt) {
    relais->setRelayStatusOnOff(false);
  }

  // Toon de fysieke meetpin en het ruwe digitale rustniveau.
  GA_SERIAL.print(F("MEETPIN (Arduino): "));
  GA_SERIAL.println(meetPin);
  GA_SERIAL.print(F("D2 RAW (rust): "));
  GA_SERIAL.println(digitalRead(meetPin));
  toonStartInstructies();
}

void loop() {
  static bool gesloten = digitalRead(meetPin) == LOW;

  if (gesloten != (digitalRead(meetPin) == LOW)) {
    gesloten = !gesloten;
    GA_SERIAL.print(F("D2 RAW (wijziging): "));
    GA_SERIAL.println(digitalRead(meetPin));
  }

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
  gesloten = digitalRead(meetPin) == LOW;

  GA_SERIAL.print(F("D2 RAW: "));
  GA_SERIAL.println(digitalRead(meetPin));
  GA_SERIAL.print(F("NO-contact: "));
  GA_SERIAL.println(gesloten ? F("GESLOTEN") : F("OPEN"));
  GA_SERIAL.println(gesloten == aan ? F("TERUGMELDING: PASS") : F("TERUGMELDING: FAIL (controleer polariteit/bedrading)"));
  GA_SERIAL.println(F("LED visueel controleren (geen automatische LED-feedback)."));
}
