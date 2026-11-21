#ifndef CONFIG_SC16IS752I2C_H
#define CONFIG_SC16IS752I2C_H

// ============================================================================
// Werkelijke kristal- of klokfrequentie aan XTAL1 (geldt voor I2C en SPI).
#ifndef EXTENDER_SC16IS752_BRONKLOK_HZ
// #define EXTENDER_SC16IS752_BRONKLOK_HZ 14745600UL  // standaard waarde: 14745600 (14,7456 MHz); bv. 1843200 of 3072000 bij een ander kristal
#endif

// ============================================================================
// EXTENDER: SC16IS752 I2C
// ============================================================================
#ifndef EXTENDER_SC16IS752_I2C_AANTAL
// #define EXTENDER_SC16IS752_I2C_AANTAL 0  // standaard waarde: 0
#endif

// ============================================================================
// DEFAULT: SC16IS752 I2C #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_SC16IS752_I2C_1
// #define I2C_ADDRESS_EXTENDER_SC16IS752_I2C_1 I2C_ADDRESS_NONE  // standaard waarde: I2C_ADDRESS_NONE  // EXCLUSIEF

#endif
#ifndef EXTENDER_SC16IS752_I2C_1_IRQ
// #define EXTENDER_SC16IS752_I2C_1_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF

#endif
#ifndef EXTENDER_SC16IS752_I2C_1_RESET
// #define EXTENDER_SC16IS752_I2C_1_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

// ============================================================================
// EXPERIMENTEEL: SC16IS752 I2C #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_SC16IS752_I2C_2
// #define I2C_ADDRESS_EXTENDER_SC16IS752_I2C_2 I2C_ADDRESS_NONE  // standaard waarde: I2C_ADDRESS_NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_SC16IS752_I2C_2_IRQ
// #define EXTENDER_SC16IS752_I2C_2_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_SC16IS752_I2C_2_RESET
// #define EXTENDER_SC16IS752_I2C_2_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#endif
