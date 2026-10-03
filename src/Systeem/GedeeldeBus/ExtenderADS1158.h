#ifndef EXTENDERADS1158_H
#define EXTENDERADS1158_H

struct ExtenderADS1158 : HardwareResourceTypeSPI {
  enum class ExtenderPins : uint8_t {
    EP_AIN0  = 0x00, // Analoge ingang AIN0
    EP_AIN1  = 0x01, // Analoge ingang AIN1
    EP_AIN2  = 0x02, // Analoge ingang AIN2
    EP_AIN3  = 0x03, // Analoge ingang AIN3
    EP_AIN4  = 0x04, // Analoge ingang AIN4
    EP_AIN5  = 0x05, // Analoge ingang AIN5
    EP_AIN6  = 0x06, // Analoge ingang AIN6
    EP_AIN7  = 0x07, // Analoge ingang AIN7
    EP_AIN8  = 0x08, // Analoge ingang AIN8
    EP_AIN9  = 0x09, // Analoge ingang AIN9
    EP_AIN10 = 0x0A, // Analoge ingang AIN10
    EP_AIN11 = 0x0B, // Analoge ingang AIN11
    EP_AIN12 = 0x0C, // Analoge ingang AIN12
    EP_AIN13 = 0x0D, // Analoge ingang AIN13
    EP_AIN14 = 0x0E, // Analoge ingang AIN14
    EP_AIN15 = 0x0F, // Analoge ingang AIN15

    EP_GPIO0 = 0x10, // General-purpose digitale I/O GPIO0
    EP_GPIO1 = 0x11, // General-purpose digitale I/O GPIO1
    EP_GPIO2 = 0x12, // General-purpose digitale I/O GPIO2
    EP_GPIO3 = 0x13, // General-purpose digitale I/O GPIO3
    EP_GPIO4 = 0x14, // General-purpose digitale I/O GPIO4
    EP_GPIO5 = 0x15, // General-purpose digitale I/O GPIO5
    EP_GPIO6 = 0x16, // General-purpose digitale I/O GPIO6
    EP_GPIO7 = 0x17  // General-purpose digitale I/O GPIO7
  };

  HardwareResourcePin START, RESET, PWDN;
  uint8_t exclusiefMetControle_[4];

  // ============================================================================
  // DEFAULT — ADS1158 #1
  // ============================================================================
  #if EXTENDER_ADS1158_AANTAL == 1
  ExtenderADS1158(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin CS, HardwareResourcePin SCK, HardwareResourcePin MISO, HardwareResourcePin MOSI, HardwareResourcePin START, HardwareResourcePin RESET, HardwareResourcePin PWDN, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, component, CS, SCK, MISO, MOSI, Extender), START(START), RESET(RESET), PWDN(PWDN) {
    exclusiefMetControle_[0] = static_cast<uint8_t>(CS);
    exclusiefMetControle_[1] = static_cast<uint8_t>(START);
    exclusiefMetControle_[2] = static_cast<uint8_t>(RESET);
    exclusiefMetControle_[3] = static_cast<uint8_t>(PWDN);
    aangemeldePinnen = { gedeeld_, 3, exclusiefMetControle_, 4 };
  }
  #endif

  // ============================================================================
  // EXPERIMENTEEL — ADS1158 — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_ADS1158_AANTAL >= 2
  struct Ads1158Pinnen { HardwareResourcePin csPin; HardwareResourcePin pwdn; HardwareResourcePin reset; HardwareResourcePin start; };

  #define BUNDEL_EXTENDER(N) { CS_PIN_EXTENDER_ADS1158_##N, EXTENDER_ADS1158_##N##_PWDN, EXTENDER_ADS1158_##N##_RESET, EXTENDER_ADS1158_##N##_START }

  static constexpr Ads1158Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderADS1158(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin SCK, HardwareResourcePin MISO, HardwareResourcePin MOSI, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, component, ExtenderLijst[teller].csPin, SCK, MISO, MOSI, Extender), START(ExtenderLijst[teller].start), RESET(ExtenderLijst[teller].reset), PWDN(ExtenderLijst[teller].pwdn) {
    exclusiefMetControle_[0] = static_cast<uint8_t>(ExtenderLijst[teller].csPin);
    exclusiefMetControle_[1] = static_cast<uint8_t>(START);
    exclusiefMetControle_[2] = static_cast<uint8_t>(RESET);
    exclusiefMetControle_[3] = static_cast<uint8_t>(PWDN);
    aangemeldePinnen = { gedeeld_, 3, exclusiefMetControle_, 4 };
  }
  #endif
};

#endif
