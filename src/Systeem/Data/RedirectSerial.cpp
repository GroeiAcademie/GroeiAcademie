#include "../../Configuratie/SystemConfig.h"

#if REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

#include "RedirectSerial.h"
#include "Data.h"
#include "microSD.h"

namespace {
  File32 FileSerial;
  bool initialiseren = false;
}

void RedirectSerial::begin(unsigned long) {
  if (FileSerial || initialiseren) return;

  initialiseren = true;

  if (::Data == nullptr) {
    ::Data = new struct Data();
    ::Data->componentCreated = true;

    const bool geslaagd = static_cast<GedeeldeBusNode*>(::Data)->aanmelden() &&
                          static_cast<GedeeldeBusNode*>(::Data)->controleren() &&
                          static_cast<GedeeldeBusNode*>(::Data)->inpluggen() &&
                          static_cast<GedeeldeBusNode*>(::Data)->activeren();
    GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(static_cast<GedeeldeBusNode*>(::Data));

    if (!geslaagd) {
      struct Data* object = ::Data;
      if (object->aangemeld) static_cast<GedeeldeBusNode*>(object)->afmelden();
      else delete object;
      ::Data = nullptr;
    }
  }

  // Rechtstreeks de eerste microSD uit de lijst van Data (N7), zonder zoeken op component.
  if (::Data != nullptr && ::Data->MICROSDviaSPI[0] != nullptr && ::Data->MICROSDviaSPI[0]->actief) {
    FileSerial = ::Data->MICROSDviaSPI[0]->open(REDIRECT_SERIAL_FILE, O_RDWR | O_CREAT | O_AT_END);   // achteraan bijschrijven
  }

  initialiseren = false;
}

RedirectSerial::operator bool() {
  return !initialiseren && static_cast<bool>(FileSerial);
}

size_t RedirectSerial::write(uint8_t byte) {
  if (initialiseren) return 0;
  if (!FileSerial) begin(0);
  if (!FileSerial) return 0;
  const size_t geschreven = FileSerial.write(byte);
  if (byte == '\n') FileSerial.flush();
  return geschreven;
}

#endif
