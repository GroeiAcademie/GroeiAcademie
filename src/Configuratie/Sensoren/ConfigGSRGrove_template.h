#ifndef CONFIG_GSRGROVE_H
#define CONFIG_GSRGROVE_H

#ifndef SENSOR_GSRGROVE_AANTAL
// #define SENSOR_GSRGROVE_AANTAL 1  // standaard waarde: 0
#endif

#ifndef SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT
// #define SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT 5000  // standaard waarde: 5000 (module op 5 V; op 3,3 V gevoed: 3300); pas aan aan sensorvoeding/schakeling (geen aangenomen VREF)
#endif

#ifndef SENSOR_GSRGROVE_CALIBRATION_VALUE
// #define SENSOR_GSRGROVE_CALIBRATION_VALUE 512  // standaard waarde: 512; Seeed-kalibratie: 10-bit waarde zonder huidcontact, na het instellen van de potentiometer
#endif

#endif
