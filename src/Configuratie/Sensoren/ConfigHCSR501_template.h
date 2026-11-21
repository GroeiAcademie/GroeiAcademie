#ifndef CONFIG_HCSR501_H
#define CONFIG_HCSR501_H

#ifndef SENSOR_HCSR501_AANTAL
// #define SENSOR_HCSR501_AANTAL 1  // standaard waarde: 0
#endif

#ifndef SENSOR_HCSR501_ADC_MEASUREMENT_RANGE_MILLIVOLT
// #define SENSOR_HCSR501_ADC_MEASUREMENT_RANGE_MILLIVOLT 2500 // standaard 2500 mV, fysiek bereik van OUT verifiëren
#endif

#ifndef SENSOR_HCSR501_ANALOG_THRESHOLD_FOR_HIGH_MILLIVOLT
// #define SENSOR_HCSR501_ANALOG_THRESHOLD_FOR_HIGH_MILLIVOLT 1500 // standaard 1500 mV: OUT via een ADC-Extender is hoog vanaf deze spanning
#endif

// Native pin D2 in deze eerste hardwaretest; later configureerbaar na goedkeuring.

#endif
