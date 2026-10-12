// ============================================================================
// Data: EEPROM
// ============================================================================
// Basis-lifecycletest voor Data EEPROM.
// ============================================================================

#include <GroeiAcademie.h>

#if !((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM)
  #error Voeg DATA_TYPE_EEPROM toe aan DATA_LAYER_CONFIG voor deze test.
#endif

#if DATA_LAYER_EEPROM_AANTAL < 1
  #error Zet DATA_LAYER_EEPROM_AANTAL op minstens 1 voor deze test.
#endif

struct EEPROM* EEPROM = nullptr;

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

  for (GedeeldeBusNode* sameParent = Data->firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
    if (sameParent->component == GedeeldeBusComponent::DATA_EEPROM) {
      EEPROM = static_cast<struct EEPROM*>(sameParent);
      break;
    }
  }

  if (EEPROM == nullptr) {
    GA_SERIAL.println(F("EEPROM: FOUT"));
    return;
  }

  const uint8_t geschrevenWaarde = 123;
  const int schrijfStatus = EEPROM->writeByte(0, geschrevenWaarde);

  if (schrijfStatus != 0) {
    GA_SERIAL.print(F("Schrijven: FOUT "));
    GA_SERIAL.println(schrijfStatus);
    return;
  }

  const uint8_t gelezenWaarde = EEPROM->readByte(0);

  GA_SERIAL.print(F("Geschreven: "));
  GA_SERIAL.println(geschrevenWaarde);
  GA_SERIAL.print(F("Gelezen: "));
  GA_SERIAL.println(gelezenWaarde);
  GA_SERIAL.println(gelezenWaarde == geschrevenWaarde ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {}
