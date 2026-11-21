// Sensor: OT3686 via CD74HC4067: DO -> Y0; de interrupt komt op de native Z-pin (moet een interruptpin zijn).
// Het kanaal blijft open na Activeren; één digitale child per CD74HC4067.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_OT3686_AANTAL != 1
  #error Zet SENSOR_OT3686_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_CD74HC4067_AANTAL == 0
  #error Zet EXTENDER_CD74HC4067_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE && DATA_LAYER_CONFIG != DATA_TYPE_DIRECT
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE of DATA_TYPE_DIRECT voor deze test.
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

OT3686* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_OT3686_CD74HC4067"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<OT3686>(&extender, GedeeldeBusComponent::OT3686, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y0)));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (!sensor) return;
  float venster = NAN, periode = NAN;
  uint32_t aantal = 0;
  bool toestand = false;
  sensor->LeesRPMViaTijdvenster(venster);
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250) return;
  laatsteRapport = millis();
  sensor->LeesRPMViaPeriode(periode);
  sensor->LeesAantalPulsen(aantal);
  sensor->LeesDigitaleToestand(toestand);
  GA_SERIAL.print(F("DO=")); GA_SERIAL.print(toestand);
  GA_SERIAL.print(F(" pulsen=")); GA_SERIAL.print(aantal);
  GA_SERIAL.print(F(" RPM_venster=")); GA_SERIAL.print(venster);
  GA_SERIAL.print(F(" RPM_periode=")); GA_SERIAL.println(periode);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  GA_SERIAL.print(F("Via Data, RPM_venster: "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(1));
  GA_SERIAL.print(F("Via Data, RPM_periode: "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(2));
  GA_SERIAL.print(F("Via Data, pulsen: "));
  GA_SERIAL.println(sensor->ReadQueueOrStackUInt32(3));
  GA_SERIAL.print(F("Via Data, DO: "));
  GA_SERIAL.println(sensor->ReadQueueOrStackValue(4));
#endif
}
