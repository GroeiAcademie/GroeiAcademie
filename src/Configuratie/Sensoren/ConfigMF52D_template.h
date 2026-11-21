#ifndef CONFIG_MF52D_H
#define CONFIG_MF52D_H

#ifndef SENSOR_MF52D_AANTAL
// #define SENSOR_MF52D_AANTAL 1  // standaard waarde: 0
#endif

#ifndef SENSOR_MF52D_ADC_BEREIK_MILLIVOLT
// #define SENSOR_MF52D_ADC_BEREIK_MILLIVOLT 5000  // standaard waarde: 5000 (module op 5 V; op 3,3 V gevoed: 3300); pas aan aan sensorvoeding/schakeling (geen aangenomen VREF)
#endif

#ifndef SENSOR_MF52X_NTC_BETA
// #define SENSOR_MF52X_NTC_BETA 3950  // standaard waarde: 3950; B-waarde van de NTC (MF52A en MF52D), volgens het datablad van de module
#endif

#ifndef SENSOR_MF52X_NTC_RESISTANCE_AT_25C_OHM
// #define SENSOR_MF52X_NTC_RESISTANCE_AT_25C_OHM 10000  // standaard waarde: 10000; weerstand van de NTC bij 25 °C (MF52A en MF52D)
#endif

#ifndef SENSOR_MF52X_SERIES_RESISTOR_OHM
// #define SENSOR_MF52X_SERIES_RESISTOR_OHM 10000  // standaard waarde: 10000; serieweerstand van de deler op de module (MF52A en MF52D)
#endif

#endif
