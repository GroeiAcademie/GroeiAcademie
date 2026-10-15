// ============================================================================
// Data: FRAM
// ============================================================================
// Basis-lifecycletest voor Data FRAM met MB85RC256V.
// ============================================================================

#include <GroeiAcademie.h>

#if !((DATA_LAYER_CONFIG) & DATA_TYPE_FRAM)
  #error Voeg DATA_TYPE_FRAM toe aan DATA_LAYER_CONFIG voor deze test.
#endif

#if DATA_LAYER_FRAM_AANTAL < 1
  #error Zet DATA_LAYER_FRAM_AANTAL op minstens 1 voor deze test.
#endif

struct FRAM_I2C* FRAM_I2C = nullptr;

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
    if (sameParent->component == GedeeldeBusComponent::DATA_FRAM) {
      FRAM_I2C = static_cast<struct FRAM_I2C*>(sameParent);
      break;
    }
  }

  if (FRAM_I2C == nullptr) {
    GA_SERIAL.println(F("FRAM_I2C: FOUT"));
    return;
  }

  GA_SERIAL.print(F("Grootte bytes: "));
  GA_SERIAL.println(FRAM_I2C->getSizeBytes());

  if (FRAM_I2C->getSizeBytes() != FRAM_MB85RC256V) {
    GA_SERIAL.println(F("Grootte: FOUT"));
    return;
  }

  const uint8_t geschrevenWaarde = 123;
  FRAM_I2C->write8(0, geschrevenWaarde);
  const int schrijfStatus = FRAM_I2C->lastError();

  if (schrijfStatus != FRAM_OK) {
    GA_SERIAL.print(F("Schrijven: FOUT "));
    GA_SERIAL.println(schrijfStatus);
    return;
  }

  const uint8_t gelezenWaarde = FRAM_I2C->read8(0);
  const int leesStatus = FRAM_I2C->lastError();

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
