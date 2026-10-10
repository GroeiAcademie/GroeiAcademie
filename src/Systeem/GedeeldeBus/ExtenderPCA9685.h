#ifndef EXTENDERPCA9685_H
#define EXTENDERPCA9685_H

struct ExtenderPCA9685 : HardwareResourceTypeI2C, PCA9685 {
  enum class ExtenderPins : uint8_t {
    EP_PWM0 = 0x00,
    EP_PWM1 = 0x01,
    EP_PWM2 = 0x02,
    EP_PWM3 = 0x03,
    EP_PWM4 = 0x04,
    EP_PWM5 = 0x05,
    EP_PWM6 = 0x06,
    EP_PWM7 = 0x07,
    EP_PWM8 = 0x08,
    EP_PWM9 = 0x09,
    EP_PWM10 = 0x0A,
    EP_PWM11 = 0x0B,
    EP_PWM12 = 0x0C,
    EP_PWM13 = 0x0D,
    EP_PWM14 = 0x0E,
    EP_PWM15 = 0x0F
  };

  // ============================================================================
  // DEFAULT: PCA9685 #1
  // ============================================================================
  #if EXTENDER_PCA9685_AANTAL == 1
  ExtenderPCA9685(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, adres, np_SDA, np_SCL, Extender), PCA9685(adres, &Wire) {}
  #endif

  // ============================================================================
  // EXPERIMENTEEL: PCA9685 #2 en hoger
  // ============================================================================
  #if EXTENDER_PCA9685_AANTAL >= 2
  struct Pca9685Pinnen { uint8_t adres; };

  #define BUNDEL_EXTENDER(N) { I2C_ADDRESS_EXTENDER_PCA9685_##N }

  static constexpr Pca9685Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderPCA9685(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, np_SDA, np_SCL, Extender), PCA9685(ExtenderLijst[teller].adres, &Wire) {}
  #endif

  bool Activeren() override {
    return PCA9685::begin() && PCA9685::isConnected();
  }
};

#endif
