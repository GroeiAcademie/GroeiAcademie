// ============================================================================
// Sensor: DS3231
// ============================================================================
// DS3231-klokmodule op I2C (SDA/SCL), knoopcel geplaatst.
// Let op: de MPU6050/MPU6500 gebruiken met AD0 laag ook adres 0x68.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_DS3231_AANTAL != 1
#error SENSOR_DS3231_AANTAL moet 1 zijn voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze hardwaretest.
#endif
HardwareResourceTypeI2C apparaat(&Native, GedeeldeBusComponent::DS3231, I2C_ADDRESS_SENSOR_DS3231_1,
 HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
DS3231* klok = nullptr;
void ToonDatumTijd(const DateTime& datumTijd) {
 char tekst[20];
 snprintf(tekst, sizeof(tekst), "%04u-%02u-%02u %02u:%02u:%02u", datumTijd.year(), datumTijd.month(), datumTijd.day(), datumTijd.hour(), datumTijd.minute(), datumTijd.second());
 GA_SERIAL.println(tekst);
}
void setup() {
 GA_SERIAL.begin(SERIAL_BAUDRATE);
 while (!GA_SERIAL) { ; }
 GA_SERIAL.println(F("TEST: Sensor_DS3231"));
 const unsigned long beginTijd = millis();
 while (!GA_SERIAL && millis()-beginTijd<SERIAL_CONNECT_TIMEOUT_MS) { ; }
 const bool parentOK = apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
 GA_SERIAL.println(parentOK ? F("I2C RESOURCE: OK") : F("I2C RESOURCE: FOUT"));
 if (parentOK) klok=GedeeldeBusNewComponent<DS3231>(&apparaat,GedeeldeBusComponent::DS3231);
 GA_SERIAL.println(klok ? F("SENSOR IDENTITEIT/LIFECYCLE: OK") : F("SENSOR IDENTITEIT/LIFECYCLE: FOUT"));
 if (!klok) return;
 DateTime datumTijd;
 if (!klok->LeesDatumTijd(datumTijd)) {
  // Nog geen geldige tijd (nieuwe knoopcel of batterijverlies): eenmalig de compileertijd instellen.
  GA_SERIAL.println(F("KLOK: geen geldige tijd, compileertijd instellen"));
  klok->ZetDatumTijd(DateTime(F(__DATE__), F(__TIME__)));
 }
}
void loop() {
 if (!klok) return;
 DateTime datumTijd;
 if (klok->LeesDatumTijd(datumTijd)) ToonDatumTijd(datumTijd);
 else GA_SERIAL.println(F("KLOK: FOUT"));
 delay(1000);
}
