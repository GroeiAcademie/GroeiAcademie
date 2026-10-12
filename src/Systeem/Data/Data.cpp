#include "Data.h"

#if (DATA_LAYER_CONFIG) != DATA_TYPE_NONE || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
namespace {
  File File[2];
}
#endif

Data::Data()
#if REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
  : Data(((DATA_LAYER_CONFIG) & ~DATA_TYPE_NONE) | DATA_TYPE_MICROSD)
#else
  : Data(DATA_LAYER_CONFIG)
#endif
{}

Data::Data(uint8_t typesActief)
  : GedeeldeBusNode(&Native, { nullptr, 0, nullptr, 0 }, GedeeldeBusComponent::GC_DATA, HardwareResourceToegang::GEDEELD),
#if REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
    typesActief((typesActief & ~DATA_TYPE_NONE) | DATA_TYPE_MICROSD)
#else
    typesActief(typesActief)
#endif
{}

struct Data* Data = nullptr;

bool Data::aanmelden() {
#ifdef TRACE
  GA_SERIAL.println("TRACE: Data::aanmelden()");
#endif
  if (aangemeld) return true;
#if REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
  if ((this->typesActief & (((DATA_LAYER_CONFIG) & ~DATA_TYPE_NONE) | DATA_TYPE_MICROSD)) != this->typesActief) return false;
#else
  if ((this->typesActief & DATA_LAYER_CONFIG) != this->typesActief) return false;
#endif
  if (!GedeeldeBusNode::aanmelden()) return false;

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM)
  if (this->typesActief & DATA_TYPE_EEPROM) {
    #if DATA_LAYER_EEPROM_AANTAL >= 1
      {
        struct EEPROM* kanaal = new struct EEPROM(this, DATA_LAYER_EEPROM_1_I2C_ADDRESS, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (!kanaal->aanmelden()) delete kanaal;
      }
    #endif
    #if DATA_LAYER_EEPROM_AANTAL >= 2
      {
        struct EEPROM* kanaal = new struct EEPROM(this, DATA_LAYER_EEPROM_2_I2C_ADDRESS, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (!kanaal->aanmelden()) delete kanaal;
      }
    #endif
  }
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
  if (this->typesActief & DATA_TYPE_MICROSD) {
    #if DATA_LAYER_MICROSD_AANTAL >= 1
      {
        struct microSD* kanaal = new struct microSD(this, DATA_LAYER_MICROSD_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (!kanaal->aanmelden()) delete kanaal;
      }
    #endif
    #if DATA_LAYER_MICROSD_AANTAL >= 2
      {
        struct microSD* kanaal = new struct microSD(this, DATA_LAYER_MICROSD_2_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (!kanaal->aanmelden()) delete kanaal;
      }
    #endif
  }
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  if (this->typesActief & DATA_TYPE_FLASH) {
    #if DATA_LAYER_FLASH_AANTAL >= 1
      {
        struct FLASH* kanaal = new struct FLASH(this, DATA_LAYER_FLASH_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (!kanaal->aanmelden()) delete kanaal;
      }
    #endif
    #if DATA_LAYER_FLASH_AANTAL >= 2
      {
        struct FLASH* kanaal = new struct FLASH(this, DATA_LAYER_FLASH_2_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (!kanaal->aanmelden()) delete kanaal;
      }
    #endif
  }
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM)
  if (this->typesActief & DATA_TYPE_EEPROM) {
    uint8_t teller = 0;
    for (GedeeldeBusNode* sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component == GedeeldeBusComponent::DATA_EEPROM) teller++;
    }
    if (teller != DATA_LAYER_EEPROM_AANTAL) return false;
  }
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
  if (this->typesActief & DATA_TYPE_MICROSD) {
    uint8_t teller = 0;
    for (GedeeldeBusNode* sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component == GedeeldeBusComponent::DATA_MICROSD) teller++;
    }
    if (teller != DATA_LAYER_MICROSD_AANTAL) return false;
  }
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  if (this->typesActief & DATA_TYPE_FLASH) {
    uint8_t teller = 0;
    for (GedeeldeBusNode* sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component == GedeeldeBusComponent::DATA_FLASH) teller++;
    }
    if (teller != DATA_LAYER_FLASH_AANTAL) return false;
  }
#endif

  return true;
}

bool Data::controleren() {
#ifdef TRACE
  GA_SERIAL.println("TRACE: Data::controleren()");
#endif
  if (gecontroleerd) return true;
  if (!GedeeldeBusNode::controleren()) return false;

  GedeeldeBusNode* sameParent = firstChild;
  while (sameParent != nullptr) {
    GedeeldeBusNode* object = sameParent->nextChild;
    if (!sameParent->controleren()) sameParent->afmelden();
    sameParent = object;
  }

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM)
  if (this->typesActief & DATA_TYPE_EEPROM) {
    uint8_t teller = 0;
    for (sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component == GedeeldeBusComponent::DATA_EEPROM) teller++;
    }
    if (teller != DATA_LAYER_EEPROM_AANTAL) return false;
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
  if (this->typesActief & DATA_TYPE_MICROSD) {
    uint8_t teller = 0;
    for (sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component == GedeeldeBusComponent::DATA_MICROSD) teller++;
    }
    if (teller != DATA_LAYER_MICROSD_AANTAL) return false;
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  if (this->typesActief & DATA_TYPE_FLASH) {
    uint8_t teller = 0;
    for (sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component == GedeeldeBusComponent::DATA_FLASH) teller++;
    }
    if (teller != DATA_LAYER_FLASH_AANTAL) return false;
  }
