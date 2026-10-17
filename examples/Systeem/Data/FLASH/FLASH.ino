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
// CS via een Extender: die Extender moet bestaan en werken vóór Data. Deze test gebruikt PCF8574 #1 (EXTENDER_PCF8574_AANTAL >= 1).
ExtenderPCF8574 extenderCS(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574, I2C_ADDRESS_EXTENDER_PCF8574_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8574_1_INT);   // NAAM VOORLOPIG: de Extender met CS
#endif

void ToonResultaat(const __FlashStringHelper* stap, bool gelukt) {
  GA_SERIAL.print(stap);
  GA_SERIAL.print(F(": "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

#ifdef DATA_LAYER_FLASH_SPI_1_CS_EXTENDER
  ToonResultaat(F("Extender voor CS"), extenderCS.aanmelden() && extenderCS.controleren() && extenderCS.inpluggen() && extenderCS.activeren());
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
