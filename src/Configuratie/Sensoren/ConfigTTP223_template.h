#ifndef CONFIG_TTP223_H
#define CONFIG_TTP223_H

// TTP223 OUT is een enkel digitaal signaal; native pin vereist.
#ifndef SENSOR_TTP223_AANTAL
// #define SENSOR_TTP223_AANTAL 1 // standaard waarde: 0
#endif

#ifndef SENSOR_TTP223_DIRECTE_MODUS
// #define SENSOR_TTP223_DIRECTE_MODUS 1 // standaard: 1 (momentary); toggle: 0 (duurtijden niet betrouwbaar)
#endif

#ifndef SENSOR_TTP223_LANGE_AANRAKING_MS
// #define SENSOR_TTP223_LANGE_AANRAKING_MS 500 // standaard 500 ms
#endif

#ifndef SENSOR_TTP223_DUBBELE_AANRAKING_MS
// #define SENSOR_TTP223_DUBBELE_AANRAKING_MS 350 // standaard 350 ms
#endif

#endif
