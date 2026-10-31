#ifndef CONFIG_MAX14830SPI_H
#define CONFIG_MAX14830SPI_H

// ============================================================================
// Werkelijke externe/kristalbronfrequentie, zonder veronderstelde standaard.
#ifndef EXTENDER_MAX14830_BRONKLOK_HZ
// #define EXTENDER_MAX14830_BRONKLOK_HZ 0  // standaard waarde: 0 (onbekend)
#endif
#ifndef EXTENDER_MAX14830_BRONKLOK_KRISTAL
// #define EXTENDER_MAX14830_BRONKLOK_KRISTAL 1  // standaard waarde: 1 (kristal); 0 = externe klok aan XIN
#endif
#ifndef EXTENDER_MAX14830_PLL_VOORDELER
// #define EXTENDER_MAX14830_PLL_VOORDELER 0  // standaard waarde: 0 (PLL uit); anders 1 t/m 63
#endif
#ifndef EXTENDER_MAX14830_PLL_VERMENIGVULDIGER
// #define EXTENDER_MAX14830_PLL_VERMENIGVULDIGER 0  // standaard waarde: 0 (PLL uit); anders 6, 48, 96 of 144
#endif

// EXTENDER: MAX14830 SPI
// ============================================================================
#ifndef EXTENDER_MAX14830_SPI_AANTAL
// #define EXTENDER_MAX14830_SPI_AANTAL 0  // standaard waarde: 0
#endif

// ============================================================================
// DEFAULT: MAX14830 SPI #1
// ============================================================================
#ifndef EXTENDER_MAX14830_SPI_1_CS_PIN
// #define EXTENDER_MAX14830_SPI_1_CS_PIN HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_MAX14830_SPI_1_IRQ
// #define EXTENDER_MAX14830_SPI_1_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

// ============================================================================
// EXPERIMENTEEL: MAX14830 SPI #2
// ============================================================================
#ifndef EXTENDER_MAX14830_SPI_2_CS_PIN
// #define EXTENDER_MAX14830_SPI_2_CS_PIN HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_MAX14830_SPI_2_IRQ
// #define EXTENDER_MAX14830_SPI_2_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#endif
