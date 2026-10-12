// ============================================================================
// Data: FLASH
// ============================================================================
// Basis-lifecycletest voor Data FLASH.
// ============================================================================

#include <GroeiAcademie.h>

#if !((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  #error Voeg DATA_TYPE_FLASH toe aan DATA_LAYER_CONFIG voor deze test.
#endif

#if DATA_LAYER_FLASH_AANTAL < 1
  #error Zet DATA_LAYER_FLASH_AANTAL op minstens 1 voor deze test.
#endif

struct DataFLASH* FLASH = nullptr;

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
    if (sameParent->component == GedeeldeBusComponent::DATA_FLASH) {
      FLASH = static_cast<struct DataFLASH*>(sameParent);
      break;
    }
  }

  if (FLASH == nullptr) {
    GA_SERIAL.println(F("FLASH: FOUT"));
    return;
  }

  const int geschrevenWaarde = 123;

  if (!FLASH->eraseSector(0)) {
    GA_SERIAL.println(F("Wissen: FOUT"));
    return;
  }

  uint32_t adres = 0;
  if (!FLASH->writeAnything(adres, geschrevenWaarde)) {
    GA_SERIAL.println(F("Schrijven: FOUT"));
    return;
  }

  int gelezenWaarde = 0;
  if (!FLASH->readAnything(adres, gelezenWaarde)) {
    GA_SERIAL.println(F("Lezen: FOUT"));
    return;
  }

  GA_SERIAL.print(F("Geschreven: "));
  GA_SERIAL.println(geschrevenWaarde);
  GA_SERIAL.print(F("Gelezen: "));
  GA_SERIAL.println(gelezenWaarde);

  GA_SERIAL.println(F("RESULTAAT: OK"));
}

void loop() {}
