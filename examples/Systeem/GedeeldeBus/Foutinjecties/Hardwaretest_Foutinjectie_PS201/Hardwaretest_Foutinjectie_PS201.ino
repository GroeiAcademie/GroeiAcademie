// GROEI ACADEMIE FrameWork v2.0.0 RC1
// Foutinjectie PS201: PixelScreen inpluggen() mislukt
// Verwachte build: normaal of DEBUG, GEDEELDE_BUS_FOUTINJECTIE 1
// Opstelling: de gewone schermopstelling
// Bewezen = de echte FrameWork-melding "_CRITICAL_PS201" verschijnt. Geen melding = NIET BEWEZEN.
// Bediening: Serial Monitor op SERIAL_BAUDRATE. toets 1 = uitleg, toets 2 = test uitvoeren.

#include <GroeiAcademie.h>

#if !GEDEELDE_BUS_FOUTINJECTIE
  #error Zet GEDEELDE_BUS_FOUTINJECTIE op 1 in UserConfig.h voor deze test (testingang).
#endif
#if !(SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  #error PS201 vereist SCREEN_TYPE_PIXELS in SCREEN_OUTPUT_CONFIG (UserConfig.h).
#endif
#if (SCREEN_OUTPUT & ~SCREEN_TYPE_PIXELS) == 0
  #error Deze test vereist naast SCREEN_TYPE_PIXELS nog een uitvoer in SCREEN_OUTPUT_CONFIG, om de melding te tonen.
#endif

const char* const VERWACHTE_CODE = _CRITICAL_PS201;

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
  GA_SERIAL.println(F("=== FOUTINJECTIE PS201: PixelScreen inpluggen() mislukt ==="));
#ifdef DEBUG
  GA_SERIAL.println(F("Build: DEBUG aan."));
#else
  GA_SERIAL.println(F("Build: DEBUG uit."));
#endif
  GA_SERIAL.println(F("Verwachte build: normaal of DEBUG, GEDEELDE_BUS_FOUTINJECTIE 1"));
  GA_SERIAL.println(F("Opstelling: de gewone schermopstelling"));
  GA_SERIAL.println(F("1. Druk 2: Screen wordt aangemaakt; de testingang laat PixelScreen falen in inpluggen()."));
  GA_SERIAL.println(F("2. De FrameWork bewaart de code in Screen en toont ze op een andere uitvoer; de sketch vergelijkt de bewaarde code."));
  GA_SERIAL.print(F("Verwacht: ")); GA_SERIAL.println(VERWACHTE_CODE);
  GA_SERIAL.println(F("Toets 1 = uitleg, Toets 2 = test uitvoeren."));
}

bool testUitgevoerd = false;

void VoerTestUit() {
  if (testUitgevoerd) { GA_SERIAL.println(F("Test al uitgevoerd: druk reset om te herhalen (RESET NODIG).")); return; }
  testUitgevoerd = true;
  GA_SERIAL.println(F("--- TEST START ---"));
  foutinjectieComponent = GedeeldeBusComponent::PIXEL_SCREEN;
  foutinjectieStap = FoutinjectieStap::INPLUGGEN;
  Screen = GedeeldeBusNewComponent<struct Screen>();
  foutinjectieStap = FoutinjectieStap::GEEN;
  if (Screen == nullptr) MeldNietAangetoond(F("Screen start met geen enkele uitvoer; de bewaarde code is niet leesbaar."));
  else MeldCodeVergelijking(Screen->Pixel.foutmelding);
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