#endif

  return true;
}

bool Data::inpluggen() {
#ifdef TRACE
  GA_SERIAL.println("TRACE: Data::inpluggen()");
#endif
  if (ingeplugd) return true;
  if (!GedeeldeBusNode::inpluggen()) return false;

  GedeeldeBusNode* sameParent = firstChild;
  while (sameParent != nullptr) {
    GedeeldeBusNode* object = sameParent->nextChild;
    if (!sameParent->inpluggen()) sameParent->afmelden();
    sameParent = object;
  }

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM)
  if (this->typesActief & DATA_TYPE_EEPROM) {
    uint8_t teller = 0;
    for (sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component == GedeeldeBusComponent::DATA_EEPROM) teller++;
    }
    if (teller != DATA_LAYER_EEPROM_AANTAL) return false;
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
  if (this->typesActief & DATA_TYPE_MICROSD) {
    uint8_t teller = 0;
    for (sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component == GedeeldeBusComponent::DATA_MICROSD) teller++;
    }
    if (teller != DATA_LAYER_MICROSD_AANTAL) return false;
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  if (this->typesActief & DATA_TYPE_FLASH) {
    uint8_t teller = 0;
    for (sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component == GedeeldeBusComponent::DATA_FLASH) teller++;
    }
    if (teller != DATA_LAYER_FLASH_AANTAL) return false;
  }
#endif

  return true;
}

bool Data::Activeren() {
#ifdef TRACE
  GA_SERIAL.println("TRACE: Data::Activeren()");
#endif
  GedeeldeBusNode* sameParent = firstChild;
  while (sameParent != nullptr) {
    GedeeldeBusNode* object = sameParent->nextChild;
    if (!sameParent->activeren()) sameParent->afmelden();
    sameParent = object;
  }

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM)
  if (this->typesActief & DATA_TYPE_EEPROM) {
    uint8_t teller = 0;
    for (sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component == GedeeldeBusComponent::DATA_EEPROM) teller++;
    }
    if (teller != DATA_LAYER_EEPROM_AANTAL) return false;
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
  if (this->typesActief & DATA_TYPE_MICROSD) {
    uint8_t teller = 0;
    for (sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component == GedeeldeBusComponent::DATA_MICROSD) teller++;
    }
    if (teller != DATA_LAYER_MICROSD_AANTAL) return false;
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  if (this->typesActief & DATA_TYPE_FLASH) {
    uint8_t teller = 0;
    for (sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component == GedeeldeBusComponent::DATA_FLASH) teller++;
    }
    if (teller != DATA_LAYER_FLASH_AANTAL) return false;
  }
#endif

  return true;
}

bool Data::afmelden() {
#ifdef TRACE
  GA_SERIAL.println("TRACE: Data::afmelden()");
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  RecordStop();
#endif
  GedeeldeBusNode* sameParent = firstChild;
  while (sameParent != nullptr) {
    GedeeldeBusNode* object = sameParent->nextChild;
    sameParent->afmelden();
    sameParent = object;
  }

  return GedeeldeBusNode::afmelden();
}

void Data::SensorWrite(int ruweData) {
  this->ruweData = ruweData;
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_DIRECT) && RFP602_DATA_LAYER_DIRECT_FIFO_SIZE > 0
  if (this->typesActief & DATA_TYPE_DIRECT) ruweDataFIFO.push(ruweData);
#endif
}

int Data::SensorRead() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_DIRECT) && RFP602_DATA_LAYER_DIRECT_FIFO_SIZE > 0
  if ((this->typesActief & DATA_TYPE_DIRECT) && !ruweDataFIFO.isEmpty()) return ruweDataFIFO.shift();
