// ============================================================================
// Stimulus — transparante ADC-laag + validatiesketch (ADS1115)
// ============================================================================
// LET OP:
// Dit is een zelfstandig hardware-validatiescript.
// Het gebruikt bewust niet de volledige Stimulus-librarylogica, en hangt
// daarom ook NIET af van UserConfig.h/SystemConfig.h — alles wat dit script
// nodig heeft, staat hieronder zelf gedefinieerd. Zo blijft dit script altijd
// werken, ongeacht hoe je UserConfig.h voor je eigen project is ingesteld.
//
// Voor gewone gebruikers:
// Gebruik dit script alleen om de hardware te valideren.
// Gebruik de normale Stimulus-voorbeelden om met de library zelf te werken.
//
// Doel: de ADS1115-route elektrisch en meetkundig testen zonder extra lagen,
// door dezelfde 4 FSR402/RFP602-lijnen uit te lezen via één vaste backend,
// zodat Arduino-ADC en ADS1115 afzonderlijk getest en vergeleken kunnen worden.
//
// Fysieke validatie:
// 1. Plaats het ADS1115-bordje op H5.
// 2. Kies het gewenste ADS1115-adres via SW1.
// 3. Standaard voor Stimulus: ADDR naar GND, adres 0x48.
//
// Vereist: Adafruit ADS1X15-library.
// ============================================================================

#define ADC_BACKEND_NATIVE  0
#define ADC_BACKEND_ADS1115 1
#define ADC_BACKEND ADC_BACKEND_ADS1115

#define ADS1115_I2C_ADDRESS 0x48
#define PIN_SENSOR_1 0
#define PIN_SENSOR_2 1
#define PIN_SENSOR_3 2
#define PIN_SENSOR_4 3
#define STIMULUS_AANTAL_KANALEN 4

#include <Wire.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;

const int sensorPin[STIMULUS_AANTAL_KANALEN] = { PIN_SENSOR_1, PIN_SENSOR_2, PIN_SENSOR_3, PIN_SENSOR_4 };
bool ads1115Aanwezig = false;
bool metingAfgerond = false;

#ifndef PRINTTOSCREEN_BESTAAT_AL
void PrintToScreen(const char* regel1, const char* regel2) {
#ifdef DEBUG
  Serial.print(F("[LCD] ")); Serial.print(regel1); Serial.print(F(" / ")); Serial.println(regel2);
#endif
}
#endif

void InitialiseerADS1115Validatie() {
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  if (!ads.begin(ADS1115_I2C_ADDRESS)) {
#ifdef DEBUG
    Serial.println(F("ADS1115 niet gevonden"));
#endif
    PrintToScreen("ADS1115", "niet gevonden");
    ads1115Aanwezig = false;
    return;
  }
  ads.setGain(GAIN_TWOTHIRDS);
  ads1115Aanwezig = true;
#else
  ads1115Aanwezig = true;
#endif
}

int RawAnalogReadValidatie(int sensorPin) {
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  if (!ads1115Aanwezig) return 0;
  return ads.readADC_SingleEnded(sensorPin);
#else
  return analogRead(sensorPin);
#endif
}

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
    Serial.print(F("Kanaal ")); Serial.print(k);
    Serial.print(F(": n=")); Serial.print(s.n);
    Serial.print(F(" gemiddelde=")); Serial.print(gemiddelde, 2);
    Serial.print(F(" stdev=")); Serial.print(stdev, 3);
    Serial.print(F(" min=")); Serial.print(s.minWaarde);
    Serial.print(F(" max=")); Serial.println(s.maxWaarde);
  }
}

const unsigned long VALIDATIE_DUUR_MS = 10000UL;
const unsigned long SAMPLE_INTERVAL_MS = 20UL;
unsigned long tStart = 0;
unsigned long laatsteSample = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  InitialiseerADS1115Validatie();

#if ADC_BACKEND == ADC_BACKEND_ADS1115
  Serial.println(F("=== Validatie: backend = ADS1115 ==="));
#else
  Serial.println(F("=== Validatie: backend = Arduino-ADC ==="));
#endif

  Serial.println(F("Controleer dat de fysieke connectorkeuze overeenkomt met deze backend."));
  tStart = millis();
}

void loop() {
  if (metingAfgerond) return;

  unsigned long nu = millis();

  if (nu - laatsteSample >= SAMPLE_INTERVAL_MS) {
    laatsteSample = nu;
    for (uint8_t k = 0; k < STIMULUS_AANTAL_KANALEN; k++) {
      int waarde = RawAnalogReadValidatie(sensorPin[k]);
      voegMetingToe(k, waarde);
    }
  }

  if (nu - tStart >= VALIDATIE_DUUR_MS) {
    Serial.println(F("--- Resultaat ---"));
    printStats();
    Serial.println(F("Meting voltooid."));
    metingAfgerond = true;
  }
}
