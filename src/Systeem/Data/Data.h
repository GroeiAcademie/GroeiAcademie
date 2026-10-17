#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

#include "../../Configuratie/SystemConfig.h"
#include "../GedeeldeBus/GedeeldeBus.h"

struct Data;

#if (DATA_LAYER_CONFIG) != DATA_TYPE_NONE || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

// Eén bestand per medium, met alle aansluitingen van dat medium.
#if ((DATA_LAYER_CONFIG) & (DATA_TYPE_EEPROM_NATIVE | DATA_TYPE_EEPROM_I2C))
  #include "EEPROM.h"
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
  #include "FLASH.h"
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FRAM_I2C)
  #include "FRAM.h"
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
  #include "microSD.h"
#endif

// Eén per channel; het geheugen van de QueueOrStack geeft de sensor op.
// Bestaat bij elk Data-type: lastValue is ook de terugval van StreamingRead() en RecordRead().
struct SensorQueueOrStack {
  int lastValue;          // laatst gemeten waarde van dit channel, ook zonder QueueOrStack
#if DATA_QUEUE_OR_STACK_ACTIEF
  int* buffer;
  uint16_t sizeQueueOrStack;
  bool LIFO;              // false = oudste waarde eerst (FIFO), true = nieuwste waarde eerst (LIFO)
  uint16_t firstIndex;    // plaats van de oudste waarde
  uint16_t countNumber;   // aantal waarden in de QueueOrStack
#endif
};

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_DIRECT)
// DIRECT: kind van Data, zonder claims (V1). De waarde wordt doorgegeven aan de lezer via lastValue en de QueueOrStack van het channel.
struct DataDIRECT : GedeeldeBusNode {
  DataDIRECT(GedeeldeBusNode* parent)
    : GedeeldeBusNode(parent, { nullptr, 0, nullptr, 0 }, GedeeldeBusComponent::DATA_DIRECT, HardwareResourceToegang::GEDEELD) {}
};
#endif

struct Data : GedeeldeBusNode {
  Data();
  Data(uint16_t typesActief);

  bool aanmelden() override;
  bool controleren() override;
  bool inpluggen() override;
  bool Activeren() override;
  bool afmelden() override;

  void WriteQueueOrStack(SensorQueueOrStack& channel, int value);
  int8_t ReadQueueOrStackStatus(SensorQueueOrStack& channel);   // 0 = waarde klaar, -1 = leeg
  int ReadQueueOrStackValue(SensorQueueOrStack& channel);       // volgende waarde; leeg = lastValue

  void StreamingStart();
  void StreamingWrite(int ruweData);
  int StreamingRead(SensorQueueOrStack& channel);               // laatste waarde uit FLASH; anders lastValue van dit channel
  void StreamingStop();

  void RecordingStart();
  void RecordingWrite(int ruweData);
  int RecordRead(SensorQueueOrStack& channel);                  // eerste waarde uit het logbestand; anders lastValue van dit channel
  void RecordingStop();

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_NATIVE)
  // adres 0 = het eerste byte na het blok van HX1838. Buiten DATA_LAYER_EEPROM_NATIVE_SIZE: lezen geeft 0, schrijven doet niets.
  uint8_t ReadNativeEEPROM(uint16_t adres);
  void WriteNativeEEPROM(uint16_t adres, uint8_t waarde);
#endif

  uint16_t typesActief;

  // Per medium de lijst van zijn chips (N7). nullptr = niet aangemaakt of afgemeld; de chip wist zijn plaats zelf in zijn destructor (N3 b).
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_NATIVE) && BOARD_VERSION != BOARD_UNO_Q
  struct DataEEPROMviaNative* EEPROMviaNative = nullptr;
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_I2C)
  struct DataEEPROMviaI2C* EEPROMviaI2C[DATA_LAYER_EEPROM_I2C_AANTAL] = {};
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
  struct DataFLASHviaSPI* FLASHviaSPI[DATA_LAYER_FLASH_SPI_AANTAL] = {};
  uint32_t adres[DATA_LAYER_FLASH_SPI_AANTAL] = {};
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FRAM_I2C)
  struct DataFRAMviaI2C* FRAMviaI2C[DATA_LAYER_FRAM_I2C_AANTAL] = {};
#endif
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
  struct DataMICROSDviaSPI* MICROSDviaSPI[DATA_LAYER_MICROSD_SPI_AANTAL] = {};
#endif
};

extern struct Data* Data;

#endif

#endif // DATA_H
