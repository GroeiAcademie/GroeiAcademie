#include "Data.h"

#if (DATA_LAYER_CONFIG) != DATA_TYPE_NONE || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
namespace {
  File32 FileRecording[DATA_LAYER_MICROSD_SPI_AANTAL];
  uint8_t aantalRegelsSindsFlush[DATA_LAYER_MICROSD_SPI_AANTAL] = {};
  unsigned long timeLastFlush[DATA_LAYER_MICROSD_SPI_AANTAL] = {};
}
#endif

// ============================================================================
// FLASH-buffer (DATA_LAYER_FLASH_SPI_x_BUFFER_SIZE > 0): metingen in RAM verzamelen en in één keer naar FLASH schrijven.
// Bij 0 wordt hier niets gecompileerd en schrijft StreamingWrite() elke meting meteen weg.
// ============================================================================
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
  #if (DATA_LAYER_FLASH_SPI_AANTAL >= 1 && DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE > 0) || (DATA_LAYER_FLASH_SPI_AANTAL >= 2 && DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE > 0)
    #define FLASH_BUFFER_ACTIEF 1   // alleen in dit bestand
  #else
    #define FLASH_BUFFER_ACTIEF 0
  #endif

  #if FLASH_BUFFER_ACTIEF
    // Een meting is één int: een buffer > 0 moet er minstens één kunnen bevatten, anders schrijft memcpy() buiten de buffer.
    static_assert(DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE == 0 || DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE >= sizeof(int),
                  "DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE moet 0 zijn, of minstens sizeof(int): 2 op de UNO R3, 4 op de 32-bit boards.");
    #if DATA_LAYER_FLASH_SPI_AANTAL >= 2
    static_assert(DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE == 0 || DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE >= sizeof(int),
                  "DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE moet 0 zijn, of minstens sizeof(int): 2 op de UNO R3, 4 op de 32-bit boards.");
    #endif
namespace {
  #if DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE > 0
  uint8_t bufferFLASH1[DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE];
  #endif
  #if DATA_LAYER_FLASH_SPI_AANTAL >= 2 && DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE > 0
  uint8_t bufferFLASH2[DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE];
  #endif

  // Per FLASH-chip: de buffer (nullptr = geen buffer), zijn grootte en hoeveel bytes erin wachten.
  uint8_t* const bufferFLASH[DATA_LAYER_FLASH_SPI_AANTAL] = {
  #if DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE > 0
    bufferFLASH1
  #else
    nullptr
  #endif
  #if DATA_LAYER_FLASH_SPI_AANTAL >= 2
    ,
    #if DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE > 0
    bufferFLASH2
    #else
    nullptr
    #endif
  #endif
  };
  const uint16_t sizeBufferFLASH[DATA_LAYER_FLASH_SPI_AANTAL] = {
    DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE
  #if DATA_LAYER_FLASH_SPI_AANTAL >= 2
    , DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE
  #endif
  };
  uint16_t countBufferFLASH[DATA_LAYER_FLASH_SPI_AANTAL] = {};

  // De wachtende bytes van één chip naar FLASH schrijven, achter de vorige.
  void FlushBufferFLASH(struct Data* data, uint8_t teller) {
    struct DataFLASHviaSPI* kanaal = data->FLASHviaSPI[teller];
    const uint16_t gevuld = countBufferFLASH[teller];
    countBufferFLASH[teller] = 0;
    if (gevuld == 0 || kanaal == nullptr || !kanaal->actief) return;
    if (kanaal->writeBuffer(data->adres[teller], bufferFLASH[teller], gevuld) == gevuld) data->adres[teller] += gevuld;
  }
}
  #endif
#endif


Data::Data()
#if REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
  : Data(((DATA_LAYER_CONFIG) & ~DATA_TYPE_NONE) | DATA_TYPE_MICROSD_SPI)
#else
  : Data(DATA_LAYER_CONFIG)
#endif
{}

Data::Data(uint16_t typesActief)
  : GedeeldeBusNode(&Native, { nullptr, 0, nullptr, 0 }, GedeeldeBusComponent::GC_DATA, HardwareResourceToegang::GEDEELD),
