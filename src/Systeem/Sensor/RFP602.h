#ifndef RFP602_H
#define RFP602_H

#include <Arduino.h>
#include "../GedeeldeBus/GedeeldeBus.h"
#include "../Data/Data.h"
#include "../../Configuratie/SystemConfig.h"

// ============================================================================
// RFP602
// ============================================================================
// Eén sensorobject voor de maximaal vier RFP602-kanalen van Stimulus.
// De sensor wordt ingeplugd op de gekozen ADC-extender.
// Voorlopig biedt de sensor alleen de directe leesfunctie RawAnalogRead().
// ============================================================================
struct RFP602 : Sensor {
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  struct Data* Data;
#endif
  uint8_t exclusief_[4];

  const int sensorPin[4] = {
#if ADC_BACKEND == ADC_BACKEND_NATIVE
    NativeArduinoPinVan(ADC_PIN_SENSOR_1),
    NativeArduinoPinVan(ADC_PIN_SENSOR_2),
    NativeArduinoPinVan(ADC_PIN_SENSOR_3),
    NativeArduinoPinVan(ADC_PIN_SENSOR_4)
#else
    static_cast<int>(ADC_PIN_SENSOR_1),
    static_cast<int>(ADC_PIN_SENSOR_2),
    static_cast<int>(ADC_PIN_SENSOR_3),
    static_cast<int>(ADC_PIN_SENSOR_4)
#endif
  };

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_DIRECT) && DATA_FIFO_ACTIEF
  // Voorlopig hier om te testen; verhuist daarna naar Data.
  // Een array van grootte 0 is geen geldige C++: dan 1 plaats, die niet gebruikt wordt (sizeFIFO = 0).
  int ruweDataFIFOchannel1[RFP602_KANAAL_1_DATA_LAYER_DIRECT_FIFO_SIZE > 0 ? RFP602_KANAAL_1_DATA_LAYER_DIRECT_FIFO_SIZE : 1];
  int ruweDataFIFOchannel2[RFP602_KANAAL_2_DATA_LAYER_DIRECT_FIFO_SIZE > 0 ? RFP602_KANAAL_2_DATA_LAYER_DIRECT_FIFO_SIZE : 1];
  int ruweDataFIFOchannel3[RFP602_KANAAL_3_DATA_LAYER_DIRECT_FIFO_SIZE > 0 ? RFP602_KANAAL_3_DATA_LAYER_DIRECT_FIFO_SIZE : 1];
  int ruweDataFIFOchannel4[RFP602_KANAAL_4_DATA_LAYER_DIRECT_FIFO_SIZE > 0 ? RFP602_KANAAL_4_DATA_LAYER_DIRECT_FIFO_SIZE : 1];
  // De sensor kiest per channel: false = FIFO (oudste eerst), true = LIFO (nieuwste eerst).
  SensorFIFO RFP602toFIFO[4] = {
    { ruweDataFIFOchannel1, RFP602_KANAAL_1_DATA_LAYER_DIRECT_FIFO_SIZE, false, 0, 0 },
    { ruweDataFIFOchannel2, RFP602_KANAAL_2_DATA_LAYER_DIRECT_FIFO_SIZE, false, 0, 0 },
    { ruweDataFIFOchannel3, RFP602_KANAAL_3_DATA_LAYER_DIRECT_FIFO_SIZE, false, 0, 0 },
    { ruweDataFIFOchannel4, RFP602_KANAAL_4_DATA_LAYER_DIRECT_FIFO_SIZE, false, 0, 0 }
  };
#endif

  unsigned long laatstePoging = 0;
  bool heraangemeldConflictGetoond = false;

  RFP602();
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  RFP602(struct Data* Data);
#endif
  RFP602(GedeeldeBusNode* parent, GedeeldeBusComponent component,
         HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
#if DATA_LAYER_CONFIG == DATA_TYPE_NONE
    : Sensor(parent, { nullptr, 0, exclusief_, AANTAL_SENSOREN_AANWEZIG }, component, Extender) {
    exclusief_[0] = static_cast<uint8_t>(ADC_PIN_SENSOR_1);
    exclusief_[1] = static_cast<uint8_t>(ADC_PIN_SENSOR_2);
    exclusief_[2] = static_cast<uint8_t>(ADC_PIN_SENSOR_3);
    exclusief_[3] = static_cast<uint8_t>(ADC_PIN_SENSOR_4);
  }
#else
    : RFP602(parent, component, ::Data, Extender) {}

  RFP602(GedeeldeBusNode* parent, GedeeldeBusComponent component, struct Data* Data,
         HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : Sensor(parent, { nullptr, 0, exclusief_, AANTAL_SENSOREN_AANWEZIG }, component, Extender), Data(Data) {
    exclusief_[0] = static_cast<uint8_t>(ADC_PIN_SENSOR_1);
    exclusief_[1] = static_cast<uint8_t>(ADC_PIN_SENSOR_2);
    exclusief_[2] = static_cast<uint8_t>(ADC_PIN_SENSOR_3);
    exclusief_[3] = static_cast<uint8_t>(ADC_PIN_SENSOR_4);
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_DIRECT) && DATA_FIFO_ACTIEF
    FIFO = RFP602toFIFO;
    numberOfChannels = AANTAL_SENSOREN_AANWEZIG;
#endif
  }
#endif

  bool Activeren() override;
  int RawAnalogRead(int sensorPin);

  // Niet-blokkerend lezen: StartAnalogRead() start een meting, IsAnalogReadKlaar() meldt of ze klaar is,
  // GetAnalogRead() geeft de waarde terug en geeft ze door aan Data. RawAnalogRead() blijft blokkerend.
  bool StartAnalogRead(int sensorPin);
  bool IsAnalogReadKlaar();
  int GetAnalogRead();
  int laatstePin = 0;
  int laatsteWaarde = 0;
};

extern struct RFP602* sensorRFP602;

#endif
