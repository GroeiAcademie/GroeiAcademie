#ifndef CONFIG_FSR_RFP_H
#define CONFIG_FSR_RFP_H

// ============================================================================
// FsrRfp: jouw instellingen ==================================================
// ============================================================================
// LET OP: FsrRfp meet via een ADC-Extender (nu: ADS1115). Daarvoor moet de ADS1X15-library van Rob Tillaart geïnstalleerd zijn via de Arduino Library Manager.
// ============================================================================
#ifndef FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1
// #define FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1  2         // standaard waarde: 2
#endif

#ifndef FSR_RFP_PROBE_SET_1
// #define FSR_RFP_PROBE_SET_1 FSR_RFP_PROBE_RFP602  // standaard waarde: FSR_RFP_PROBE_RFP602; of FSR_RFP_PROBE_FSR400, FSR_RFP_PROBE_FSR402
#endif

#ifndef FSR_RFP_PROBE_SET_2
// #define FSR_RFP_PROBE_SET_2 FSR_RFP_PROBE_RFP602  // standaard waarde: FSR_RFP_PROBE_RFP602; alleen bij set 2
#endif

// ============================================================================
// SENSOREN ===================================================================
// ============================================================================

// ============================================================================
// FsrRfp DATA
// ============================================================================
#ifndef FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
// #define FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 0  // 0 = laatste gemeten waarde; > 0 = RAM QueueOrStack (FIFO of LIFO)
#endif

#ifndef FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
// #define FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1  // standaard waarde: FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1; QueueOrStack-grootte voor channel 1 (FsrRfp-sensor 1)
#endif

#ifndef FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
// #define FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1  // standaard waarde: FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1; QueueOrStack-grootte voor channel 2 (FsrRfp-sensor 2)
#endif

#ifndef FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
// #define FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1  // standaard waarde: FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1; QueueOrStack-grootte voor channel 3 (FsrRfp-sensor 3)
#endif

#ifndef FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
// #define FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1  // standaard waarde: FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1; QueueOrStack-grootte voor channel 4 (FsrRfp-sensor 4)
#endif


// ============================================================================
// FsrRfp
// ============================================================================
#ifndef FSR_RFP_ADC_BEREIK_MILLIVOLT
// #define FSR_RFP_ADC_BEREIK_MILLIVOLT 5000  // standaard: 5000 (spanningsdeler op 5 V; op 3,3 V: 3300); afstemmen op analoge schakeling
#endif


// Standaardkanalen van de FsrRfp-sensoren op de ADC-Extender (ADS1115: ingangen AIN0 tot AIN3).
// Een .ino kan per FsrRfp-instantie ook zelf vier andere kanalen opgeven.
#ifndef FSR_RFP_ADC_PIN_SENSOR_1_SET_1
// #define FSR_RFP_ADC_PIN_SENSOR_1_SET_1 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0)  // standaard waarde
#endif

#ifndef FSR_RFP_ADC_PIN_SENSOR_2_SET_1
// #define FSR_RFP_ADC_PIN_SENSOR_2_SET_1 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN1)  // standaard waarde
#endif

#ifndef FSR_RFP_ADC_PIN_SENSOR_3_SET_1
// #define FSR_RFP_ADC_PIN_SENSOR_3_SET_1 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN2)  // standaard waarde
#endif

#ifndef FSR_RFP_ADC_PIN_SENSOR_4_SET_1
// #define FSR_RFP_ADC_PIN_SENSOR_4_SET_1 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN3)  // standaard waarde
#endif

// ============================================================================
// FsrRfp SET 2 (optioneel)
// ============================================================================
// Een tweede set bestaat alleen als FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 gedefinieerd is (2 of 4). Zonder die regel: geen extra geheugen of code.
// De .ino geeft set 2 haar eigen ADC-Extender als parent, en set = 2 als laatste argument van de constructor.
// Voorbeeld: examples/Systeem/GedeeldeBus/Sensoren/Sensor_FsrRfp_TweeSetsOpTweeADS1115
#ifndef FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2
// #define FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2  2         // standaard: niet ingesteld (geen set 2)
#endif

#ifndef FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
// #define FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 0  // 0 = laatste gemeten waarde; > 0 = RAM QueueOrStack (FIFO of LIFO)
#endif

#ifndef FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
// #define FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2  // standaard waarde: FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
#endif

#ifndef FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
// #define FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2  // standaard waarde: FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
#endif

#ifndef FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
// #define FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2  // standaard waarde: FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
#endif

#ifndef FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
// #define FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2  // standaard waarde: FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
#endif

#ifndef FSR_RFP_ADC_PIN_SENSOR_1_SET_2
// #define FSR_RFP_ADC_PIN_SENSOR_1_SET_2 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0)  // standaard waarde
#endif

#ifndef FSR_RFP_ADC_PIN_SENSOR_2_SET_2
// #define FSR_RFP_ADC_PIN_SENSOR_2_SET_2 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN1)  // standaard waarde
#endif

#ifndef FSR_RFP_ADC_PIN_SENSOR_3_SET_2
// #define FSR_RFP_ADC_PIN_SENSOR_3_SET_2 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN2)  // standaard waarde
#endif

#ifndef FSR_RFP_ADC_PIN_SENSOR_4_SET_2
// #define FSR_RFP_ADC_PIN_SENSOR_4_SET_2 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN3)  // standaard waarde
#endif

#endif
