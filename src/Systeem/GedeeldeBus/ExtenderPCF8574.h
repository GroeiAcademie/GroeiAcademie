#ifndef EXTENDERPCF8574_H
#define EXTENDERPCF8574_H

#if EXTENDER_PCF8574_LIBRARY == EXTENDER_PCF8574_LIBRARY_ROB_TILLAART
struct ExtenderPCF8574 : HardwareResourceTypeI2C, PCF8574 {
#else
struct ExtenderPCF8574 : HardwareResourceTypeI2C {
#endif
  enum class ExtenderPins : uint8_t {
    EP_P0 = 0x00, // Quasi-bidirectionele digitale I/O P0
    EP_P1 = 0x01, // Quasi-bidirectionele digitale I/O P1
    EP_P2 = 0x02, // Quasi-bidirectionele digitale I/O P2
    EP_P3 = 0x03, // Quasi-bidirectionele digitale I/O P3
    EP_P4 = 0x04, // Quasi-bidirectionele digitale I/O P4
    EP_P5 = 0x05, // Quasi-bidirectionele digitale I/O P5
    EP_P6 = 0x06, // Quasi-bidirectionele digitale I/O P6
    EP_P7 = 0x07  // Quasi-bidirectionele digitale I/O P7
  };

  HardwareResourcePin np_INT;
  uint8_t exclusiefMetInt_[2];
  uint8_t schaduwByte_ = 0xFF;  // PCF8574 heeft geen apart richtingsregister; input-pinnen staan default HIGH.
  uint8_t laatsteFout_ = 0;

  // ============================================================================
  // PCF8574-BASIS — gedeeld door INPUT_PCF8574 en EXTENDER_PCF8574
  // ============================================================================
  ExtenderPCF8574(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourcePin np_INT, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
#if EXTENDER_PCF8574_LIBRARY == EXTENDER_PCF8574_LIBRARY_ROB_TILLAART
    : HardwareResourceTypeI2C(parent, component, adres, np_SDA, np_SCL, Extender), PCF8574(adres, &Wire), np_INT(np_INT) {
#else
    : HardwareResourceTypeI2C(parent, component, adres, np_SDA, np_SCL, Extender), np_INT(np_INT) {
#endif
    exclusiefMetInt_[0] = exclusief_[0];
    exclusiefMetInt_[1] = static_cast<uint8_t>(np_INT);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetInt_, 2 };
  }

  // ============================================================================
  // EXPERIMENTEEL — PCF8574 — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_PCF8574_AANTAL >= 2
  struct Pcf8574Pinnen { uint8_t adres; HardwareResourcePin intPin; };

  #define BUNDEL_EXTENDER(N) { I2C_ADDRESS_EXTENDER_PCF8574_##N, EXTENDER_PCF8574_##N##_INT }

  static constexpr Pcf8574Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderPCF8574(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
#if EXTENDER_PCF8574_LIBRARY == EXTENDER_PCF8574_LIBRARY_ROB_TILLAART
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, np_SDA, np_SCL, Extender), PCF8574(ExtenderLijst[teller].adres, &Wire), np_INT(ExtenderLijst[teller].intPin) {
#else
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, np_SDA, np_SCL, Extender), np_INT(ExtenderLijst[teller].intPin) {
#endif
    exclusiefMetInt_[0] = exclusief_[0];
    exclusiefMetInt_[1] = static_cast<uint8_t>(np_INT);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetInt_, 2 };
  }
  #endif

#if EXTENDER_PCF8574_LIBRARY == EXTENDER_PCF8574_LIBRARY_ROB_TILLAART
  bool begin() {
    InitialiserenGedeeldeBus(GedeeldeBusType::I2C);
    bool gelukt = PCF8574::begin();
    laatsteFout_ = static_cast<uint8_t>(PCF8574::lastError());
    return gelukt;
  }
#else
  bool begin(uint8_t beginWaarde = 0xFF) {
    schaduwByte_ = beginWaarde;
    InitialiserenGedeeldeBus(GedeeldeBusType::I2C);
    write8(schaduwByte_);
    return laatsteFout_ == 0;
  }
#endif

