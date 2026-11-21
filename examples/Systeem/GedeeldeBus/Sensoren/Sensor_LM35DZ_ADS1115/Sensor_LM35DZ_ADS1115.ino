// ============================================================================
// Sensor: LM35DZ via ADS1115
// Gebaseerd op de bestaande Sensor_LM35DZ.ino en Extender_ADS1115.ino.
// Analoge uitgang naar kanaal 0; sluit voeding en massa volgens hardware aan.
// Geen automatische bevestiging van fysieke aanwezigheid of meetkwaliteit.
// ============================================================================

#include <GroeiAcademie.h>

#if SENSOR_LM35DZ_AANTAL != 1
  #error Zet SENSOR_LM35DZ_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_ADS1115_AANTAL == 1
ExtenderADS1115 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_ADS1115_1_ALERT_RDY
);
#else
ExtenderADS1115 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1115,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

LM35DZ* sensorLM35DZ = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_LM35DZ_ADS1115"));
  GA_SERIAL.println(F("MEETTEST: Verwarm licht; vergelijk ruwe ADC met temperatuurconversie."));
  sensorLM35DZ = GedeeldeBusNewComponent<LM35DZ>(
    &extender, GedeeldeBusComponent::LM35DZ,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
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
