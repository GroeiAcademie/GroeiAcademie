#ifndef CONFIG_SC16IS752SPI_H
#define CONFIG_SC16IS752SPI_H

// ============================================================================
// Werkelijke kristal- of klokfrequentie aan XTAL1 (geldt voor I2C en SPI).
#ifndef EXTENDER_SC16IS752_BRONKLOK_HZ
// #define EXTENDER_SC16IS752_BRONKLOK_HZ 14745600UL  // standaard waarde: 14745600 (14,7456 MHz); bv. 1843200 of 3072000 bij een ander kristal
#endif

// ============================================================================
// EXTENDER: SC16IS752 SPI
// ============================================================================
#ifndef EXTENDER_SC16IS752_SPI_AANTAL
// #define EXTENDER_SC16IS752_SPI_AANTAL 0  // standaard waarde: 0
#endif

// ============================================================================
// DEFAULT: SC16IS752 SPI #1
// ============================================================================
#ifndef EXTENDER_SC16IS752_SPI_1_CS_PIN
// #define EXTENDER_SC16IS752_SPI_1_CS_PIN HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_SC16IS752_SPI_1_IRQ
// #define EXTENDER_SC16IS752_SPI_1_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_SC16IS752_SPI_1_RESET
// #define EXTENDER_SC16IS752_SPI_1_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

// ============================================================================
// EXPERIMENTEEL: SC16IS752 SPI #2
// ============================================================================
#ifndef EXTENDER_SC16IS752_SPI_2_CS_PIN
// #define EXTENDER_SC16IS752_SPI_2_CS_PIN HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_SC16IS752_SPI_2_IRQ
// #define EXTENDER_SC16IS752_SPI_2_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_SC16IS752_SPI_2_RESET
// #define EXTENDER_SC16IS752_SPI_2_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#endif
