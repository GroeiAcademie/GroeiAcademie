#ifndef EXTENDERDS2482V800_H
#define EXTENDERDS2482V800_H

struct ExtenderDS2482v800 : HardwareResourceTypeI2C, Adafruit_DS248x {
  enum class ExtenderPins : uint8_t {
    EP_IO0 = 0x00, // 1-Wire I/O-kanaal IO0
    EP_IO1 = 0x01, // 1-Wire I/O-kanaal IO1
    EP_IO2 = 0x02, // 1-Wire I/O-kanaal IO2
    EP_IO3 = 0x03, // 1-Wire I/O-kanaal IO3
    EP_IO4 = 0x04, // 1-Wire I/O-kanaal IO4
    EP_IO5 = 0x05, // 1-Wire I/O-kanaal IO5
    EP_IO6 = 0x06, // 1-Wire I/O-kanaal IO6
    EP_IO7 = 0x07  // 1-Wire I/O-kanaal IO7
  };

  // ============================================================================
  // DEFAULT — DS2482-800 #1
  // ============================================================================
  #if EXTENDER_DS2482_800_AANTAL == 1
  using HardwareResourceTypeI2C::HardwareResourceTypeI2C;
  #endif

  // ============================================================================
  // EXPERIMENTEEL — DS2482-800 — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_DS2482_800_AANTAL >= 2
  struct Ds2482_800Pinnen { uint8_t adres; };

  #define BUNDEL_EXTENDER(N) { I2C_ADDRESS_EXTENDER_DS2482_800_##N }

  static constexpr Ds2482_800Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderDS2482v800(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, np_SDA, np_SCL, Extender) {}
  #endif

  bool Activeren() override {
    return Adafruit_DS248x::begin(&Wire, adres);
  }

};

#endif
