// Sensor: HC-SR04 via CD74HC4067: TRIG -> Y0, ECHO -> Y1, na elkaar op de native Z-pin (één CD74HC4067).
// Activeren kiest Y0 en zet Z laag (TRIG in rust). Meten (kanaalwissel naar ECHO) hoort bij de functies. Pull-down op TRIG aanbevolen.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_HCSR04_AANTAL != 1
  #error Zet SENSOR_HCSR04_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_CD74HC4067_AANTAL == 0
  #error Zet EXTENDER_CD74HC4067_AANTAL in UserConfig.h op minstens 1 voor deze test.
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

HSR0x* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_HSR0x_CD74HC4067"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<HSR0x>(&extender, GedeeldeBusComponent::HCSR04, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y0)), static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y1)));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  // De fabrikant vraagt minstens 60 ms tussen twee metingen.
  static unsigned long lastMeasurementStartMS = 0;
  if (millis() - lastMeasurementStartMS < 100UL) return;
  lastMeasurementStartMS = millis();
  float waarde = NAN;
  if (!sensor->LeesHoofdwaarde(waarde)) return;
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250UL) return;
  laatsteRapport = millis();
  GA_SERIAL.print(F("Afstand (mm): "));
  GA_SERIAL.println(waarde);
}
