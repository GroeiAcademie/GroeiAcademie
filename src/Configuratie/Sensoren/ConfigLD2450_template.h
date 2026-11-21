#ifndef CONFIG_LD2450_H
#define CONFIG_LD2450_H

#ifndef SENSOR_LD2450_AANTAL
// #define SENSOR_LD2450_AANTAL 1  // standaard waarde: 0
#endif

// UART-poort wordt extern aangeleverd. Standaard baudrate volgens Hi-Link: 256000.

#ifndef SENSOR_LD2450_SILENT_TIMEOUT_MS
// #define SENSOR_LD2450_SILENT_TIMEOUT_MS 1000  // standaard waarde: 1000; zonder geldig frame gedurende deze tijd geeft IsModuleSilent() true
#endif

#endif
