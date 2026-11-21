// Sensor: FsrRfp via CD74HC4067.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// RFP-sensor: originele vier kanaalconfiguraties blijven gebruikt; bij actieve twee kanalen lezen we alleen twee.

#include <GroeiAcademie.h>

#if EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT == 0
  #error Zet EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT op de werkelijke analoge referentie van het board: Z hangt aan een native analoge pin.
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

FsrRfp* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_FsrRfp_CD74HC4067"));
  GA_SERIAL.println(F("MEETTEST: Belast elke geconfigureerde drukpin apart; de kanalen moeten afzonderlijk reageren."));
  sensor = GedeeldeBusNewComponent<FsrRfp>(&extender, GedeeldeBusComponent::FsrRfp, 0, 1, 2, 3, HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  for (uint8_t sensorNummer = 0; sensorNummer < FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1; sensorNummer++) {
    int32_t waarde = 0;
    GA_SERIAL.print(F("Drukpin ")); GA_SERIAL.print(sensorNummer);
    if (sensor->RawAnalogRead(sensor->sensorPin[sensorNummer], waarde)) {
      GA_SERIAL.print(F(" ADC: ")); GA_SERIAL.println(waarde);
    } else {
      GA_SERIAL.println(F(": UITLEESFOUT"));
    }
  }
  delay(250);
}