  bool Activeren() override {
#if EXTENDER_PCF8574_LIBRARY == EXTENDER_PCF8574_LIBRARY_ROB_TILLAART
    return begin() && PCF8574::isConnected();
#else
    return begin(0xFF);
#endif
  }

  void write8(uint8_t waarde) {
#ifdef TRACE
    GA_SERIAL.print("TRACE: PCF8574 write8 adres=0x");
    GA_SERIAL.println(adres, HEX);
#endif
    schaduwByte_ = waarde;
#if EXTENDER_PCF8574_LIBRARY == EXTENDER_PCF8574_LIBRARY_ROB_TILLAART
    PCF8574::write8(waarde);
    laatsteFout_ = static_cast<uint8_t>(PCF8574::lastError());
#else
    Wire.beginTransmission(adres);
    Wire.write(waarde);
    laatsteFout_ = Wire.endTransmission();
#endif
#ifdef DEBUG
    GA_DEBUG_PRINT("DEBUG: PCF8574 write8 endTransmission=");
    GA_DEBUG_PRINTLN(laatsteFout_);
#endif
  }

  uint8_t read8() {
#ifdef TRACE
    GA_SERIAL.print("TRACE: PCF8574 read8 adres=0x");
    GA_SERIAL.println(adres, HEX);
#endif
#if EXTENDER_PCF8574_LIBRARY == EXTENDER_PCF8574_LIBRARY_ROB_TILLAART
    uint8_t gelezen = PCF8574::read8();
    laatsteFout_ = static_cast<uint8_t>(PCF8574::lastError());
#ifdef DEBUG
    GA_DEBUG_PRINT("DEBUG: PCF8574 RobTillaart lastError=");
    GA_DEBUG_PRINTLN(laatsteFout_);
#endif
    return gelezen;
#else
    uint8_t ontvangen = Wire.requestFrom(adres, (uint8_t)1);
#ifdef DEBUG
    GA_DEBUG_PRINT("DEBUG: PCF8574 requestFrom=");
    GA_DEBUG_PRINTLN(ontvangen);
    GA_DEBUG_PRINT("DEBUG: PCF8574 available=");
    GA_DEBUG_PRINTLN(Wire.available());
#endif
    if (ontvangen == 0 || !Wire.available()) {
      laatsteFout_ = 1;
#ifdef DEBUG
      GA_DEBUG_PRINTLN("DEBUG: PCF8574 read8 fout");
#endif
      return 0;
    }

    laatsteFout_ = 0;
#ifdef DEBUG
    GA_DEBUG_PRINTLN("DEBUG: PCF8574 read8 OK");
#endif
    return Wire.read();
#endif
  }

  uint8_t lastError() const {
    return laatsteFout_;
  }

  // Chipspecifieke pin-functies (PCF8574_library-API, xreef, nagebouwd op onze eigen write8()/read8(): de chip zelf heeft geen richtingsregister, 
  // enkel één gedeelde byte voor alle 8 pinnen - "pinMode" zet hier enkel de schaduwwaarde zodat een INPUT-pin HIGH blijft (nodig om ze te kunnen lezen).
  void pinMode(ExtenderPins pin, uint8_t modus) {
    uint8_t bit = 1 << static_cast<uint8_t>(pin);
    if (modus == INPUT) schaduwByte_ |= bit;
    else                schaduwByte_ &= ~bit;
    write8(schaduwByte_);
  }

  void digitalWrite(ExtenderPins pin, uint8_t waarde) {
    uint8_t bit = 1 << static_cast<uint8_t>(pin);
    if (waarde == HIGH) schaduwByte_ |= bit;
    else                schaduwByte_ &= ~bit;
    write8(schaduwByte_);
  }

  int digitalRead(ExtenderPins pin) {
    uint8_t gelezen = read8();
    return (gelezen & (1 << static_cast<uint8_t>(pin))) ? HIGH : LOW;
  }

  uint8_t digitalReadAll() {
    return read8();   // alle 8 pinnen tegelijk, als één byte (bit 0 = P0, ... bit 7 = P7)
  }
};

#endif
