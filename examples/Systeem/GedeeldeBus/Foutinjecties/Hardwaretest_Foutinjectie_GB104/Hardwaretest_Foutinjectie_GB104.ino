// GROEI ACADEMIE FrameWork v2.0.0 RC1
// Foutinjectie GB104: parent exclusief al bezet door een andere child
// Verwachte build: normaal of DEBUG
// Opstelling: geen extra hardware
// Bewezen = de echte FrameWork-melding "_FATAL_GB104" verschijnt. Geen melding = NIET BEWEZEN.
// Bediening: Serial Monitor op SERIAL_BAUDRATE. toets 1 = uitleg, toets 2 = test uitvoeren.

#include <GroeiAcademie.h>

const char* const VERWACHTE_CODE = _FATAL_GB104;

GedeeldeBusNode ouder(&Native, { nullptr, 0, nullptr, 0 }, GedeeldeBusComponent::GC_EXTENDER, HardwareResourceToegang::EXCLUSIEF);
GedeeldeBusNode kindA(&ouder, { nullptr, 0, nullptr, 0 }, GedeeldeBusComponent::GC_SENSOR);
GedeeldeBusNode kindB(&ouder, { nullptr, 0, nullptr, 0 }, GedeeldeBusComponent::GC_SENSOR);

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
  GA_SERIAL.println(F("=== FOUTINJECTIE GB104: parent exclusief al bezet door een andere child ==="));
#ifdef DEBUG
  GA_SERIAL.println(F("Build: DEBUG aan."));
#else
  GA_SERIAL.println(F("Build: DEBUG uit."));
#endif
  GA_SERIAL.println(F("Verwachte build: normaal of DEBUG"));
  GA_SERIAL.println(F("Opstelling: geen extra hardware"));
  GA_SERIAL.println(F("1. Druk 2: een Extender-node met EXCLUSIEF krijgt twee children."));
  GA_SERIAL.print(F("Verwacht: ")); GA_SERIAL.println(VERWACHTE_CODE);
  GA_SERIAL.println(F("Toets 1 = uitleg, Toets 2 = test uitvoeren."));
}

bool testUitgevoerd = false;

void VoerTestUit() {
  if (testUitgevoerd) { GA_SERIAL.println(F("Test al uitgevoerd: druk reset om te herhalen (RESET NODIG).")); return; }
  testUitgevoerd = true;
  GA_SERIAL.println(F("--- TEST START ---"));
  kindA.aanmelden();
  kindA.controleren();
  kindB.aanmelden();
  const bool gecontroleerd = kindB.controleren();
  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&kindB);
  if (gecontroleerd) MeldNietAangetoond(F("de FrameWork aanvaardde de foute configuratie."));
  else MeldHandmatig(VERWACHTE_CODE);
  ToonTestKlaar();
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  ToonUitleg();
}

void loop() {
  const char toets = LeesToets();
  if (toets == '1') ToonUitleg();
  else if (toets == '2') VoerTestUit();
}
