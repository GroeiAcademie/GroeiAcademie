// Sensor: INMP441_CD74HC4067_Verwerking
// Verwerking: geluidsniveau (RMS en piek-tot-piek) van het linkerslot.
// BCLK en WS kunnen meerdere microfoons delen; alleen SD wordt geschakeld. Boards: zie INMP441_MS3625.h. Kanaal blijft open.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_INMP441_AANTAL != 1
  #error Zet SENSOR_INMP441_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_CD74HC4067_AANTAL == 0
  #error Zet EXTENDER_CD74HC4067_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if !GEDEELDE_BUS_MIXED_PINS
  #error Zet GEDEELDE_BUS_MIXED_PINS op 1 in UserConfig.h voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE && DATA_LAYER_CONFIG != DATA_TYPE_DIRECT
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE of DATA_TYPE_DIRECT voor deze test.
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

INMP441_MS3625* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_INMP441_CD74HC4067_Verwerking"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<INMP441_MS3625>(&Native, GedeeldeBusComponent::INMP441, HardwareResourcePin::NP_D3, HardwareResourcePin::NP_D4, &extender, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y0)));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}
void loop() {
  if (!sensor) return;
  int32_t samples[64];
  (void)sensor->LeesSamples(samples, 64);   // vult het venster van het geluidsniveau
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250UL) return;
  laatsteRapport = millis();
  float rootMeanSquare = NAN;
  int32_t peakToPeak = 0;
  if (sensor->ReadSoundLevelRootMeanSquare(rootMeanSquare) && sensor->ReadSoundLevelPeakToPeak(peakToPeak)) {
    GA_SERIAL.print(F("RMS (24-bit): ")); GA_SERIAL.print(rootMeanSquare);
    GA_SERIAL.print(F("  Piek-tot-piek: ")); GA_SERIAL.println(peakToPeak);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    GA_SERIAL.print(F("Via Data, RMS (24-bit): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackFloat(2));
    GA_SERIAL.print(F("Via Data, piek-tot-piek: "));
    GA_SERIAL.println(sensor->ReadQueueOrStackValue(3));
#endif
  }
}
