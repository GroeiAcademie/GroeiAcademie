// ============================================================================
// Sensor: BioAmpEXGPill
// ============================================================================
// Upside Down Labs BioAmp EXG Pill, OUT via ADC
// Bron: https://docs.upsidedownlabs.tech/hardware/bioamp/bioamp-exg-pill/index.html
// Biosignalen: nooit op een persoon testen zonder geschikte elektrische isolatie/veiligheidscontrole.
// Aansluiting gebruikte I2C-parent/Extender: SDA -> board-SDA, SCL -> board-SCL; GND -> board-GND.
// UNO R4 Minima/WiFi: fysieke SDA- en SCL-header (bij R4 Minima ook A4=SDA, A5=SCL); UNO Q: board-SDA/SCL.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22 (bestaand FrameWork-boardprofiel).
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; DEV: SDA=GPIO8, SCL=GPIO9 (FrameWork-profiel).
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21 (ook Maker/Grove 6).
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8 (bestaand FrameWork-boardprofiel).
// Voeding VCC en GND volgens het concrete modulebord; controleer I2C-logicaniveau, pull-ups en eventuele levelshifting.
// Bij een analoge sensor is dit de bus van de ADC-Extender, NIET een I2C-aansluiting op de sensor zelf.
// Sluit de analoge sensoruitgang op het in de code/configuratie gebruikte ADC-kanaal aan.
// De analoge sensoruitgang gaat in deze sketch naar ADS1115 AIN0.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_BIOAMPEXGPILL_AANTAL != 1
#error Zet SENSOR_BIOAMPEXGPILL_AANTAL op 1 voor deze hardwaretest.
#endif
#if EXTENDER_ADS1115_AANTAL < 1
#error Zet EXTENDER_ADS1115_AANTAL op minstens 1.
#endif
// De ADS1115 haalt hoogstens zoveel metingen per seconde als EXTENDER_ADS1115_DATARATE toelaat (4 = 128 SPS, 5 = 250, 6 = 475, 7 = 860).
#if (SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EMG ? 500 : SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EEG ? 256 : SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EOG ? 75 : 125) > (EXTENDER_ADS1115_DATARATE == 7 ? 860 : EXTENDER_ADS1115_DATARATE == 6 ? 475 : EXTENDER_ADS1115_DATARATE == 5 ? 250 : EXTENDER_ADS1115_DATARATE == 4 ? 128 : EXTENDER_ADS1115_DATARATE == 3 ? 64 : EXTENDER_ADS1115_DATARATE == 2 ? 32 : EXTENDER_ADS1115_DATARATE == 1 ? 16 : 8)
#error EXTENDER_ADS1115_DATARATE is te traag voor de samplefrequentie van de gekozen SENSOR_BIOAMPEXGPILL_MEETMETHODE; verhoog EXTENDER_ADS1115_DATARATE.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE && DATA_LAYER_CONFIG != DATA_TYPE_DIRECT
#error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE of DATA_TYPE_DIRECT.
#endif
ExtenderADS1115 extenderADS1115(&Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::NP_SDA,
  HardwareResourcePin::NP_SCL, EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD);
BioAmpEXGPill* sensorBioAmpEXGPill = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_BioAmpEXGPill"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
  sensorBioAmpEXGPill = GedeeldeBusNewComponent<BioAmpEXGPill>(&extenderADS1115,
    GedeeldeBusComponent::BioAmpEXGPill,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensorBioAmpEXGPill ? F("BioAmpEXGPill lifecycle: component aangemaakt (fysieke werking nog niet bewezen)") : F("BioAmpEXGPill lifecycle: FOUT"));
}
void loop() {
  if (sensorBioAmpEXGPill == nullptr) return;
  float waarde = NAN;
  if (!sensorBioAmpEXGPill->LeesHoofdwaarde(waarde)) return;
  // Een piek bij elke sample onthouden, zodat een korte piek tussen twee rapporten niet verloren gaat.
  static bool peakDetectedSinceLastReport = false;
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_ECG
  { bool piek = false; if (sensorBioAmpEXGPill->LeesHartslagPiek(piek) && piek) peakDetectedSinceLastReport = true; }
#elif SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EOG
  { bool knipper = false; if (sensorBioAmpEXGPill->LeesKnipperen(knipper) && knipper) peakDetectedSinceLastReport = true; }
#endif
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250UL) return;
  laatsteRapport = millis();
  GA_SERIAL.print(F("Gefilterd biosignaal: "));
  GA_SERIAL.println(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  GA_SERIAL.print(F("Via Data, ruwe ADC-waarde: "));
  GA_SERIAL.println(sensorBioAmpEXGPill->ReadQueueOrStackValue(1));
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE != BIOAMPEXGPILL_MEETMETHODE_RAW
  GA_SERIAL.print(F("Via Data, gefilterd biosignaal: "));
  GA_SERIAL.println(sensorBioAmpEXGPill->ReadQueueOrStackFloat(2));
#endif
#endif
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EMG
  if (sensorBioAmpEXGPill->LeesEMGEnvelop(waarde)) {
    GA_SERIAL.print(F("EMG-envelop: ")); GA_SERIAL.println(waarde);
  }
#elif SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_ECG
  GA_SERIAL.println(peakDetectedSinceLastReport ? F("ECG-piek sinds vorig rapport: JA") : F("ECG-piek sinds vorig rapport: NEE"));
  peakDetectedSinceLastReport = false;
#elif SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EOG
  GA_SERIAL.println(peakDetectedSinceLastReport ? F("EOG-knipperpiek sinds vorig rapport: JA") : F("EOG-knipperpiek sinds vorig rapport: NEE"));
  peakDetectedSinceLastReport = false;
#endif
}
