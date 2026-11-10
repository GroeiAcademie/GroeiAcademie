// ============================================================================
// Data: NRF24L01 (ontvanger)
// ============================================================================
// Tweede Arduino: ontvangt de GB-records van de zender (voorbeeld NRF24L01) en toont ze.
// Deze sketch gebruikt de RF24-library rechtstreeks: Data hoeft hier niet actief te zijn.
// Radiokanaal en adres moeten gelijk zijn aan DATA_LAYER_NRF24L01_SPI_1_RF_CHANNEL en
// DATA_LAYER_NRF24L01_SPI_1_ADRES van de zender (standaard 76 en "GROEI").
// ============================================================================

#include <GroeiAcademie.h>
#include <RF24.h>

const uint8_t pinCE = 5;   // vrije pinnen; pas aan aan de eigen bedrading
const uint8_t pinCS = 6;
const uint8_t radiokanaal = 76;
const uint8_t adres[6] = "GROEI";

RF24 radio(pinCE, pinCS);

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: NRF24L01_Ontvanger"));

  const bool gelukt = radio.begin() && radio.isChipConnected();
  GA_SERIAL.println(gelukt ? F("RADIO: OK") : F("RADIO: FOUT"));
  if (!gelukt) return;
  radio.setChannel(radiokanaal);
  radio.setPayloadSize(15);   // één GB-record per pakket
  radio.setAutoAck(false);    // zoals de zender: geen ontvangstbevestiging
  radio.openReadingPipe(1, adres);
  radio.startListening();
}

void loop() {
  if (!radio.available()) return;
  uint8_t record[15];
  radio.read(record, sizeof(record));
  uint8_t controle = 0;
  for (uint8_t index = 0; index < 14; ++index) controle ^= record[index];
  if (record[0] != 0x47 || record[1] != 0x42 || controle != record[14]) {
    GA_SERIAL.println(F("Ongeldig record"));
    return;
  }
  const uint16_t component = static_cast<uint16_t>(record[2] | (record[3] << 8));
  GA_SERIAL.print(F("Component 0x"));
  GA_SERIAL.print(component, HEX);
  GA_SERIAL.print(F(", channel "));
  GA_SERIAL.print(record[8]);
  GA_SERIAL.print(F(": "));
  if (record[9] == 1) {
    float waarde;
    memcpy(&waarde, record + 10, 4);
    GA_SERIAL.println(waarde);
  } else if (record[9] == 2) {
    uint32_t waarde;
    memcpy(&waarde, record + 10, 4);
    GA_SERIAL.println(waarde);
  } else {
    int32_t waarde;
    memcpy(&waarde, record + 10, 4);
    GA_SERIAL.println(waarde);
  }
}
