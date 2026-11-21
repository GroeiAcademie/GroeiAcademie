#include "Data.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#if (DATA_LAYER_CONFIG) != DATA_TYPE_NONE || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
namespace {
  File32 FileRecordingSensor[DATA_LAYER_MICROSD_SPI_AANTAL];
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
                  "DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE moet 0 zijn, of minstens sizeof(int): 4 op de ondersteunde 32-bit boards.");
    #if DATA_LAYER_FLASH_SPI_AANTAL >= 2
    static_assert(DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE == 0 || DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE >= sizeof(int),
                  "DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE moet 0 zijn, of minstens sizeof(int): 4 op de ondersteunde 32-bit boards.");
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
    if (gevuld == 0) return;
    // Wachtende records die niet weggeschreven kunnen worden: een schrijffout van deze streaming.
    if (kanaal == nullptr || !kanaal->actief || !kanaal->IsRuntimeEigenaar(data)) { data->streamingWriteFailed = true; return; }
    if (data->adres[teller] > kanaal->size() || gevuld > kanaal->size() - data->adres[teller]) { data->streamingWriteFailed = true; return; }
    // RUN87: het startadres ligt in volledig ongebruikte FLASH-sectoren.
    // Tijdens een opname nooit een sector wissen: oudere opnames blijven intact.
    if (kanaal->writeBuffer(data->adres[teller], bufferFLASH[teller], gevuld) == gevuld) {
      data->adres[teller] += gevuld;
      countBufferFLASH[teller] = 0;
    } else data->streamingWriteFailed = true;
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

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_NRF24L01_SPI)
  if (this->typesActief & DATA_TYPE_NRF24L01_SPI) {
    #if DATA_LAYER_NRF24L01_SPI_AANTAL >= 1
      {
        struct DataNRF24L01viaSPI* kanaal = new struct DataNRF24L01viaSPI(this, static_cast<HardwareResourcePin>(DATA_LAYER_NRF24L01_SPI_1_CS_PIN), static_cast<HardwareResourcePin>(DATA_LAYER_NRF24L01_SPI_1_CE_PIN), HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, HardwareResourceToegang::GEDEELD);
        kanaal->componentCreated = true;
        if (kanaal->aanmelden()) NRF24L01viaSPI[0] = kanaal;
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
  // Stop en ontgrendel alle opnameobjecten voordat hun nodes verdwijnen.
  StreamingStop();
  RecordingStop();
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
// Automatische kanaaltabellen zijn per Sensor-instantie. Externe tabellen, zoals FsrRfp,
// blijven volledig eigendom van de bestaande aanroeper.
#if (DATA_LAYER_CONFIG) != DATA_TYPE_NONE
Sensor::~Sensor() {
  if (QueueOrStackAutomatisch != nullptr) {
#if DATA_QUEUE_OR_STACK_ACTIEF
    for (uint8_t index = 0; index < numberOfChannels; ++index) {
      free(QueueOrStackAutomatisch[index].buffer);
    }
#endif
    free(QueueOrStackAutomatisch);
    QueueOrStackAutomatisch = nullptr;
    QueueOrStack = nullptr;
  }
}
#endif

void Data::WriteQueueOrStack(SensorQueueOrStack& channel, int value) {
  channel.lastValue = value;
#if DATA_QUEUE_OR_STACK_ACTIEF
  if (channel.valueType != 0) { channel.firstIndex = 0; channel.countNumber = 0; }
#endif
  channel.valueType = 0;
#if DATA_QUEUE_OR_STACK_ACTIEF
  if (channel.sizeQueueOrStack == 0 || channel.buffer == nullptr) return;
  unsigned int index = channel.firstIndex + channel.countNumber;
  if (index >= channel.sizeQueueOrStack) index -= channel.sizeQueueOrStack;
  channel.buffer[index] = value;
  if (channel.countNumber < channel.sizeQueueOrStack) channel.countNumber++;
  else if (++channel.firstIndex >= channel.sizeQueueOrStack) channel.firstIndex = 0;   // vol: de oudste waarde wordt overschreven
#endif
}

// Dezelfde bestaande FIFO/LIFO-indexering, maar met behoud van het werkelijke type.
void Data::WriteQueueOrStack(SensorQueueOrStack& channel, float value) {
#if DATA_QUEUE_OR_STACK_ACTIEF
  if (channel.valueType != 1) { channel.firstIndex = 0; channel.countNumber = 0; }
#endif
  channel.valueType = 1;
  channel.lastValueFloat = value;
#if DATA_QUEUE_OR_STACK_ACTIEF
  if (channel.sizeQueueOrStack == 0 || channel.bufferFloat == nullptr) return;
  unsigned int index = channel.firstIndex + channel.countNumber;
  if (index >= channel.sizeQueueOrStack) index -= channel.sizeQueueOrStack;
  channel.bufferFloat[index] = value;
  if (channel.countNumber < channel.sizeQueueOrStack) channel.countNumber++;
  else if (++channel.firstIndex >= channel.sizeQueueOrStack) channel.firstIndex = 0;
#endif
}

void Data::WriteQueueOrStack(SensorQueueOrStack& channel, uint32_t value) {
#if DATA_QUEUE_OR_STACK_ACTIEF
  if (channel.valueType != 2) { channel.firstIndex = 0; channel.countNumber = 0; }
#endif
  channel.valueType = 2;
  channel.lastValueUInt32 = value;
#if DATA_QUEUE_OR_STACK_ACTIEF
  if (channel.sizeQueueOrStack == 0 || channel.bufferUInt32 == nullptr) return;
  unsigned int index = channel.firstIndex + channel.countNumber;
  if (index >= channel.sizeQueueOrStack) index -= channel.sizeQueueOrStack;
  channel.bufferUInt32[index] = value;
  if (channel.countNumber < channel.sizeQueueOrStack) channel.countNumber++;
  else if (++channel.firstIndex >= channel.sizeQueueOrStack) channel.firstIndex = 0;
#endif
}

float Data::ReadQueueOrStackFloat(SensorQueueOrStack& channel) {
  if (channel.valueType != 1) return NAN;
#if DATA_QUEUE_OR_STACK_ACTIEF
  if (channel.countNumber != 0 && channel.bufferFloat != nullptr) {
    unsigned int index = channel.LIFO ? channel.firstIndex + channel.countNumber - 1 : channel.firstIndex;
    if (index >= channel.sizeQueueOrStack) index -= channel.sizeQueueOrStack;
    const float value = channel.bufferFloat[index];
    if (!channel.LIFO && ++channel.firstIndex >= channel.sizeQueueOrStack) channel.firstIndex = 0;
    channel.countNumber--;
    return value;
  }
#endif
  return channel.lastValueFloat;
}

uint32_t Data::ReadQueueOrStackUInt32(SensorQueueOrStack& channel) {
  if (channel.valueType != 2) return 0;
#if DATA_QUEUE_OR_STACK_ACTIEF
  if (channel.countNumber != 0 && channel.bufferUInt32 != nullptr) {
    unsigned int index = channel.LIFO ? channel.firstIndex + channel.countNumber - 1 : channel.firstIndex;
    if (index >= channel.sizeQueueOrStack) index -= channel.sizeQueueOrStack;
    const uint32_t value = channel.bufferUInt32[index];
    if (!channel.LIFO && ++channel.firstIndex >= channel.sizeQueueOrStack) channel.firstIndex = 0;
    channel.countNumber--;
    return value;
  }
#endif
  return channel.lastValueUInt32;
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
  // typevast: de int-API haalt geen float- of uint32-buffer leeg en geeft de laatste waarde veilig als int.
  // NaN = 0; +oneindig en te groot = INT_MAX; -oneindig en te klein = INT_MIN.
  if (channel.valueType == 1) {
    const float waarde = channel.lastValueFloat;
    if (isnan(waarde)) return 0;
    if (waarde >= static_cast<float>(INT_MAX)) return INT_MAX;
    if (waarde <= static_cast<float>(INT_MIN)) return INT_MIN;
    return static_cast<int>(waarde);
  }
  if (channel.valueType == 2) return channel.lastValueUInt32 > static_cast<uint32_t>(INT_MAX) ? INT_MAX : static_cast<int>(channel.lastValueUInt32);
#if DATA_QUEUE_OR_STACK_ACTIEF
  if (channel.countNumber == 0) return channel.lastValue;
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
// Eerst de kanaaltabel en de buffers één keer toewijzen (geen realloc tijdens runtime), daarna de hardware activeren
// en de vaste routeidentiteit berekenen. Bij een RAM-tekort mislukt activeren(), zodat het meteen zichtbaar is;
// de hardware is dan nog niet geactiveerd, dus er valt niets terug te draaien.
bool Sensor::activeren() {
  const uint8_t aantal = AantalChannels();
  SensorQueueOrStack* tabel = nullptr;
  if (QueueOrStack == nullptr && aantal != 0) {   // geen externe tabel (bv. FsrRfp) en wel Data-kanalen
    tabel = static_cast<SensorQueueOrStack*>(calloc(aantal, sizeof(SensorQueueOrStack)));
    if (tabel == nullptr) return false;
#if DATA_QUEUE_OR_STACK_ACTIEF
    for (uint8_t index = 0; index < aantal; ++index) {
      // Configureerbare capaciteit per sensor/component en meetkanaal.
      const uint32_t sizeQueueOrStack = DATA_LAYER_SENSOR_QUEUE_OR_STACK_SIZE_FOR(component, index + 1);
      tabel[index].sizeQueueOrStack = sizeQueueOrStack <= 65535UL ? static_cast<uint16_t>(sizeQueueOrStack) : 0;
      if (tabel[index].sizeQueueOrStack == 0) continue;
      tabel[index].bufferUInt32 = static_cast<uint32_t*>(calloc(tabel[index].sizeQueueOrStack, sizeof(uint32_t)));
      if (tabel[index].bufferUInt32 == nullptr) {
        for (uint8_t vrij = 0; vrij < index; ++vrij) free(tabel[vrij].buffer);
        free(tabel);
        return false;
      }
    }
#endif
  }
  if (!GedeeldeBusNode::activeren()) {
    // Hardware niet geactiveerd: het vooraf gereserveerde geheugen weer vrijgeven.
    if (tabel != nullptr) {
#if DATA_QUEUE_OR_STACK_ACTIEF
      for (uint8_t vrij = 0; vrij < aantal; ++vrij) free(tabel[vrij].buffer);
#endif
      free(tabel);
    }
    return false;
  }
  // Stabiele identiteit van de configuratie-route: component, pinclaims en ouderketen.
  // GedeeldeBusNode::id blijft de runtime-ID en wordt niet gewijzigd.
  uint32_t id = 2166136261UL;
  for (const GedeeldeBusNode* kanaal = this; kanaal != nullptr; kanaal = kanaal->parent) {
    const uint16_t component = static_cast<uint16_t>(kanaal->component);
    id = (id ^ static_cast<uint8_t>(component)) * 16777619UL;
    id = (id ^ static_cast<uint8_t>(component >> 8)) * 16777619UL;
    id = (id ^ kanaal->aangemeldePinnen.aantalGedeeld) * 16777619UL;
    for (uint8_t n = 0; n < kanaal->aangemeldePinnen.aantalGedeeld; ++n)
      id = (id ^ kanaal->aangemeldePinnen.gedeeldePinnen[n]) * 16777619UL;
    id = (id ^ kanaal->aangemeldePinnen.aantalExclusief) * 16777619UL;
    for (uint8_t n = 0; n < kanaal->aangemeldePinnen.aantalExclusief; ++n)
      id = (id ^ kanaal->aangemeldePinnen.exclusievePinnen[n]) * 16777619UL;
#if EXTENDER_TCA9548A_AANTAL > 0
    id = (id ^ kanaal->tcaChannel) * 16777619UL;
#endif
    // Twee identieke sensoren op een toegestane gedeelde bus blijven binnen dezelfde
    // vaste configuratie onderscheidbaar door hun positie onder dezelfde ouder.
    for (const GedeeldeBusNode* broer = kanaal->parent ? kanaal->parent->firstChild : nullptr;
         broer != nullptr && broer != kanaal; broer = broer->nextChild) {
      if (broer->component == kanaal->component)
        id = (id ^ 0xA5U) * 16777619UL;
    }
  }
  vasteRouteIdentiteit = id;
  if (tabel == nullptr) return true;   // externe tabel (bv. FsrRfp), geen Data-kanalen of al eerder toegewezen
  QueueOrStackAutomatisch = tabel;
  QueueOrStack = tabel;
  numberOfChannels = aantal;
  return true;
}

// Overgeërfd door elke sensor: geeft het channel van de sensor door aan Data.
void Sensor::WriteQueueOrStack(uint8_t numberChannel, int value) {
  if (::Data == nullptr || QueueOrStack == nullptr || numberChannel == 0 || numberChannel > numberOfChannels) return;
  SensorQueueOrStack& channel = QueueOrStack[numberChannel - 1];
  ::Data->WriteQueueOrStack(channel, value);
#if ((DATA_LAYER_CONFIG) & (DATA_TYPE_FLASH_SPI | DATA_TYPE_MICROSD_SPI | DATA_TYPE_NRF24L01_SPI))
  ::Data->StreamingWrite(*this, numberChannel, channel);
#endif
}

void Sensor::WriteQueueOrStack(uint8_t numberChannel, float value) {
  if (::Data == nullptr || QueueOrStack == nullptr || numberChannel == 0 || numberChannel > numberOfChannels) return;
  SensorQueueOrStack& channel = QueueOrStack[numberChannel - 1];
  ::Data->WriteQueueOrStack(channel, value);
#if ((DATA_LAYER_CONFIG) & (DATA_TYPE_FLASH_SPI | DATA_TYPE_MICROSD_SPI | DATA_TYPE_NRF24L01_SPI))
  ::Data->StreamingWrite(*this, numberChannel, channel);
#endif
}

void Sensor::WriteQueueOrStack(uint8_t numberChannel, uint32_t value) {
  if (::Data == nullptr || QueueOrStack == nullptr || numberChannel == 0 || numberChannel > numberOfChannels) return;
  SensorQueueOrStack& channel = QueueOrStack[numberChannel - 1];
  ::Data->WriteQueueOrStack(channel, value);
#if ((DATA_LAYER_CONFIG) & (DATA_TYPE_FLASH_SPI | DATA_TYPE_MICROSD_SPI | DATA_TYPE_NRF24L01_SPI))
  ::Data->StreamingWrite(*this, numberChannel, channel);
#endif
}

float Sensor::ReadQueueOrStackFloat(uint8_t numberChannel) {
  if (::Data == nullptr || QueueOrStack == nullptr || numberChannel == 0 || numberChannel > numberOfChannels) return NAN;
  return ::Data->ReadQueueOrStackFloat(QueueOrStack[numberChannel - 1]);
}

uint32_t Sensor::ReadQueueOrStackUInt32(uint8_t numberChannel) {
  if (::Data == nullptr || QueueOrStack == nullptr || numberChannel == 0 || numberChannel > numberOfChannels) return 0;
  return ::Data->ReadQueueOrStackUInt32(QueueOrStack[numberChannel - 1]);
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
  // Een kopie: teruglezen mag de live waarde en de buffer van het channel niet overschrijven.
  SensorQueueOrStack channel = QueueOrStack[numberChannel - 1];
  if (::Data->StreamingRead(*this, numberChannel, channel)) {
    return channel.valueType == 0 ? channel.lastValue : 0; // overige types via ReadQueueOrStackFloat/UInt32
  }
  return QueueOrStack[numberChannel - 1].valueType == 0 ? QueueOrStack[numberChannel - 1].lastValue : 0;   // lastValue deelt zijn geheugen met float en uint32 (union)
}

int Sensor::RecordRead(uint8_t numberChannel) {
  if (::Data == nullptr || QueueOrStack == nullptr || numberChannel == 0 || numberChannel > numberOfChannels) return 0;   // sensor bestaat niet
  // Een kopie: teruglezen mag de live waarde en de buffer van het channel niet overschrijven.
  SensorQueueOrStack channel = QueueOrStack[numberChannel - 1];
  if (::Data->RecordRead(*this, numberChannel, channel)) {
    return channel.valueType == 0 ? channel.lastValue : 0; // overige types via ReadQueueOrStackFloat/UInt32
  }
  return QueueOrStack[numberChannel - 1].valueType == 0 ? QueueOrStack[numberChannel - 1].lastValue : 0;   // lastValue deelt zijn geheugen met float en uint32 (union)
}

// Getypeerde overloads met de bestaande leesfunctienamen.
bool Sensor::StreamingRead(uint8_t numberChannel, float& value) {
  if (::Data == nullptr || QueueOrStack == nullptr || numberChannel == 0 || numberChannel > numberOfChannels) return false;
  SensorQueueOrStack channel = QueueOrStack[numberChannel - 1];   // kopie: de live waarde blijft
  if (!::Data->StreamingRead(*this, numberChannel, channel) || channel.valueType != 1) return false;
  value = channel.lastValueFloat;
  return true;
}
bool Sensor::StreamingRead(uint8_t numberChannel, uint32_t& value) {
  if (::Data == nullptr || QueueOrStack == nullptr || numberChannel == 0 || numberChannel > numberOfChannels) return false;
  SensorQueueOrStack channel = QueueOrStack[numberChannel - 1];   // kopie: de live waarde blijft
  if (!::Data->StreamingRead(*this, numberChannel, channel) || channel.valueType != 2) return false;
  value = channel.lastValueUInt32;
  return true;
}
bool Sensor::RecordRead(uint8_t numberChannel, float& value) {
  if (::Data == nullptr || QueueOrStack == nullptr || numberChannel == 0 || numberChannel > numberOfChannels) return false;
  SensorQueueOrStack channel = QueueOrStack[numberChannel - 1];   // kopie: de live waarde blijft
  if (!::Data->RecordRead(*this, numberChannel, channel) || channel.valueType != 1) return false;
  value = channel.lastValueFloat;
  return true;
}
bool Sensor::RecordRead(uint8_t numberChannel, uint32_t& value) {
  if (::Data == nullptr || QueueOrStack == nullptr || numberChannel == 0 || numberChannel > numberOfChannels) return false;
  SensorQueueOrStack channel = QueueOrStack[numberChannel - 1];   // kopie: de live waarde blijft
  if (!::Data->RecordRead(*this, numberChannel, channel) || channel.valueType != 2) return false;
  value = channel.lastValueUInt32;
  return true;
}
#endif

// ============================================================================
// STREAMING (FLASH) en RECORDING (microSD): rechtstreeks via de lijsten (N7), zonder zoeken op component.
// ============================================================================
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
// De chip van deze plaats verdwijnt: zijn wachtende records vervallen, zodat ze nooit op een andere chip terechtkomen.
// Een lege buffer verandert niets; zonder buffer doet deze functie niets.
void Data::DiscardBufferFLASH(uint8_t teller) {
  #if FLASH_BUFFER_ACTIEF
  if (teller >= DATA_LAYER_FLASH_SPI_AANTAL || countBufferFLASH[teller] == 0) return;
  countBufferFLASH[teller] = 0;
  streamingWriteFailed = true;   // die records zijn verloren
  #else
  (void)teller;
  #endif
}

// Eén FLASH-chip starten: reserveren, een vorige buffer wegschrijven en het startadres zoeken.
// true = de chip is gestart en mag records krijgen.
bool Data::StartFlashStreaming(uint8_t teller) {
  if (teller >= DATA_LAYER_FLASH_SPI_AANTAL) return false;
  struct DataFLASHviaSPI* kanaal = FLASHviaSPI[teller];
  // Elke geconfigureerde chip telt; een chip die ontbreekt of niet start, krijgt geen records van deze streaming.
  if (kanaal == nullptr || !kanaal->ReserveerRuntime(this)) { streamingWriteFailed = true; return false; }
  #if FLASH_BUFFER_ACTIEF
  // Een vorige, nog niet gestopte stream eerst volledig opslaan.
  FlushBufferFLASH(this, teller);
  if (countBufferFLASH[teller] != 0) {
    // De vorige metingen konden niet worden weggeschreven.
    // Het schrijfadres blijft staan, zodat een volgende start opnieuw op de juiste plaats probeert.
    kanaal->VrijgevenRuntime(this);
    streamingWriteFailed = true;
    return false;
  }
  #endif
  // De hele chip controleren: ook oude opnames na een herstart of na een onderbroken opname mogen niet geraakt worden.
  // Een sector is pas vrij als AL zijn bytes 0xFF zijn.
  // Geen bestaande sector wissen.
  // Start steeds op de grens na de laatste gebruikte sector.
  adres[teller] = 0;
  const uint32_t grootte = kanaal->size();
  uint8_t record[64];
  bool gelezen = true;
  for (uint32_t sector = 0; sector < grootte; sector += SFLASH_SECTOR_SIZE) {
    bool gewist = true;
    const uint32_t einde = (grootte - sector < SFLASH_SECTOR_SIZE) ? grootte : sector + SFLASH_SECTOR_SIZE;
    for (uint32_t index = sector; index < einde; index += sizeof(record)) {
      const uint32_t aantal = (einde - index < sizeof(record)) ? einde - index : sizeof(record);
      if (kanaal->readBuffer(index, record, aantal) != aantal) { gelezen = false; break; }
      for (uint32_t n = 0; n < aantal; ++n) {
        if (record[n] != 0xFF) { gewist = false; break; }
      }
      if (!gewist) break;
    }
    if (!gelezen) break;
    if (!gewist) adres[teller] = einde;
  }
  // Bij een leesfout niets schrijven; zeker geen bestaande FLASH wissen.
  if (!gelezen) adres[teller] = grootte;
  if (!gelezen || adres[teller] >= grootte) { kanaal->VrijgevenRuntime(this); streamingWriteFailed = true; }
  else flashStreamingStarted[teller] = true;
  #if FLASH_BUFFER_ACTIEF
  countBufferFLASH[teller] = 0;
  #endif
  return flashStreamingStarted[teller];
}
#endif

void Data::StreamingStart() {
  if (streamingActief) {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
    // Een ander medium houdt de streaming actief: een FLASH-chip die niet startte, opnieuw proberen.
    if (this->typesActief & DATA_TYPE_FLASH_SPI) {
      for (uint8_t teller = 0; teller < DATA_LAYER_FLASH_SPI_AANTAL; teller++) {
        if (!flashStreamingStarted[teller]) StartFlashStreaming(teller);
      }
    }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
    // Na RecordingStop() loopt de streaming nog, maar de microSD-opname niet: die opnieuw starten.
    if ((this->typesActief & DATA_TYPE_MICROSD_SPI) && !recordingActief) RecordingStart();
#endif
    return;
  }
  bool flashGereserveerd = false;
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
  streamingWriteFailed = false;   // alleen een nieuwe streaming begint zonder fout
  if (this->typesActief & DATA_TYPE_FLASH_SPI) {
    for (uint8_t teller = 0; teller < DATA_LAYER_FLASH_SPI_AANTAL; teller++) {
      if (StartFlashStreaming(teller)) flashGereserveerd = true;
    }
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_NRF24L01_SPI)
  if (this->typesActief & DATA_TYPE_NRF24L01_SPI) {
    for (uint8_t teller = 0; teller < DATA_LAYER_NRF24L01_SPI_AANTAL; teller++) {
      struct DataNRF24L01viaSPI* kanaal = NRF24L01viaSPI[teller];
      if (kanaal != nullptr && kanaal->actief && kanaal->ReserveerRuntime(this)) flashGereserveerd = true;   // de radio telt als streamingmedium
    }
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  if (this->typesActief & DATA_TYPE_MICROSD_SPI) RecordingStart();
#endif
  streamingActief = flashGereserveerd || recordingActief;
}

// S-09: 15-byte GB-record: 2 magic, 2 component, 4 stabiele route-ID,
// kanaal, valueType, 4 waardebytes, XOR. Oud GA-formaat blijft in SENSOR.DAT
// herkenbaar zodat het overslaan van eerdere records geen uitlijning verbreekt.
// Bestaande FsrRfp-logbestanden blijven ongemoeid.
#if ((DATA_LAYER_CONFIG) & (DATA_TYPE_FLASH_SPI | DATA_TYPE_MICROSD_SPI | DATA_TYPE_NRF24L01_SPI))
void Data::StreamingWrite(const Sensor& sensor, uint8_t numberChannel, const SensorQueueOrStack& channel) {
  if ((!streamingActief && !recordingActief) || numberChannel == 0 || channel.valueType > 2) return;
  const uint32_t id = sensor.vasteRouteIdentiteit;   // één keer berekend in Sensor::activeren()
  uint8_t record[15] = { 0x47, 0x42, static_cast<uint8_t>(static_cast<uint16_t>(sensor.component)),
    static_cast<uint8_t>(static_cast<uint16_t>(sensor.component) >> 8),
    static_cast<uint8_t>(id), static_cast<uint8_t>(id >> 8),
    static_cast<uint8_t>(id >> 16), static_cast<uint8_t>(id >> 24),
    numberChannel, channel.valueType, 0, 0, 0, 0, 0 };
  if (channel.valueType == 0) {
    // int is 32-bit op de ondersteunde boards: altijd sign-extended int32 opslaan.
    const int32_t waarde = channel.lastValue;
    memcpy(record + 10, &waarde, sizeof(waarde));
  } else if (channel.valueType == 1) {
    static_assert(sizeof(float) == 4, "S-09: binaire float vereist 32 bit");
    memcpy(record + 10, &channel.lastValueFloat, 4);
  } else {
    memcpy(record + 10, &channel.lastValueUInt32, 4);
  }
  for (uint8_t index = 0; index < 14; ++index) record[14] ^= record[index];
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
  if (streamingActief && (typesActief & DATA_TYPE_FLASH_SPI)) {
    for (uint8_t teller = 0; teller < DATA_LAYER_FLASH_SPI_AANTAL; ++teller) {
      DataFLASHviaSPI* kanaal = FLASHviaSPI[teller];
      // Alleen een chip die echt gestart is; zonder exclusieve opslag geeft IsRuntimeEigenaar() altijd true.
      if (!flashStreamingStarted[teller]) continue;
      // Een gestarte chip die verdween: dit record ontbreekt.
      if (kanaal == nullptr) { streamingWriteFailed = true; continue; }
      // Een gestarte chip die uitviel of niet meer toegankelijk is: dit record ontbreekt.
      if (!kanaal->actief || !kanaal->IsRuntimeEigenaar(this)) { streamingWriteFailed = true; continue; }
#if FLASH_BUFFER_ACTIEF
      if (bufferFLASH[teller] != nullptr && sizeBufferFLASH[teller] >= sizeof(record)) {
        if (countBufferFLASH[teller] + sizeof(record) > sizeBufferFLASH[teller]) FlushBufferFLASH(this, teller);
        // Past het record na de flush nog niet in de buffer, dan ontbreekt het.
        if (countBufferFLASH[teller] + sizeof(record) > sizeBufferFLASH[teller]) { streamingWriteFailed = true; continue; }
        memcpy(bufferFLASH[teller] + countBufferFLASH[teller], record, sizeof(record));
        countBufferFLASH[teller] += sizeof(record);
        continue;
      }
      FlushBufferFLASH(this, teller); // kleine buffer niet gebruiken voor een groter record
#endif
      // Chip vol of een mislukte write: dit record ontbreekt.
      if (adres[teller] > kanaal->size() || sizeof(record) > kanaal->size() - adres[teller]) { streamingWriteFailed = true; continue; }
      // Bestaande sectoren en voorgaande sessies nooit wissen.
      if (kanaal->writeBuffer(adres[teller], record, sizeof(record)) == sizeof(record))
        adres[teller] += sizeof(record);
      else streamingWriteFailed = true;
    }
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  if (recordingActief && (typesActief & DATA_TYPE_MICROSD_SPI)) {
    for (uint8_t teller = 0; teller < DATA_LAYER_MICROSD_SPI_AANTAL; ++teller) {
      DataMICROSDviaSPI* kanaal = MICROSDviaSPI[teller];
      // Een geconfigureerde kaart die tijdens de opname niet toegankelijk is: dit record ontbreekt.
      if (kanaal == nullptr || !kanaal->actief || !kanaal->IsRuntimeEigenaar(this)) { recordingWriteFailed = true; continue; }
      // De getypeerde opname houdt één bestand open per recording-sessie.
      // Geen bestandsopening of -sluiting per sample.
      if (!FileRecordingSensor[teller])
        FileRecordingSensor[teller] = kanaal->open("SENSOR.DAT", O_WRONLY | O_CREAT | O_APPEND);
      // Een bestand dat niet opengaat of een mislukte write: dit record ontbreekt, en RecordingStop() meldt dat.
      if (!FileRecordingSensor[teller]) { recordingWriteFailed = true; continue; }
      // Zoals bij FLASH: een mislukte write telt niet mee als geschreven record.
      if (FileRecordingSensor[teller].write(record, sizeof(record)) != sizeof(record)) { recordingWriteFailed = true; continue; }
      // Een record telt als regel: flushen na DATA_LAYER_MICROSD_SPI_FLUSH_NA_AANTAL_REGEL records of na DATA_LAYER_MICROSD_SPI_FLUSH_NA_MS.
      if (++aantalRegelsSindsFlush[teller] >= DATA_LAYER_MICROSD_SPI_FLUSH_NA_AANTAL_REGEL ||
          millis() - timeLastFlush[teller] >= DATA_LAYER_MICROSD_SPI_FLUSH_NA_MS) {
        // sync() in plaats van flush(), want flush() geeft geen resultaat.
        if (!FileRecordingSensor[teller].sync()) recordingWriteFailed = true;
        aantalRegelsSindsFlush[teller] = 0;
        timeLastFlush[teller] = millis();
      }
    }
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_NRF24L01_SPI)
  if (streamingActief && (typesActief & DATA_TYPE_NRF24L01_SPI)) {
    for (uint8_t teller = 0; teller < DATA_LAYER_NRF24L01_SPI_AANTAL; ++teller) {
      DataNRF24L01viaSPI* kanaal = NRF24L01viaSPI[teller];
      if (kanaal == nullptr || !kanaal->actief || !kanaal->IsRuntimeEigenaar(this)) continue;
      // Hetzelfde GB-record als in FLASH en SENSOR.DAT; zonder auto-ack wacht dit niet op een ontvanger.
      kanaal->RF24::write(record, sizeof(record));
    }
  }
#endif
}
#endif

// De laatste passende waarde terugzoeken zonder ooit de verkeerde sensor/het verkeerde
// kanaal te retourneren. Bij geen match blijft de bestaande lastValue behouden.
bool Data::StreamingRead(const Sensor& sensor, uint8_t numberChannel, SensorQueueOrStack& channel) {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
  const uint32_t id = sensor.vasteRouteIdentiteit;   // één keer berekend in Sensor::activeren()
#if FLASH_BUFFER_ACTIEF
  for (uint8_t teller = 0; teller < DATA_LAYER_FLASH_SPI_AANTAL; ++teller) FlushBufferFLASH(this, teller);
  // Na een mislukte flush staan de nieuwste records nog in RAM: daar eerst zoeken, van nieuw naar oud.
  for (uint8_t teller = 0; teller < DATA_LAYER_FLASH_SPI_AANTAL; ++teller) {
    if (!(typesActief & DATA_TYPE_FLASH_SPI) || bufferFLASH[teller] == nullptr) continue;
    uint8_t record[15];
    for (uint16_t index = countBufferFLASH[teller]; index >= sizeof(record); index -= sizeof(record)) {
      memcpy(record, bufferFLASH[teller] + index - sizeof(record), sizeof(record));
      if (record[0] != 0x47 || record[1] != 0x42 ||
          record[2] != static_cast<uint8_t>(static_cast<uint16_t>(sensor.component)) ||
          record[3] != static_cast<uint8_t>(static_cast<uint16_t>(sensor.component) >> 8) ||
          record[4] != static_cast<uint8_t>(id) || record[5] != static_cast<uint8_t>(id >> 8) ||
          record[6] != static_cast<uint8_t>(id >> 16) || record[7] != static_cast<uint8_t>(id >> 24) ||
          record[8] != numberChannel || record[9] > 2) continue;
      uint8_t controle = 0;
      for (uint8_t n = 0; n < 14; ++n) controle ^= record[n];
      if (controle != record[14]) continue;
#if DATA_QUEUE_OR_STACK_ACTIEF
      channel.firstIndex = 0;
      channel.countNumber = 0;
#endif
      channel.valueType = record[9];
      if (channel.valueType == 0) { int32_t waarde; memcpy(&waarde, record + 10, 4); channel.lastValue = static_cast<int>(waarde); }
      else if (channel.valueType == 1) memcpy(&channel.lastValueFloat, record + 10, 4);
      else memcpy(&channel.lastValueUInt32, record + 10, 4);
      return true;
    }
  }
#endif
  for (uint8_t teller = 0; teller < DATA_LAYER_FLASH_SPI_AANTAL; ++teller) {
    DataFLASHviaSPI* kanaal = FLASHviaSPI[teller];
    if (!(typesActief & DATA_TYPE_FLASH_SPI) || kanaal == nullptr || !kanaal->actief ||
        !kanaal->RuntimeToegang(this)) continue;
    uint8_t record[15];
    uint32_t einde = adres[teller];
    // Na een herstart begint adres[] opnieuw op 0. Lees de oude records dan
    // rechtstreeks uit FLASH, zonder StreamingStart() of een schrijf-/wisactie.
    // Gebruik de laatste gebruikte sectorgrens: ook een record dat op 0xFF
    // eindigt, blijft zo volledig leesbaar.
    if (einde == 0 && !streamingActief) {
      uint8_t blok[64];
      bool gelezen = true;
      for (uint32_t sectorEinde = kanaal->size(); sectorEinde > 0;) {
        const uint32_t sectorBegin = ((sectorEinde - 1) / SFLASH_SECTOR_SIZE) * SFLASH_SECTOR_SIZE;
        bool gebruikt = false;
        for (uint32_t index = sectorBegin; index < sectorEinde; index += sizeof(blok)) {
          const uint32_t aantal = (sectorEinde - index < sizeof(blok)) ? sectorEinde - index : sizeof(blok);
          if (kanaal->readBuffer(index, blok, aantal) != aantal) { gelezen = false; break; }
          for (uint32_t n = 0; n < aantal; ++n) {
            if (blok[n] != 0xFF) { gebruikt = true; break; }
          }
          if (gebruikt) break;
        }
        if (!gelezen) break;
        if (gebruikt) { einde = sectorEinde; break; }
        sectorEinde = sectorBegin;
      }
      if (!gelezen) continue;  // bij een leesfout nooit een onbewezen record teruggeven
    }
    for (uint32_t index = einde; index >= sizeof(record); --index) {
      if (kanaal->readBuffer(index - sizeof(record), record, sizeof(record)) != sizeof(record)) continue;
      if (record[0] != 0x47 || record[1] != 0x42 ||
          record[2] != static_cast<uint8_t>(static_cast<uint16_t>(sensor.component)) ||
          record[3] != static_cast<uint8_t>(static_cast<uint16_t>(sensor.component) >> 8) ||
          record[4] != static_cast<uint8_t>(id) || record[5] != static_cast<uint8_t>(id >> 8) ||
          record[6] != static_cast<uint8_t>(id >> 16) || record[7] != static_cast<uint8_t>(id >> 24) ||
          record[8] != numberChannel || record[9] > 2) continue;
      uint8_t controle = 0;
      for (uint8_t n = 0; n < 14; ++n) controle ^= record[n];
      if (controle != record[14]) continue;
#if DATA_QUEUE_OR_STACK_ACTIEF
      channel.firstIndex = 0;
      channel.countNumber = 0;
#endif
      channel.valueType = record[9];
      if (channel.valueType == 0) { int32_t waarde; memcpy(&waarde, record + 10, 4); channel.lastValue = static_cast<int>(waarde); }
      else if (channel.valueType == 1) memcpy(&channel.lastValueFloat, record + 10, 4);
      else memcpy(&channel.lastValueUInt32, record + 10, 4);
      return true;
    }
  }
#else
  (void)sensor; (void)numberChannel; (void)channel;
#endif
  return false;
}

void Data::StreamingStop() {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
  for (uint8_t teller = 0; teller < DATA_LAYER_FLASH_SPI_AANTAL; teller++) {
    // Een gestarte chip die intussen uitviel, verdween of zijn eigendom verloor: de streaming is niet volledig.
    if (flashStreamingStarted[teller] && (FLASHviaSPI[teller] == nullptr || !FLASHviaSPI[teller]->actief || !FLASHviaSPI[teller]->IsRuntimeEigenaar(this))) streamingWriteFailed = true;
    flashStreamingStarted[teller] = false;   // na het stoppen krijgt geen enkele chip nog records
    DataFLASHviaSPI* kanaal = FLASHviaSPI[teller];
    if (kanaal == nullptr || !kanaal->IsRuntimeEigenaar(this)) continue;
#if FLASH_BUFFER_ACTIEF
    FlushBufferFLASH(this, teller); // eerst de eigen resterende gegevens wegschrijven
#endif
    kanaal->VrijgevenRuntime(this);
  }
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_NRF24L01_SPI)
  for (uint8_t teller = 0; teller < DATA_LAYER_NRF24L01_SPI_AANTAL; teller++) {
    DataNRF24L01viaSPI* kanaal = NRF24L01viaSPI[teller];
    if (kanaal != nullptr) kanaal->VrijgevenRuntime(this);
  }
#endif
  streamingActief = false;
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  if (this->typesActief & DATA_TYPE_MICROSD_SPI) RecordingStop();
#endif
}

void Data::RecordingStart() {
  if (recordingActief) return;
  // Enkel een daadwerkelijk gereserveerd opnameobject als actief aanmerken.
  recordingActief = false;
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  if (!(this->typesActief & DATA_TYPE_MICROSD_SPI)) return;
  recordingWriteFailed = false;   // een nieuwe opname begint zonder fout

  for (uint8_t teller = 0; teller < DATA_LAYER_MICROSD_SPI_AANTAL; teller++) {
    struct DataMICROSDviaSPI* kanaal = MICROSDviaSPI[teller];
    // Elke geconfigureerde kaart moet deelnemen; een kaart die ontbreekt of niet start, maakt de opname onvolledig.
    if (kanaal == nullptr || !kanaal->ReserveerRuntime(this)) { recordingWriteFailed = true; continue; }
    if (!FileRecordingSensor[teller])
      FileRecordingSensor[teller] = kanaal->open("SENSOR.DAT", O_WRONLY | O_CREAT | O_APPEND);
    aantalRegelsSindsFlush[teller] = 0;
    timeLastFlush[teller] = millis();   // de flushtijd telt vanaf de start van de opname
    if (FileRecordingSensor[teller]) recordingActief = true;
    else { kanaal->VrijgevenRuntime(this); recordingWriteFailed = true; }
  }
#endif
}

bool Data::RecordRead(const Sensor& sensor, uint8_t numberChannel, SensorQueueOrStack& channel) {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  // Een lopende opname niet stilzwijgend onderbreken. Na RecordingStop()
  // kan dezelfde opname veilig teruggelezen worden.
  if (recordingActief) return false;
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  const uint32_t id = sensor.vasteRouteIdentiteit;   // één keer berekend in Sensor::activeren()
  for (uint8_t teller = 0; teller < DATA_LAYER_MICROSD_SPI_AANTAL; ++teller) {
    DataMICROSDviaSPI* kanaal = MICROSDviaSPI[teller];
    if (!(typesActief & DATA_TYPE_MICROSD_SPI) || kanaal == nullptr || !kanaal->actief ||
        !kanaal->RuntimeToegang(this)) continue;
    File32 bestand = kanaal->open("SENSOR.DAT", O_RDONLY);
    if (!bestand) continue;
    uint8_t record[15];
    bool gevonden = false;
    while (bestand.available() >= 12) {
      // Elke header is GA (RUN85, 12 bytes) of GB (RUN86, 15 bytes).
      // Daardoor blijft een bestand met beide recordversies correct doorzoekbaar.
      if (bestand.read(record, 2) != 2) break;
      // Geen header: één byte verder zoeken, zodat een verschoven record na een onvolledig record weer gevonden wordt.
      if (record[0] != 0x47 || (record[1] != 0x41 && record[1] != 0x42)) { bestand.seekCur(-1); continue; }
      if (record[1] == 0x41) {
        if (bestand.read(record + 2, 10) != 10) break;
        continue; // runtime-ID uit RUN85 niet als blijvende identificatie gebruiken
      }
      if (bestand.read(record + 2, 13) != 13) break;
      // Eerst de checksum: een foute checksum is geen echt record (bv. na een onderbroken write).
      // Dan één byte na het begin van deze header opnieuw zoeken.
      uint8_t controle = 0;
      for (uint8_t n = 0; n < 14; ++n) controle ^= record[n];
      if (controle != record[14]) { bestand.seekCur(-14); continue; }
      // Een geldig record van een andere sensor of een ander channel: gewoon verder.
      if (record[0] != 0x47 || record[1] != 0x42 ||
          record[2] != static_cast<uint8_t>(static_cast<uint16_t>(sensor.component)) ||
          record[3] != static_cast<uint8_t>(static_cast<uint16_t>(sensor.component) >> 8) ||
          record[4] != static_cast<uint8_t>(id) || record[5] != static_cast<uint8_t>(id >> 8) ||
          record[6] != static_cast<uint8_t>(id >> 16) || record[7] != static_cast<uint8_t>(id >> 24) ||
          record[8] != numberChannel || record[9] > 2) continue;
#if DATA_QUEUE_OR_STACK_ACTIEF
      channel.firstIndex = 0;
      channel.countNumber = 0;
#endif
      channel.valueType = record[9];
      if (channel.valueType == 0) { int32_t waarde; memcpy(&waarde, record + 10, 4); channel.lastValue = static_cast<int>(waarde); }
      else if (channel.valueType == 1) memcpy(&channel.lastValueFloat, record + 10, 4);
      else memcpy(&channel.lastValueUInt32, record + 10, 4);
      gevonden = true;
    }
    bestand.close();
    if (gevonden) return true;
  }
#else
  (void)sensor; (void)numberChannel; (void)channel;
#endif
  return false;
}

bool Data::RecordingStop() {
  // true = er liep een opname, en alles wat open stond, is volledig weggeschreven en gesloten.
  // Zonder lopende opname is er niets bewaard: false.
  bool gelukt = recordingActief;
  recordingActief = false;
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  for (uint8_t teller = 0; teller < DATA_LAYER_MICROSD_SPI_AANTAL; teller++) {
    DataMICROSDviaSPI* kanaal = MICROSDviaSPI[teller];
    if (FileRecordingSensor[teller]) {
      if (kanaal == nullptr || !kanaal->actief) {
        // Uitgeschakelde kaart (bv. een onbetrouwbare CS): geen sync() of close(), want die schrijven naar de kaart.
        // Zonder kaartobject (afgemeld) bestaat de kaart van dit bestand niet meer; ook dan geen kaarttoegang.
        // Het bestandsobject alleen in het geheugen loslaten; de toewijzing doet geen kaarttoegang (SdFat - Adafruit Fork 2.3.103).
        FileRecordingSensor[teller] = File32();
        gelukt = false;
      } else {
        if (!FileRecordingSensor[teller].sync()) gelukt = false;
        // De CS kan tijdens sync() uitvallen; dan niet meer sluiten.
        if (!kanaal->actief) {
          FileRecordingSensor[teller] = File32();
          gelukt = false;
        } else if (!FileRecordingSensor[teller].close()) gelukt = false;
        // Ook een CS-fout tijdens close() maakt de stop onvolledig.
        if (!kanaal->actief) gelukt = false;
      }
      aantalRegelsSindsFlush[teller] = 0;
    }
    if (kanaal != nullptr) kanaal->VrijgevenRuntime(this);
  }
  // Een mislukte write of sync tijdens de opname: niet alle records zijn bewaard.
  if (recordingWriteFailed) gelukt = false;
#endif
  return gelukt;
}

#endif
