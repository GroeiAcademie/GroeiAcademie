// Sensor: FsrRfp via ADS7953.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// RFP-sensor: originele vier kanaalconfiguraties blijven gebruikt; bij actieve twee kanalen lezen we alleen twee.

#include <GroeiAcademie.h>

#if EXTENDER_ADS7953_VOEDING_MILLIVOLT == 0
  #error Zet EXTENDER_ADS7953_VOEDING_MILLIVOLT op de werkelijke +VA (2700..5250 mV): het bereik van deze sensor vraagt 2xVREF.
#endif

#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if (FSR_RFP_ADC_BEREIK_MILLIVOLT > EXTENDER_ADS7953_REFERENTIE_MILLIVOLT && (FSR_RFP_ADC_BEREIK_MILLIVOLT > 2 * EXTENDER_ADS7953_REFERENTIE_MILLIVOLT || EXTENDER_ADS7953_VOEDING_MILLIVOLT < 2 * EXTENDER_ADS7953_REFERENTIE_MILLIVOLT)) || \
    (EXTENDER_ADS7953_VOEDING_MILLIVOLT != 0 && EXTENDER_ADS7953_VOEDING_MILLIVOLT < EXTENDER_ADS7953_REFERENTIE_MILLIVOLT)
  #error FSR_RFP_ADC_BEREIK_MILLIVOLT is groter dan wat de ADS7953 meet: het dubbele bereik (2 x EXTENDER_ADS7953_REFERENTIE_MILLIVOLT) vraagt EXTENDER_ADS7953_VOEDING_MILLIVOLT van minstens 2 x de referentie; stel beide in volgens je hardware, of verlaag het bereik tot de werkelijke uitgangsspanning van de sensor.
#endif
#if EXTENDER_ADS7953_AANTAL == 0
  #error Zet EXTENDER_ADS7953_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_ADS7953_AANTAL == 1
ExtenderADS7953 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7953,
  EXTENDER_ADS7953_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI
);
#else
ExtenderADS7953 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7953,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

FsrRfp* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_FsrRfp_ADS7953"));
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
