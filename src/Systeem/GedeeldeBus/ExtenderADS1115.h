#ifndef EXTENDERADS1115_H
#define EXTENDERADS1115_H

struct ExtenderADS1115 : HardwareResourceTypeI2C, Adafruit_ADS1115 {
  enum class ExtenderPins : uint8_t {
    EP_AIN0 = 0x00, // Analoge ingang AIN0
    EP_AIN1 = 0x01, // Analoge ingang AIN1
    EP_AIN2 = 0x02, // Analoge ingang AIN2
    EP_AIN3 = 0x03  // Analoge ingang AIN3
  };

  HardwareResourcePin np_ALERT_RDY;
  uint8_t exclusiefMetAlert_[2];

  // ============================================================================
  // ADS1115-BASIS — gedeeld door ADC_ADS1115 en EXTENDER_ADS1115
  // ============================================================================
  ExtenderADS1115(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourcePin np_ALERT_RDY, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, adres, np_SDA, np_SCL, Extender), np_ALERT_RDY(np_ALERT_RDY) {
    exclusiefMetAlert_[0] = exclusief_[0];
    exclusiefMetAlert_[1] = static_cast<uint8_t>(np_ALERT_RDY);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetAlert_, 2 };
  }

  bool begin() {
    InitialiserenGedeeldeBus(GedeeldeBusType::I2C);
    return Adafruit_ADS1115::begin(adres);
  }

  bool Activeren() override {
    return begin();
  }

  // ============================================================================
  // EXPERIMENTEEL — ADS1115 — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_ADS1115_AANTAL >= 2
  struct Ads1115Pinnen { uint8_t adres; HardwareResourcePin alertRdy; };

  #define BUNDEL_EXTENDER(N) { I2C_ADDRESS_EXTENDER_ADS1115_##N, EXTENDER_ADS1115_##N##_ALERT_RDY }

  static constexpr Ads1115Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderADS1115(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, np_SDA, np_SCL, Extender), np_ALERT_RDY(ExtenderLijst[teller].alertRdy) {
    exclusiefMetAlert_[0] = exclusief_[0];
    exclusiefMetAlert_[1] = static_cast<uint8_t>(np_ALERT_RDY);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetAlert_, 2 };
  }
  #endif
};

#endif
