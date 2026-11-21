// Sensor: BioAmpEXGPill via CD74HC4067.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// Analoge uitgang naar kanaal 0; bij een CD74HC4067 loopt Z via de geconfigureerde native ADC-ingang.

#include <GroeiAcademie.h>

#if SENSOR_BIOAMPEXGPILL_AANTAL != 1
  #error Zet SENSOR_BIOAMPEXGPILL_AANTAL op 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_CD74HC4067_AANTAL == 0
  #error Zet EXTENDER_CD74HC4067_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_CD74HC4067_AANTAL == 1
ExtenderCD74HC4067 extender(
  &Native,
  GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067,
  EXTENDER_CD74HC4067_1_S0, EXTENDER_CD74HC4067_1_S1, EXTENDER_CD74HC4067_1_S2, EXTENDER_CD74HC4067_1_S3, EXTENDER_CD74HC4067_1_EN, EXTENDER_CD74HC4067_1_Z
);
#else
ExtenderCD74HC4067 extender(
  &Native,
  GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067,
  0
);
#endif

BioAmpEXGPill* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_BioAmpEXGPill_CD74HC4067"));
  GA_SERIAL.println(F("MEETTEST: Controleer signaal via de geconfigureerde ECG/EMG/EOG/EEG-meetmethode; geen medische diagnose."));
  sensor = GedeeldeBusNewComponent<BioAmpEXGPill>(&extender, GedeeldeBusComponent::BioAmpEXGPill, static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y0), HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  float waarde = NAN;
  // De bestaande Sensorklasse voert de geselecteerde filter en piekdetectie uit.
  // Een piek bij elke sample onthouden, zodat een korte piek tussen twee rapporten niet verloren gaat.
  static bool peakDetectedSinceLastReport = false;
  if (sensor->LeesHoofdwaarde(waarde)) {
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_ECG
    { bool piek = false; if (sensor->LeesHartslagPiek(piek) && piek) peakDetectedSinceLastReport = true; }
#elif SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EOG
    { bool knipper = false; if (sensor->LeesKnipperen(knipper) && knipper) peakDetectedSinceLastReport = true; }
#endif
    static unsigned long laatsteRapport = 0;
    if (millis() - laatsteRapport >= 200UL) {
      laatsteRapport = millis();
      GA_SERIAL.print(F("Biosignaal gefilterd: ")); GA_SERIAL.println(waarde);
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EMG
      if (sensor->LeesEMGEnvelop(waarde)) { GA_SERIAL.print(F("EMG-envelop: ")); GA_SERIAL.println(waarde); }
#elif SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_ECG
      GA_SERIAL.print(F("ECG-piek sinds vorig rapport: ")); GA_SERIAL.println(peakDetectedSinceLastReport ? F("JA") : F("NEE"));
      peakDetectedSinceLastReport = false;
#elif SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EOG
      GA_SERIAL.print(F("EOG-knipperpiek sinds vorig rapport: ")); GA_SERIAL.println(peakDetectedSinceLastReport ? F("JA") : F("NEE"));
      peakDetectedSinceLastReport = false;
#endif
    }
  }
  if (sensor->samplefrequentieNietHaalbaar) {
    static unsigned long laatsteRapport = 0;
    if (millis() - laatsteRapport >= 1000UL) {
      laatsteRapport = millis();
      GA_SERIAL.println(F("FOUT: ADC/sampleketen haalt de ingestelde meetfrequentie niet."));
    }
  }
}
