#ifndef EXTENDERMAX14830I2C_H
#define EXTENDERMAX14830I2C_H

struct ExtenderMAX14830I2C : HardwareResourceTypeI2C {
  enum class ExtenderPins : uint8_t {
    EP_UART0  = 0x00, // UART0: RX0/TX0 en bijhorende flow-controlfuncties
    EP_UART1  = 0x01, // UART1: RX1/TX1 en bijhorende flow-controlfuncties
    EP_UART2  = 0x02, // UART2: RX2/TX2 en bijhorende flow-controlfuncties
    EP_UART3  = 0x03, // UART3: RX3/TX3 en bijhorende flow-controlfuncties
    EP_GPIO0  = 0x04, // GPIO0, gekoppeld aan UART0
    EP_GPIO1  = 0x05, // GPIO1, gekoppeld aan UART0
    EP_GPIO2  = 0x06, // GPIO2, gekoppeld aan UART0
    EP_GPIO3  = 0x07, // GPIO3, gekoppeld aan UART0
    EP_GPIO4  = 0x08, // GPIO4, gekoppeld aan UART1
    EP_GPIO5  = 0x09, // GPIO5, gekoppeld aan UART1
    EP_GPIO6  = 0x0A, // GPIO6, gekoppeld aan UART1
    EP_GPIO7  = 0x0B, // GPIO7, gekoppeld aan UART1
    EP_GPIO8  = 0x0C, // GPIO8, gekoppeld aan UART2
    EP_GPIO9  = 0x0D, // GPIO9, gekoppeld aan UART2
    EP_GPIO10 = 0x0E, // GPIO10, gekoppeld aan UART2
    EP_GPIO11 = 0x0F, // GPIO11, gekoppeld aan UART2
    EP_GPIO12 = 0x10, // GPIO12, gekoppeld aan UART3
    EP_GPIO13 = 0x11, // GPIO13, gekoppeld aan UART3
    EP_GPIO14 = 0x12, // GPIO14, gekoppeld aan UART3
    EP_GPIO15 = 0x13  // GPIO15, gekoppeld aan UART3
  };

  HardwareResourcePin IRQ;
  uint8_t exclusiefMetIrq_[2];

  // ============================================================================
  // DEFAULT — MAX14830 I2C #1
  // ============================================================================
  #if EXTENDER_MAX14830_I2C_AANTAL == 1
  ExtenderMAX14830I2C(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin SDA, HardwareResourcePin SCL, HardwareResourcePin IRQ, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, adres, SDA, SCL, Extender), IRQ(IRQ) {
    exclusiefMetIrq_[0] = exclusief_[0];
    exclusiefMetIrq_[1] = static_cast<uint8_t>(IRQ);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetIrq_, 2 };
  }
  #endif

  // ============================================================================
  // EXPERIMENTEEL — MAX14830 I2C — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_MAX14830_I2C_AANTAL >= 2
  struct Max14830I2cPinnen { uint8_t adres; HardwareResourcePin irq; };

  #define BUNDEL_EXTENDER(N) { I2C_ADDRESS_EXTENDER_MAX14830_I2C_##N, EXTENDER_MAX14830_I2C_##N##_IRQ }

  static constexpr Max14830I2cPinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderMAX14830I2C(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin SDA, HardwareResourcePin SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, SDA, SCL, Extender), IRQ(ExtenderLijst[teller].irq) {
    exclusiefMetIrq_[0] = exclusief_[0];
    exclusiefMetIrq_[1] = static_cast<uint8_t>(IRQ);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetIrq_, 2 };
  }
  #endif
};

#endif
