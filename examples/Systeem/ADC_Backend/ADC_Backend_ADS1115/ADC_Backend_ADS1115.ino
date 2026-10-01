// ============================================================================
// ADC Backend — ADS1115
// ============================================================================
// Valideert de ADS1115-route via de v2.0.0 GedeeldeBus-structuur.
// ADC_ADS1115 gebruikt dezelfde technische codebasis als ExtenderADS1115.
// De sensorlaag wordt in deze test nog niet gebruikt.
// ============================================================================

#include <Systeem/GedeeldeBus/GedeeldeBus.h>
#include <Configuratie/SystemConfig.h>

#if ADC_BACKEND != ADC_BACKEND_ADS1115
  #error Deze validatie vereist ADC_BACKEND_ADS1115.
#endif

#define STIMULUS_AANTAL_KANALEN 4

const int sensorPin[STIMULUS_AANTAL_KANALEN] = {
  ADC_PIN_SENSOR_1,
  ADC_PIN_SENSOR_2,
  ADC_PIN_SENSOR_3,
  ADC_PIN_SENSOR_4
};

bool ads1115Aanwezig = false;
bool metingAfgerond = false;

struct KanaalStats {
  long n = 0;
  double som = 0;
  double somKwadraat = 0;
  int minWaarde = 32767;
  int maxWaarde = -32768;
};

KanaalStats stats[STIMULUS_AANTAL_KANALEN];

void voegMetingToe(uint8_t kanaal, int waarde) {
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
    GA_SERIAL.print(F("Kanaal ")); GA_SERIAL.print(k);
    GA_SERIAL.print(F(": n=")); GA_SERIAL.print(s.n);
    GA_SERIAL.print(F(" gemiddelde=")); GA_SERIAL.print(gemiddelde, 2);
    GA_SERIAL.print(F(" stdev=")); GA_SERIAL.print(stdev, 3);
    GA_SERIAL.print(F(" min=")); GA_SERIAL.print(s.minWaarde);
    GA_SERIAL.print(F(" max=")); GA_SERIAL.println(s.maxWaarde);
  }
}

const unsigned long VALIDATIE_DUUR_MS = 10000UL;
const unsigned long SAMPLE_INTERVAL_MS = 20UL;
unsigned long tStart = 0;
unsigned long laatsteSample = 0;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  if (!ADC_ADS1115.aanmelden() || !ADC_ADS1115.controleren() || !ADC_ADS1115.inpluggen() || !ADC_ADS1115.activeren()) {
    GA_SERIAL.println(F("ADC_ADS1115 kon niet worden ingeplugd."));
    while (true) { ; }
  }

  ads1115Aanwezig = ADC_ADS1115.ingeplugd;
  ADC_ADS1115.setGain(GAIN_TWOTHIRDS);
  GA_SERIAL.println(F("=== Validatie: ADC_ADS1115 ingeplugd ==="));
  tStart = millis();
}

void loop() {
  if (metingAfgerond || !ads1115Aanwezig) return;

  unsigned long nu = millis();

  if (nu - laatsteSample >= SAMPLE_INTERVAL_MS) {
    laatsteSample = nu;
    for (uint8_t k = 0; k < STIMULUS_AANTAL_KANALEN; k++) {
      voegMetingToe(k, ADC_ADS1115.readADC_SingleEnded(sensorPin[k]));
    }
  }

  if (nu - tStart >= VALIDATIE_DUUR_MS) {
    GA_SERIAL.println(F("--- Resultaat ---"));
    printStats();
    GA_SERIAL.println(F("Meting voltooid."));
    metingAfgerond = true;
  }
}
