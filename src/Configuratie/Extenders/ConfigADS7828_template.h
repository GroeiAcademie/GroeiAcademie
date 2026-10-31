#ifndef CONFIG_ADS7828_H
#define CONFIG_ADS7828_H

// ============================================================================
// EXTENDER: ADS7828
// ============================================================================
#ifndef EXTENDER_ADS7828_AANTAL
// #define EXTENDER_ADS7828_AANTAL 0  // standaard waarde: 0
#endif

// ============================================================================
// DEFAULT: ADS7828 #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_ADS7828_1
// #define I2C_ADDRESS_EXTENDER_ADS7828_1 I2C_ADDRESS_NONE  // standaard waarde: I2C_ADDRESS_NONE  // EXCLUSIEF
#endif

// ============================================================================
// EXPERIMENTEEL: ADS7828 #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_ADS7828_2
// #define I2C_ADDRESS_EXTENDER_ADS7828_2 I2C_ADDRESS_NONE  // standaard waarde: I2C_ADDRESS_NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS7828_EXTERNE_REFERENTIE
// #define EXTENDER_ADS7828_EXTERNE_REFERENTIE 0  // 0 interne 2,5 V; 1 externe referentie op REF-pin
#endif
#ifndef EXTENDER_ADS7828_REFERENTIE_MILLIVOLT
// #define EXTENDER_ADS7828_REFERENTIE_MILLIVOLT 2500  // externe VREF in mV; controleer de hardware en datasheet
#endif

#endif
