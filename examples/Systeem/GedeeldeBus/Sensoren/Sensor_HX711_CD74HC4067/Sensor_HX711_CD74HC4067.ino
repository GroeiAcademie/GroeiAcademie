// Sensor: HX711 via twee CD74HC4067: DOUT -> Y0 van CD74HC4067 #1, PD_SCK -> Y0 van CD74HC4067 #2 (MIXED).
// Twee lijnen tegelijk vragen twee Z-pinnen. EXTENDER_CD74HC4067_AANTAL 2 is experimenteel (ExtenderLijst 1 en 2).
// Beide kanalen blijven open na Activeren.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_HX711_AANTAL != 1
  #error Zet SENSOR_HX711_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_CD74HC4067_AANTAL < 2
  #error Zet EXTENDER_CD74HC4067_AANTAL op 2 (experimenteel) voor deze test.
#endif
#if !GEDEELDE_BUS_MIXED_PINS
  #error Zet GEDEELDE_BUS_MIXED_PINS op 1 in UserConfig.h voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_CD74HC4067_AANTAL >= 2
ExtenderCD74HC4067 extender(&Native, GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067, 0);
ExtenderCD74HC4067 extenderCD74HC4067_2(&Native, GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067, 1);
#endif

SensorHX711* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_HX711_CD74HC4067"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren() &&
                      extenderCD74HC4067_2.aanmelden() && extenderCD74HC4067_2.controleren() && extenderCD74HC4067_2.inpluggen() && extenderCD74HC4067_2.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<SensorHX711>(&extender, GedeeldeBusComponent::HX711, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y0)), &extenderCD74HC4067_2, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y0)));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  float waarde = NAN;
  if (!sensor->LeesHoofdwaarde(waarde)) return;
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250UL) return;
  laatsteRapport = millis();
  GA_SERIAL.print(F("Ruwe loadcell-waarde (niet gekalibreerd): "));
  GA_SERIAL.println(waarde);
}
