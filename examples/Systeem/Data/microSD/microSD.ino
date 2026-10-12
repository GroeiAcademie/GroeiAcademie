// ============================================================================
// Data: microSD
// ============================================================================
// Basis-lifecycletest voor Data microSD.
// ============================================================================

#include <GroeiAcademie.h>

#if !((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  #error Voeg DATA_TYPE_MICROSD toe aan DATA_LAYER_CONFIG voor deze test.
#endif

#if DATA_LAYER_MICROSD_AANTAL < 1
  #error Zet DATA_LAYER_MICROSD_AANTAL op minstens 1 voor deze test.
#endif

struct microSD* microSD = nullptr;

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
    if (sameParent->component == GedeeldeBusComponent::DATA_MICROSD) {
      microSD = static_cast<struct microSD*>(sameParent);
      break;
    }
  }

  if (microSD == nullptr) {
    GA_SERIAL.println(F("microSD: FOUT"));
    return;
  }

  File myFile = microSD->open("test.txt", FILE_WRITE);
  if (myFile) {
    myFile.println("testing 1, 2, 3.");
    myFile.close();
  } else {
    GA_SERIAL.println(F("Schrijven: FOUT"));
    return;
  }

  myFile = microSD->open("test.txt");
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
