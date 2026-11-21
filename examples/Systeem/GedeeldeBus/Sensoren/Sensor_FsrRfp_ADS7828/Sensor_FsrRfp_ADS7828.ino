// Sensor: FsrRfp via ADS7828.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// RFP-sensor: originele vier kanaalconfiguraties blijven gebruikt; bij actieve twee kanalen lezen we alleen twee.

#include <GroeiAcademie.h>

#if !EXTENDER_ADS7828_EXTERNE_REFERENTIE
  #error Zet EXTENDER_ADS7828_EXTERNE_REFERENTIE op 1 en EXTENDER_ADS7828_REFERENTIE_MILLIVOLT op de werkelijke REF: het bereik van deze sensor ligt boven 2500 mV.
#endif

#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if FSR_RFP_ADC_BEREIK_MILLIVOLT > (EXTENDER_ADS7828_EXTERNE_REFERENTIE ? EXTENDER_ADS7828_REFERENTIE_MILLIVOLT : 2500)
  #error FSR_RFP_ADC_BEREIK_MILLIVOLT is groter dan wat de ADS7828 meet (interne referentie 2500 mV): stel EXTENDER_ADS7828_EXTERNE_REFERENTIE en EXTENDER_ADS7828_REFERENTIE_MILLIVOLT in volgens je hardware, of verlaag het bereik tot de werkelijke uitgangsspanning van de sensor.
#endif
#if EXTENDER_ADS7828_AANTAL == 0
  #error Zet EXTENDER_ADS7828_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_ADS7828_AANTAL == 1
ExtenderADS7828 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7828,
  I2C_ADDRESS_EXTENDER_ADS7828_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL
);
#else
ExtenderADS7828 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7828,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

FsrRfp* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_FsrRfp_ADS7828"));
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
