// GROEI ACADEMIE FrameWork v2.0.0 RC1
// Foutinjectie GB405: heractiveren mislukt: I2C-busfout of timeout
// Verwachte build: DEBUG
// Opstelling: een PCF8574 op TEST_ADRES_PCF8574, los te koppelen via een schakelaar of jumper in VCC
// Bewezen = de echte FrameWork-melding "<foutniveau>: GB405" verschijnt. Geen melding = NIET BEWEZEN.
// Aanname: een vastgehouden SCL geeft endTransmission() 4 of 5; meet het werkelijke resultaat per board.
// Bediening: Serial Monitor op SERIAL_BAUDRATE. toets 1 = uitleg, toets 2 = fase 1 referentie, toets 3 = fase 2 storing, toets 4 = fase 3 herstel.

#include <GroeiAcademie.h>

#ifndef DEBUG
  #error Deze test verwacht een DEBUG-build: zet #define DEBUG in UserConfig.h.
#endif
#if EXTENDER_PCF8574_AANTAL < 1
  #error Deze test vereist EXTENDER_PCF8574_AANTAL >= 1 in UserConfig.h.
#endif

const char* const VERWACHTE_CODE = _GB405;

// Pas aan: het adres van een aangesloten PCF8574.
const uint8_t TEST_ADRES_PCF8574 = I2C_ADDRESS_EXTENDER_PCF8574_1;
ExtenderPCF8574* extender = nullptr;

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
  GA_SERIAL.println(F("=== FOUTINJECTIE GB405: heractiveren mislukt: I2C-busfout of timeout ==="));
#ifdef DEBUG
  GA_SERIAL.println(F("Build: DEBUG aan."));
#else
  GA_SERIAL.println(F("Build: DEBUG uit."));
#endif
  GA_SERIAL.println(F("Verwachte build: DEBUG"));
  GA_SERIAL.println(F("Opstelling: een PCF8574 op TEST_ADRES_PCF8574, los te koppelen via een schakelaar of jumper in VCC"));
  GA_SERIAL.println(F("1. Druk 2: FASE 1 REFERENTIE: het component start en wordt 10 keer gelezen."));
  GA_SERIAL.println(F("2. FASE 2: Verbind SCL via 1 kOhm met GND en houd dat vast. Druk dan 3; de sketch wacht tot de FrameWork de storing vaststelt (maximaal 30 s)."));
  GA_SERIAL.println(F("3. FASE 3: Haal de verbinding met GND weg. Druk dan 4; de sketch wacht tot het component weer werkt (maximaal 30 s)."));
  GA_SERIAL.println(F("Aanname: een vastgehouden SCL geeft endTransmission() 4 of 5; meet het werkelijke resultaat per board."));
  GA_SERIAL.print(F("Verwacht: ")); GA_SERIAL.println(VERWACHTE_CODE);
  GA_SERIAL.println(F("Toets 1 = uitleg, Toets 2 = fase 1 referentie, Toets 3 = fase 2 storing, Toets 4 = fase 3 herstel."));
}

uint8_t fase = 0;   // 0 = referentie, 1 = wacht op storing, 2 = storing loopt, 3 = wacht op herstel, 4 = herstel loopt, 5 = klaar
unsigned long faseStart = 0;
unsigned long vorigeToegang = 0;
const unsigned long MAXIMALE_WACHTTIJD_FASE_MS = 30000UL;

bool laatsteToegangGelukt = false;

// Eén gewone toegang via de FrameWork-foutladder (D042). laatsteToegangGelukt = deze toegang zelf slaagde.
void EenToegang() {
  laatsteToegangGelukt = false;
  if (extender == nullptr) return;
  if (!extender->ComponentBeschikbaarTijdensRuntime()) return;
  extender->read8();
  extender->ResultaatComponentToegangTijdensRuntimeVerwerken(extender->PCF8574::lastError() == 0);
  laatsteToegangGelukt = extender->PCF8574::lastError() == 0;
}

