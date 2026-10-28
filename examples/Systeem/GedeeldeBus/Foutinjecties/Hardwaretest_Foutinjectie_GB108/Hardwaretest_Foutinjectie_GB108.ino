// GROEI ACADEMIE FrameWork v2.0.0 RC1
// Foutinjectie GB108: verplichte resource ontbreekt
// Verwachte build: normaal of DEBUG
// Opstelling: geen extra hardware
// Bewezen = de echte FrameWork-melding "_FATAL_GB108" verschijnt. Geen melding = NIET BEWEZEN.
// Bediening: Serial Monitor op SERIAL_BAUDRATE. toets 1 = uitleg, toets 2 = test uitvoeren, toets 3 = PixelScreen zonder DC.

#include <GroeiAcademie.h>

const char* const VERWACHTE_CODE = _FATAL_GB108;

HardwareResourceTypeSPI zonderCS(&Native, GedeeldeBusComponent::GC_SENSOR, HardwareResourcePin::NONE, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI);
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
PixelScreen zonderDC(&Native, GedeeldeBusComponent::PIXEL_SCREEN, HardwareResourcePin::NP_D8, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, HardwareResourcePin::NONE, HardwareResourcePin::NONE);
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
  GA_SERIAL.println(F("=== FOUTINJECTIE GB108: verplichte resource ontbreekt ==="));
#ifdef DEBUG
  GA_SERIAL.println(F("Build: DEBUG aan."));
#else
  GA_SERIAL.println(F("Build: DEBUG uit."));
#endif
  GA_SERIAL.println(F("Verwachte build: normaal of DEBUG"));
  GA_SERIAL.println(F("Opstelling: geen extra hardware"));
  GA_SERIAL.println(F("1. Druk 2: een SPI-node zonder CS-pin wordt gecontroleerd."));
  GA_SERIAL.println(F("2. Met SCREEN_TYPE_PIXELS: druk 3 voor een PixelScreen-node zonder DC-pin."));
  GA_SERIAL.print(F("Verwacht: ")); GA_SERIAL.println(VERWACHTE_CODE);
  GA_SERIAL.println(F("Toets 1 = uitleg, Toets 2 = test uitvoeren, Toets 3 = PixelScreen zonder DC."));
}

bool testDCUitgevoerd = false;
void VoerTestDCUit() {
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  if (testDCUitgevoerd) { GA_SERIAL.println(F("Test al uitgevoerd: druk reset om te herhalen (RESET NODIG).")); return; }
  testDCUitgevoerd = true;
  zonderDC.aanmelden();
  const bool gecontroleerd = zonderDC.controleren();
  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&zonderDC);
  if (gecontroleerd) MeldNietAangetoond(F("de FrameWork aanvaardde de foute configuratie."));
  else MeldHandmatig(VERWACHTE_CODE);
#else
  GA_SERIAL.println(F("Niet beschikbaar: SCREEN_TYPE_PIXELS staat niet in SCREEN_OUTPUT_CONFIG."));
#endif
}

bool testUitgevoerd = false;

void VoerTestUit() {
  if (testUitgevoerd) { GA_SERIAL.println(F("Test al uitgevoerd: druk reset om te herhalen (RESET NODIG).")); return; }
  testUitgevoerd = true;
  GA_SERIAL.println(F("--- TEST START ---"));
  zonderCS.aanmelden();
  const bool gecontroleerd = zonderCS.controleren();
  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&zonderCS);
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
  else if (toets == '3') VoerTestDCUit();
}
