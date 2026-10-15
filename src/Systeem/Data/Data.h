#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

#include "../../Configuratie/SystemConfig.h"
#include "../GedeeldeBus/GedeeldeBus.h"

struct Data;

#if (DATA_LAYER_CONFIG) != DATA_TYPE_NONE || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM)
  #include "EEPROM.h"
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  #include "FLASH.h"
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FRAM)
  #include "FRAM_I2C.h"
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
  #include "microSD.h"
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_DIRECT)
struct SensorQueueOrStack {   // één per channel; het geheugen van de QueueOrStack geeft de sensor op
  int lastValue;          // laatst gemeten waarde van dit channel, ook zonder QueueOrStack
#if DATA_QUEUE_OR_STACK_ACTIEF
  int* buffer;
  uint16_t sizeQueueOrStack;
  bool LIFO;              // false = oudste waarde eerst (FIFO), true = nieuwste waarde eerst (LIFO)
  uint16_t firstIndex;    // plaats van de oudste waarde
  uint16_t countNumber;   // aantal waarden in de QueueOrStack
#endif
};
#endif

struct Data : GedeeldeBusNode {
  Data();
  Data(uint8_t typesActief);

  bool aanmelden() override;
  bool controleren() override;
  bool inpluggen() override;
  bool Activeren() override;
  bool afmelden() override;

  void SensorWrite(int ruweData);
  int SensorRead();

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_DIRECT)
  void WriteQueueOrStack(SensorQueueOrStack& channel, int value);
  int8_t ReadQueueOrStackStatus(SensorQueueOrStack& channel);   // 0 = waarde klaar, -1 = leeg
  int ReadQueueOrStackValue(SensorQueueOrStack& channel);       // volgende waarde; leeg = lastValue
#endif

  void StreamingStart();
  void StreamingWrite(int ruweData);
  int StreamingRead();
  void StreamingStop();

  void RecordingStart();
  void RecordingWrite(int ruweData);
  int RecordRead();
  void RecordingStop();

  uint8_t typesActief;
  int ruweData = 0;

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  uint32_t adres[2] = { 0, 0 };
#endif
};

extern struct Data* Data;

#endif

#endif // DATA_H
