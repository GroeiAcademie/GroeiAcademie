#include "../../Configuratie/SystemConfig.h"

#if REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

#include "RedirectSerial.h"
#include "Data.h"
#include "microSD.h"

namespace {
  File FileSerial;
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

  if (::Data != nullptr) {
    for (GedeeldeBusNode* sameParent = ::Data->firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component != GedeeldeBusComponent::DATA_MICROSD || !sameParent->actief) continue;

      struct DataMicroSD* kanaal = static_cast<struct DataMicroSD*>(sameParent);
      if (kanaal->np_CS != DATA_LAYER_MICROSD_1_CS_PIN) continue;

#if defined(ARDUINO_ARCH_ESP32)
      FileSerial = kanaal->open(REDIRECT_SERIAL_FILE, FILE_APPEND);
#else
      FileSerial = kanaal->open(REDIRECT_SERIAL_FILE, FILE_WRITE);
#endif
      break;
    }
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
