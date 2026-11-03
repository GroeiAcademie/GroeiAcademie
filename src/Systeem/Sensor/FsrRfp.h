#ifndef FSR_RFP_H
#define FSR_RFP_H

#include <Arduino.h>
#include "../GedeeldeBus/GedeeldeBus.h"
#include "../Data/Data.h"
#include "../../Configuratie/SystemConfig.h"

// ============================================================================
// FsrRfp
// ============================================================================
// Eén sensorobject voor de maximaal vier FsrRfp-kanalen van één set (1b: set 1, of set 2 als die bestaat).
// De sensor wordt ingeplugd op de gekozen ADC-extender.
// Voorlopig biedt de sensor alleen de directe leesfunctie RawAnalogRead().
// ============================================================================
struct FsrRfp : Sensor {
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  struct Data* Data;
#endif
  uint8_t exclusief_[4];

  const int sensorPin[4];

#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
 // Voorlopig hier om te testen; verhuist daarna naar Data. Ook zonder DIRECT: lastValue per channel.
#if DATA_QUEUE_OR_STACK_ACTIEF
  // B1: elk object heeft zijn eigen buffers (nooit gedeeld). Met set 2: de grootste van set 1 en set 2 per channel;
  // de constructor zet sizeQueueOrStack op de grootte van de eigen set.
  // Een array van grootte 0 is geen geldige C++: dan 1 plaats, die niet gebruikt wordt (sizeQueueOrStack = 0).
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
  int ruweDataQueueOrStackChannel1[(FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) > 0 ? (FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) : 1];
  int ruweDataQueueOrStackChannel2[(FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) > 0 ? (FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) : 1];
  int ruweDataQueueOrStackChannel3[(FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) > 0 ? (FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) : 1];
  int ruweDataQueueOrStackChannel4[(FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) > 0 ? (FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 ? FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2) : 1];
#else
  int ruweDataQueueOrStackChannel1[FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > 0 ? FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : 1];
  int ruweDataQueueOrStackChannel2[FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > 0 ? FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : 1];
  int ruweDataQueueOrStackChannel3[FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > 0 ? FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : 1];
  int ruweDataQueueOrStackChannel4[FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 > 0 ? FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 : 1];
#endif
#endif
  // De sensor kiest per channel: false = FIFO (oudste eerst), true = LIFO (nieuwste eerst).
  SensorQueueOrStack FsrRfptoQueueOrStack[4] = { { 0 }, { 0 }, { 0 }, { 0 } };
#endif

  const uint8_t set;   // 1b: 1 of 2
  const uint8_t probe; // FSR_RFP_PROBE_RFP602, FSR_RFP_PROBE_FSR400 of FSR_RFP_PROBE_FSR402 (FSR_RFP_PROBE_SET_1 / _SET_2)

  unsigned long laatstePoging = 0;
  bool heraangemeldConflictGetoond = false;

  // Runtime-herstel: eigen foutladder alleen met een override (FSR_RFP_CHANNEL_<N>_..._SET_1 of FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1),
  // dan in plaats van die van de ADC-Extender (één ladder per meting). Per kanaal gaat voor per sensor. Meldt GB408; vanaf het
  // uitschakelniveau gaat de ADS1115 uit, en daarmee alle kanalen: een tolerant kanaal beschermt de chip niet tegen een strenger kanaal.
#if ((defined(FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)) || (defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && (defined(FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2))))
  ToestandTijdensRuntime toestandTijdensRuntimePerChannel[4];
#elif (defined(FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || (defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && defined(FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)))
  ToestandTijdensRuntime toestandTijdensRuntime;
#endif
 // V5: geen ADS1115viaParent() meer; FsrRfp meet via de analoge functies van zijn parent, op elke ADC-Extender.
  void MetingTijdensRuntimeVerwerken(int sensorPin, bool gelukt);
  bool OpnieuwAanmeldenNaPauze();

  // 1b: set = 1 (standaard, zoals voorheen) of 2 (alleen als FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 bestaat).
#if EXTENDER_ADS1115_AANTAL > 0
  // Met de standaardkanalen FSR_RFP_ADC_PIN_SENSOR_1_SET_n t/m _4_SET_n van de eigen set.
  FsrRfp(GedeeldeBusNode* parent, GedeeldeBusComponent component,
         HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t set = 1)
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
    : FsrRfp(parent, component, set == 2 ? FSR_RFP_ADC_PIN_SENSOR_1_SET_2 : FSR_RFP_ADC_PIN_SENSOR_1_SET_1, set == 2 ? FSR_RFP_ADC_PIN_SENSOR_2_SET_2 : FSR_RFP_ADC_PIN_SENSOR_2_SET_1, set == 2 ? FSR_RFP_ADC_PIN_SENSOR_3_SET_2 : FSR_RFP_ADC_PIN_SENSOR_3_SET_1, set == 2 ? FSR_RFP_ADC_PIN_SENSOR_4_SET_2 : FSR_RFP_ADC_PIN_SENSOR_4_SET_1, Extender, set) {}
#else
    : FsrRfp(parent, component, FSR_RFP_ADC_PIN_SENSOR_1_SET_1, FSR_RFP_ADC_PIN_SENSOR_2_SET_1, FSR_RFP_ADC_PIN_SENSOR_3_SET_1, FSR_RFP_ADC_PIN_SENSOR_4_SET_1, Extender, set) {}
#endif
#endif

