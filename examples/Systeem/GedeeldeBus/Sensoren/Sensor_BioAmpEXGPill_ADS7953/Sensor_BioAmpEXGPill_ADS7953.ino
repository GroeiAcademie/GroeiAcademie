// Sensor: BioAmpEXGPill via ADS7953.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// Analoge uitgang naar kanaal 0; bij een CD74HC4067 loopt Z via de geconfigureerde native ADC-ingang.

#include <GroeiAcademie.h>

#if EXTENDER_ADS7953_VOEDING_MILLIVOLT == 0
  #error Zet EXTENDER_ADS7953_VOEDING_MILLIVOLT op de werkelijke +VA (2700..5250 mV): het bereik van deze sensor vraagt 2xVREF.
#endif

#if (SENSOR_BIOAMPEXGPILL_ADC_BEREIK_MILLIVOLT > EXTENDER_ADS7953_REFERENTIE_MILLIVOLT && (SENSOR_BIOAMPEXGPILL_ADC_BEREIK_MILLIVOLT > 2 * EXTENDER_ADS7953_REFERENTIE_MILLIVOLT || EXTENDER_ADS7953_VOEDING_MILLIVOLT < 2 * EXTENDER_ADS7953_REFERENTIE_MILLIVOLT)) || \
    (EXTENDER_ADS7953_VOEDING_MILLIVOLT != 0 && EXTENDER_ADS7953_VOEDING_MILLIVOLT < EXTENDER_ADS7953_REFERENTIE_MILLIVOLT)
  #error SENSOR_BIOAMPEXGPILL_ADC_BEREIK_MILLIVOLT is groter dan wat de ADS7953 meet: het dubbele bereik (2 x EXTENDER_ADS7953_REFERENTIE_MILLIVOLT) vraagt EXTENDER_ADS7953_VOEDING_MILLIVOLT van minstens 2 x de referentie; stel beide in volgens je hardware, of verlaag het bereik tot de werkelijke uitgangsspanning van de sensor.
#endif
#if SENSOR_BIOAMPEXGPILL_AANTAL != 1
  #error Zet SENSOR_BIOAMPEXGPILL_AANTAL op 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_ADS7953_AANTAL == 0
  #error Zet EXTENDER_ADS7953_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_ADS7953_AANTAL == 1
ExtenderADS7953 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7953,
  EXTENDER_ADS7953_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI
);
#else
ExtenderADS7953 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7953,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

BioAmpEXGPill* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_BioAmpEXGPill_ADS7953"));
  GA_SERIAL.println(F("MEETTEST: Controleer signaal via de geconfigureerde ECG/EMG/EOG/EEG-meetmethode; geen medische diagnose."));
  sensor = GedeeldeBusNewComponent<BioAmpEXGPill>(&extender, GedeeldeBusComponent::BioAmpEXGPill, static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH0), HardwareResourceToegang::GEDEELD);
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
