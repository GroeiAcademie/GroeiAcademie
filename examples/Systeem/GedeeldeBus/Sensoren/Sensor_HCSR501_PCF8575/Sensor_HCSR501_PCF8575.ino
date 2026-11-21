// Sensor: HCSR501 via PCF8575.
// Bestaande SensorDigitaal::LeesHoofdwaarde() leest de OUT-pin via ReadExtenderPin().
// OUT -> EP_P00 op PCF8575; voeding, gemeenschappelijke massa en logicaniveaus controleren.
// Geen fysiek ID: een geslaagde lifecycle bewijst niet dat de Sensor aangesloten is.

#include <GroeiAcademie.h>

#if SENSOR_HCSR501_AANTAL != 1
  #error Zet SENSOR_HCSR501_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_PCF8575_AANTAL == 0
  #error Zet EXTENDER_PCF8575_AANTAL op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_PCF8575_AANTAL == 1
ExtenderPCF8575 extender(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8575,
  I2C_ADDRESS_EXTENDER_PCF8575_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8575_1_INT
);
#else
ExtenderPCF8575 extender(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8575,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

HCSR501* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_HCSR501_PCF8575"));

  GA_SERIAL.println(F("MEETTEST: Beweeg voor de PIR; controleer 0/1 en terugkeer naar rust."));
  const bool parentOK = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(parentOK ? F("EXTENDER: OK") : F("EXTENDER: FOUT"));
  if (!parentOK) return;

  sensor = GedeeldeBusNewComponent<HCSR501>(&extender, GedeeldeBusComponent::HCSR501, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P00)));
  GA_SERIAL.println(sensor ? F("SENSOR: OK") : F("SENSOR: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 200UL) return;
  laatsteRapport = millis();
  float waarde = NAN;
  if (sensor->LeesHoofdwaarde(waarde)) {
    GA_SERIAL.print(F("Detectie OUT (0/1): "));
    GA_SERIAL.println(waarde);
  } else {
    GA_SERIAL.println(F("Detectie OUT: UITLEESFOUT"));
  }
}