  FsrRfp(GedeeldeBusNode* parent, GedeeldeBusComponent component,
         int sensorPin1, int sensorPin2, int sensorPin3, int sensorPin4,
         HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t set = 1)
#if DATA_LAYER_CONFIG == DATA_TYPE_NONE
    : Sensor(parent, { nullptr, 0, exclusief_,
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
                     static_cast<uint8_t>(set == 2 ? FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 : FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1)
#else
                     FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1
#endif
                     }, component, Extender),
      sensorPin{sensorPin1, sensorPin2, sensorPin3, sensorPin4}, set(set), probe(set == 2 ? FSR_RFP_PROBE_SET_2 : FSR_RFP_PROBE_SET_1) {
    for (uint8_t i = 0; i < 4; i++) exclusief_[i] = static_cast<uint8_t>(sensorPin[i]);
  }
#else
    : FsrRfp(parent, component, ::Data, sensorPin1, sensorPin2, sensorPin3, sensorPin4, Extender, set) {}

#if EXTENDER_ADS1115_AANTAL > 0
  FsrRfp(GedeeldeBusNode* parent, GedeeldeBusComponent component, struct Data* Data,
         HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t set = 1)
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
    : FsrRfp(parent, component, Data, set == 2 ? FSR_RFP_ADC_PIN_SENSOR_1_SET_2 : FSR_RFP_ADC_PIN_SENSOR_1_SET_1, set == 2 ? FSR_RFP_ADC_PIN_SENSOR_2_SET_2 : FSR_RFP_ADC_PIN_SENSOR_2_SET_1, set == 2 ? FSR_RFP_ADC_PIN_SENSOR_3_SET_2 : FSR_RFP_ADC_PIN_SENSOR_3_SET_1, set == 2 ? FSR_RFP_ADC_PIN_SENSOR_4_SET_2 : FSR_RFP_ADC_PIN_SENSOR_4_SET_1, Extender, set) {}
#else
    : FsrRfp(parent, component, Data, FSR_RFP_ADC_PIN_SENSOR_1_SET_1, FSR_RFP_ADC_PIN_SENSOR_2_SET_1, FSR_RFP_ADC_PIN_SENSOR_3_SET_1, FSR_RFP_ADC_PIN_SENSOR_4_SET_1, Extender, set) {}
#endif
#endif

  FsrRfp(GedeeldeBusNode* parent, GedeeldeBusComponent component, struct Data* Data,
         int sensorPin1, int sensorPin2, int sensorPin3, int sensorPin4,
         HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t set = 1)
    : Sensor(parent, { nullptr, 0, exclusief_,
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
                     static_cast<uint8_t>(set == 2 ? FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 : FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1)
#else
                     FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1
#endif
                     }, component, Extender), Data(Data),
      sensorPin{sensorPin1, sensorPin2, sensorPin3, sensorPin4}, set(set), probe(set == 2 ? FSR_RFP_PROBE_SET_2 : FSR_RFP_PROBE_SET_1) {
    for (uint8_t i = 0; i < 4; i++) exclusief_[i] = static_cast<uint8_t>(sensorPin[i]);
#if DATA_QUEUE_OR_STACK_ACTIEF
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
    if (set == 2) {
      FsrRfptoQueueOrStack[0] = { 0, ruweDataQueueOrStackChannel1, FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2, false, 0, 0 };
      FsrRfptoQueueOrStack[1] = { 0, ruweDataQueueOrStackChannel2, FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2, false, 0, 0 };
      FsrRfptoQueueOrStack[2] = { 0, ruweDataQueueOrStackChannel3, FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2, false, 0, 0 };
      FsrRfptoQueueOrStack[3] = { 0, ruweDataQueueOrStackChannel4, FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2, false, 0, 0 };
    } else
#endif
    {
      FsrRfptoQueueOrStack[0] = { 0, ruweDataQueueOrStackChannel1, FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1, false, 0, 0 };
      FsrRfptoQueueOrStack[1] = { 0, ruweDataQueueOrStackChannel2, FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1, false, 0, 0 };
      FsrRfptoQueueOrStack[2] = { 0, ruweDataQueueOrStackChannel3, FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1, false, 0, 0 };
      FsrRfptoQueueOrStack[3] = { 0, ruweDataQueueOrStackChannel4, FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1, false, 0, 0 };
    }
#endif
    QueueOrStack = FsrRfptoQueueOrStack;
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
    numberOfChannels = static_cast<uint8_t>(set == 2 ? FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 : FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1);
#else
    numberOfChannels = FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1;
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

extern struct FsrRfp* sensorFsrRfpSet1;
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
extern struct FsrRfp* sensorFsrRfpSet2;   // 1b: set 2
#endif

#endif
