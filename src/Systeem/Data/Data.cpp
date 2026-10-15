#include "Data.h"

#if (DATA_LAYER_CONFIG) != DATA_TYPE_NONE || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
namespace {
  File FileRecording[2];
  uint8_t aantalRegelsSindsFlush[2] = { 0, 0 };
  unsigned long timeLastFlush[2] = { 0, 0 };
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
        struct DataEEPROM* kanaal = new struct DataEEPROM(this, DATA_LAYER_EEPROM_1_I2C_ADDRESS, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (!kanaal->aanmelden()) delete kanaal;
      }
    #endif
    #if DATA_LAYER_EEPROM_AANTAL >= 2
      {
        struct DataEEPROM* kanaal = new struct DataEEPROM(this, DATA_LAYER_EEPROM_2_I2C_ADDRESS, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
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
        struct DataFLASH* kanaal = new struct DataFLASH(this, DATA_LAYER_FLASH_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (!kanaal->aanmelden()) delete kanaal;
      }
    #endif
    #if DATA_LAYER_FLASH_AANTAL >= 2
      {
        struct DataFLASH* kanaal = new struct DataFLASH(this, DATA_LAYER_FLASH_2_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (!kanaal->aanmelden()) delete kanaal;
      }
    #endif
  }
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FRAM)
  if (this->typesActief & DATA_TYPE_FRAM) {
    #if DATA_LAYER_FRAM_AANTAL >= 1
      {
        struct FRAM_I2C* kanaal = new struct FRAM_I2C(this, DATA_LAYER_FRAM_1_I2C_ADDRESS, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (!kanaal->aanmelden()) delete kanaal;
      }
    #endif
    #if DATA_LAYER_FRAM_AANTAL >= 2
      {
        struct FRAM_I2C* kanaal = new struct FRAM_I2C(this, DATA_LAYER_FRAM_2_I2C_ADDRESS, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
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
        struct DataMicroSD* kanaal = new struct DataMicroSD(this, DATA_LAYER_MICROSD_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (!kanaal->aanmelden()) delete kanaal;
      }
    #endif
    #if DATA_LAYER_MICROSD_AANTAL >= 2
      {
        struct DataMicroSD* kanaal = new struct DataMicroSD(this, DATA_LAYER_MICROSD_2_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (!kanaal->aanmelden()) delete kanaal;
      }
    #endif
  }
#endif

  // Data faalt alleen wanneer geen enkel gevraagd medium overblijft. DIRECT (RAM-QueueOrStack) is geen kind en blijft altijd over.
  uint8_t teller = 0;
  for (GedeeldeBusNode* sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) teller++;
  if (teller == 0 && !(this->typesActief & DATA_TYPE_DIRECT)) return false;

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

  // Data faalt alleen wanneer geen enkel gevraagd medium overblijft. DIRECT (RAM-QueueOrStack) is geen kind en blijft altijd over.
  uint8_t teller = 0;
  for (sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) teller++;
  if (teller == 0 && !(this->typesActief & DATA_TYPE_DIRECT)) return false;

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

  // Data faalt alleen wanneer geen enkel gevraagd medium overblijft. DIRECT (RAM-QueueOrStack) is geen kind en blijft altijd over.
  uint8_t teller = 0;
  for (sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) teller++;
  if (teller == 0 && !(this->typesActief & DATA_TYPE_DIRECT)) return false;

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

  // Data faalt alleen wanneer geen enkel gevraagd medium overblijft. DIRECT (RAM-QueueOrStack) is geen kind en blijft altijd over.
  uint8_t teller = 0;
  for (sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) teller++;
  if (teller == 0 && !(this->typesActief & DATA_TYPE_DIRECT)) return false;

  return true;
}

bool Data::afmelden() {
#ifdef TRACE
  GA_SERIAL.println("TRACE: Data::afmelden()");
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  RecordingStop();
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
}

int Data::SensorRead() {
  return ruweData;
}

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_DIRECT)
void Data::WriteQueueOrStack(SensorQueueOrStack& channel, int value) {
  channel.lastValue = value;
#if DATA_QUEUE_OR_STACK_ACTIEF
  if (!(this->typesActief & DATA_TYPE_DIRECT) || channel.sizeQueueOrStack == 0) return;
  channel.buffer[(channel.firstIndex + channel.countNumber) % channel.sizeQueueOrStack] = value;
  if (channel.countNumber < channel.sizeQueueOrStack) channel.countNumber++;
  else channel.firstIndex = (channel.firstIndex + 1) % channel.sizeQueueOrStack;   // vol: de oudste waarde wordt overschreven
#endif
}

int8_t Data::ReadQueueOrStackStatus(SensorQueueOrStack& channel) {
#if DATA_QUEUE_OR_STACK_ACTIEF
  if (!(this->typesActief & DATA_TYPE_DIRECT) || channel.countNumber == 0) return -1;   // leeg
  return 0;   // waarde klaar
#else
  (void)channel;
  return -1;   // zonder QueueOrStack: altijd leeg
#endif
}

int Data::ReadQueueOrStackValue(SensorQueueOrStack& channel) {
#if DATA_QUEUE_OR_STACK_ACTIEF
  if (!(this->typesActief & DATA_TYPE_DIRECT) || channel.countNumber == 0) return channel.lastValue;   // leeg: laatst gemeten waarde van dit channel
  int value;
  if (channel.LIFO) value = channel.buffer[(channel.firstIndex + channel.countNumber - 1) % channel.sizeQueueOrStack];   // nieuwste eerst
  else {
    value = channel.buffer[channel.firstIndex];   // oudste eerst
    channel.firstIndex = (channel.firstIndex + 1) % channel.sizeQueueOrStack;
  }
  channel.countNumber--;
  return value;
#else
  return channel.lastValue;   // zonder QueueOrStack: laatst gemeten waarde van dit channel
#endif
}

// Overgeërfd door elke sensor: geeft het channel van de sensor door aan Data.
void Sensor::WriteQueueOrStack(uint8_t numberChannel, int value) {
  if (::Data == nullptr || QueueOrStack == nullptr || numberChannel == 0 || numberChannel > numberOfChannels) return;
  ::Data->WriteQueueOrStack(QueueOrStack[numberChannel - 1], value);
}

int8_t Sensor::ReadQueueOrStackStatus(uint8_t numberChannel) {
  if (::Data == nullptr || QueueOrStack == nullptr || numberChannel == 0 || numberChannel > numberOfChannels) return -2;   // sensor bestaat niet
  return ::Data->ReadQueueOrStackStatus(QueueOrStack[numberChannel - 1]);
}

int Sensor::ReadQueueOrStackValue(uint8_t numberChannel) {
  if (::Data == nullptr || QueueOrStack == nullptr || numberChannel == 0 || numberChannel > numberOfChannels) return 0;   // sensor bestaat niet
  return ::Data->ReadQueueOrStackValue(QueueOrStack[numberChannel - 1]);
}
#endif

void Data::StreamingStart() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  if (this->typesActief & DATA_TYPE_FLASH) {
    for (GedeeldeBusNode* sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component != GedeeldeBusComponent::DATA_FLASH || !sameParent->actief) continue;

      struct DataFLASH* kanaal = static_cast<struct DataFLASH*>(sameParent);
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
  if (this->typesActief & DATA_TYPE_MICROSD) RecordingStart();
#endif
}

void Data::StreamingWrite(int ruweData) {
  this->ruweData = ruweData;
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  if (this->typesActief & DATA_TYPE_FLASH) {
    for (GedeeldeBusNode* sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component != GedeeldeBusComponent::DATA_FLASH || !sameParent->actief) continue;

      struct DataFLASH* kanaal = static_cast<struct DataFLASH*>(sameParent);
      uint8_t teller = 0;
#if DATA_LAYER_FLASH_AANTAL >= 2
      if (kanaal->np_CS == DATA_LAYER_FLASH_2_CS_PIN) teller = 1;
#endif
      if (kanaal->writeAnything(adres[teller], ruweData)) adres[teller] += sizeof(ruweData);
    }
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  if (this->typesActief & DATA_TYPE_MICROSD) RecordingWrite(ruweData);
#endif
}

int Data::StreamingRead() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  if ((this->typesActief & DATA_TYPE_FLASH) && adres[0] >= sizeof(ruweData)) {
    for (GedeeldeBusNode* sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component != GedeeldeBusComponent::DATA_FLASH || !sameParent->actief) continue;

      struct DataFLASH* kanaal = static_cast<struct DataFLASH*>(sameParent);
      if (kanaal->np_CS != DATA_LAYER_FLASH_1_CS_PIN) continue;

      int gelezen = ruweData;
      if (kanaal->readAnything(adres[0] - sizeof(ruweData), gelezen)) ruweData = gelezen;
      return ruweData;
    }
  }
#endif
  return SensorRead();
}

void Data::StreamingStop() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  if (this->typesActief & DATA_TYPE_MICROSD) RecordingStop();
#endif
}

void Data::RecordingStart() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  if (!(this->typesActief & DATA_TYPE_MICROSD)) return;

