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

  HardwareResourcePin np_START, np_RESET, np_PWDN;
  uint8_t exclusiefMetControle_[4];

  // TI ADS1158: auto-scan met exact één single-ended channel tegenover AINCOM.
  // In fixed-channel mode verwijst MUXSCH AINN=0x0F naar AIN15, NIET AINCOM.
  // Alle SPI-commando's worden volledig onder eigen CS verzonden.
  bool metingBezig = false;
  uint8_t metingChannel = 0;
  bool meetwaardeKlaar = false;
  unsigned long tijdstipConversiestartMicros = 0;
  int16_t laatsteMeetwaarde = 0;
  // Datasheet: +VREF geeft code 0x7800 (30720); 0x7FFF hoort bij 1,06 x VREF. De schaal 0..0x7FFF loopt dus tot VREF x 32767 / 30720.
  uint16_t GetExtenderReadAnalogEffectiefBereikMillivolt() const override {
    return static_cast<uint16_t>((static_cast<uint32_t>(EXTENDER_ADS1158_REFERENTIE_MILLIVOLT) * 32767UL) / 30720UL);
  }
  void ADS1158SchrijfRegister(uint8_t adres, uint8_t waarde) {
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    WriteCS(LOW);
    SPI.transfer(static_cast<uint8_t>(0x60 | (adres & 0x0F))); // C[2:0]=011 WRITE
    SPI.transfer(waarde);
    WriteCS(HIGH);
    SPI.endTransaction();
  }
  void ADS1158Pulse() {
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    WriteCS(LOW);
    SPI.transfer(0x80); // C[2:0]=100, één conversie met START laag
    WriteCS(HIGH);
    SPI.endTransaction();
  }
  bool Activeren() override {
    metingBezig = false;
    meetwaardeKlaar = false;
    if (!ingeplugd || parent == nullptr || !parent->actief) return false;
    // START moet LOW zijn voor de PULSE-commandowerking, ook bij recovery.
    if (np_START != HardwareResourcePin::NONE) {
      digitalWrite(NativeArduinoPinVan(np_START), LOW);
      pinMode(NativeArduinoPinVan(np_START), OUTPUT);
    }
    if (np_PWDN != HardwareResourcePin::NONE) {
      digitalWrite(NativeArduinoPinVan(np_PWDN), HIGH);
      pinMode(NativeArduinoPinVan(np_PWDN), OUTPUT);
    }
    // CONFIG0: MUXMOD=0 (auto-scan), STAT=1; geen externe BYPAS.
    ADS1158SchrijfRegister(0x00, 0x02);
    // CONFIG1: IDLMOD=1 (sleep), DRATE=11 (auto-scan), datasheet default.
    ADS1158SchrijfRegister(0x01, 0x83);
    // Geen differential of interne monitor: het gewenste single-ended channel
    // wordt voor elke afzonderlijke PULSE geselecteerd via MUXSG0/MUXSG1.
    ADS1158SchrijfRegister(0x03, 0x00); // MUXDIF
    ADS1158SchrijfRegister(0x04, 0x00); // MUXSG0
    ADS1158SchrijfRegister(0x05, 0x00); // MUXSG1
    ADS1158SchrijfRegister(0x06, 0x00); // SYSRED
    return true;
  }
  int8_t StartExtenderReadAnalog(uint8_t pin, uint16_t bereikMillivolt) override {
    if (pin > static_cast<uint8_t>(ExtenderPins::EP_AIN15)) return -2;
    if (bereikMillivolt == 0) return 1;
    if (bereikMillivolt > EXTENDER_ADS1158_REFERENTIE_MILLIVOLT) return -2;
    if (metingBezig) return 0;
    if (!actief) return -1;
    // Auto-scan: exact één AIN0..AIN15 tegenover AINCOM.
    // Begin altijd met de lege groep om de channel-pointer correct te resetten.
    if (pin < 8) {
      ADS1158SchrijfRegister(0x05, 0x00);
      ADS1158SchrijfRegister(0x04, static_cast<uint8_t>(1U << pin));
    } else {
      ADS1158SchrijfRegister(0x04, 0x00);
      ADS1158SchrijfRegister(0x05, static_cast<uint8_t>(1U << (pin - 8)));
    }
    metingChannel = pin;
    meetwaardeKlaar = false;
    metingBezig = true;
    ADS1158Pulse();
    tijdstipConversiestartMicros = micros();
    return 1;
  }
  bool IsExtenderReadAnalogReady() override {
    // Auto-scan: zonder DRDY-aansluiting voorlopig conservatief 2 ms wachten.
    // Get controleert bovendien de geldige STATUS/NEW/CHID-bits; dit is
    // nog geen vervanging van een fysiek gevalideerde DRDY-controle.
    return !metingBezig || !actief || (micros() - tijdstipConversiestartMicros >= 2000UL);
  }
  bool GetExtenderReadAnalogValue(int32_t& waarde) override {
    waarde = 0;
    if (!metingBezig || !IsExtenderReadAnalogReady() || !actief) return false;
    metingBezig = false;
    // Auto-scan met één channel: statusbyte heeft NEW, OVF, SUPPLY en CHID.
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    WriteCS(LOW);
    SPI.transfer(0x30); // READ CHANNEL DATA, MUL=1
    const uint8_t status = SPI.transfer(0x00);
    const uint8_t msb = SPI.transfer(0x00);
    const uint8_t lsb = SPI.transfer(0x00);
    WriteCS(HIGH);
    SPI.endTransaction();
    // Single-ended channels AIN0..AIN15 hebben CHID 0x08..0x17.
    // Accepteer nooit een oude sample, oversturing of een ander channel.
    if ((status & 0x80) == 0 || (status & 0x60) != 0 ||
        (status & 0x1F) != static_cast<uint8_t>(0x08 + metingChannel)) return false;
    laatsteMeetwaarde = static_cast<int16_t>((static_cast<uint16_t>(msb) << 8) | lsb);
    // Negatieve differential input is geen geldige unipolaire sensorwaarde.
    if (laatsteMeetwaarde < 0) return false;
#if ADC_BITS <= 15
    waarde = static_cast<int32_t>(laatsteMeetwaarde) >> (15 - ADC_BITS);
#else
    waarde = static_cast<int32_t>(laatsteMeetwaarde) << (ADC_BITS - 15);
#endif
    return true;
  }

  // ============================================================================
  // DEFAULT: ADS1158 #1
  // ============================================================================
  #if EXTENDER_ADS1158_AANTAL == 1
  ExtenderADS1158(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_CS, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, HardwareResourcePin np_START, HardwareResourcePin np_RESET, HardwareResourcePin np_PWDN, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, component, np_CS, np_SCK, np_MISO, np_MOSI, Extender), np_START(np_START), np_RESET(np_RESET), np_PWDN(np_PWDN) {
    exclusiefMetControle_[0] = static_cast<uint8_t>(np_CS);
    exclusiefMetControle_[1] = static_cast<uint8_t>(np_START);
    exclusiefMetControle_[2] = static_cast<uint8_t>(np_RESET);
    exclusiefMetControle_[3] = static_cast<uint8_t>(np_PWDN);
    aangemeldePinnen = { gedeeld_, 3, exclusiefMetControle_, 4 };
  }
  #endif

  // ============================================================================
  // EXPERIMENTEEL: ADS1158 #2
  // ============================================================================
  #if EXTENDER_ADS1158_AANTAL >= 2
  struct Ads1158Pinnen { HardwareResourcePin csPin; HardwareResourcePin pwdn; HardwareResourcePin reset; HardwareResourcePin start; };

  #define BUNDEL_EXTENDER(N) { EXTENDER_ADS1158_##N##_CS_PIN, EXTENDER_ADS1158_##N##_PWDN, EXTENDER_ADS1158_##N##_RESET, EXTENDER_ADS1158_##N##_START }

  static constexpr Ads1158Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderADS1158(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, component, ExtenderLijst[teller].csPin, np_SCK, np_MISO, np_MOSI, Extender), np_START(ExtenderLijst[teller].start), np_RESET(ExtenderLijst[teller].reset), np_PWDN(ExtenderLijst[teller].pwdn) {
    exclusiefMetControle_[0] = static_cast<uint8_t>(ExtenderLijst[teller].csPin);
    exclusiefMetControle_[1] = static_cast<uint8_t>(np_START);
    exclusiefMetControle_[2] = static_cast<uint8_t>(np_RESET);
    exclusiefMetControle_[3] = static_cast<uint8_t>(np_PWDN);
    aangemeldePinnen = { gedeeld_, 3, exclusiefMetControle_, 4 };
  }
  #endif
};

#endif
