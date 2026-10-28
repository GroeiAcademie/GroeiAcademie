// GROEI ACADEMIE FrameWork v2.0.0 RC1
// Foutinjectie IN302: HX1838-ontvangstlijn blijft permanent LOW
// Verwachte build: normaal of DEBUG
// Opstelling: de HX1838 aangesloten; een draad om OUT met GND te verbinden
// Bewezen = de echte FrameWork-melding "_CRITICAL_IN302" verschijnt. Geen melding = NIET BEWEZEN.
// OPEN: de LOW-controle (twee metingen, 5 ms ertussen) moet nog op de 11 boards gevalideerd worden (TODO in Input.cpp).
// Bediening: Serial Monitor op SERIAL_BAUDRATE. toets 1 = uitleg, toets 2 = fase 1 referentie, toets 3 = fase 2 storing, toets 4 = fase 3 herstel.

#include <GroeiAcademie.h>

#if !((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
  #error IN302 vereist INPUT_TYPE_HX1838 in INPUT_KANAAL_CONFIG (UserConfig.h).
#endif

const char* const VERWACHTE_CODE = _CRITICAL_IN302;

const uint8_t TEST_TYPE = INPUT_TYPE_HX1838;   // alleen dit kanaal

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
  GA_SERIAL.println(F("=== FOUTINJECTIE IN302: HX1838-ontvangstlijn blijft permanent LOW ==="));
#ifdef DEBUG
  GA_SERIAL.println(F("Build: DEBUG aan."));
#else
  GA_SERIAL.println(F("Build: DEBUG uit."));
#endif
  GA_SERIAL.println(F("Verwachte build: normaal of DEBUG"));
  GA_SERIAL.println(F("Opstelling: de HX1838 aangesloten; een draad om OUT met GND te verbinden"));
  GA_SERIAL.println(F("1. Druk 2: FASE 1 REFERENTIE in de juiste opstelling."));
  GA_SERIAL.println(F("2. FASE 2: Verbind de OUT-lijn van de HX1838 met GND. Druk dan 3."));
  GA_SERIAL.println(F("3. FASE 3: Haal de verbinding met GND weg. Druk dan 4."));
  GA_SERIAL.println(F("OPEN: de LOW-controle (twee metingen, 5 ms ertussen) moet nog op de 11 boards gevalideerd worden (TODO in Input.cpp)."));
  GA_SERIAL.print(F("Verwacht: ")); GA_SERIAL.println(VERWACHTE_CODE);
  GA_SERIAL.println(F("Toets 1 = uitleg, Toets 2 = fase 1 referentie, Toets 3 = fase 2 storing, Toets 4 = fase 3 herstel."));
}

uint8_t fase = 0;   // 0 = referentie, 1 = storing, 2 = herstel, 3 = klaar

void MaakAan() {
  Input = GedeeldeBusNewComponent<struct Input>(TEST_TYPE);
}
bool Werkt() {
  // Twee aparte bewijzen: Lifecycle OK (aanmelden tot activeren) en Functionele Input OK (een echte toets via het bedoelde kanaal).
  if (Input == nullptr) { GA_SERIAL.println(F("Lifecycle NIET OK: Input startte niet.")); return false; }
  GA_SERIAL.println(F("Lifecycle OK: aanmelden tot activeren geslaagd."));
  for (uint8_t i = 0; i < 20; i++) { Input->OpvragenHuidigeToetsAanslag(false); delay(5); }   // een eerder ontvangen toets weglezen
  GA_SERIAL.println(F("Druk binnen 10 s een toets op de afstandsbediening (HX1838)."));
  const unsigned long start = millis();
  while (millis() - start < 10000UL) {
    const InputResultaat invoer = Input->OpvragenHuidigeToetsAanslag(false);
    if (invoer.inputKanaal == InputKanaal::HX1838) { GA_SERIAL.println(F("Functionele Input OK: toets ontvangen via de afstandsbediening (HX1838).")); return true; }
    delay(5);
  }
  GA_SERIAL.println(F("Functionele Input NIET OK: geen toets via de afstandsbediening (HX1838) binnen 10 s."));
  return false;
}
void Verwijder() {
  if (Input != nullptr) static_cast<GedeeldeBusNode*>(Input)->afmelden();
  Input = nullptr;
}

void Referentie() {
  if (fase != 0) { GA_SERIAL.println(F("Niet nu: volg de fasen of druk reset.")); return; }
  GA_SERIAL.println(F("--- FASE 1 REFERENTIE ---"));
  MaakAan();
  if (!Werkt()) {
    MeldNietAangetoond(F("de referentie werkt niet: controleer de opstelling."));
    GA_SERIAL.println(F("RESET NODIG."));
    ToonTestKlaar();
    fase = 3;
    return;
  }
  GA_SERIAL.println(F("Referentie OK."));
  Verwijder();
  GA_SERIAL.println(F("FASE 2: Verbind de OUT-lijn van de HX1838 met GND. Druk dan 3."));
  fase = 1;
}

void Storing() {
  if (fase != 1) { GA_SERIAL.println(F("Niet nu: volg de fasen of druk reset.")); return; }
  GA_SERIAL.println(F("--- FASE 2 STORING ---"));
  MaakAan();
  if (Werkt()) {
    MeldNietAangetoond(F("het component werkt ondanks de storing."));
  } else {
    MeldHandmatig(VERWACHTE_CODE);
  }
  Verwijder();
  GA_SERIAL.println(F("FASE 3: Haal de verbinding met GND weg. Druk dan 4."));
  fase = 2;
}

void Herstel() {
  if (fase != 2) { GA_SERIAL.println(F("Niet nu: volg de fasen of druk reset.")); return; }
  GA_SERIAL.println(F("--- FASE 3 HERSTEL ---"));
  MaakAan();
  if (Werkt()) GA_SERIAL.println(F("HERSTEL BEVESTIGD: het component werkt weer."));
  else { MeldNietAangetoond(F("HERSTEL NIET BEVESTIGD: controleer de opstelling.")); GA_SERIAL.println(F("RESET NODIG.")); }
  Verwijder();
  ToonTestKlaar();
  fase = 3;
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
  else if (toets == '2') Referentie();
  else if (toets == '3') Storing();
  else if (toets == '4') Herstel();
}
