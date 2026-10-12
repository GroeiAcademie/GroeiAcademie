// ============================================================================
// Sensor: RFP602 microSD
// ============================================================================
// RFP602 via Data RECORDING naar microSD.
// De bestandsnaam komt uit RFP602_DATA_RECORD_FILE.
// ============================================================================

#include <GroeiAcademie.h>

#if !((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  #error Voeg DATA_TYPE_MICROSD toe aan DATA_LAYER_CONFIG voor deze test.
#endif

void ToonResultaat(const __FlashStringHelper* stap, bool gelukt) {
  GA_SERIAL.print(stap);
  GA_SERIAL.print(F(": "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
}

void setup() {
#if ADC_BITS == 12 || ADC_BITS == 14
  analogReadResolution(ADC_BITS);
#endif

  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_TYPE_MICROSD);
  ToonResultaat(F("Data aanmelden/controleren/inpluggen/activeren"), Data != nullptr);

  if (Data == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  sensorRFP602 = GedeeldeBusNewComponent<struct RFP602>(Data);
  const bool gelukt = sensorRFP602 != nullptr;
  ToonResultaat(F("Sensor aanmelden/controleren/inpluggen/activeren"), gelukt);

  if (!gelukt) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  Data->RecordStart();
  GA_SERIAL.println(F("RESULTAAT: OK"));
}

void loop() {
  for (uint8_t sensorNummer = 0; sensorNummer < AANTAL_SENSOREN_AANWEZIG; sensorNummer++) {
    const int ruweData = sensorRFP602->RawAnalogRead(sensorRFP602->sensorPin[sensorNummer]);
    Data->RecordWrite(ruweData);
    GA_SERIAL.print(F("Sensor "));
    GA_SERIAL.print(sensorNummer + 1);
    GA_SERIAL.print(F(": "));
    GA_SERIAL.println(ruweData);
  }

  GA_SERIAL.println();
  delay(250);
}
