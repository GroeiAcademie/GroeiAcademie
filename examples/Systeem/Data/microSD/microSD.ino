// ============================================================================
// Data: microSD
// ============================================================================
// Basis-lifecycletest voor Data microSD SPI.
// ============================================================================

#include <GroeiAcademie.h>

#if !((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  #error Voeg DATA_TYPE_MICROSD_SPI toe aan DATA_LAYER_CONFIG voor deze test.
#endif

#if DATA_LAYER_MICROSD_SPI_AANTAL < 1
  #error Zet DATA_LAYER_MICROSD_SPI_AANTAL op minstens 1 voor deze test.
#endif

#ifdef DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER
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

#ifdef DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER
  ToonResultaat(F("Extender voor CS"), extenderCS.aanmelden() && extenderCS.controleren() && extenderCS.inpluggen() && extenderCS.activeren());
#endif

  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>();
  ToonResultaat(F("Data aanmelden/controleren/inpluggen/activeren"), Data != nullptr);

  if (Data == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  struct DataMICROSDviaSPI* microSD = Data->MICROSDviaSPI[0];

  if (microSD == nullptr) {
    GA_SERIAL.println(F("microSD: FOUT"));
    return;
  }

  File32 myFile = microSD->open("test.txt", O_RDWR | O_CREAT | O_AT_END);
  if (myFile) {
    myFile.println("testing 1, 2, 3.");
    myFile.close();
  } else {
    GA_SERIAL.println(F("Schrijven: FOUT"));
    return;
  }

  myFile = microSD->open("test.txt", O_RDONLY);
  if (myFile) {
    while (myFile.available()) {
      GA_SERIAL.write(myFile.read());
    }
    myFile.close();
  } else {
    GA_SERIAL.println(F("Lezen: FOUT"));
  }
}

void loop() {}
