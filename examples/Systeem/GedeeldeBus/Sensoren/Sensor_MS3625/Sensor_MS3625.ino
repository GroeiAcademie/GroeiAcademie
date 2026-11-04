// ============================================================================
// Sensor: MS3625
// ============================================================================
// MS3625: MEMS-microfoon via I2S, compatibel met de INMP441 (omnidirectioneel). Alleen ESP32-S3 (Ardi32 of DEV).
// De meetcode is die van de klasse INMP441; de componentcode MS3625 houdt de fysieke module herkenbaar.
// BCLK = D3, WS = D4, SD = D5 op de shieldheader; L/R naar GND (linkerkanaal), VDD 3,3 V. Pas de pinnen aan aan de eigen bedrading.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_INMP441_AANTAL != 1
#error Zet SENSOR_INMP441_AANTAL op 1: de MS3625 gebruikt de klasse INMP441.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze hardwaretest.
#endif
INMP441* microfoon = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MS3625"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  const bool parentOK = Native.aanmelden() && Native.controleren() && Native.inpluggen() && Native.activeren();
  GA_SERIAL.println(parentOK ? F("PARENT: OK") : F("PARENT: FOUT"));
  if (!parentOK) return;
  microfoon = GedeeldeBusNewComponent<INMP441>(&Native, GedeeldeBusComponent::MS3625,
    HardwareResourcePin::NP_D3, HardwareResourcePin::NP_D4, HardwareResourcePin::NP_D5);
  GA_SERIAL.println(microfoon ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}
void loop() {
  if (!microfoon) return;
  int32_t samples[64];
  const size_t aantal = microfoon->LeesSamples(samples, 64);
  if (aantal == 0) return;
  int32_t piek = 0;
  for (size_t index = 0; index < aantal; ++index) {
    const int32_t waarde = samples[index] >> 8;   // 24-bit microfoondata
    if (abs(waarde) > piek) piek = abs(waarde);
  }
  GA_SERIAL.println(piek);
  delay(100);
}
