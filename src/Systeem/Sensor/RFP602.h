#ifndef RFP602_H
#define RFP602_H

#include <Arduino.h>
#include "../GedeeldeBus/GedeeldeBus.h"
#include "../Data/Data.h"
#include "../../Configuratie/SystemConfig.h"

// ============================================================================
// RFP602
// ============================================================================
// Eén sensorobject voor de maximaal vier RFP602-kanalen van één set (1b: set 1, of set 2 als die bestaat).
// De sensor wordt ingeplugd op de gekozen ADC-extender.
// Voorlopig biedt de sensor alleen de directe leesfunctie RawAnalogRead().
// ============================================================================
struct RFP602 : Sensor {
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  struct Data* Data;
#endif
  uint8_t exclusief_[4];

  const int sensorPin[4];

#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  // Voorlopig hier om te testen; verhuist daarna naar Data. Ook zonder DIRECT: lastValue per channel (V3).
#if DATA_QUEUE_OR_STACK_ACTIEF
  // B1: elk object heeft zijn eigen buffers (nooit gedeeld). Met set 2: de grootste van set 1 en set 2 per channel;
  // de constructor zet sizeQueueOrStack op de grootte van de eigen set.
  // Een array van grootte 0 is geen geldige C++: dan 1 plaats, die niet gebruikt wordt (sizeQueueOrStack = 0).
#if defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
  int ruweDataQueueOrStackChannel1[(RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) > 0 ? (RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) : 1];
  int ruweDataQueueOrStackChannel2[(RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) > 0 ? (RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) : 1];
  int ruweDataQueueOrStackChannel3[(RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) > 0 ? (RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) : 1];
  int ruweDataQueueOrStackChannel4[(RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) > 0 ? (RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) : 1];
#else
  int ruweDataQueueOrStackChannel1[RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > 0 ? RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : 1];
  int ruweDataQueueOrStackChannel2[RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > 0 ? RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : 1];
  int ruweDataQueueOrStackChannel3[RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > 0 ? RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : 1];
  int ruweDataQueueOrStackChannel4[RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > 0 ? RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : 1];
#endif
#endif
  // De sensor kiest per channel: false = FIFO (oudste eerst), true = LIFO (nieuwste eerst).
  SensorQueueOrStack RFP602toQueueOrStack[4] = { { 0 }, { 0 }, { 0 }, { 0 } };
#endif

  const uint8_t set;   // 1b: 1 of 2

  unsigned long laatstePoging = 0;
  bool heraangemeldConflictGetoond = false;

  // Runtime-herstel (D042): eigen foutladder alleen met een override (RFP602_CHANNEL_<N>_..._SET_1 of RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1),
  // dan in plaats van die van de ADC-Extender (één ladder per meting). Per kanaal gaat voor per sensor. Meldt GB408; vanaf het
  // uitschakelniveau gaat de ADS1115 uit, en daarmee alle kanalen: een tolerant kanaal beschermt de chip niet tegen een strenger kanaal.
#if ((defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)) || (defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && (defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2))))
  ToestandTijdensRuntime toestandTijdensRuntimePerChannel[4];
#elif (defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || (defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)))
  ToestandTijdensRuntime toestandTijdensRuntime;
#endif
  // V5: geen ADS1115viaParent() meer; RFP602 meet via de analoge functies van zijn parent (V8), op elke ADC-Extender.
  void MetingTijdensRuntimeVerwerken(int sensorPin, bool gelukt);
  bool OpnieuwAanmeldenNaPauze();

  // 1b: set = 1 (standaard, zoals voorheen) of 2 (alleen als RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 bestaat).
#if EXTENDER_ADS1115_AANTAL > 0
  // Met de standaardkanalen RFP602_ADC_PIN_SENSOR_1_SET_n t/m _4_SET_n van de eigen set.
  RFP602(GedeeldeBusNode* parent, GedeeldeBusComponent component,
         HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t set = 1)
#if defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
    : RFP602(parent, component, set == 2 ? RFP602_ADC_PIN_SENSOR_1_SET_2 : RFP602_ADC_PIN_SENSOR_1_SET_1, set == 2 ? RFP602_ADC_PIN_SENSOR_2_SET_2 : RFP602_ADC_PIN_SENSOR_2_SET_1, set == 2 ? RFP602_ADC_PIN_SENSOR_3_SET_2 : RFP602_ADC_PIN_SENSOR_3_SET_1, set == 2 ? RFP602_ADC_PIN_SENSOR_4_SET_2 : RFP602_ADC_PIN_SENSOR_4_SET_1, Extender, set) {}
#else
    : RFP602(parent, component, RFP602_ADC_PIN_SENSOR_1_SET_1, RFP602_ADC_PIN_SENSOR_2_SET_1, RFP602_ADC_PIN_SENSOR_3_SET_1, RFP602_ADC_PIN_SENSOR_4_SET_1, Extender, set) {}
#endif
#endif

