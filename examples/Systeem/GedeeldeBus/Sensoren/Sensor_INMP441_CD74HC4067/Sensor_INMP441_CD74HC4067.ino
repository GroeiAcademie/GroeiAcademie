// Sensor: INMP441 met SD via CD74HC4067 (Y0, Z = native SD-pin); BCLK op native D3 en WS op native D4 (MIXED).
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
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
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
  GA_SERIAL.println(F("TEST: Sensor_INMP441_CD74HC4067"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<INMP441_MS3625>(&Native, GedeeldeBusComponent::INMP441, HardwareResourcePin::NP_D3, HardwareResourcePin::NP_D4, &extender, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y0)));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (!sensor) return;
  int32_t samples[64];
  const size_t aantal = sensor->LeesSamples(samples, 64);
  if (aantal == 0) return;
  int32_t piek = 0;
  for (size_t index = 0; index < aantal; ++index) {
    const int32_t waarde = samples[index] >> 8;   // 24-bit microfoondata
    if (abs(waarde) > piek) piek = abs(waarde);
  }
  GA_SERIAL.println(piek);
  delay(100);
}
