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

  void StreamStart();
  void StreamWrite(int ruweData);
  int StreamRead();
  void StreamStop();

  void RecordStart();
  void RecordWrite(int ruweData);
  int RecordRead();
  void RecordStop();

  uint8_t typesActief;
  int ruweData = 0;

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_DIRECT) && RFP602_DATA_LAYER_DIRECT_FIFO_SIZE > 0
  CircularBuffer<int, RFP602_DATA_LAYER_DIRECT_FIFO_SIZE> ruweDataFIFO;
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)
  uint32_t adres[2] = { 0, 0 };
#endif
};

extern struct Data* Data;

#endif

#endif // DATA_H
