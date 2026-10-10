// ============================================================================
// Sensor: RFP602
// ============================================================================
// Basis-lifecycletest voor RFP602.
// UserConfig.h kiest de ADC-backend:
//   ADC_BACKEND_NATIVE
//   ADC_BACKEND_ADS1115
// Dezelfde sketch test beide routes.
// ============================================================================

#include <GroeiAcademie.h>

#if ADC_BACKEND != ADC_BACKEND_NATIVE && ADC_BACKEND != ADC_BACKEND_ADS1115
  #error Sensor_RFP602 ondersteunt in deze test ADC_BACKEND_NATIVE of ADC_BACKEND_ADS1115.
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

  GA_SERIAL.println(F("=== RFP602 lifecycle-test ==="));
#if ADC_BACKEND == ADC_BACKEND_NATIVE
  GA_SERIAL.println(F("ADC_BACKEND: NATIVE"));
#else
  GA_SERIAL.println(F("ADC_BACKEND: ADS1115"));
#endif

  sensorRFP602 = GedeeldeBusNewComponent<struct RFP602>();
  const bool gelukt = sensorRFP602 != nullptr;
  ToonResultaat(F("aanmelden/controleren/inpluggen/activeren"), gelukt);

  if (!gelukt) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  for (uint8_t sensorNummer = 0; sensorNummer < AANTAL_SENSOREN_AANWEZIG; sensorNummer++) {
    GA_SERIAL.print(F("Sensor "));
    GA_SERIAL.print(sensorNummer + 1);
    GA_SERIAL.print(F(": "));
    GA_SERIAL.println(sensorRFP602->RawAnalogRead(sensorRFP602->sensorPin[sensorNummer]));
  }

  GA_SERIAL.println(F("RESULTAAT: OK"));
}

void loop() {
  for (uint8_t sensorNummer = 0; sensorNummer < AANTAL_SENSOREN_AANWEZIG; sensorNummer++) {
    GA_SERIAL.print(F("Sensor "));
    GA_SERIAL.print(sensorNummer + 1);
    GA_SERIAL.print(F(": "));
    GA_SERIAL.println(sensorRFP602->RawAnalogRead(sensorRFP602->sensorPin[sensorNummer]));
  }

  GA_SERIAL.println();
  delay(250);
}