  for (GedeeldeBusNode* sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
    if (sameParent->component != GedeeldeBusComponent::DATA_MICROSD || !sameParent->actief) continue;

    struct DataMicroSD* kanaal = static_cast<struct DataMicroSD*>(sameParent);
    uint8_t teller = 0;
#if DATA_LAYER_MICROSD_AANTAL >= 2
    if (kanaal->np_CS == DATA_LAYER_MICROSD_2_CS_PIN) teller = 1;
#endif
    if (!FileRecording[teller]) {
#if defined(ARDUINO_ARCH_ESP32)
      FileRecording[teller] = kanaal->open(RFP602_DATA_RECORD_FILE, FILE_APPEND);
#else
      FileRecording[teller] = kanaal->open(RFP602_DATA_RECORD_FILE, FILE_WRITE);
#endif
    }
  }
#endif
}

void Data::RecordingWrite(int ruweData) {
  this->ruweData = ruweData;
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  if (!(this->typesActief & DATA_TYPE_MICROSD)) return;

  RecordingStart();
  for (uint8_t teller = 0; teller < DATA_LAYER_MICROSD_AANTAL && teller < 2; teller++) {
    if (FileRecording[teller]) {
      FileRecording[teller].println(ruweData);
      if (++aantalRegelsSindsFlush[teller] >= DATA_LAYER_MICROSD_FLUSH_NA_AANTAL_REGEL ||
          millis() - timeLastFlush[teller] >= DATA_LAYER_MICROSD_FLUSH_NA_MS) {
        FileRecording[teller].flush();
        aantalRegelsSindsFlush[teller] = 0;
        timeLastFlush[teller] = millis();
      }
    }
  }
#endif
}

int Data::RecordRead() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  if (this->typesActief & DATA_TYPE_MICROSD) {
    RecordingStop();

    for (GedeeldeBusNode* sameParent = firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent->component != GedeeldeBusComponent::DATA_MICROSD || !sameParent->actief) continue;

      struct DataMicroSD* kanaal = static_cast<struct DataMicroSD*>(sameParent);
      if (kanaal->np_CS != DATA_LAYER_MICROSD_1_CS_PIN) continue;

      FileRecording[0] = kanaal->open(RFP602_DATA_RECORD_FILE);
      if (!FileRecording[0]) return SensorRead();
      const int gelezen = FileRecording[0].parseInt();
      FileRecording[0].close();
      ruweData = gelezen;
      return ruweData;
    }
  }
#endif
  return SensorRead();
}

void Data::RecordingStop() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD)
  for (uint8_t teller = 0; teller < DATA_LAYER_MICROSD_AANTAL && teller < 2; teller++) {
    if (FileRecording[teller]) {
      FileRecording[teller].flush();
      FileRecording[teller].close();
      aantalRegelsSindsFlush[teller] = 0;
    }
  }
#endif
}

#endif
