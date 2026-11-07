#include "Data.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)

// ============================================================================
// TransportFLASHviaSPI
// ============================================================================
void TransportFLASHviaSPI::begin(void) {
#if GEDEELDE_BUS_MIXED_PINS
  if (extenderCS == nullptr)   // alleen bij CS op het board; met CS op een Extender heeft HardwareResourceTypeSPI::inpluggen() CS al als uitgang en HIGH gezet
#endif
  pinMode(pinCS, OUTPUT);
  WriteCS(HIGH);   // begin() heeft geen foutresultaat; elke transactie controleert CS opnieuw.
  InitialiserenGedeeldeBus(GedeeldeBusType::SPI);
}

void TransportFLASHviaSPI::end(void) {
  // De SPI-bus is gedeeld: niet stoppen.
}

bool TransportFLASHviaSPI::runCommand(uint8_t command) {
  SPI.beginTransaction(SPISettings(WriteClock, MSBFIRST, SPI_MODE0));
  if (!WriteCS(HIGH) || !WriteCS(LOW)) { SPI.endTransaction(); return false; }
  SPI.transfer(command);
  const bool gelukt = WriteCS(HIGH);
  SPI.endTransaction();
  return gelukt;
}

bool TransportFLASHviaSPI::readCommand(uint8_t command, uint8_t* response, uint32_t len) {
  SPI.beginTransaction(SPISettings(ReadClock, MSBFIRST, SPI_MODE0));
  if (!WriteCS(HIGH) || !WriteCS(LOW)) { SPI.endTransaction(); return false; }
  SPI.transfer(command);
  while (len--) *response++ = SPI.transfer(0xFF);
  const bool gelukt = WriteCS(HIGH);
  SPI.endTransaction();
  return gelukt;
}

bool TransportFLASHviaSPI::writeCommand(uint8_t command, uint8_t const* data, uint32_t len) {
  SPI.beginTransaction(SPISettings(WriteClock, MSBFIRST, SPI_MODE0));
  if (!WriteCS(HIGH) || !WriteCS(LOW)) { SPI.endTransaction(); return false; }
  SPI.transfer(command);
  while (len--) SPI.transfer(*data++);
  const bool gelukt = WriteCS(HIGH);
  SPI.endTransaction();
  return gelukt;
}

// bitsShifted: aantal bits waarover het adres naar rechts schuift, om de adresbytes van hoog naar laag te versturen.
bool TransportFLASHviaSPI::eraseCommand(uint8_t command, uint32_t addr) {
  SPI.beginTransaction(SPISettings(WriteClock, MSBFIRST, SPI_MODE0));
  if (!WriteCS(HIGH) || !WriteCS(LOW)) { SPI.endTransaction(); return false; }
  SPI.transfer(command);
  for (int8_t bitsShifted = (_addr_len - 1) * 8; bitsShifted >= 0; bitsShifted -= 8) SPI.transfer((uint8_t)(addr >> bitsShifted));
  const bool gelukt = WriteCS(HIGH);
  SPI.endTransaction();
  return gelukt;
}

bool TransportFLASHviaSPI::readMemory(uint32_t addr, uint8_t* data, uint32_t len) {
  SPI.beginTransaction(SPISettings(ReadClock, MSBFIRST, SPI_MODE0));
  if (!WriteCS(HIGH) || !WriteCS(LOW)) { SPI.endTransaction(); return false; }
  SPI.transfer(_cmd_read);
  for (int8_t bitsShifted = (_addr_len - 1) * 8; bitsShifted >= 0; bitsShifted -= 8) SPI.transfer((uint8_t)(addr >> bitsShifted));
  if (_cmd_read == SFLASH_CMD_FAST_READ) SPI.transfer(0xFF);   // Fast Read heeft één extra dummy-byte
  while (len--) *data++ = SPI.transfer(0xFF);
  const bool gelukt = WriteCS(HIGH);
  SPI.endTransaction();
  return gelukt;
}

bool TransportFLASHviaSPI::writeMemory(uint32_t addr, uint8_t const* data, uint32_t len) {
  SPI.beginTransaction(SPISettings(WriteClock, MSBFIRST, SPI_MODE0));
  if (!WriteCS(HIGH) || !WriteCS(LOW)) { SPI.endTransaction(); return false; }
  SPI.transfer(SFLASH_CMD_PAGE_PROGRAM);
  for (int8_t bitsShifted = (_addr_len - 1) * 8; bitsShifted >= 0; bitsShifted -= 8) SPI.transfer((uint8_t)(addr >> bitsShifted));
  while (len--) SPI.transfer(*data++);
  const bool gelukt = WriteCS(HIGH);
  SPI.endTransaction();
  return gelukt;
}

// ============================================================================
// DataFLASHviaSPI
// ============================================================================
DataFLASHviaSPI::~DataFLASHviaSPI() {
  struct Data* data = static_cast<struct Data*>(parent);   // de ouder (Data) van deze chip
  if (data == nullptr) return;
  for (uint8_t teller = 0; teller < DATA_LAYER_FLASH_SPI_AANTAL; teller++) {
    if (data->FLASHviaSPI[teller] == this) data->FLASHviaSPI[teller] = nullptr;
  }
}

bool DataFLASHviaSPI::Activeren() {
  // Adafruit_SPIFlashBase heeft geen foutcode: fout = 0. Teruglezen: de JEDEC-ID moet bekend zijn (niet 0 en niet 0xFFFFFF).
  const bool gelukt = Adafruit_SPIFlashBase::begin();
  const uint32_t id = gelukt ? Adafruit_SPIFlashBase::getJEDECID() : 0;   // JEDEC-ID = fabrikant- en typecode van de chip
  return FoutDiagnoseSPI(gelukt, 0, gelukt && id != 0 && id != 0xFFFFFFUL);
}

#endif
