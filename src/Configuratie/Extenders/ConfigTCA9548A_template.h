#ifndef CONFIG_TCA9548A_H
#define CONFIG_TCA9548A_H

// ============================================================================
// EXTENDER: TCA9548A
// ============================================================================
#ifndef EXTENDER_TCA9548A_AANTAL
// #define EXTENDER_TCA9548A_AANTAL 0  // standaard waarde: 0
#endif

// ============================================================================
// DEFAULT: TCA9548A #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_TCA9548A_1
// #define I2C_ADDRESS_EXTENDER_TCA9548A_1 I2C_ADDRESS_NONE  // standaard waarde: I2C_ADDRESS_NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_TCA9548A_1_RESET
// #define EXTENDER_TCA9548A_1_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

// ============================================================================
// EXPERIMENTEEL: TCA9548A #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_TCA9548A_2
// #define I2C_ADDRESS_EXTENDER_TCA9548A_2 I2C_ADDRESS_NONE  // standaard waarde: I2C_ADDRESS_NONE  // EXCLUSIEF

#endif
#ifndef EXTENDER_TCA9548A_2_RESET
// #define EXTENDER_TCA9548A_2_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#endif
