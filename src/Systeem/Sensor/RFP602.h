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

#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  // Voorlopig hier om te testen; verhuist daarna naar Data. Ook zonder DIRECT: lastValue per channel (V3).
#if DATA_QUEUE_OR_STACK_ACTIEF
  // Een array van grootte 0 is geen geldige C++: dan 1 plaats, die niet gebruikt wordt (sizeQueueOrStack = 0).
  int ruweDataQueueOrStackchannel1[RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE > 0 ? RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE : 1];
  int ruweDataQueueOrStackchannel2[RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE > 0 ? RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE : 1];
  int ruweDataQueueOrStackchannel3[RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE > 0 ? RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE : 1];
  int ruweDataQueueOrStackchannel4[RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE > 0 ? RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE : 1];
  // De sensor kiest per channel: false = FIFO (oudste eerst), true = LIFO (nieuwste eerst).
  SensorQueueOrStack RFP602toQueueOrStack[4] = {
    { 0, ruweDataQueueOrStackchannel1, RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE, false, 0, 0 },
    { 0, ruweDataQueueOrStackchannel2, RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE, false, 0, 0 },
    { 0, ruweDataQueueOrStackchannel3, RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE, false, 0, 0 },
    { 0, ruweDataQueueOrStackchannel4, RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE, false, 0, 0 }
  };
#else
  SensorQueueOrStack RFP602toQueueOrStack[4] = { { 0 }, { 0 }, { 0 }, { 0 } };   // alleen lastValue per channel
#endif
#endif

  unsigned long laatstePoging = 0;
  bool heraangemeldConflictGetoond = false;

  // Runtime-herstel (D042): eigen foutladder alleen met een override (RFP602_CHANNEL_<N>_ of RFP602_RUNTIME_DREMPELS_FOUTNIVEAU),
  // dan in plaats van die van de ADS1115 (één ladder per meting). Per kanaal gaat voor per sensor. Meldt GB408; vanaf het
  // uitschakelniveau gaat de ADS1115 uit, en daarmee alle kanalen: een tolerant kanaal beschermt de chip niet tegen een strenger kanaal.
#if ADC_BACKEND == ADC_BACKEND_ADS1115 && (defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU) || defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU) || defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU) || defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU))
  ToestandTijdensRuntime toestandTijdensRuntimePerKanaal[4];
#elif ADC_BACKEND == ADC_BACKEND_ADS1115 && defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU)
  ToestandTijdensRuntime toestandTijdensRuntime;
#endif
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  void MetingTijdensRuntimeVerwerken(int sensorPin, bool gelukt);
  bool nietBlokkerendeMetingMislukt = false;   // een stap van StartAnalogRead()/IsAnalogReadKlaar() mislukte; GetAnalogRead() geeft het resultaat door
#endif
  bool OpnieuwAanmeldenNaPauze();

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
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    QueueOrStack = RFP602toQueueOrStack;
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
