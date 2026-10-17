// ============================================================================
// Data: FRAM
// ============================================================================
// Basis-lifecycletest voor Data FRAM I2C met MB85RC256V.
// ============================================================================

#include <GroeiAcademie.h>

#if !((DATA_LAYER_CONFIG) & DATA_TYPE_FRAM_I2C)
  #error Voeg DATA_TYPE_FRAM_I2C toe aan DATA_LAYER_CONFIG voor deze test.
#endif

#if DATA_LAYER_FRAM_I2C_AANTAL < 1
  #error Zet DATA_LAYER_FRAM_I2C_AANTAL op minstens 1 voor deze test.
#endif

void ToonResultaat(const __FlashStringHelper* stap, bool gelukt) {
  GA_SERIAL.print(stap);
  GA_SERIAL.print(F(": "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>();
  ToonResultaat(F("Data aanmelden/controleren/inpluggen/activeren"), Data != nullptr);

  if (Data == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  if (Data->FRAMviaI2C[0] == nullptr) {
    GA_SERIAL.println(F("FRAM I2C: FOUT"));
    return;
  }

  GA_SERIAL.print(F("Grootte bytes: "));
  GA_SERIAL.println(Data->FRAMviaI2C[0]->getSizeBytes());

  if (Data->FRAMviaI2C[0]->getSizeBytes() != FRAM_MB85RC256V) {
    GA_SERIAL.println(F("Grootte: FOUT"));
    return;
  }

  const uint8_t geschrevenWaarde = 123;
  Data->FRAMviaI2C[0]->write8(0, geschrevenWaarde);
  const int schrijfStatus = Data->FRAMviaI2C[0]->lastError();

  if (schrijfStatus != FRAM_OK) {
    GA_SERIAL.print(F("Schrijven: FOUT "));
    GA_SERIAL.println(schrijfStatus);
    return;
  }

  const uint8_t gelezenWaarde = Data->FRAMviaI2C[0]->read8(0);
  const int leesStatus = Data->FRAMviaI2C[0]->lastError();

  if (leesStatus != FRAM_OK) {
    GA_SERIAL.print(F("Lezen: FOUT "));
    GA_SERIAL.println(leesStatus);
    return;
  }

  GA_SERIAL.print(F("Geschreven: "));
  GA_SERIAL.println(geschrevenWaarde);
  GA_SERIAL.print(F("Gelezen: "));
  GA_SERIAL.println(gelezenWaarde);
  GA_SERIAL.println(gelezenWaarde == geschrevenWaarde ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {}
