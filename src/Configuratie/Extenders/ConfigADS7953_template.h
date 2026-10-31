#ifndef CONFIG_ADS7953_H
#define CONFIG_ADS7953_H

// ============================================================================
// EXTERNE referentiespanning op REF, in mV.
#ifndef EXTENDER_ADS7953_REFERENTIE_MILLIVOLT
// #define EXTENDER_ADS7953_REFERENTIE_MILLIVOLT 2500  // standaard waarde: 2500
#endif

// +VA: werkelijke analoge voeding, bijvoorbeeld 3300 of 5000 mV.
// TI: bereik 2xVREF is alleen geldig wanneer +VA >= 2xVREF.
// Zonder ingestelde voeding (0) wordt 2xVREF veilig geweigerd.
#ifndef EXTENDER_ADS7953_VOEDING_MILLIVOLT
// #define EXTENDER_ADS7953_VOEDING_MILLIVOLT 0  // standaard waarde: 0 (onbekend)
#endif

// EXTENDER: ADS7953
// ============================================================================
#ifndef EXTENDER_ADS7953_AANTAL
// #define EXTENDER_ADS7953_AANTAL 0  // standaard waarde: 0
#endif

// ============================================================================
// DEFAULT: ADS7953 #1
// ============================================================================
#ifndef EXTENDER_ADS7953_1_CS_PIN
// #define EXTENDER_ADS7953_1_CS_PIN HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_ADS7953_1_GPIO0_TO_UNO
// #define EXTENDER_ADS7953_1_GPIO0_TO_UNO HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_ADS7953_1_GPIO1_TO_UNO
// #define EXTENDER_ADS7953_1_GPIO1_TO_UNO HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_ADS7953_1_GPIO2_TO_UNO
// #define EXTENDER_ADS7953_1_GPIO2_TO_UNO HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_ADS7953_1_GPIO3_TO_UNO
// #define EXTENDER_ADS7953_1_GPIO3_TO_UNO HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

// ============================================================================
// EXPERIMENTEEL: ADS7953 #2
// ============================================================================
#ifndef EXTENDER_ADS7953_2_CS_PIN
// #define EXTENDER_ADS7953_2_CS_PIN HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_ADS7953_2_GPIO0_TO_UNO
// #define EXTENDER_ADS7953_2_GPIO0_TO_UNO HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_ADS7953_2_GPIO1_TO_UNO
// #define EXTENDER_ADS7953_2_GPIO1_TO_UNO HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_ADS7953_2_GPIO2_TO_UNO
// #define EXTENDER_ADS7953_2_GPIO2_TO_UNO HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_ADS7953_2_GPIO3_TO_UNO
// #define EXTENDER_ADS7953_2_GPIO3_TO_UNO HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#endif
