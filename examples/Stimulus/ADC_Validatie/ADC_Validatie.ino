// ============================================================================
// Stimulus — transparante ADC-laag + validatiesketch (Arduino-ADC vs ADS1115)
// ============================================================================
// Doel: dezelfde 4 FSR402/RFP602-lijnen uitlezen via twee mogelijke backends,
// zonder dat de rest van de Stimulus-code hoeft te weten welke backend actief is.
//
// Gebruik voor validatie (schema 3, solder-jumpers):
//   1. Zet de solder-jumpers van alle 4 lijnen op dezelfde kant (bv. A = Arduino).
//   2. Zet STIMULUS_ADC_BACKEND hieronder op dezelfde kant (ARDUINO).
//   3. Flash, laat 10 seconden lopen, noteer de Serial-output (gemiddelde/stdev/min/max).
//   4. Verzet de jumpers naar de andere kant (B = ADS1115), zet de #define ook om,
//      herflash, herhaal de meting met dezelfde fysieke druk.
//   5. Vergelijk beide resultaten.
//
// Vereist: Adafruit_ADS1X15-library (voor de ADS1115-backend).
// ============================================================================

#include <Wire.h>
#if defined(__has_include)
  #if __has_include(<Adafruit_ADS1X15.h>)
    #include <Adafruit_ADS1X15.h>
  #endif
#endif

// ---- Backend-configuratie ----
#define STIMULUS_ADC_BACKEND_ARDUINO 0
#define STIMULUS_ADC_BACKEND_ADS1115 1

// Wissel deze regel + herflash om van backend te wisselen.
// Denk eraan de solder-jumpers (schema 3) mee te verzetten naar dezelfde kant!
#define STIMULUS_ADC_BACKEND STIMULUS_ADC_BACKEND_ARDUINO

#define STIMULUS_ADS1115_ADDRESS 0x48   // gereserveerd adres voor Stimulus (zie Stimulus_Schema_1_2_3.docx)
#define STIMULUS_AANTAL_KANALEN 4

#define DEBUG 1

#if STIMULUS_ADC_BACKEND == STIMULUS_ADC_BACKEND_ADS1115
Adafruit_ADS1115 ads;
#endif

const uint8_t stimulusPinnen[STIMULUS_AANTAL_KANALEN] = {A0, A1, A2, A3};
bool ads1115Aanwezig = false;

// PrintToLCD wordt verondersteld al te bestaan in de huidige Stimulus-code.
// Stub hier enkel zodat dit bestand op zichzelf compileert bij losse tests.
#ifndef PRINTTOLCD_BESTAAT_AL
void PrintToLCD(const char* regel1, const char* regel2) {
#if DEBUG
  Serial.print(F("[LCD] ")); Serial.print(regel1); Serial.print(F(" / ")); Serial.println(regel2);
#endif
}
#endif

void initStimulusAnalog() {
#if STIMULUS_ADC_BACKEND == STIMULUS_ADC_BACKEND_ADS1115
  if (!ads.begin(STIMULUS_ADS1115_ADDRESS)) {
#if DEBUG
    Serial.println(F("ADS1115 niet gevonden"));
#endif
    PrintToLCD("ADS1115", "niet gevonden");
    ads1115Aanwezig = false;
    return; // geen while(1) -- loop() blijft functioneren, zie eerdere aanvulling
  }
  ads.setGain(GAIN_TWOTHIRDS); // geschikt voor single-ended metingen tot ~5V
  ads1115Aanwezig = true;
#else
  ads1115Aanwezig = true; // niet van toepassing, Arduino-ADC is altijd beschikbaar
#endif
}

int16_t leesStimulusRaw(uint8_t kanaal) {
#if STIMULUS_ADC_BACKEND == STIMULUS_ADC_BACKEND_ADS1115
  if (!ads1115Aanwezig) return -1;
  return ads.readADC_SingleEnded(kanaal);
#else
  return analogRead(stimulusPinnen[kanaal]);
#endif
}

// ---- Validatiestatistieken per kanaal ----
struct KanaalStats {
  long n = 0;
  double som = 0;
  double somKwadraat = 0;
  int16_t minWaarde = 32767;
  int16_t maxWaarde = -32768;
};

KanaalStats stats[STIMULUS_AANTAL_KANALEN];

void voegMetingToe(uint8_t kanaal, int16_t waarde) {
  KanaalStats &s = stats[kanaal];
  s.n++;
  s.som += waarde;
  s.somKwadraat += (double)waarde * waarde;
  if (waarde < s.minWaarde) s.minWaarde = waarde;
  if (waarde > s.maxWaarde) s.maxWaarde = waarde;
}

void printStats() {
  for (uint8_t k = 0; k < STIMULUS_AANTAL_KANALEN; k++) {
    KanaalStats &s = stats[k];
    if (s.n == 0) continue;
    double gemiddelde = s.som / s.n;
    double variantie = (s.somKwadraat / s.n) - (gemiddelde * gemiddelde);
    double stdev = sqrt(variantie > 0 ? variantie : 0.0);
    Serial.print(F("Kanaal ")); Serial.print(k);
    Serial.print(F(": n=")); Serial.print(s.n);
    Serial.print(F(" gemiddelde=")); Serial.print(gemiddelde, 2);
    Serial.print(F(" stdev=")); Serial.print(stdev, 3);
    Serial.print(F(" min=")); Serial.print(s.minWaarde);
    Serial.print(F(" max=")); Serial.println(s.maxWaarde);
  }
}

const unsigned long VALIDATIE_DUUR_MS = 10000UL;  // 10 seconden per meting
const unsigned long SAMPLE_INTERVAL_MS = 20UL;    // 50Hz, ruim voldoende voor trage druksignalen

unsigned long tStart = 0;
unsigned long laatsteSample = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  initStimulusAnalog();

#if STIMULUS_ADC_BACKEND == STIMULUS_ADC_BACKEND_ADS1115
  Serial.println(F("=== Validatie: backend = ADS1115 ==="));
#else
  Serial.println(F("=== Validatie: backend = Arduino-ADC ==="));
#endif
  Serial.println(F("Controleer dat de solder-jumpers (schema 3) overeenkomen met deze backend."));

  tStart = millis();
}

void loop() {
  unsigned long nu = millis();

  if (nu - laatsteSample >= SAMPLE_INTERVAL_MS) {
    laatsteSample = nu;
    for (uint8_t k = 0; k < STIMULUS_AANTAL_KANALEN; k++) {
      int16_t waarde = leesStimulusRaw(k);
      voegMetingToe(k, waarde);
    }
  }

  if (nu - tStart >= VALIDATIE_DUUR_MS) {
    Serial.println(F("--- Resultaat ---"));
    printStats();
    Serial.println(F("Meting voltooid. Verzet jumpers + backend-define voor de andere kant, herflash."));
    while (true) { delay(1000); } // bewuste stop, enkel voor deze validatiesketch
  }
}
