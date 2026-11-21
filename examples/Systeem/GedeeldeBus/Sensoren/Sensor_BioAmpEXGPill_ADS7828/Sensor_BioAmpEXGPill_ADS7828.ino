// Sensor: BioAmpEXGPill via ADS7828.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// Analoge uitgang naar kanaal 0; bij een CD74HC4067 loopt Z via de geconfigureerde native ADC-ingang.

#include <GroeiAcademie.h>

#if SENSOR_BIOAMPEXGPILL_ADC_BEREIK_MILLIVOLT > (EXTENDER_ADS7828_EXTERNE_REFERENTIE ? EXTENDER_ADS7828_REFERENTIE_MILLIVOLT : 2500)
  #error SENSOR_BIOAMPEXGPILL_ADC_BEREIK_MILLIVOLT is groter dan wat de ADS7828 meet (interne referentie 2500 mV): stel EXTENDER_ADS7828_EXTERNE_REFERENTIE en EXTENDER_ADS7828_REFERENTIE_MILLIVOLT in volgens je hardware, of verlaag het bereik tot de werkelijke uitgangsspanning van de sensor.
#endif
#if SENSOR_BIOAMPEXGPILL_AANTAL != 1
  #error Zet SENSOR_BIOAMPEXGPILL_AANTAL op 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_ADS7828_AANTAL == 0
  #error Zet EXTENDER_ADS7828_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_ADS7828_AANTAL == 1
ExtenderADS7828 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7828,
  I2C_ADDRESS_EXTENDER_ADS7828_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL
);
#else
ExtenderADS7828 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7828,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

BioAmpEXGPill* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_BioAmpEXGPill_ADS7828"));
  GA_SERIAL.println(F("MEETTEST: Controleer signaal via de geconfigureerde ECG/EMG/EOG/EEG-meetmethode; geen medische diagnose."));
  sensor = GedeeldeBusNewComponent<BioAmpEXGPill>(&extender, GedeeldeBusComponent::BioAmpEXGPill, static_cast<uint8_t>(ExtenderADS7828::ExtenderPins::EP_CH0), HardwareResourceToegang::GEDEELD);
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
