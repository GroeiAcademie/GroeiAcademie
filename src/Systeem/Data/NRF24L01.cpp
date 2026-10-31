#include "Data.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_NRF24L01_SPI)
// Fabrikant: https://www.nordicsemi.com/Products/nRF24-series
// Official API: https://nrf24.github.io/RF24/

static_assert(sizeof(DATA_LAYER_NRF24L01_SPI_1_ADRES) == 6, "DATA_LAYER_NRF24L01_SPI_1_ADRES moet precies 5 tekens zijn.");

DataNRF24L01viaSPI::~DataNRF24L01viaSPI() {
  struct Data* data = static_cast<struct Data*>(parent);   // de ouder (Data) van deze chip
  if (data == nullptr) return;
  for (uint8_t teller = 0; teller < DATA_LAYER_NRF24L01_SPI_AANTAL; teller++) {
    if (data->NRF24L01viaSPI[teller] == this) data->NRF24L01viaSPI[teller] = nullptr;
  }
}

bool DataNRF24L01viaSPI::Activeren() {
  if (np_CE == HardwareResourcePin::NONE) return false;
  // begin() en isChipConnected() lezen registers terug: zonder module geen geldige antwoorden.
  const bool gelukt = RF24::begin() && RF24::isChipConnected();
  if (gelukt) {
    RF24::setChannel(DATA_LAYER_NRF24L01_SPI_1_RF_CHANNEL);
    RF24::setPayloadSize(15);   // één GB-record van 15 bytes per pakket
    RF24::setAutoAck(false);
    RF24::openWritingPipe(reinterpret_cast<const uint8_t*>(DATA_LAYER_NRF24L01_SPI_1_ADRES));
    RF24::stopListening();
  }
  return FoutDiagnoseSPI(gelukt, gelukt ? 0 : 1, gelukt);
}

#endif
