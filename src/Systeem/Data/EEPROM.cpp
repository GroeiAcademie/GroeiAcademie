#include "Data.h"

// ============================================================================
// EEPROM NATIVE
// ============================================================================
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_NATIVE) && BOARD_VERSION != BOARD_UNO_Q

DataEEPROMviaNative::~DataEEPROMviaNative() {
  struct Data* data = static_cast<struct Data*>(parent);   // de ouder (Data) van deze chip
  if (data != nullptr && data->EEPROMviaNative == this) data->EEPROMviaNative = nullptr;
}

bool DataEEPROMviaNative::Activeren() {
  // ESP32 en RP2040: zelfde totale grootte als Input, zodat het blok van HX1838 behouden blijft.
#if BOARD_VERSION == BOARD_ESP32_D1_UNO_R32 || BOARD_VERSION == BOARD_ESP32S3_ARDI32 || BOARD_VERSION == BOARD_ESP32S3_DEV
  if (!EEPROM.begin(HX1838_EEPROM_NATIVE_SIZE + DATA_LAYER_EEPROM_NATIVE_SIZE)) return false;
#elif BOARD_VERSION == BOARD_RP2040_CYTRON_MAKER_UNO
  EEPROM.begin(HX1838_EEPROM_NATIVE_SIZE + DATA_LAYER_EEPROM_NATIVE_SIZE);
#endif
  // Past het blok van HX1838 en de ruimte van Data in de native EEPROM van dit board?
  return (HX1838_EEPROM_NATIVE_SIZE + DATA_LAYER_EEPROM_NATIVE_SIZE) <= EEPROM.length();
}

// adres 0 = het eerste byte na het blok van HX1838.
uint8_t Data::ReadNativeEEPROM(uint16_t adres) {
  if (EEPROMviaNative == nullptr || adres >= DATA_LAYER_EEPROM_NATIVE_SIZE) return 0;
  return EEPROM.read(HX1838_EEPROM_NATIVE_SIZE + adres);
}

void Data::WriteNativeEEPROM(uint16_t adres, uint8_t waarde) {
  if (EEPROMviaNative == nullptr || adres >= DATA_LAYER_EEPROM_NATIVE_SIZE) return;
  const int positie = HX1838_EEPROM_NATIVE_SIZE + adres;   // echt adres in de native EEPROM
#if BOARD_VERSION == BOARD_ESP32_D1_UNO_R32 || BOARD_VERSION == BOARD_ESP32S3_ARDI32 || BOARD_VERSION == BOARD_ESP32S3_DEV || BOARD_VERSION == BOARD_RP2040_CYTRON_MAKER_UNO
  // Alleen schrijven wanneer de waarde verandert; meteen commit(), want een volgende begin() (bv. van Input) leest het geheugen opnieuw in.
  if (EEPROM.read(positie) == waarde) return;
  EEPROM.write(positie, waarde);
  EEPROM.commit();
#else
  EEPROM.update(positie, waarde);   // schrijft alleen wanneer de waarde verandert: minder slijtage
#endif
}

#elif ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_NATIVE) && BOARD_VERSION == BOARD_UNO_Q

// UNO Q: geen EEPROM.h in de core, het medium bestaat niet (D8). Lezen geeft 0, schrijven doet niets.
uint8_t Data::ReadNativeEEPROM(uint16_t adres) { (void)adres; return 0; }
void Data::WriteNativeEEPROM(uint16_t adres, uint8_t waarde) { (void)adres; (void)waarde; }

#endif

// ============================================================================
// EEPROM I2C
// ============================================================================
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_I2C)

DataEEPROMviaI2C::~DataEEPROMviaI2C() {
  struct Data* data = static_cast<struct Data*>(parent);   // de ouder (Data) van deze chip
  if (data == nullptr) return;
  for (uint8_t teller = 0; teller < DATA_LAYER_EEPROM_I2C_AANTAL; teller++) {
    if (data->EEPROMviaI2C[teller] == this) data->EEPROMviaI2C[teller] = nullptr;
  }
}

bool DataEEPROMviaI2C::Activeren() {
  // I2C_eeprom heeft geen functie die een foutcode teruggeeft: fout = 0.
  const bool gelukt = I2C_eeprom::begin() && I2C_eeprom::isConnected();
  return FoutDiagnoseI2C(gelukt, 0, gelukt);
}

#endif
