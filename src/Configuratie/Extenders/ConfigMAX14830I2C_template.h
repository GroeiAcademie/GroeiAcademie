#ifndef CONFIG_MAX14830I2C_H
#define CONFIG_MAX14830I2C_H

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

// EXTENDER: MAX14830 I2C
// ============================================================================
#ifndef EXTENDER_MAX14830_I2C_AANTAL
// #define EXTENDER_MAX14830_I2C_AANTAL 0  // standaard waarde: 0
#endif

// ============================================================================
// DEFAULT: MAX14830 I2C #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_MAX14830_I2C_1
// #define I2C_ADDRESS_EXTENDER_MAX14830_I2C_1 I2C_ADDRESS_NONE  // standaard waarde: I2C_ADDRESS_NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_MAX14830_I2C_1_IRQ
// #define EXTENDER_MAX14830_I2C_1_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

// ============================================================================
// EXPERIMENTEEL: MAX14830 I2C #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_MAX14830_I2C_2
// #define I2C_ADDRESS_EXTENDER_MAX14830_I2C_2 I2C_ADDRESS_NONE  // standaard waarde: I2C_ADDRESS_NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_MAX14830_I2C_2_IRQ
// #define EXTENDER_MAX14830_I2C_2_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#endif