#endif
  return ruweData;
}

void Data::StreamStart() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  if (this->typesActief & DATA_TYPE_FLASH) {
    for (GedeeldeBusNode* sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component != GedeeldeBusComponent::DATA_FLASH || !sameParent->actief) continue;

      struct FLASH* kanaal = static_cast<struct FLASH*>(sameParent);
      uint8_t teller = 0;
#if DATA_LAYER_FLASH_AANTAL >= 2
      if (kanaal->np_CS == DATA_LAYER_FLASH_2_CS_PIN) teller = 1;
#endif
      adres[teller] = 0;
      kanaal->eraseSector(0);
    }
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  if (this->typesActief & DATA_TYPE_MICROSD) RecordStart();
#endif
}

void Data::StreamWrite(int ruweData) {
  this->ruweData = ruweData;
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  if (this->typesActief & DATA_TYPE_FLASH) {
    for (GedeeldeBusNode* sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component != GedeeldeBusComponent::DATA_FLASH || !sameParent->actief) continue;

      struct FLASH* kanaal = static_cast<struct FLASH*>(sameParent);
      uint8_t teller = 0;
#if DATA_LAYER_FLASH_AANTAL >= 2
      if (kanaal->np_CS == DATA_LAYER_FLASH_2_CS_PIN) teller = 1;
#endif
      if (kanaal->writeAnything(adres[teller], ruweData)) adres[teller] += sizeof(ruweData);
    }
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  if (this->typesActief & DATA_TYPE_MICROSD) RecordWrite(ruweData);
#endif
}

int Data::StreamRead() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  if ((this->typesActief & DATA_TYPE_FLASH) && adres[0] >= sizeof(ruweData)) {
    for (GedeeldeBusNode* sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component != GedeeldeBusComponent::DATA_FLASH || !sameParent->actief) continue;

      struct FLASH* kanaal = static_cast<struct FLASH*>(sameParent);
      if (kanaal->np_CS != DATA_LAYER_FLASH_1_CS_PIN) continue;

      int gelezen = ruweData;
      if (kanaal->readAnything(adres[0] - sizeof(ruweData), gelezen)) ruweData = gelezen;
      return ruweData;
    }
  }
#endif
  return SensorRead();
}

void Data::StreamStop() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  if (this->typesActief & DATA_TYPE_MICROSD) RecordStop();
#endif
}

void Data::RecordStart() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  if (!(this->typesActief & DATA_TYPE_MICROSD)) return;

  for (GedeeldeBusNode* sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
    if (sameParent->component != GedeeldeBusComponent::DATA_MICROSD || !sameParent->actief) continue;

    struct microSD* kanaal = static_cast<struct microSD*>(sameParent);
    uint8_t teller = 0;
#if DATA_LAYER_MICROSD_AANTAL >= 2
    if (kanaal->np_CS == DATA_LAYER_MICROSD_2_CS_PIN) teller = 1;
#endif
    if (!File[teller]) File[teller] = kanaal->open(RFP602_DATA_RECORD_FILE, FILE_WRITE);
  }
#endif
}

void Data::RecordWrite(int ruweData) {
  this->ruweData = ruweData;
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  if (!(this->typesActief & DATA_TYPE_MICROSD)) return;

  RecordStart();
  for (uint8_t teller = 0; teller < DATA_LAYER_MICROSD_AANTAL && teller < 2; teller++) {
    if (File[teller]) File[teller].println(ruweData);
  }
#endif
}

int Data::RecordRead() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  if (this->typesActief & DATA_TYPE_MICROSD) {
    RecordStop();

    for (GedeeldeBusNode* sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component != GedeeldeBusComponent::DATA_MICROSD || !sameParent->actief) continue;

      struct microSD* kanaal = static_cast<struct microSD*>(sameParent);
      if (kanaal->np_CS != DATA_LAYER_MICROSD_1_CS_PIN) continue;

      File[0] = kanaal->open(RFP602_DATA_RECORD_FILE);
      if (!File[0]) return SensorRead();
      const int gelezen = File[0].parseInt();
      File[0].close();
      ruweData = gelezen;
      return ruweData;
    }
  }
#endif
  return SensorRead();
}

void Data::RecordStop() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  for (uint8_t teller = 0; teller < DATA_LAYER_MICROSD_AANTAL && teller < 2; teller++) {
    if (File[teller]) {
      File[teller].flush();
      File[teller].close();
    }
  }
#endif
}

#endif
