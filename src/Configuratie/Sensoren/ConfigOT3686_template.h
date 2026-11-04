#ifndef CONFIG_OT3686_H
#define CONFIG_OT3686_H

// OT3686: OUT/DO naar native interrupt-pin; AO niet gebruiken (volgens Otronic niet functioneel).
#ifndef SENSOR_OT3686_AANTAL
// #define SENSOR_OT3686_AANTAL 1  // standaard waarde: 0
#endif

#ifndef SENSOR_OT3686_AANTAL_SLEUVEN
// #define SENSOR_OT3686_AANTAL_SLEUVEN 20 // standaard: 20; aantal pulsen per omwenteling van jouw wiel
#endif

#ifndef SENSOR_OT3686_MEETINTERVAL_MS
// #define SENSOR_OT3686_MEETINTERVAL_MS 100 // standaard 100 ms
#endif

#ifndef SENSOR_OT3686_STILSTAND_TIMEOUT_MS
// #define SENSOR_OT3686_STILSTAND_TIMEOUT_MS 1000 // standaard 1000 ms
#endif

#ifndef SENSOR_OT3686_MINIMALE_PULSPERIODE_US
// #define SENSOR_OT3686_MINIMALE_PULSPERIODE_US 100 // standaard 100 us, ontstoort hele korte flanken
#endif

#endif
