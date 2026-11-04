#ifndef CONFIG_BIOAMPEXGPILL_H
#define CONFIG_BIOAMPEXGPILL_H

// standaard uit. Eén exemplaar via bestaande ADC-Extender.
#ifndef SENSOR_BIOAMPEXGPILL_AANTAL
// #define SENSOR_BIOAMPEXGPILL_AANTAL 1  // standaard waarde: 0

// ADS7828 met interne 2500 mV-referentie: signalen daarboven worden afgekapt.
// Bij hogere analoge uitgangsspanning: geschikte spanningsdeler of externe
// referentie gebruiken en de effectieve spanning in de Extender configureren.
// BioAmp EXG Pill: RAW (FixedSampling), ECG, EMG, EOG of EEG.
// Upside Down Labs: RAW/ECG 125 Hz, EMG 500 Hz, EOG 75 Hz, EEG 256 Hz.
#ifndef SENSOR_BIOAMPEXGPILL_MEETMETHODE
// #define SENSOR_BIOAMPEXGPILL_MEETMETHODE BIOAMPEXGPILL_MEETMETHODE_RAW  // standaard
#endif

#ifndef SENSOR_BIOAMPEXGPILL_ENVELOP_VENSTER
// #define SENSOR_BIOAMPEXGPILL_ENVELOP_VENSTER 128  // standaard waarde: 128; aantal samples voor de EMG-envelop (alleen bij EMG; 4 bytes RAM per sample)
#endif

#ifndef SENSOR_BIOAMPEXGPILL_ADC_BEREIK_MILLIVOLT
// #define SENSOR_BIOAMPEXGPILL_ADC_BEREIK_MILLIVOLT 5000  // standaard waarde: 5000 (module op 5 V; op 3,3 V gevoed: 3300); pas aan aan de werkelijk aangesloten sensorspanning
#endif

#endif