// Tijdens fase 2 tot 4: elke 200 ms een toegang.
void ToegangTijdensRuntime() {
  if (fase < 2 || fase == 5 || millis() - vorigeToegang < 200) return;
  vorigeToegang = millis();
  EenToegang();
}

void Referentie() {
  if (fase != 0) { GA_SERIAL.println(F("Niet nu: volg de fasen of druk reset.")); return; }
  GA_SERIAL.println(F("--- FASE 1 REFERENTIE ---"));
  extender = GedeeldeBusNewComponent<ExtenderPCF8574>(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574, TEST_ADRES_PCF8574, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourcePin::NONE);
  if (extender == nullptr) {
    MeldNietAangetoond(F("de PCF8574 werkt niet bij de start: controleer de opstelling."));
    GA_SERIAL.println(F("RESET NODIG."));
    ToonTestKlaar();
    fase = 5;
    return;
  }
  for (uint8_t i = 0; i < 10; i++) {
    EenToegang();
    delay(20);
  }
  if (extender->toestandTijdensRuntime.huidigFoutniveau != 0 || extender->toestandTijdensRuntime.aantalOpeenvolgendeFouten != 0) {
    MeldNietAangetoond(F("de referentie heeft fouten: controleer de opstelling."));
    GA_SERIAL.println(F("RESET NODIG."));
    ToonTestKlaar();
    fase = 5;
    return;
  }
  GA_SERIAL.println(F("Referentie OK: 10 toegangen zonder fout."));
  GA_SERIAL.println(F("FASE 2: Verbind SCL via 1 kOhm met GND en houd dat vast. Druk dan 3."));
  fase = 1;
}

void Storing() {
  if (fase != 1) { GA_SERIAL.println(F("Niet nu: volg de fasen of druk reset.")); return; }
  GA_SERIAL.println(F("--- FASE 2 STORING: wachten op de FrameWork ---"));

  faseStart = millis();
  fase = 2;
}

void Herstel() {
  if (fase != 3) { GA_SERIAL.println(F("Niet nu: volg de fasen of druk reset.")); return; }
  GA_SERIAL.println(F("--- FASE 3 HERSTEL: wachten op de FrameWork ---"));

  faseStart = millis();
  fase = 4;
}

void VolgFase() {
  if (fase == 2) {
    if (extender->toestandTijdensRuntime.oorzaakHerstelGemeld) {
      GA_SERIAL.println(F("Storing vastgesteld door de FrameWork."));
      MeldHandmatig(VERWACHTE_CODE);
      GA_SERIAL.println(F("FASE 3: Haal de verbinding met GND weg. Druk dan 4."));
      fase = 3;
    } else if (millis() - faseStart > MAXIMALE_WACHTTIJD_FASE_MS) {
      MeldNietAangetoond(F("geen storing vastgesteld binnen 30 s."));
      GA_SERIAL.println(F("RESET NODIG."));
      ToonTestKlaar();
      fase = 5;
    }
  } else if (fase == 4) {
    if (extender->toestandTijdensRuntime.huidigFoutniveau == 0 && extender->toestandTijdensRuntime.aantalOpeenvolgendeFouten == 0 && laatsteToegangGelukt) {
      GA_SERIAL.println(F("HERSTEL BEVESTIGD: foutladder op 0 en de laatste toegang slaagde zelf."));
      ToonTestKlaar();
      fase = 5;
    } else if (millis() - faseStart > MAXIMALE_WACHTTIJD_FASE_MS) {
      MeldNietAangetoond(F("HERSTEL NIET BEVESTIGD binnen 30 s."));
      GA_SERIAL.println(F("RESET NODIG."));
      ToonTestKlaar();
      fase = 5;
    }
  }
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  ToonUitleg();
}

void loop() {
  ToegangTijdensRuntime();
  VolgFase();
  const char toets = LeesToets();
  if (toets == '1') ToonUitleg();
  else if (toets == '2') Referentie();
  else if (toets == '3') Storing();
  else if (toets == '4') Herstel();
}
