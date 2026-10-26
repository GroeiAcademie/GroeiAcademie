#ifndef CONFIG_RFP602_H
#define CONFIG_RFP602_H

// ============================================================================
// RFP602: jouw instellingen ==================================================
// ============================================================================
// LET OP: RFP602 meet via een ADC-Extender (nu: ADS1115). Daarvoor moet de ADS1X15-library van Rob Tillaart geïnstalleerd zijn via de Arduino Library Manager.
// ============================================================================
#ifndef RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1
// #define RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1  2         // standaard waarde: 2
#endif

// ============================================================================
// SENSOREN ===================================================================
// ============================================================================

// ============================================================================
// RFP602 DATA
// ============================================================================
#ifndef RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
// #define RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 0  // 0 = laatste gemeten waarde; > 0 = RAM QueueOrStack (FIFO of LIFO)
#endif

#ifndef RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
// #define RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1  // standaard waarde: RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1; QueueOrStack-grootte voor channel 1 (RFP602-sensor 1)
#endif
#ifndef RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
// #define RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1  // standaard waarde: RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1; QueueOrStack-grootte voor channel 2 (RFP602-sensor 2)
#endif
#ifndef RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
// #define RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1  // standaard waarde: RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1; QueueOrStack-grootte voor channel 3 (RFP602-sensor 3)
#endif
#ifndef RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
// #define RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1  // standaard waarde: RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1; QueueOrStack-grootte voor channel 4 (RFP602-sensor 4)
#endif

#ifndef RFP602_DATA_RECORD_FILE_SET_1
// #define RFP602_DATA_RECORD_FILE_SET_1 "RFP602.log"
#endif


// ============================================================================
// RFP602
// ============================================================================

// Standaardkanalen van de RFP602-sensoren op de ADC-Extender (ADS1115: ingangen AIN0 tot AIN3).
// Een .ino kan per RFP602-instantie ook zelf vier andere kanalen opgeven.
#ifndef RFP602_ADC_PIN_SENSOR_1_SET_1
// #define RFP602_ADC_PIN_SENSOR_1_SET_1 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0)  // standaard waarde
#endif
#ifndef RFP602_ADC_PIN_SENSOR_2_SET_1
// #define RFP602_ADC_PIN_SENSOR_2_SET_1 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN1)  // standaard waarde
#endif
#ifndef RFP602_ADC_PIN_SENSOR_3_SET_1
// #define RFP602_ADC_PIN_SENSOR_3_SET_1 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN2)  // standaard waarde
#endif
#ifndef RFP602_ADC_PIN_SENSOR_4_SET_1
// #define RFP602_ADC_PIN_SENSOR_4_SET_1 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN3)  // standaard waarde
#endif

// ============================================================================
// RFP602 SET 2 (optioneel)
// ============================================================================
// Een tweede set bestaat alleen als RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 gedefinieerd is (2 of 4). Zonder die regel: geen extra geheugen of code.
// De .ino geeft set 2 haar eigen ADC-Extender als parent, en set = 2 als laatste argument van de constructor.
// Voorbeeld: examples/Systeem/GedeeldeBus/Sensoren/Sensor_RFP602_TweeSetsOpTweeADS1115
#ifndef RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2
// #define RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2  2         // standaard: niet ingesteld (geen set 2)
#endif
#ifndef RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
// #define RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 0  // 0 = laatste gemeten waarde; > 0 = RAM QueueOrStack (FIFO of LIFO)
#endif
#ifndef RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
// #define RFP602_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2  // standaard waarde: RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
#endif
#ifndef RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
// #define RFP602_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2  // standaard waarde: RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
#endif
#ifndef RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
// #define RFP602_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2  // standaard waarde: RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
#endif
#ifndef RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
// #define RFP602_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2  // standaard waarde: RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
#endif
// RFP602_DATA_RECORD_FILE_SET_2 bestaat nog niet: de Data-laag schrijft één opnamebestand (RFP602_DATA_RECORD_FILE_SET_1).
#ifndef RFP602_ADC_PIN_SENSOR_1_SET_2
// #define RFP602_ADC_PIN_SENSOR_1_SET_2 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0)  // standaard waarde
#endif
#ifndef RFP602_ADC_PIN_SENSOR_2_SET_2
// #define RFP602_ADC_PIN_SENSOR_2_SET_2 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN1)  // standaard waarde
#endif
#ifndef RFP602_ADC_PIN_SENSOR_3_SET_2
// #define RFP602_ADC_PIN_SENSOR_3_SET_2 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN2)  // standaard waarde
#endif
#ifndef RFP602_ADC_PIN_SENSOR_4_SET_2
// #define RFP602_ADC_PIN_SENSOR_4_SET_2 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN3)  // standaard waarde
#endif
#endif
