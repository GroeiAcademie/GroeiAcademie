// ============================================================================
// Data: EEPROM
// ============================================================================
// Basis-lifecycletest voor Data EEPROM: EEPROM I2C en/of EEPROM NATIVE (native EEPROM van het board).
// ============================================================================

#include <GroeiAcademie.h>

#if !((DATA_LAYER_CONFIG) & (DATA_TYPE_EEPROM_I2C | DATA_TYPE_EEPROM_NATIVE))
  #error Voeg DATA_TYPE_EEPROM_I2C en/of DATA_TYPE_EEPROM_NATIVE toe aan DATA_LAYER_CONFIG voor deze test.
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

  const uint8_t geschrevenWaarde = 123;

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_I2C)
  if (Data->EEPROMviaI2C[0] == nullptr) {
    GA_SERIAL.println(F("EEPROM I2C: FOUT"));
    return;
  }

  const int schrijfStatus = Data->EEPROMviaI2C[0]->writeByte(0, geschrevenWaarde);

  if (schrijfStatus != 0) {
    GA_SERIAL.print(F("Schrijven: FOUT "));
    GA_SERIAL.println(schrijfStatus);
    return;
  }

  const uint8_t gelezenWaarde = Data->EEPROMviaI2C[0]->readByte(0);

  GA_SERIAL.print(F("EEPROM I2C geschreven: "));
  GA_SERIAL.println(geschrevenWaarde);
  GA_SERIAL.print(F("EEPROM I2C gelezen: "));
  GA_SERIAL.println(gelezenWaarde);
  GA_SERIAL.println(gelezenWaarde == geschrevenWaarde ? F("RESULTAAT EEPROM I2C: OK") : F("RESULTAAT EEPROM I2C: FOUT"));
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_NATIVE)
  Data->WriteNativeEEPROM(0, geschrevenWaarde);
  const uint8_t readNative = Data->ReadNativeEEPROM(0);   // gelezen waarde uit de native EEPROM

  GA_SERIAL.print(F("EEPROM NATIVE geschreven: "));
  GA_SERIAL.println(geschrevenWaarde);
  GA_SERIAL.print(F("EEPROM NATIVE gelezen: "));
  GA_SERIAL.println(readNative);
  GA_SERIAL.println(readNative == geschrevenWaarde ? F("RESULTAAT EEPROM NATIVE: OK") : F("RESULTAAT EEPROM NATIVE: FOUT"));
#endif
}

void loop() {}
