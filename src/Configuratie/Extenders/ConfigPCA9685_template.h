#ifndef CONFIG_PCA9685_H
#define CONFIG_PCA9685_H

// ============================================================================
// EXTENDER: PCA9685
// ============================================================================
#ifndef EXTENDER_PCA9685_AANTAL
// #define EXTENDER_PCA9685_AANTAL 0  // standaard waarde: 0
#endif

// ============================================================================
// DEFAULT: PCA9685 #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_PCA9685_1
// #define I2C_ADDRESS_EXTENDER_PCA9685_1 I2C_ADDRESS_NONE  // standaard waarde: I2C_ADDRESS_NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_PCA9685_1_PWM_FREQUENTIE
// #define EXTENDER_PCA9685_1_PWM_FREQUENTIE 50  // standaard: niet ingesteld (frequentie van de chip); 50 Hz voor servo's, hoger voor LED's
#endif
#ifndef EXTENDER_PCA9685_1_OE_PIN
// #define EXTENDER_PCA9685_1_OE_PIN 7  // standaard: niet ingesteld; pin van het board naar OE van de PCA9685 (noodstop in hardware). LET OP: wordt niet gereserveerd, kies een vrije pin
#endif

// ============================================================================
// EXPERIMENTEEL: PCA9685 #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_PCA9685_2
// #define I2C_ADDRESS_EXTENDER_PCA9685_2 I2C_ADDRESS_NONE  // standaard waarde: I2C_ADDRESS_NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_PCA9685_2_PWM_FREQUENTIE
// #define EXTENDER_PCA9685_2_PWM_FREQUENTIE 50  // standaard: niet ingesteld (frequentie van de chip); 50 Hz voor servo's, hoger voor LED's
#endif
#ifndef EXTENDER_PCA9685_2_OE_PIN
// #define EXTENDER_PCA9685_2_OE_PIN 7  // standaard: niet ingesteld; pin van het board naar OE van de PCA9685 (noodstop in hardware). LET OP: wordt niet gereserveerd, kies een vrije pin
#endif
#endif
