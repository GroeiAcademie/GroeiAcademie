// Sensor: DHTxx via CD74HC4067.
// DATA -> CD74HC4067 Y0; Z van de CD74HC4067 op een native digitale pin (EXTENDER_CD74HC4067_1_Z).
// De DHT-driver werkt op de native Z-pin; de CD74HC4067 kiest Y0 tijdens Activeren (SelectAccessForChild).
// Pull-up op DATA aan de sensorkant (module). Weerstand van de schakelaar (Ron 70..160 ohm) fysiek controleren.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_DHT11_AANTAL + SENSOR_DHT22_AANTAL != 1
  #error Zet SENSOR_DHT11_AANTAL of SENSOR_DHT22_AANTAL op 1 (niet beide) voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif
#if EXTENDER_CD74HC4067_AANTAL == 0
  #error Zet EXTENDER_CD74HC4067_AANTAL in UserConfig.h op minstens 1 voor deze test.
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

#if SENSOR_DHT22_AANTAL == 1
const GedeeldeBusComponent fysiekType = GedeeldeBusComponent::DHT22;
#else
const GedeeldeBusComponent fysiekType = GedeeldeBusComponent::DHT11;
#endif
DHTxx* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_DHTxx_CD74HC4067"));
  const bool parentOK = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(parentOK ? F("EXTENDER: OK") : F("EXTENDER: FOUT"));
  if (!parentOK) return;
  sensor = GedeeldeBusNewComponent<DHTxx>(&extender, fysiekType, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y0)));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  float waarde = NAN;
  if (!sensor->LeesHoofdwaarde(waarde)) return;
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 2000UL) return;   // de DHT-library meet hooguit om de 2 s
  laatsteRapport = millis();
  GA_SERIAL.print(F("Temperatuur (C): "));
  GA_SERIAL.println(waarde);
}
