// GROEI ACADEMIE FrameWork v2.0.0 RC1
// Foutinjectie GB202: inpluggen() mislukt: bus start niet
// Verwachte build: DEBUG, GEDEELDE_BUS_FOUTINJECTIE 1
// Opstelling: geen extra hardware
// Bewezen = de echte FrameWork-melding "_CRITICAL_GB202" verschijnt. Geen melding = NIET BEWEZEN.
// Bediening: Serial Monitor op SERIAL_BAUDRATE. toets 1 = uitleg, toets 2 = test uitvoeren.

#include <GroeiAcademie.h>

#if !GEDEELDE_BUS_FOUTINJECTIE
  #error Zet GEDEELDE_BUS_FOUTINJECTIE op 1 in UserConfig.h voor deze test (testingang).
#endif
#ifndef DEBUG
  #error Deze test verwacht een DEBUG-build: zet #define DEBUG in UserConfig.h.
#endif
#if EXTENDER_PCF8574_AANTAL < 1
  #error Deze test vereist EXTENDER_PCF8574_AANTAL >= 1 in UserConfig.h.
#endif

const char* const VERWACHTE_CODE = _CRITICAL_GB202;

// Pas aan: een PCF8574-adres (0x20 tot 0x27 of 0x38 tot 0x3F) waarop GEEN chip zit.
const uint8_t TEST_ADRES_ZONDER_CHIP = 0x26;
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
  GA_SERIAL.println(F("=== FOUTINJECTIE GB202: inpluggen() mislukt: bus start niet ==="));
#ifdef DEBUG
  GA_SERIAL.println(F("Build: DEBUG aan."));
#else
  GA_SERIAL.println(F("Build: DEBUG uit."));
#endif
  GA_SERIAL.println(F("Verwachte build: DEBUG, GEDEELDE_BUS_FOUTINJECTIE 1"));
  GA_SERIAL.println(F("Opstelling: geen extra hardware"));
  GA_SERIAL.println(F("1. Druk 2: de testingang laat de I2C-bus voor een PCF8574-Extender niet starten."));
  GA_SERIAL.print(F("Verwacht: ")); GA_SERIAL.println(VERWACHTE_CODE);
  GA_SERIAL.println(F("Toets 1 = uitleg, Toets 2 = test uitvoeren."));
}

bool testUitgevoerd = false;

void VoerTestUit() {
  if (testUitgevoerd) { GA_SERIAL.println(F("Test al uitgevoerd: druk reset om te herhalen (RESET NODIG).")); return; }
  testUitgevoerd = true;
  GA_SERIAL.println(F("--- TEST START ---"));
  foutinjectieComponent = GedeeldeBusComponent::DIGITAL_PINS_PCF8574;
  foutinjectieStap = FoutinjectieStap::BUSSTART;
  extender = GedeeldeBusNewComponent<ExtenderPCF8574>(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574, TEST_ADRES_ZONDER_CHIP, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourcePin::NONE);
  foutinjectieStap = FoutinjectieStap::GEEN;
  if (extender != nullptr) MeldNietAangetoond(F("de PCF8574 werkt ondanks de testingang."));
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
