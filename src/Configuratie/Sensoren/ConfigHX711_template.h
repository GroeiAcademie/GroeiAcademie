#ifndef CONFIG_HX711_H
#define CONFIG_HX711_H

#ifndef SENSOR_HX711_AANTAL
// #define SENSOR_HX711_AANTAL 1  // standaard waarde: 0
#endif

// Native D2 = DOUT, native D3 = PD_SCK; timing via GPIO-Extender is niet bewezen.

#ifndef SENSOR_HX711_CALIBRATION_FACTOR
// #define SENSOR_HX711_CALIBRATION_FACTOR 0.0f  // standaard waarde: 0.0f (niet gekalibreerd, ReadWeightGram() geeft false); ruwe tellingen per gram; bepaal met een gekend gewicht: (telling met gewicht - tarra) / gram
#endif

#ifndef SENSOR_HX711_TARE_SAMPLE_COUNT
// #define SENSOR_HX711_TARE_SAMPLE_COUNT 10  // standaard waarde: 10; aantal metingen dat Tare() middelt
#endif

#endif
