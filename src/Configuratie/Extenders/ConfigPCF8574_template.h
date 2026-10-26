#ifndef CONFIG_PCF8574_H
#define CONFIG_PCF8574_H

// ============================================================================
// EXTENDER: PCF8574
// ============================================================================
#ifndef EXTENDER_PCF8574_AANTAL
// #define EXTENDER_PCF8574_AANTAL 0  // standaard waarde: 0
#endif

// ============================================================================
// DEFAULT: PCF8574 #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_PCF8574_1
// #define I2C_ADDRESS_EXTENDER_PCF8574_1 I2C_ADDRESS_NONE  // standaard waarde: I2C_ADDRESS_NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_PCF8574_1_INT
// #define EXTENDER_PCF8574_1_INT HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

// ============================================================================
// EXPERIMENTEEL: PCF8574 #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_PCF8574_2
// #define I2C_ADDRESS_EXTENDER_PCF8574_2 I2C_ADDRESS_NONE  // standaard waarde: I2C_ADDRESS_NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_PCF8574_2_INT
// #define EXTENDER_PCF8574_2_INT HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#endif
