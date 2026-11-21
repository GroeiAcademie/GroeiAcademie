// Sensor: TTP223 via PCF8574.
// OUT -> PCF8574 EP_P0; voeding, gemeenschappelijke massa en logicaniveaus controleren.
// inpluggen() zet OUT als ingang via SetExtenderPinMode(), zoals SensorDigitaal (HW201, HCSR501).
// Geen fysiek ID: een geslaagde lifecycle bewijst niet dat de Sensor aangesloten is.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_TTP223_AANTAL != 1
  #error Zet SENSOR_TTP223_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_PCF8574_AANTAL == 0
  #error Zet EXTENDER_PCF8574_AANTAL op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_PCF8574_AANTAL == 1
ExtenderPCF8574 extender(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574,
  I2C_ADDRESS_EXTENDER_PCF8574_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8574_1_INT
);
#else
ExtenderPCF8574 extender(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

TTP223* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_TTP223_PCF8574"));
  const bool parentOK = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(parentOK ? F("EXTENDER: OK") : F("EXTENDER: FOUT"));
  if (!parentOK) return;
  sensor = GedeeldeBusNewComponent<TTP223>(&extender, GedeeldeBusComponent::TTP223, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P0)));
  GA_SERIAL.println(sensor ? F("SENSOR: OK") : F("SENSOR: FOUT"));
}

void loop() {
  if (!sensor) return;
  float status = NAN;
  uint32_t aantal = 0;
  unsigned long duur = 0;
  if (sensor->LeesHoofdwaarde(status)) {
    sensor->LeesAantalAanrakingen(aantal);
    sensor->LeesLaatsteAanraakduur(duur);
    static unsigned long laatsteRapport = 0;
    if (millis() - laatsteRapport >= 200) {
      laatsteRapport = millis();
      GA_SERIAL.print(F("aanraakstatus=")); GA_SERIAL.print(status);
      GA_SERIAL.print(F(" aantal=")); GA_SERIAL.print(aantal);
      GA_SERIAL.print(F(" duur_ms=")); GA_SERIAL.println(duur);
    }
  }
}
