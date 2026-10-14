#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

#include "../../Configuratie/SystemConfig.h"
#include "../GedeeldeBus/GedeeldeBus.h"

struct Data;

#if (DATA_LAYER_CONFIG) != DATA_TYPE_NONE || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

#if DATA_FIFO_ACTIEF
  #include <CircularBuffer.hpp>
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM)
  #include "EEPROM.h"
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
  #include "microSD.h"
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  #include "FLASH.h"
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_DIRECT) && DATA_FIFO_ACTIEF
struct SensorFIFO {   // één FIFO per channel; het geheugen geeft de sensor op
  int* buffer;
  uint16_t sizeFIFO;
  bool LIFO;          // false = oudste waarde eerst (FIFO), true = nieuwste waarde eerst (LIFO)
  uint16_t first;     // plaats van de oudste waarde
  uint16_t count;     // aantal waarden in de FIFO
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

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_DIRECT) && DATA_FIFO_ACTIEF
  void WriteFIFO(SensorFIFO& channel, int value);
  bool ReadFIFO(SensorFIFO& channel, int& value);
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
