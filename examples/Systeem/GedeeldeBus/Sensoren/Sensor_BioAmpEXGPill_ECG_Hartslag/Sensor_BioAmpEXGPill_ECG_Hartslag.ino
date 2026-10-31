// Upside Down Labs BioAmp EXG Pill, OUT via ADC (ADS1115 AIN0)
// Bron: https://docs.upsidedownlabs.tech/hardware/bioamp/bioamp-exg-pill/index.html
// Officieel voorbeeld: HeartBeatDetection.ino
// Biosignalen: nooit op een persoon testen zonder geschikte elektrische isolatie/veiligheidscontrole (VEILIGHEID.md).
#include <GroeiAcademie.h>
#if SENSOR_BIOAMPEXGPILL_AANTAL != 1
#error Zet SENSOR_BIOAMPEXGPILL_AANTAL op 1 voor deze hardwaretest.
#endif
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE != BIOAMPEXGPILL_MEETMETHODE_ECG
#error Zet SENSOR_BIOAMPEXGPILL_MEETMETHODE op BIOAMPEXGPILL_MEETMETHODE_ECG voor deze hardwaretest.
#endif
#if EXTENDER_ADS1115_AANTAL < 1
#error Zet EXTENDER_ADS1115_AANTAL op minstens 1.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE.
#endif
ExtenderADS1115 extenderADS1115(&Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::NP_SDA,
  HardwareResourcePin::NP_SCL, EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD);
BioAmpEXGPill* sensorBioAmpEXGPill = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  sensorBioAmpEXGPill = GedeeldeBusNewComponent<BioAmpEXGPill>(&extenderADS1115,
    GedeeldeBusComponent::BioAmpEXGPill,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensorBioAmpEXGPill ? F("BioAmpEXGPill lifecycle: OK") : F("BioAmpEXGPill lifecycle: FOUT"));
}
// Per gefilterd sample: is dit een R-piek? Uit de tijd tussen twee pieken volgt de hartslag (BPM).
unsigned long tijdstipVorigePiek = 0;
bool vorigePiek = false;
void loop() {
  if (!sensorBioAmpEXGPill) return;
  float sample;
  if (!sensorBioAmpEXGPill->LeesHoofdwaarde(sample)) return;   // vast ritme: alleen een nieuw sample telt
  bool piek = false;
  if (!sensorBioAmpEXGPill->LeesHartslagPiek(piek)) return;
  if (piek && !vorigePiek) {   // begin van een piek
    const unsigned long nu = millis();
    if (tijdstipVorigePiek != 0) {
      const unsigned long interval = nu - tijdstipVorigePiek;
      if (interval > 0) {
        GA_SERIAL.print(F("Hartslag (BPM): "));
        GA_SERIAL.println(60000UL / interval);
      }
    }
    tijdstipVorigePiek = nu;
  }
  vorigePiek = piek;
}
