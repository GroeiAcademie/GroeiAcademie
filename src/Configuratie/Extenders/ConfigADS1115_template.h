#ifndef CONFIG_ADS1115_H
#define CONFIG_ADS1115_H

// ============================================================================
// EXTENDER: ADS1115
// ============================================================================
#ifndef EXTENDER_ADS1115_AANTAL
// #define EXTENDER_ADS1115_AANTAL 1  // standaard waarde: 1
#endif

// ============================================================================
// DEFAULT: ADS1115 #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_ADS1115_1
// #define I2C_ADDRESS_EXTENDER_ADS1115_1 0x48  // standaard waarde: 0x48  // EXCLUSIEF
#endif

#ifndef EXTENDER_ADS1115_1_ALERT_RDY
// #define EXTENDER_ADS1115_1_ALERT_RDY HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

// ============================================================================
// EXPERIMENTEEL: ADS1115 #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_ADS1115_2
// #define I2C_ADDRESS_EXTENDER_ADS1115_2 I2C_ADDRESS_NONE  // standaard waarde: I2C_ADDRESS_NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_ADS1115_2_ALERT_RDY
// #define EXTENDER_ADS1115_2_ALERT_RDY HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#endif
