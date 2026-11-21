#ifndef CONFIG_LD2410D_H
#define CONFIG_LD2410D_H

#ifndef SENSOR_LD2410D_AANTAL
// #define SENSOR_LD2410D_AANTAL 1  // standaard waarde: 0
#endif

// Volgens Hi-Link HLK-LD2410D protocol V1.01: UART standaard 115200 baud.
// Seriele poort wordt extern aangeleverd, geen onbewezen UART-Extender-koppeling.

#ifndef SENSOR_LD2410D_SILENT_TIMEOUT_MS
// #define SENSOR_LD2410D_SILENT_TIMEOUT_MS 1000  // standaard waarde: 1000; zonder geldig frame gedurende deze tijd geeft IsModuleSilent() true
#endif

#endif
