// ============================================================================
// Sensor: LM35DZ via ADS1158
// Gebaseerd op de bestaande Sensor_LM35DZ.ino en Extender_ADS1158.ino.
// Analoge uitgang naar kanaal 0; sluit voeding en massa volgens hardware aan.
// Geen automatische bevestiging van fysieke aanwezigheid of meetkwaliteit.
// ============================================================================

#include <GroeiAcademie.h>

#if SENSOR_LM35DZ_AANTAL != 1
  #error Zet SENSOR_LM35DZ_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_ADS1158_AANTAL == 0
  #error Zet EXTENDER_ADS1158_AANTAL op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_ADS1158_AANTAL == 1
ExtenderADS1158 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1158,
  EXTENDER_ADS1158_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, EXTENDER_ADS1158_1_START, EXTENDER_ADS1158_1_RESET, EXTENDER_ADS1158_1_PWDN
);
#else
ExtenderADS1158 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1158,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

LM35DZ* sensorLM35DZ = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_LM35DZ_ADS1158"));
  GA_SERIAL.println(F("MEETTEST: Verwarm licht; vergelijk ruwe ADC met temperatuurconversie."));
  sensorLM35DZ = GedeeldeBusNewComponent<LM35DZ>(
    &extender, GedeeldeBusComponent::LM35DZ,
    static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensorLM35DZ ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}
void loop() {
  if (sensorLM35DZ == nullptr) return;
  int32_t waarde = 0;
  if (sensorLM35DZ->RawAnalogRead(waarde)) {
    GA_SERIAL.print(F("LM35DZ ADC: ")); GA_SERIAL.println(waarde);
    GA_SERIAL.print(F("Temperatuur (C): ")); GA_SERIAL.println(sensorLM35DZ->TemperatuurC());
  } else {
    GA_SERIAL.println(F("LM35DZ ADC: UITLEESFOUT (ADC-bereik/referentie controleren)"));
  }
}
