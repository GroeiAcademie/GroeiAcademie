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

#define SERIAL_BAUDRATE 115200 

#if defined(ARDUINO_ESP32S3_DEV)
  #define GA_SERIAL Serial0
#else
  #define GA_SERIAL Serial
#endif

#include <Wire.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;

const int sensorPin[STIMULUS_AANTAL_KANALEN] = { PIN_SENSOR_1, PIN_SENSOR_2, PIN_SENSOR_3, PIN_SENSOR_4 };
bool ads1115Aanwezig = false;
bool metingAfgerond  = false;

#ifndef PRINTTOSCREEN_BESTAAT_AL
void PrintToScreen(const char* regel1, const char* regel2) {
#ifdef DEBUG
  GA_SERIAL.print(F("[LCD] ")); GA_SERIAL.print(regel1); GA_SERIAL.print(F(" / ")); GA_SERIAL.println(regel2);
#endif
}
#endif

void InitialiseerADS1115Validatie() {
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  if (!ads.begin(ADS1115_I2C_ADDRESS)) {
#ifdef DEBUG
    GA_SERIAL.println(F("ADS1115 niet gevonden"));
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
  while (!GA_SERIAL) { ; } // Wacht hier totdat er een seriële verbinding is

  Wire.begin();
  InitialiseerADS1115Validatie();

#if ADC_BACKEND == ADC_BACKEND_ADS1115
  GA_SERIAL.println(F("=== Validatie: backend = ADS1115 ==="));
#else
  GA_SERIAL.println(F("=== Validatie: backend = Arduino-ADC ==="));
#endif

  GA_SERIAL.println(F("Controleer dat de fysieke connectorkeuze overeenkomt met deze backend."));
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
    GA_SERIAL.println(F("--- Resultaat ---"));
    printStats();
    GA_SERIAL.println(F("Meting voltooid."));
    metingAfgerond = true;
  }
}
