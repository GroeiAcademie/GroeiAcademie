#ifndef CONFIG_MAX4466_H
#define CONFIG_MAX4466_H

// standaard uit. Eén exemplaar via bestaande ADC-Extender.
#ifndef SENSOR_MAX4466_AANTAL
// #define SENSOR_MAX4466_AANTAL 1  // standaard waarde: 0

// ADS7828 met interne 2500 mV-referentie: signalen daarboven worden afgekapt.
// Bij hogere analoge uitgangsspanning: geschikte spanningsdeler of externe
// referentie gebruiken en de effectieve spanning in de Extender configureren.
#ifndef SENSOR_MAX4466_ADC_BEREIK_MILLIVOLT
// #define SENSOR_MAX4466_ADC_BEREIK_MILLIVOLT 5000  // standaard waarde: 5000 (module op 5 V; op 3,3 V gevoed: 3300); pas aan aan de werkelijk aangesloten sensorspanning
#endif

#endif
