// GROEI ACADEMIE FrameWork v2.0.0 RC1
// Foutinjectie IN101: InputKanalen controleren() mislukt
// Verwachte build: normaal of DEBUG, GEDEELDE_BUS_FOUTINJECTIE 1
// Opstelling: de gewone invoeropstelling
// Bewezen = de echte FrameWork-melding "_FATAL_IN101" verschijnt. Geen melding = NIET BEWEZEN.
// Bediening: Serial Monitor op SERIAL_BAUDRATE. toets 1 = uitleg, toets 2 = test uitvoeren.

#include <GroeiAcademie.h>

#if !GEDEELDE_BUS_FOUTINJECTIE
  #error Zet GEDEELDE_BUS_FOUTINJECTIE op 1 in UserConfig.h voor deze test (testingang).
#endif
#if (INPUT_KANAAL_CONFIG) == INPUT_TYPE_NONE
  #error Deze test vereist een INPUT_KANAAL_CONFIG met INPUT_TYPE_HX1838 en/of INPUT_TYPE_PCF8574.
#endif

const char* const VERWACHTE_CODE = _FATAL_IN101;

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
const GedeeldeBusComponent TEST_KANAAL = GedeeldeBusComponent::INPUT_HX1838;
#else
const GedeeldeBusComponent TEST_KANAAL = GedeeldeBusComponent::INPUT_PCF8574;
#endif

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
  GA_SERIAL.println(F("=== FOUTINJECTIE IN101: InputKanalen controleren() mislukt ==="));
#ifdef DEBUG
  GA_SERIAL.println(F("Build: DEBUG aan."));
#else
  GA_SERIAL.println(F("Build: DEBUG uit."));
#endif
  GA_SERIAL.println(F("Verwachte build: normaal of DEBUG, GEDEELDE_BUS_FOUTINJECTIE 1"));
  GA_SERIAL.println(F("Opstelling: de gewone invoeropstelling"));
  GA_SERIAL.println(F("1. Druk 2: Input wordt aangemaakt; de testingang laat het eerste kanaal (HX1838, anders PCF8574) falen in controleren()."));
  GA_SERIAL.println(F("2. Heeft de configuratie twee kanalen, dan blijft het andere kanaal werken. Input bewaart deze code niet."));
  GA_SERIAL.print(F("Verwacht: ")); GA_SERIAL.println(VERWACHTE_CODE);
  GA_SERIAL.println(F("Toets 1 = uitleg, Toets 2 = test uitvoeren."));
}

bool testUitgevoerd = false;

void VoerTestUit() {
  if (testUitgevoerd) { GA_SERIAL.println(F("Test al uitgevoerd: druk reset om te herhalen (RESET NODIG).")); return; }
  testUitgevoerd = true;
  GA_SERIAL.println(F("--- TEST START ---"));
  foutinjectieComponent = TEST_KANAAL;
  foutinjectieStap = FoutinjectieStap::CONTROLEREN;
  Input = GedeeldeBusNewComponent<struct Input>();
  foutinjectieStap = FoutinjectieStap::GEEN;
  GA_SERIAL.println(Input != nullptr ? F("Input werkt met het andere kanaal.") : F("Input heeft geen werkend kanaal."));
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
