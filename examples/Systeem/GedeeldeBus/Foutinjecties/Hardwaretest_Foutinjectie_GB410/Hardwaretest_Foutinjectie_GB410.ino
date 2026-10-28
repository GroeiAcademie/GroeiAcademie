// GROEI ACADEMIE FrameWork v2.0.0 RC1
// Foutinjectie GB410: meting tijdens runtime hersteld (Sensor-ladder)
// Verwachte build: normaal of DEBUG
// Opstelling: RFP602 op een ADS1115, los te koppelen via een schakelaar of jumper in VCC
// Bewezen = de echte FrameWork-melding "_GB410" verschijnt. Geen melding = NIET BEWEZEN.
// Bediening: Serial Monitor op SERIAL_BAUDRATE. toets 1 = uitleg, toets 2 = fase 1 referentie, toets 3 = fase 2 storing, toets 4 = fase 3 herstel.

#include <GroeiAcademie.h>

#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL in UserConfig.h op minstens 1: RFP602 meet via een ADS1115-Extender.
#endif
#if !defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
  #error Deze test vereist een eigen Sensor-ladder, bv. in UserConfig.h: #define RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1 { {1, 3, 5, 10, 20}, {0UL, 100UL, 500UL, 2000UL, 10000UL}, ScreenData::TYPE_FATAL, ScreenData::TYPE_CRITICAL }
#endif

const char* const VERWACHTE_CODE = _GB410;

ExtenderADS1115 extenderADS1115(
  &Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD
);

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
  GA_SERIAL.println(F("=== FOUTINJECTIE GB410: meting tijdens runtime hersteld (Sensor-ladder) ==="));
#ifdef DEBUG
  GA_SERIAL.println(F("Build: DEBUG aan."));
#else
  GA_SERIAL.println(F("Build: DEBUG uit."));
#endif
  GA_SERIAL.println(F("Verwachte build: normaal of DEBUG"));
  GA_SERIAL.println(F("Opstelling: RFP602 op een ADS1115, los te koppelen via een schakelaar of jumper in VCC"));
  GA_SERIAL.println(F("1. Druk 2: FASE 1 REFERENTIE: het component start en wordt 10 keer gelezen."));
  GA_SERIAL.println(F("2. FASE 2: Koppel de ADS1115 los. Druk dan 3; de sketch wacht tot de FrameWork de storing vaststelt (maximaal 30 s)."));
  GA_SERIAL.println(F("3. FASE 3: Sluit de ADS1115 weer aan; bij herstel meldt de FrameWork GB410. Druk dan 4; de sketch wacht tot het component weer werkt (maximaal 30 s)."));
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
  if (sensorRFP602set1 == nullptr) return;
  sensorRFP602set1->RawAnalogRead(sensorRFP602set1->sensorPin[0]);
  laatsteToegangGelukt = sensorRFP602set1->toestandTijdensRuntime.aantalOpeenvolgendeFouten == 0;
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
  sensorRFP602set1 = GedeeldeBusNewComponent<struct RFP602>(
      &extenderADS1115, GedeeldeBusComponent::RFP602,
      RFP602_ADC_PIN_SENSOR_1_SET_1, RFP602_ADC_PIN_SENSOR_2_SET_1, RFP602_ADC_PIN_SENSOR_3_SET_1, RFP602_ADC_PIN_SENSOR_4_SET_1,
      HardwareResourceToegang::GEDEELD);
  if (sensorRFP602set1 == nullptr) {
    MeldNietAangetoond(F("RFP602 of ADS1115 werkt niet bij de start: controleer de opstelling."));
    GA_SERIAL.println(F("RESET NODIG."));
    ToonTestKlaar();
    fase = 5;
    return;
  }
  for (uint8_t i = 0; i < 10; i++) {
    EenToegang();
    delay(20);
  }
  if (sensorRFP602set1->toestandTijdensRuntime.huidigFoutniveau != 0 || sensorRFP602set1->toestandTijdensRuntime.aantalOpeenvolgendeFouten != 0) {
    MeldNietAangetoond(F("de referentie heeft fouten: controleer de opstelling."));
    GA_SERIAL.println(F("RESET NODIG."));
    ToonTestKlaar();
    fase = 5;
    return;
  }
  GA_SERIAL.println(F("Referentie OK: 10 toegangen zonder fout."));
  GA_SERIAL.println(F("FASE 2: Koppel de ADS1115 los. Druk dan 3."));
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
    if (sensorRFP602set1->toestandTijdensRuntime.huidigFoutniveau != 0) {
      GA_SERIAL.println(F("Storing vastgesteld door de FrameWork (voorwaarde voor het herstel)."));
      GA_SERIAL.println(F("FASE 3: Sluit de ADS1115 weer aan; bij herstel meldt de FrameWork GB410. Druk dan 4."));
      fase = 3;
    } else if (millis() - faseStart > MAXIMALE_WACHTTIJD_FASE_MS) {
      MeldNietAangetoond(F("geen storing vastgesteld binnen 30 s."));
      GA_SERIAL.println(F("RESET NODIG."));
      ToonTestKlaar();
      fase = 5;
    }
  } else if (fase == 4) {
    if (sensorRFP602set1->toestandTijdensRuntime.huidigFoutniveau == 0 && sensorRFP602set1->toestandTijdensRuntime.aantalOpeenvolgendeFouten == 0 && laatsteToegangGelukt) {
      GA_SERIAL.println(F("HERSTEL BEVESTIGD: foutladder op 0 en de laatste toegang slaagde zelf."));
      MeldHandmatig(VERWACHTE_CODE);
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
