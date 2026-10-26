#ifndef CONFIG_PCF8575_H
#define CONFIG_PCF8575_H

// ============================================================================
// EXTENDER: PCF8575
// ============================================================================
#ifndef EXTENDER_PCF8575_AANTAL
// #define EXTENDER_PCF8575_AANTAL 0  // standaard waarde: 0
#endif

// ============================================================================
// DEFAULT: PCF8575 #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_PCF8575_1
// #define I2C_ADDRESS_EXTENDER_PCF8575_1 I2C_ADDRESS_NONE  // standaard waarde: I2C_ADDRESS_NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_PCF8575_1_INT
// #define EXTENDER_PCF8575_1_INT HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

// ============================================================================
// EXPERIMENTEEL: PCF8575 #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_PCF8575_2
// #define I2C_ADDRESS_EXTENDER_PCF8575_2 I2C_ADDRESS_NONE  // standaard waarde: I2C_ADDRESS_NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_PCF8575_2_INT
// #define EXTENDER_PCF8575_2_INT HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#endif
