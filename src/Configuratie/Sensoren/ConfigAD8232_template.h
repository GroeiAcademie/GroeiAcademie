#ifndef CONFIG_AD8232_H
#define CONFIG_AD8232_H

// standaard uit. Eén exemplaar via bestaande ADC-Extender.
#ifndef SENSOR_AD8232_AANTAL
// #define SENSOR_AD8232_AANTAL 1  // standaard waarde: 0
#endif

#ifndef SENSOR_AD8232_ADC_BEREIK_MILLIVOLT
// #define SENSOR_AD8232_ADC_BEREIK_MILLIVOLT 3300  // standaard waarde: 3300; pas aan aan de werkelijk aangesloten sensorspanning
#endif

#ifndef SENSOR_AD8232_SAMPLE_RATE_HZ
// #define SENSOR_AD8232_SAMPLE_RATE_HZ 125  // standaard waarde: 125 (haalbaar met een ADS1115 op 128 SPS); samplefrequentie voor de hartslag (vast ritme); de ADC-Extender moet dit halen
#endif

#endif
