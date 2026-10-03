#ifndef EXTENDERADS7828_H
#define EXTENDERADS7828_H

struct ExtenderADS7828 : HardwareResourceTypeI2C {
  enum class ExtenderPins : uint8_t {
    EP_CH0 = 0x00, // Analoge ingang kanaal 0
    EP_CH1 = 0x01, // Analoge ingang kanaal 1
    EP_CH2 = 0x02, // Analoge ingang kanaal 2
    EP_CH3 = 0x03, // Analoge ingang kanaal 3
    EP_CH4 = 0x04, // Analoge ingang kanaal 4
    EP_CH5 = 0x05, // Analoge ingang kanaal 5
    EP_CH6 = 0x06, // Analoge ingang kanaal 6
    EP_CH7 = 0x07  // Analoge ingang kanaal 7
  };

  // ============================================================================
  // DEFAULT — ADS7828 #1
  // ============================================================================
  #if EXTENDER_ADS7828_AANTAL == 1
  using HardwareResourceTypeI2C::HardwareResourceTypeI2C;
  #endif

  // ============================================================================
  // EXPERIMENTEEL — ADS7828 — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_ADS7828_AANTAL >= 2
  struct Ads7828Pinnen { uint8_t adres; };

  #define BUNDEL_EXTENDER(N) { I2C_ADDRESS_EXTENDER_ADS7828_##N }

  static constexpr Ads7828Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderADS7828(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin SDA, HardwareResourcePin SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, SDA, SCL, Extender) {}
  #endif
};

#endif