  RFP602(GedeeldeBusNode* parent, GedeeldeBusComponent component,
         int sensorPin1, int sensorPin2, int sensorPin3, int sensorPin4,
         HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t set = 1)
#if DATA_LAYER_CONFIG == DATA_TYPE_NONE
    : Sensor(parent, { nullptr, 0, exclusief_,
#if defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
                     static_cast<uint8_t>(set == 2 ? RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 : RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1)
#else
                     RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1
#endif
                     }, component, Extender),
      sensorPin{sensorPin1, sensorPin2, sensorPin3, sensorPin4}, set(set) {
    for (uint8_t i = 0; i < 4; i++) exclusief_[i] = static_cast<uint8_t>(sensorPin[i]);
  }
#else
    : RFP602(parent, component, ::Data, sensorPin1, sensorPin2, sensorPin3, sensorPin4, Extender, set) {}

#if EXTENDER_ADS1115_AANTAL > 0
  RFP602(GedeeldeBusNode* parent, GedeeldeBusComponent component, struct Data* Data,
         HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t set = 1)
#if defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
    : RFP602(parent, component, Data, set == 2 ? RFP602_ADC_PIN_SENSOR_1_SET_2 : RFP602_ADC_PIN_SENSOR_1_SET_1, set == 2 ? RFP602_ADC_PIN_SENSOR_2_SET_2 : RFP602_ADC_PIN_SENSOR_2_SET_1, set == 2 ? RFP602_ADC_PIN_SENSOR_3_SET_2 : RFP602_ADC_PIN_SENSOR_3_SET_1, set == 2 ? RFP602_ADC_PIN_SENSOR_4_SET_2 : RFP602_ADC_PIN_SENSOR_4_SET_1, Extender, set) {}
#else
    : RFP602(parent, component, Data, RFP602_ADC_PIN_SENSOR_1_SET_1, RFP602_ADC_PIN_SENSOR_2_SET_1, RFP602_ADC_PIN_SENSOR_3_SET_1, RFP602_ADC_PIN_SENSOR_4_SET_1, Extender, set) {}
#endif
#endif

  RFP602(GedeeldeBusNode* parent, GedeeldeBusComponent component, struct Data* Data,
         int sensorPin1, int sensorPin2, int sensorPin3, int sensorPin4,
         HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t set = 1)
    : Sensor(parent, { nullptr, 0, exclusief_,
#if defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
                     static_cast<uint8_t>(set == 2 ? RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 : RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1)
#else
                     RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1
#endif
                     }, component, Extender), Data(Data),
      sensorPin{sensorPin1, sensorPin2, sensorPin3, sensorPin4}, set(set) {
    for (uint8_t i = 0; i < 4; i++) exclusief_[i] = static_cast<uint8_t>(sensorPin[i]);
#if DATA_QUEUE_OR_STACK_ACTIEF
#if defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
    if (set == 2) {
      RFP602toQueueOrStack[0] = { 0, ruweDataQueueOrStackChannel1, RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2, false, 0, 0 };
      RFP602toQueueOrStack[1] = { 0, ruweDataQueueOrStackChannel2, RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2, false, 0, 0 };
      RFP602toQueueOrStack[2] = { 0, ruweDataQueueOrStackChannel3, RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2, false, 0, 0 };
      RFP602toQueueOrStack[3] = { 0, ruweDataQueueOrStackChannel4, RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2, false, 0, 0 };
    } else
#endif
    {
      RFP602toQueueOrStack[0] = { 0, ruweDataQueueOrStackChannel1, RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1, false, 0, 0 };
      RFP602toQueueOrStack[1] = { 0, ruweDataQueueOrStackChannel2, RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1, false, 0, 0 };
      RFP602toQueueOrStack[2] = { 0, ruweDataQueueOrStackChannel3, RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1, false, 0, 0 };
      RFP602toQueueOrStack[3] = { 0, ruweDataQueueOrStackChannel4, RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1, false, 0, 0 };
    }
#endif
    QueueOrStack = RFP602toQueueOrStack;
#if defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
    numberOfChannels = static_cast<uint8_t>(set == 2 ? RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 : RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1);
#else
    numberOfChannels = RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1;
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

extern struct RFP602* sensorRFP602set1;
#if defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
extern struct RFP602* sensorRFP602set2;   // 1b: set 2
#endif

#endif
