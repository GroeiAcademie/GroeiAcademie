// ============================================================================
// Data: NRF24L01 (zender)
// ============================================================================
// Elke meting die naar Data gaat, wordt als GB-record van 15 bytes ook draadloos verstuurd.
// Meetbron in deze test: een DHT11 of DHT22 op D2. Ontvangen: NRF24L01_Ontvanger op een tweede Arduino.
// Aansluiten: NRF24L01 op 3,3 V (condensator van 10 µF tot 100 µF over de voeding), SCK/MISO/MOSI op hardware-SPI,
// CS (CSN) en CE op vrije pinnen, bv. DATA_LAYER_NRF24L01_SPI_1_CS_PIN HardwareResourcePin::NP_D6 en DATA_LAYER_NRF24L01_SPI_1_CE_PIN HardwareResourcePin::NP_D5.
// ============================================================================

#include <GroeiAcademie.h>

#if !((DATA_LAYER_CONFIG) & DATA_TYPE_NRF24L01_SPI)
  #error Voeg DATA_TYPE_NRF24L01_SPI toe aan DATA_LAYER_CONFIG voor deze test.
#endif
#if SENSOR_DHT11_AANTAL + SENSOR_DHT22_AANTAL != 1
  #error Zet SENSOR_DHT11_AANTAL of SENSOR_DHT22_AANTAL op 1 (niet beide) voor deze test.
#endif
#if SENSOR_DHT22_AANTAL == 1
const GedeeldeBusComponent fysiekType = GedeeldeBusComponent::DHT22;
#else
const GedeeldeBusComponent fysiekType = GedeeldeBusComponent::DHT11;
#endif

DHTxx* sensor = nullptr;

void ToonResultaat(const __FlashStringHelper* stap, bool gelukt) {
  GA_SERIAL.print(stap);
  GA_SERIAL.print(F(": "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_TYPE_NRF24L01_SPI);
  ToonResultaat(F("Data aanmelden/controleren/inpluggen/activeren"), Data != nullptr);
  if (Data == nullptr || Data->NRF24L01viaSPI[0] == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  sensor = GedeeldeBusNewComponent<DHTxx>(&Native, fysiekType, HardwareResourcePin::NP_D2);
  ToonResultaat(F("Sensor aanmelden/controleren/inpluggen/activeren"), sensor != nullptr);
  if (sensor == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  Data->StreamingStart();
  GA_SERIAL.println(F("RESULTAAT: OK"));
}

void loop() {
  if (sensor == nullptr) return;
  float temperatuur;
  // Een geslaagde meting schrijft temperatuur en vochtigheid naar Data, en dus ook naar de radio.
  if (sensor->LeesHoofdwaarde(temperatuur)) {
    GA_SERIAL.print(F("Verstuurd: "));
    GA_SERIAL.println(temperatuur);
  } else {
    GA_SERIAL.println(F("Meting: FOUT"));
  }
  delay(2000);   // DHT22: hooguit één meting per 2 seconden
}
