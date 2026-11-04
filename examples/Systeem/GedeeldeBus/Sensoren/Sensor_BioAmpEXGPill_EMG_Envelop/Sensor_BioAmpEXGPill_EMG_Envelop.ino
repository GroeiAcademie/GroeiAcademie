// ============================================================================
// Sensor: BioAmpEXGPill_EMG_Envelop
// ============================================================================
// Upside Down Labs BioAmp EXG Pill, OUT via ADC (ADS1115 AIN0)
// Bron: https://docs.upsidedownlabs.tech/hardware/bioamp/bioamp-exg-pill/index.html
// Officieel voorbeeld: EMGEnvelop.ino
// Biosignalen: nooit op een persoon testen zonder geschikte elektrische isolatie/veiligheidscontrole (VEILIGHEID.md).
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_BIOAMPEXGPILL_AANTAL != 1
#error Zet SENSOR_BIOAMPEXGPILL_AANTAL op 1 voor deze hardwaretest.
#endif
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE != BIOAMPEXGPILL_MEETMETHODE_EMG
#error Zet SENSOR_BIOAMPEXGPILL_MEETMETHODE op BIOAMPEXGPILL_MEETMETHODE_EMG voor deze hardwaretest.
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
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_BioAmpEXGPill_EMG_Envelop"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  sensorBioAmpEXGPill = GedeeldeBusNewComponent<BioAmpEXGPill>(&extenderADS1115,
    GedeeldeBusComponent::BioAmpEXGPill,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensorBioAmpEXGPill ? F("BioAmpEXGPill lifecycle: OK") : F("BioAmpEXGPill lifecycle: FOUT"));
}
// Per gefilterd sample: de EMG-envelop (voortschrijdend gemiddelde van |EMG|, maal 2), een maat voor spierkracht.
unsigned long tijdstipLaatsteUitvoer = 0;
void loop() {
  if (!sensorBioAmpEXGPill) return;
  float sample;
  if (!sensorBioAmpEXGPill->LeesHoofdwaarde(sample)) return;   // vast ritme: alleen een nieuw sample telt
  float envelop;
  if (!sensorBioAmpEXGPill->LeesEMGEnvelop(envelop)) return;
  // 500 samples per seconde is te veel voor de seriële monitor: om de 20 ms tonen.
  if (millis() - tijdstipLaatsteUitvoer < 20) return;
  tijdstipLaatsteUitvoer = millis();
  GA_SERIAL.println(envelop);
}
