// Sensor: DS3231 via TCA9548A.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// TCA-kanaal 0. Sensor-I2C-adres moet overeenkomen met de werkelijke module.

#include <GroeiAcademie.h>

#if SENSOR_DS3231_AANTAL != 1
  #error Zet SENSOR_DS3231_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_TCA9548A_AANTAL == 0
  #error Zet EXTENDER_TCA9548A_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_TCA9548A_AANTAL == 1
ExtenderTCA9548A extender(
  &Native,
  GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A,
  I2C_ADDRESS_EXTENDER_TCA9548A_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_TCA9548A_1_RESET
);
#else
ExtenderTCA9548A extender(
  &Native,
  GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

HardwareResourceTypeI2C apparaat(&extender, GedeeldeBusComponent::DS3231, I2C_ADDRESS_SENSOR_DS3231_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD, 0);
DS3231* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_DS3231_TCA9548A"));
  GA_SERIAL.println(F("MEETTEST: Controleer de klokvoortgang; geen automatische klokinstelling."));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(gelukt ? F("TCA: OK") : F("TCA: FOUT"));
  if (!gelukt) return;
  const bool parentOK = apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
  GA_SERIAL.println(parentOK ? F("I2C: OK") : F("I2C: FOUT"));
  if (!parentOK) return;
  sensor = GedeeldeBusNewComponent<DS3231>(&apparaat, GedeeldeBusComponent::DS3231);
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  DateTime datumTijd;
  if (sensor->LeesDatumTijd(datumTijd)) {
    GA_SERIAL.print(F("Datum/tijd: "));
    GA_SERIAL.print(datumTijd.year()); GA_SERIAL.print('-');
    GA_SERIAL.print(datumTijd.month()); GA_SERIAL.print('-');
    GA_SERIAL.print(datumTijd.day()); GA_SERIAL.print(' ');
    GA_SERIAL.print(datumTijd.hour()); GA_SERIAL.print(':');
    GA_SERIAL.print(datumTijd.minute()); GA_SERIAL.print(':');
    GA_SERIAL.println(datumTijd.second());
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    GA_SERIAL.print(F("Unix-tijd (s): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackUInt32(1));
    GA_SERIAL.print(F("Chiptemperatuur (C): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackFloat(2));
#endif
  } else {
    GA_SERIAL.println(F("Datum/tijd: GEEN GELDIGE METING (controleer voeding en klokinstelling)"));
  }
  delay(250);
}
