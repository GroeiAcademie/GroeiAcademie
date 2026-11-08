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

  // De OE-pin is, net als de I2C-adresclaim, een exclusieve GedeeldeBus-resource.
  uint8_t exclusiefMetOe_[2];

  // ============================================================================
  // DEFAULT: PCA9685 #1
  // ============================================================================
  #if EXTENDER_PCA9685_AANTAL == 1
  ExtenderPCA9685(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t tcaChannel = 0xFF)
    : HardwareResourceTypeI2C(parent, component, adres, np_SDA, np_SCL, Extender, tcaChannel), PCA9685(adres, &Wire) {
    exclusiefMetOe_[0] = exclusief_[0];
    exclusiefMetOe_[1] = static_cast<uint8_t>(HardwareResourcePin::NONE);
#if defined(EXTENDER_PCA9685_1_OE_PIN)
    if (adres == I2C_ADDRESS_EXTENDER_PCA9685_1) {
      exclusiefMetOe_[1] = static_cast<uint8_t>(ArduinoUnoShieldPinOmzettenNaarHardwareResourcePin(EXTENDER_PCA9685_1_OE_PIN));
    }
#endif
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetOe_, 2 };
  }
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

  ExtenderPCA9685(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t tcaChannel = 0xFF)
    : HardwareResourceTypeI2C(parent, component, (teller < 2 ? ExtenderLijst[teller].adres : 0xFF), np_SDA, np_SCL, Extender, tcaChannel), PCA9685((teller < 2 ? ExtenderLijst[teller].adres : 0xFF), &Wire) {
    exclusiefMetOe_[0] = exclusief_[0];
    exclusiefMetOe_[1] = static_cast<uint8_t>(HardwareResourcePin::NONE);
#if defined(EXTENDER_PCA9685_1_OE_PIN)
    if (teller == 0) {
      exclusiefMetOe_[1] = static_cast<uint8_t>(ArduinoUnoShieldPinOmzettenNaarHardwareResourcePin(EXTENDER_PCA9685_1_OE_PIN));
    }
#endif
#if defined(EXTENDER_PCA9685_2_OE_PIN)
    if (teller == 1) {
      exclusiefMetOe_[1] = static_cast<uint8_t>(ArduinoUnoShieldPinOmzettenNaarHardwareResourcePin(EXTENDER_PCA9685_2_OE_PIN));
    }
#endif
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetOe_, 2 };
  }
  #endif

  bool Activeren() override {
    // Een expliciet ingestelde OE-pin moet een bekende native boardpin zijn.
#if defined(EXTENDER_PCA9685_1_OE_PIN)
    if (PCA9685::getAddress() == I2C_ADDRESS_EXTENDER_PCA9685_1 &&
        exclusiefMetOe_[1] == static_cast<uint8_t>(HardwareResourcePin::NONE)) return false;
#endif
#if defined(EXTENDER_PCA9685_2_OE_PIN)
    if (PCA9685::getAddress() == I2C_ADDRESS_EXTENDER_PCA9685_2 &&
        exclusiefMetOe_[1] == static_cast<uint8_t>(HardwareResourcePin::NONE)) return false;
#endif
    // V7: ALLCALL uit. Met ALLCALL aan antwoordt de chip ook op het all-call-adres (standaard 0x70), het adres van een TCA9548A.
    // Het FrameWork gebruikt ALLCALL niet. Tot deze regel staat ALLCALL na het opstarten van de chip nog aan.
    const bool gelukt = PCA9685::begin(PCA9685_MODE1_AUTOINCR) && PCA9685::isConnected();

    // Teruglezen: begin() schreef MODE1 met AUTOINCR en zonder ALLCALL; dat moet terug te lezen zijn.
    const uint8_t verwacht = PCA9685_MODE1_AUTOINCR | PCA9685_MODE1_ALLCALL;
    bool terugGelezen = gelukt && ((PCA9685::getMode1() & verwacht) == PCA9685_MODE1_AUTOINCR);

    // V9: PWM-frequentie per PCA9685, alleen als ze ingesteld is (anders blijft de standaard van de chip).
#if defined(EXTENDER_PCA9685_1_PWM_FREQUENTIE)
    if (gelukt && PCA9685::getAddress() == I2C_ADDRESS_EXTENDER_PCA9685_1) terugGelezen = (PCA9685::setFrequency(EXTENDER_PCA9685_1_PWM_FREQUENTIE) == PCA9685_OK) && terugGelezen;
#endif
#if defined(EXTENDER_PCA9685_2_PWM_FREQUENTIE)
    if (gelukt && PCA9685::getAddress() == I2C_ADDRESS_EXTENDER_PCA9685_2) terugGelezen = (PCA9685::setFrequency(EXTENDER_PCA9685_2_PWM_FREQUENTIE) == PCA9685_OK) && terugGelezen;
#endif

    // Punt 3: OE-pin (noodstop in hardware), alleen als ze ingesteld is. setOutputEnablePin() zet de uitgangen eerst uit (HIGH);
    // alleen als de chip goed opstartte gaan ze aan (LOW). De OE-pin is exclusief geclaimd in GedeeldeBus.
#if defined(EXTENDER_PCA9685_1_OE_PIN)
    if (PCA9685::getAddress() == I2C_ADDRESS_EXTENDER_PCA9685_1) {
      terugGelezen = PCA9685::setOutputEnablePin(EXTENDER_PCA9685_1_OE_PIN) && terugGelezen;
      if (gelukt && terugGelezen) terugGelezen = PCA9685::setOutputEnable(true);
    }
#endif
#if defined(EXTENDER_PCA9685_2_OE_PIN)
    if (PCA9685::getAddress() == I2C_ADDRESS_EXTENDER_PCA9685_2) {
      terugGelezen = PCA9685::setOutputEnablePin(EXTENDER_PCA9685_2_OE_PIN) && terugGelezen;
      if (gelukt && terugGelezen) terugGelezen = PCA9685::setOutputEnable(true);
    }
#endif

    return FoutDiagnoseI2C(gelukt, PCA9685::lastError(), terugGelezen);
  }

  // Punt 3: bij uitval alle 16 uitgangen in één keer uit via de OE-pin, ook als de I2C-bus hangt.
  // Zonder OE-pin voor deze chip doet setOutputEnable() niets; daarom ook een poging via I2C (alle kanalen uit).
  // Die poging lukt alleen als de bus nog werkt; hangt de bus, dan wacht ze op de timeout van Wire.
  void StopSturing() override {
#if defined(EXTENDER_PCA9685_1_OE_PIN) || defined(EXTENDER_PCA9685_2_OE_PIN)
    PCA9685::setOutputEnable(false);
#endif
    PCA9685::allOFF();
  }

  bool ReadExtenderPWMFrequency(uint16_t& frequentie) override {
    if (!ComponentBeschikbaarTijdensRuntime()) return false;

    const uint16_t waarde = PCA9685::getFrequency(false);
    const bool gelukt = waarde != 0 && PCA9685::lastError() == PCA9685_OK;
    ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);

    if (gelukt) frequentie = waarde;
    return gelukt;
  }

  // V9: een PWM-kanaal zetten voor een child (bv. later een servo), zonder dat die de PCA9685 kent. waarde: 0 tot 4095.
  // Runtime-herstel: eerst ComponentBeschikbaarTijdensRuntime(), daarna het resultaat van de library doorgeven.
  bool WriteExtenderPWM(uint8_t pin, uint16_t waarde) override {
    if (pin > static_cast<uint8_t>(ExtenderPins::EP_PWM15)) return false;
    if (!ComponentBeschikbaarTijdensRuntime()) return false;
    const bool gelukt = PCA9685::setPWM(pin, waarde > 4095 ? 4095 : waarde) == PCA9685_OK;
    ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
    return gelukt;
  }
};

#endif
