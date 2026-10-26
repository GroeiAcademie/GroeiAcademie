// ============================================================================
// Sensor: RFP602 microSD
// ============================================================================
// RFP602 via Data RECORDING naar microSD.
// De bestandsnaam komt uit RFP602_DATA_RECORD_FILE_SET_1.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL in UserConfig.h op minstens 1: RFP602 meet via een ADS1115-Extender.
#endif

ExtenderADS1115 extenderADS1115(
  &Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD
);


#if !((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  #error Voeg DATA_TYPE_MICROSD_SPI toe aan DATA_LAYER_CONFIG voor deze test.
#endif

void ToonResultaat(const __FlashStringHelper* stap, bool gelukt) {
  GA_SERIAL.print(stap);
  GA_SERIAL.print(F(": "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_TYPE_MICROSD_SPI);
  ToonResultaat(F("Data aanmelden/controleren/inpluggen/activeren"), Data != nullptr);

  if (Data == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  sensorRFP602set1 = GedeeldeBusNewComponent<struct RFP602>(&extenderADS1115, GedeeldeBusComponent::RFP602, Data);
  const bool gelukt = sensorRFP602set1 != nullptr;
  ToonResultaat(F("Sensor aanmelden/controleren/inpluggen/activeren"), gelukt);

  if (!gelukt) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  Data->RecordingStart();
  GA_SERIAL.println(F("RESULTAAT: OK"));
}

void loop() {
  if (sensorRFP602set1 == nullptr || Data == nullptr) return;
  for (uint8_t sensorNummer = 0; sensorNummer < RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1; sensorNummer++) {
    const int ruweData = sensorRFP602set1->RawAnalogRead(sensorRFP602set1->sensorPin[sensorNummer]);
    Data->RecordingWrite(ruweData);
    GA_SERIAL.print(F("Sensor "));
    GA_SERIAL.print(sensorNummer + 1);
    GA_SERIAL.print(F(": "));
    GA_SERIAL.println(ruweData);
  }

  GA_SERIAL.println();
  delay(250);
}