#if REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
    typesActief((typesActief & ~DATA_TYPE_NONE) | DATA_TYPE_MICROSD_SPI)
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
  if ((this->typesActief & (((DATA_LAYER_CONFIG) & ~DATA_TYPE_NONE) | DATA_TYPE_MICROSD_SPI)) != this->typesActief) return false;
#else
  if ((this->typesActief & DATA_LAYER_CONFIG) != this->typesActief) return false;
#endif
  if (!GedeeldeBusNode::aanmelden()) return false;

  // Elk kind komt bij een geslaagde aanmelding in de lijst van zijn medium (N7); bij een mislukte wordt het verwijderd.
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_DIRECT)
  if (this->typesActief & DATA_TYPE_DIRECT) {
    struct DataDIRECT* kanaal = new struct DataDIRECT(this);
    kanaal->componentCreated = true;
    if (!kanaal->aanmelden()) delete kanaal;
  }
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_NATIVE) && BOARD_VERSION != BOARD_UNO_Q
  if (this->typesActief & DATA_TYPE_EEPROM_NATIVE) {
    struct DataEEPROMviaNative* kanaal = new struct DataEEPROMviaNative(this);
    kanaal->componentCreated = true;
    if (kanaal->aanmelden()) EEPROMviaNative = kanaal;
    else delete kanaal;
  }
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_I2C)
  if (this->typesActief & DATA_TYPE_EEPROM_I2C) {
    #if DATA_LAYER_EEPROM_I2C_AANTAL >= 1
      {
        struct DataEEPROMviaI2C* kanaal = new struct DataEEPROMviaI2C(this, DATA_LAYER_EEPROM_I2C_1_I2C_ADDRESS, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (kanaal->aanmelden()) EEPROMviaI2C[0] = kanaal;
        else delete kanaal;
      }
    #endif
    #if DATA_LAYER_EEPROM_I2C_AANTAL >= 2
      {
        struct DataEEPROMviaI2C* kanaal = new struct DataEEPROMviaI2C(this, DATA_LAYER_EEPROM_I2C_2_I2C_ADDRESS, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (kanaal->aanmelden()) EEPROMviaI2C[1] = kanaal;
        else delete kanaal;
      }
    #endif
  }
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
  if (this->typesActief & DATA_TYPE_FLASH_SPI) {
    #if DATA_LAYER_FLASH_SPI_AANTAL >= 1
      {
        struct DataFLASHviaSPI* kanaal = new struct DataFLASHviaSPI(this, static_cast<HardwareResourcePin>(DATA_LAYER_FLASH_SPI_1_CS_PIN), HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
  #ifdef DATA_LAYER_FLASH_SPI_1_CS_EXTENDER
        // CS op een Extender: DATA_LAYER_FLASH_SPI_1_CS_PIN is de pin op die Extender.
        static_assert(DATA_LAYER_FLASH_SPI_1_CS_EXTENDER == GedeeldeBusComponent::DIGITAL_PINS_PCF8574 ||
                      DATA_LAYER_FLASH_SPI_1_CS_EXTENDER == GedeeldeBusComponent::DIGITAL_PINS_PCF8575 ||
                      DATA_LAYER_FLASH_SPI_1_CS_EXTENDER == GedeeldeBusComponent::DIGITAL_PINS_MCP23017,
                      "DATA_LAYER_FLASH_SPI_1_CS_EXTENDER: alleen DIGITAL_PINS_PCF8574, DIGITAL_PINS_PCF8575 of DIGITAL_PINS_MCP23017.");
        if (kanaal->ConnectCStoExtender(DATA_LAYER_FLASH_SPI_1_CS_EXTENDER, DATA_LAYER_FLASH_SPI_1_CS_EXTENDER_I2C_ADDRESS)) {
          kanaal->transport.extenderCS = kanaal->extenderCS[0];
          kanaal->transport.pinCS = static_cast<uint8_t>(kanaal->np_CS);
        }
  #endif
        if (kanaal->aanmelden()) FLASHviaSPI[0] = kanaal;
        else delete kanaal;
      }
    #endif
    #if DATA_LAYER_FLASH_SPI_AANTAL >= 2
      {
        struct DataFLASHviaSPI* kanaal = new struct DataFLASHviaSPI(this, static_cast<HardwareResourcePin>(DATA_LAYER_FLASH_SPI_2_CS_PIN), HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
  #ifdef DATA_LAYER_FLASH_SPI_2_CS_EXTENDER
        // CS op een Extender: DATA_LAYER_FLASH_SPI_2_CS_PIN is de pin op die Extender.
        static_assert(DATA_LAYER_FLASH_SPI_2_CS_EXTENDER == GedeeldeBusComponent::DIGITAL_PINS_PCF8574 ||
                      DATA_LAYER_FLASH_SPI_2_CS_EXTENDER == GedeeldeBusComponent::DIGITAL_PINS_PCF8575 ||
                      DATA_LAYER_FLASH_SPI_2_CS_EXTENDER == GedeeldeBusComponent::DIGITAL_PINS_MCP23017,
                      "DATA_LAYER_FLASH_SPI_2_CS_EXTENDER: alleen DIGITAL_PINS_PCF8574, DIGITAL_PINS_PCF8575 of DIGITAL_PINS_MCP23017.");
        if (kanaal->ConnectCStoExtender(DATA_LAYER_FLASH_SPI_2_CS_EXTENDER, DATA_LAYER_FLASH_SPI_2_CS_EXTENDER_I2C_ADDRESS)) {
          kanaal->transport.extenderCS = kanaal->extenderCS[0];
          kanaal->transport.pinCS = static_cast<uint8_t>(kanaal->np_CS);
        }
  #endif
        if (kanaal->aanmelden()) FLASHviaSPI[1] = kanaal;
        else delete kanaal;
      }
    #endif
  }
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FRAM_I2C)
  if (this->typesActief & DATA_TYPE_FRAM_I2C) {
    #if DATA_LAYER_FRAM_I2C_AANTAL >= 1
      {
        struct DataFRAMviaI2C* kanaal = new struct DataFRAMviaI2C(this, DATA_LAYER_FRAM_I2C_1_I2C_ADDRESS, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (kanaal->aanmelden()) FRAMviaI2C[0] = kanaal;
        else delete kanaal;
      }
    #endif
    #if DATA_LAYER_FRAM_I2C_AANTAL >= 2
      {
        struct DataFRAMviaI2C* kanaal = new struct DataFRAMviaI2C(this, DATA_LAYER_FRAM_I2C_2_I2C_ADDRESS, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (kanaal->aanmelden()) FRAMviaI2C[1] = kanaal;
        else delete kanaal;
      }
    #endif
  }
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
  if (this->typesActief & DATA_TYPE_MICROSD_SPI) {
    #if DATA_LAYER_MICROSD_SPI_AANTAL >= 1
      {
        struct DataMICROSDviaSPI* kanaal = new struct DataMICROSDviaSPI(this, static_cast<HardwareResourcePin>(DATA_LAYER_MICROSD_SPI_1_CS_PIN), HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
  #ifdef DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER
        // CS op een Extender: DATA_LAYER_MICROSD_SPI_1_CS_PIN is de pin op die Extender.
        static_assert(DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER == GedeeldeBusComponent::DIGITAL_PINS_PCF8574 ||
                      DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER == GedeeldeBusComponent::DIGITAL_PINS_PCF8575 ||
                      DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER == GedeeldeBusComponent::DIGITAL_PINS_MCP23017,
                      "DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER: alleen DIGITAL_PINS_PCF8574, DIGITAL_PINS_PCF8575 of DIGITAL_PINS_MCP23017.");
        kanaal->ConnectCStoExtender(DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER, DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER_I2C_ADDRESS);
  #endif
        if (kanaal->aanmelden()) MICROSDviaSPI[0] = kanaal;
        else delete kanaal;
      }
    #endif
    #if DATA_LAYER_MICROSD_SPI_AANTAL >= 2
      {
        struct DataMICROSDviaSPI* kanaal = new struct DataMICROSDviaSPI(this, static_cast<HardwareResourcePin>(DATA_LAYER_MICROSD_SPI_2_CS_PIN), HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
  #ifdef DATA_LAYER_MICROSD_SPI_2_CS_EXTENDER
        // CS op een Extender: DATA_LAYER_MICROSD_SPI_2_CS_PIN is de pin op die Extender.
        static_assert(DATA_LAYER_MICROSD_SPI_2_CS_EXTENDER == GedeeldeBusComponent::DIGITAL_PINS_PCF8574 ||
                      DATA_LAYER_MICROSD_SPI_2_CS_EXTENDER == GedeeldeBusComponent::DIGITAL_PINS_PCF8575 ||
                      DATA_LAYER_MICROSD_SPI_2_CS_EXTENDER == GedeeldeBusComponent::DIGITAL_PINS_MCP23017,
                      "DATA_LAYER_MICROSD_SPI_2_CS_EXTENDER: alleen DIGITAL_PINS_PCF8574, DIGITAL_PINS_PCF8575 of DIGITAL_PINS_MCP23017.");
        kanaal->ConnectCStoExtender(DATA_LAYER_MICROSD_SPI_2_CS_EXTENDER, DATA_LAYER_MICROSD_SPI_2_CS_EXTENDER_I2C_ADDRESS);
  #endif
        if (kanaal->aanmelden()) MICROSDviaSPI[1] = kanaal;
        else delete kanaal;
      }
    #endif
  }
#endif

  // Data faalt alleen wanneer geen enkel gevraagd medium overblijft (DIRECT is ook een kind).
  return firstChild != nullptr;
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

  // Data faalt alleen wanneer geen enkel gevraagd medium overblijft (DIRECT is ook een kind).
  return firstChild != nullptr;
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

  // Data faalt alleen wanneer geen enkel gevraagd medium overblijft (DIRECT is ook een kind).
  return firstChild != nullptr;
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

  // Data faalt alleen wanneer geen enkel gevraagd medium overblijft (DIRECT is ook een kind).
  return firstChild != nullptr;
}

bool Data::afmelden() {
#ifdef TRACE
  GA_SERIAL.println("TRACE: Data::afmelden()");
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  RecordingStop();
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI) && FLASH_BUFFER_ACTIEF
  for (uint8_t teller = 0; teller < DATA_LAYER_FLASH_SPI_AANTAL; teller++) FlushBufferFLASH(this, teller);   // vóór de chips verdwijnen
#endif
  GedeeldeBusNode* sameParent = firstChild;
  while (sameParent != nullptr) {
    GedeeldeBusNode* object = sameParent->nextChild;
    sameParent->afmelden();
    sameParent = object;
  }

  return GedeeldeBusNode::afmelden();
}

// ============================================================================
// QueueOrStack: los van DIRECT (A8). Werkt zodra de sensor een grootte > 0 opgeeft.
// ============================================================================
void Data::WriteQueueOrStack(SensorQueueOrStack& channel, int value) {
  channel.lastValue = value;
#if DATA_QUEUE_OR_STACK_ACTIEF
  if (channel.sizeQueueOrStack == 0) return;
  unsigned int index = channel.firstIndex + channel.countNumber;
  if (index >= channel.sizeQueueOrStack) index -= channel.sizeQueueOrStack;
  channel.buffer[index] = value;
  if (channel.countNumber < channel.sizeQueueOrStack) channel.countNumber++;
  else if (++channel.firstIndex >= channel.sizeQueueOrStack) channel.firstIndex = 0;   // vol: de oudste waarde wordt overschreven
#endif
}

int8_t Data::ReadQueueOrStackStatus(SensorQueueOrStack& channel) {
#if DATA_QUEUE_OR_STACK_ACTIEF
  if (channel.countNumber == 0) return -1;                          // leeg
  if (channel.countNumber == channel.sizeQueueOrStack) return 1;   // waarde klaar, en vol
  return 0;                                                         // waarde klaar
#else
  (void)channel;
  return -1;   // zonder QueueOrStack: altijd leeg
#endif
}

int Data::ReadQueueOrStackValue(SensorQueueOrStack& channel) {
#if DATA_QUEUE_OR_STACK_ACTIEF
  if (channel.countNumber == 0) return channel.lastValue;   // leeg: laatst gemeten waarde van dit channel
  int value;
  if (channel.LIFO) {
    unsigned int index = channel.firstIndex + channel.countNumber - 1;
    if (index >= channel.sizeQueueOrStack) index -= channel.sizeQueueOrStack;
    value = channel.buffer[index];   // nieuwste eerst
  }
  else {
    value = channel.buffer[channel.firstIndex];   // oudste eerst
    if (++channel.firstIndex >= channel.sizeQueueOrStack) channel.firstIndex = 0;
  }
  channel.countNumber--;
  return value;
#else
  return channel.lastValue;   // zonder QueueOrStack: laatst gemeten waarde van dit channel
#endif
}

#if (DATA_LAYER_CONFIG) != DATA_TYPE_NONE
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

int Sensor::StreamingRead(uint8_t numberChannel) {
  if (::Data == nullptr || QueueOrStack == nullptr || numberChannel == 0 || numberChannel > numberOfChannels) return 0;   // sensor bestaat niet
  return ::Data->StreamingRead(QueueOrStack[numberChannel - 1]);
}

int Sensor::RecordRead(uint8_t numberChannel) {
  if (::Data == nullptr || QueueOrStack == nullptr || numberChannel == 0 || numberChannel > numberOfChannels) return 0;   // sensor bestaat niet
  return ::Data->RecordRead(QueueOrStack[numberChannel - 1]);
}
#endif

// ============================================================================
// STREAMING (FLASH) en RECORDING (microSD): rechtstreeks via de lijsten (N7), zonder zoeken op component.
// ============================================================================
void Data::StreamingStart() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
  if (this->typesActief & DATA_TYPE_FLASH_SPI) {
    for (uint8_t teller = 0; teller < DATA_LAYER_FLASH_SPI_AANTAL; teller++) {
      struct DataFLASHviaSPI* kanaal = FLASHviaSPI[teller];
      if (kanaal == nullptr || !kanaal->actief) continue;
  #if FLASH_BUFFER_ACTIEF
      countBufferFLASH[teller] = 0;   // nieuwe stroom: wat nog in de buffer zat, vervalt
  #endif
      adres[teller] = 0;
      kanaal->eraseSector(0);
    }
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  if (this->typesActief & DATA_TYPE_MICROSD_SPI) RecordingStart();
#endif
}

void Data::StreamingWrite(int ruweData) {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
  if (this->typesActief & DATA_TYPE_FLASH_SPI) {
    for (uint8_t teller = 0; teller < DATA_LAYER_FLASH_SPI_AANTAL; teller++) {
      struct DataFLASHviaSPI* kanaal = FLASHviaSPI[teller];
      if (kanaal == nullptr || !kanaal->actief) continue;
  #if FLASH_BUFFER_ACTIEF
      if (bufferFLASH[teller] != nullptr) {
        // In de buffer; vol = in één keer naar FLASH.
        if (countBufferFLASH[teller] + sizeof(ruweData) > sizeBufferFLASH[teller]) FlushBufferFLASH(this, teller);
        memcpy(bufferFLASH[teller] + countBufferFLASH[teller], &ruweData, sizeof(ruweData));
        countBufferFLASH[teller] += sizeof(ruweData);
        if (countBufferFLASH[teller] + sizeof(ruweData) > sizeBufferFLASH[teller]) FlushBufferFLASH(this, teller);
        continue;
      }
  #endif
      if (kanaal->writeBuffer(adres[teller], reinterpret_cast<const uint8_t*>(&ruweData), sizeof(ruweData)) == sizeof(ruweData)) adres[teller] += sizeof(ruweData);
    }
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  if (this->typesActief & DATA_TYPE_MICROSD_SPI) RecordingWrite(ruweData);
#endif
  (void)ruweData;
}

int Data::StreamingRead(SensorQueueOrStack& channel) {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
  struct DataFLASHviaSPI* kanaal = FLASHviaSPI[0];
  #if FLASH_BUFFER_ACTIEF
  // De laatste meting kan nog in de buffer wachten.
  if ((this->typesActief & DATA_TYPE_FLASH_SPI) && bufferFLASH[0] != nullptr && countBufferFLASH[0] >= sizeof(int)) {
    int gelezen;
    memcpy(&gelezen, bufferFLASH[0] + countBufferFLASH[0] - sizeof(gelezen), sizeof(gelezen));
    return gelezen;
  }
  #endif
  if ((this->typesActief & DATA_TYPE_FLASH_SPI) && kanaal != nullptr && kanaal->actief && adres[0] >= sizeof(int)) {
    int gelezen;
    if (kanaal->readBuffer(adres[0] - sizeof(gelezen), reinterpret_cast<uint8_t*>(&gelezen), sizeof(gelezen)) == sizeof(gelezen)) return gelezen;
  }
#endif
  return channel.lastValue;   // terugval: laatst gemeten waarde van dit channel (V3)
}

void Data::StreamingStop() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI) && FLASH_BUFFER_ACTIEF
  for (uint8_t teller = 0; teller < DATA_LAYER_FLASH_SPI_AANTAL; teller++) FlushBufferFLASH(this, teller);   // wat nog wacht, naar FLASH
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  if (this->typesActief & DATA_TYPE_MICROSD_SPI) RecordingStop();
#endif
}

void Data::RecordingStart() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  if (!(this->typesActief & DATA_TYPE_MICROSD_SPI)) return;

  for (uint8_t teller = 0; teller < DATA_LAYER_MICROSD_SPI_AANTAL; teller++) {
    struct DataMICROSDviaSPI* kanaal = MICROSDviaSPI[teller];
    if (kanaal == nullptr || !kanaal->actief) continue;
    if (!FileRecording[teller]) FileRecording[teller] = kanaal->open(RFP602_DATA_RECORD_FILE, O_RDWR | O_CREAT | O_AT_END);   // achteraan bijschrijven
  }
#endif
}

void Data::RecordingWrite(int ruweData) {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  if (!(this->typesActief & DATA_TYPE_MICROSD_SPI)) return;

  RecordingStart();
  for (uint8_t teller = 0; teller < DATA_LAYER_MICROSD_SPI_AANTAL; teller++) {
    if (FileRecording[teller]) {
      FileRecording[teller].println(ruweData);
      if (++aantalRegelsSindsFlush[teller] >= DATA_LAYER_MICROSD_SPI_FLUSH_NA_AANTAL_REGEL ||
          millis() - timeLastFlush[teller] >= DATA_LAYER_MICROSD_SPI_FLUSH_NA_MS) {
        FileRecording[teller].flush();
        aantalRegelsSindsFlush[teller] = 0;
        timeLastFlush[teller] = millis();
      }
    }
  }
#endif
  (void)ruweData;
}

int Data::RecordRead(SensorQueueOrStack& channel) {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  struct DataMICROSDviaSPI* kanaal = MICROSDviaSPI[0];
  if ((this->typesActief & DATA_TYPE_MICROSD_SPI) && kanaal != nullptr && kanaal->actief) {
    RecordingStop();
    FileRecording[0] = kanaal->open(RFP602_DATA_RECORD_FILE, O_RDONLY);
    if (FileRecording[0]) {
      const int gelezen = FileRecording[0].parseInt();
      FileRecording[0].close();
      return gelezen;
    }
  }
#endif
  return channel.lastValue;   // terugval: laatst gemeten waarde van dit channel (V3)
}

void Data::RecordingStop() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  for (uint8_t teller = 0; teller < DATA_LAYER_MICROSD_SPI_AANTAL; teller++) {
    if (FileRecording[teller]) {
      FileRecording[teller].flush();
      FileRecording[teller].close();
      aantalRegelsSindsFlush[teller] = 0;
    }
  }
#endif
}

#endif
