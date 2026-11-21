// ============================================================================
// Sensor: MS3625
// ============================================================================
// MS3625: MEMS-microfoon via I2S, compatibel met de INMP441 (omnidirectioneel). Boards: zie INMP441_MS3625.h.
// De meetcode is die van de klasse INMP441_MS3625; de componentcode MS3625 houdt de fysieke module herkenbaar.
// BCLK = D3, WS = D4, SD = D5 op de shieldheader; L/R naar GND (linkerkanaal), VDD 3,3 V. Pas de pinnen aan aan de eigen bedrading.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_MS3625_AANTAL != 1
#error Zet SENSOR_MS3625_AANTAL op 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE && DATA_LAYER_CONFIG != DATA_TYPE_DIRECT
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE of DATA_TYPE_DIRECT zijn voor deze hardwaretest.
#endif
INMP441_MS3625* microfoon = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_MS3625"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
  const bool parentOK = Native.aanmelden() && Native.controleren() && Native.inpluggen() && Native.activeren();
  GA_SERIAL.println(parentOK ? F("PARENT: OK") : F("PARENT: FOUT"));
  if (!parentOK) return;
  microfoon = GedeeldeBusNewComponent<INMP441_MS3625>(&Native, GedeeldeBusComponent::MS3625,
    HardwareResourcePin::NP_D3, HardwareResourcePin::NP_D4, HardwareResourcePin::NP_D5);
  GA_SERIAL.println(microfoon ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}
void loop() {
  if (!microfoon) return;
  int32_t samples[64];
  // Doorlopend lezen: zo vult LeesSamples() het venster van het geluidsniveau op het ritme van de microfoon.
  const size_t aantal = microfoon->LeesSamples(samples, 64);
  if (aantal == 0) return;
  // Rapporteren om de 250 ms, zoals Sensor_INMP441_CD74HC4067_Verwerking.
  // Via Data staat het laatst afgesloten venster; de rechtstreekse RMS toont het venster dat nog loopt.
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250UL) return;
  laatsteRapport = millis();
  int32_t piek = 0;
  for (size_t index = 0; index < aantal; ++index) {
    const int32_t waarde = samples[index] >> 8;   // 24-bit microfoondata
    if (abs(waarde) > piek) piek = abs(waarde);
  }
  GA_SERIAL.println(piek);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  GA_SERIAL.print(F("Via Data, laatste I2S-woord: "));
  GA_SERIAL.println(microfoon->ReadQueueOrStackValue(1));
  GA_SERIAL.print(F("Via Data, RMS (24-bit): "));
  GA_SERIAL.println(microfoon->ReadQueueOrStackFloat(2));
  GA_SERIAL.print(F("Via Data, piek-tot-piek: "));
  GA_SERIAL.println(microfoon->ReadQueueOrStackValue(3));
#endif
}
