// ============================================================================
// Data: FLASH
// ============================================================================
// Basis-lifecycletest voor Data FLASH SPI.
// ============================================================================

#include <GroeiAcademie.h>

#if !((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
  #error Voeg DATA_TYPE_FLASH_SPI toe aan DATA_LAYER_CONFIG voor deze test.
#endif

#if DATA_LAYER_FLASH_SPI_AANTAL < 1
  #error Zet DATA_LAYER_FLASH_SPI_AANTAL op minstens 1 voor deze test.
#endif

#ifdef DATA_LAYER_FLASH_SPI_1_CS_EXTENDER
// CS via een Extender: de gekozen Extender moet werken vóór Data.
GedeeldeBusNode* extenderCS = nullptr;
#endif

void ToonResultaat(const __FlashStringHelper* stap, bool gelukt) {
  GA_SERIAL.print(stap);
  GA_SERIAL.print(F(": "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: FLASH"));

#ifdef DATA_LAYER_FLASH_SPI_1_CS_EXTENDER
#if EXTENDER_PCF8574_AANTAL > 0
  if (DATA_LAYER_FLASH_SPI_1_CS_EXTENDER == GedeeldeBusComponent::DIGITAL_PINS_PCF8574) extenderCS = GedeeldeBusNewComponent<ExtenderPCF8574>(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574, I2C_ADDRESS_EXTENDER_PCF8574_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8574_1_INT);
#endif

#if EXTENDER_PCF8575_AANTAL > 0
  if (DATA_LAYER_FLASH_SPI_1_CS_EXTENDER == GedeeldeBusComponent::DIGITAL_PINS_PCF8575) extenderCS = GedeeldeBusNewComponent<ExtenderPCF8575>(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8575, I2C_ADDRESS_EXTENDER_PCF8575_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8575_1_INT);
#endif

#if EXTENDER_MCP23017_AANTAL > 0
  if (DATA_LAYER_FLASH_SPI_1_CS_EXTENDER == GedeeldeBusComponent::DIGITAL_PINS_MCP23017) extenderCS = GedeeldeBusNewComponent<ExtenderMCP23017>(&Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23017, I2C_ADDRESS_EXTENDER_MCP23017_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_MCP23017_1_INTA, EXTENDER_MCP23017_1_INTB, EXTENDER_MCP23017_1_RESET);
#endif

  ToonResultaat(F("Extender voor CS"), extenderCS != nullptr);
#endif

  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>();
  ToonResultaat(F("Data aanmelden/controleren/inpluggen/activeren"), Data != nullptr);

  if (Data == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  if (Data->FLASHviaSPI[0] == nullptr) {
    GA_SERIAL.println(F("FLASH: FOUT"));
    return;
  }

  const int geschrevenWaarde = 123;

  if (!Data->FLASHviaSPI[0]->eraseSector(0)) {
    GA_SERIAL.println(F("Wissen: FOUT"));
    return;
  }

  const uint32_t adres = 0;
  if (Data->FLASHviaSPI[0]->writeBuffer(adres, reinterpret_cast<const uint8_t*>(&geschrevenWaarde), sizeof(geschrevenWaarde)) != sizeof(geschrevenWaarde)) {
    GA_SERIAL.println(F("Schrijven: FOUT"));
    return;
  }

  int gelezenWaarde = 0;
  if (Data->FLASHviaSPI[0]->readBuffer(adres, reinterpret_cast<uint8_t*>(&gelezenWaarde), sizeof(gelezenWaarde)) != sizeof(gelezenWaarde)) {
    GA_SERIAL.println(F("Lezen: FOUT"));
    return;
  }

  GA_SERIAL.print(F("Geschreven: "));
  GA_SERIAL.println(geschrevenWaarde);
  GA_SERIAL.print(F("Gelezen: "));
  GA_SERIAL.println(gelezenWaarde);

  GA_SERIAL.println(gelezenWaarde == geschrevenWaarde ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {}
