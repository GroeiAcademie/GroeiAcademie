#ifndef EXTENDERTCA9548A_H
#define EXTENDERTCA9548A_H

struct ExtenderTCA9548A : HardwareResourceTypeI2C {
  enum class ExtenderPins : uint8_t {
    EP_CH0 = 0x00, // I2C-kanaal 0: SD0 + SC0
    EP_CH1 = 0x01, // I2C-kanaal 1: SD1 + SC1
    EP_CH2 = 0x02, // I2C-kanaal 2: SD2 + SC2
    EP_CH3 = 0x03, // I2C-kanaal 3: SD3 + SC3
    EP_CH4 = 0x04, // I2C-kanaal 4: SD4 + SC4
    EP_CH5 = 0x05, // I2C-kanaal 5: SD5 + SC5
    EP_CH6 = 0x06, // I2C-kanaal 6: SD6 + SC6
    EP_CH7 = 0x07  // I2C-kanaal 7: SD7 + SC7
  };

  HardwareResourcePin RESET;
  uint8_t exclusiefMetReset_[2];

  // ============================================================================
  // DEFAULT — TCA9548A #1
  // ============================================================================
  #if EXTENDER_TCA9548A_AANTAL == 1
  ExtenderTCA9548A(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin SDA, HardwareResourcePin SCL, HardwareResourcePin RESET, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, adres, SDA, SCL, Extender), RESET(RESET) {
    exclusiefMetReset_[0] = exclusief_[0];
    exclusiefMetReset_[1] = static_cast<uint8_t>(RESET);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetReset_, 2 };
  }
  #endif

  // ============================================================================
  // EXPERIMENTEEL — TCA9548A — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_TCA9548A_AANTAL >= 2
  struct Tca9548aPinnen { uint8_t adres; HardwareResourcePin reset; };

  #define BUNDEL_EXTENDER(N) { I2C_ADDRESS_EXTENDER_TCA9548A_##N, EXTENDER_TCA9548A_##N##_RESET }

  static constexpr Tca9548aPinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderTCA9548A(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin SDA, HardwareResourcePin SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, SDA, SCL, Extender), RESET(ExtenderLijst[teller].reset) {
    exclusiefMetReset_[0] = exclusief_[0];
    exclusiefMetReset_[1] = static_cast<uint8_t>(RESET);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetReset_, 2 };
  }
  #endif
};

#endif
