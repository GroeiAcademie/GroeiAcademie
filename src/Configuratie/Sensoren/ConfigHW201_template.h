#ifndef CONFIG_HW201_H
#define CONFIG_HW201_H

#ifndef SENSOR_HW201_AANTAL
// #define SENSOR_HW201_AANTAL 1  // standaard waarde: 0
#endif

#ifndef SENSOR_HW201_ADC_MEASUREMENT_RANGE_MILLIVOLT
// #define SENSOR_HW201_ADC_MEASUREMENT_RANGE_MILLIVOLT 2500 // standaard 2500 mV, fysiek bereik van OUT en AO verifiëren
#endif

#ifndef SENSOR_HW201_ANALOG_THRESHOLD_FOR_HIGH_MILLIVOLT
// #define SENSOR_HW201_ANALOG_THRESHOLD_FOR_HIGH_MILLIVOLT 1500 // standaard 1500 mV: OUT via een ADC-Extender is hoog vanaf deze spanning
#endif

// Eerste hardwaretest: OUT via native D2. 3,3V board: signaalniveaus controleren.
// Alternatieve GPIO-Extenderroute nog afzonderlijk te controleren.

#endif
