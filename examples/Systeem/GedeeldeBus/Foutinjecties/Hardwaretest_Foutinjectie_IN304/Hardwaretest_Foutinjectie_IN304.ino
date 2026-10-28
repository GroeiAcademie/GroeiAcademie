// GROEI ACADEMIE FrameWork v2.0.0 RC1
// Foutinjectie IN304: HX1838-pin niet bruikbaar voor het interruptmechanisme
// Verwachte build: normaal of DEBUG, met HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE 1
// Opstelling: HX1838 op een pin zonder attachInterrupt-ondersteuning
// Bewezen = de echte FrameWork-melding "_CRITICAL_IN304" verschijnt. Geen melding = NIET BEWEZEN.
// Aanname: alleen op boards waar USE_ATTACH_INTERRUPT en NOT_AN_INTERRUPT bestaan (TODO in Input.cpp).
// Bediening: Serial Monitor op SERIAL_BAUDRATE. toets 1 = uitleg, toets 2 = test uitvoeren.

#include <GroeiAcademie.h>

#if !((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
  #error IN304 vereist INPUT_TYPE_HX1838 in INPUT_KANAAL_CONFIG (UserConfig.h).
#endif
#if !HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE
  #error IN304 vereist HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE 1 in UserConfig.h.
#endif

const char* const VERWACHTE_CODE = _CRITICAL_IN304;

// Resultaatsoorten: AUTOMATISCH BEVESTIGD (de sketch vond de exacte code), HANDMATIG CONTROLEREN (lees de melding zelf),
// NIET AANGETOOND (de verwachte toestand werd niet bereikt). De zwakste uitslag bepaalt de eindstatus.
uint8_t uitslag = 0;   // 0 = nog geen, 1 = AUTOMATISCH BEVESTIGD, 2 = HANDMATIG CONTROLEREN, 3 = NIET AANGETOOND
void ZetUitslag(uint8_t nieuw) { if (nieuw > uitslag) uitslag = nieuw; }
void ToonTestKlaar() {
  GA_SERIAL.print(F("--- TEST KLAAR: "));
  if (uitslag == 1) GA_SERIAL.print(F("AUTOMATISCH BEVESTIGD"));
  else if (uitslag == 2) GA_SERIAL.print(F("HANDMATIG CONTROLEREN"));
  else GA_SERIAL.print(F("NIET AANGETOOND"));
  GA_SERIAL.println(F(". Druk reset om te herhalen. ---"));
}
void MeldAutomatisch(const char* code) {
  ZetUitslag(1);
  GA_SERIAL.print(F("RESULTAAT: AUTOMATISCH BEVESTIGD: "));
  GA_SERIAL.println(code);
}
void MeldHandmatig(const char* code) {
  ZetUitslag(2);
  GA_SERIAL.print(F("RESULTAAT: HANDMATIG CONTROLEREN: zoek "));
  GA_SERIAL.print(code);
  GA_SERIAL.println(F(" hierboven of op het scherm."));
}
void MeldNietAangetoond(const __FlashStringHelper* reden) {
  ZetUitslag(3);
  GA_SERIAL.print(F("RESULTAAT: NIET AANGETOOND: "));
  GA_SERIAL.println(reden);
}
void MeldCodeVergelijking(const char* gevonden) {
  if (gevonden != nullptr && strcmp(gevonden, VERWACHTE_CODE) == 0) { MeldAutomatisch(VERWACHTE_CODE); return; }
  GA_SERIAL.print(F("Opgeslagen code: "));
  GA_SERIAL.println(gevonden != nullptr ? gevonden : "geen");
  MeldNietAangetoond(F("de opgeslagen code is niet de verwachte code."));
}

char LeesToets() {
  if (!GA_SERIAL.available()) return 0;
  const char toets = GA_SERIAL.read();
  while (GA_SERIAL.available()) GA_SERIAL.read();
  return toets;
}

void ToonUitleg() {
  GA_SERIAL.println(F("=== FOUTINJECTIE IN304: HX1838-pin niet bruikbaar voor het interruptmechanisme ==="));
#ifdef DEBUG
  GA_SERIAL.println(F("Build: DEBUG aan."));
#else
  GA_SERIAL.println(F("Build: DEBUG uit."));
#endif
  GA_SERIAL.println(F("Verwachte build: normaal of DEBUG, met HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE 1"));
  GA_SERIAL.println(F("Opstelling: HX1838 op een pin zonder attachInterrupt-ondersteuning"));
  GA_SERIAL.println(F("1. Configuratietest, geen fysieke fasen: zet HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE op 1 en kies een HX1838_ONTVANGER_PIN zonder interrupt."));
  GA_SERIAL.println(F("2. Druk 2: Input wordt aangemaakt."));
  GA_SERIAL.println(F("Aanname: alleen op boards waar USE_ATTACH_INTERRUPT en NOT_AN_INTERRUPT bestaan (TODO in Input.cpp)."));
  GA_SERIAL.print(F("Verwacht: ")); GA_SERIAL.println(VERWACHTE_CODE);
  GA_SERIAL.println(F("Toets 1 = uitleg, Toets 2 = test uitvoeren."));
}

bool testUitgevoerd = false;

void VoerTestUit() {
  if (testUitgevoerd) { GA_SERIAL.println(F("Test al uitgevoerd: druk reset om te herhalen (RESET NODIG).")); return; }
  testUitgevoerd = true;
  GA_SERIAL.println(F("--- TEST START ---"));
  Input = GedeeldeBusNewComponent<struct Input>();
  GA_SERIAL.println(Input != nullptr ? F("Input werkt (met een ander kanaal).") : F("Input heeft geen werkend kanaal."));
  MeldHandmatig(VERWACHTE_CODE);
  ToonTestKlaar();
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  Screen = GedeeldeBusNewComponent<struct Screen>();
  if (Screen == nullptr) GA_SERIAL.println(F("Let op: Screen start niet; de melding verschijnt dan niet."));
  ToonUitleg();
}

void loop() {
  const char toets = LeesToets();
  if (toets == '1') ToonUitleg();
  else if (toets == '2') VoerTestUit();
}
