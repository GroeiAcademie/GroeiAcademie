#ifndef EXTENDERPCF8574_H
#define EXTENDERPCF8574_H

struct ExtenderPCF8574 : HardwareResourceTypeI2C {
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
    : HardwareResourceTypeI2C(parent, component, adres, np_SDA, np_SCL, Extender), np_INT(np_INT) {
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
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, np_SDA, np_SCL, Extender), np_INT(ExtenderLijst[teller].intPin) {
    exclusiefMetInt_[0] = exclusief_[0];
    exclusiefMetInt_[1] = static_cast<uint8_t>(np_INT);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetInt_, 2 };
  }
  #endif

  bool begin(uint8_t beginWaarde = 0xFF) {
    schaduwByte_ = beginWaarde;
    InitialiserenGedeeldeBus(GedeeldeBusType::I2C);
    WriteByte(schaduwByte_);
    return laatsteFout_ == 0;
  }

  void WriteByte(uint8_t waarde) {
#ifdef TRACE
    GA_SERIAL.print("TRACE: PCF8574 WriteByte adres=0x");
    GA_SERIAL.println(adres, HEX);
#endif
    schaduwByte_ = waarde;
    Wire.beginTransmission(adres);
    Wire.write(waarde);
    laatsteFout_ = Wire.endTransmission();
#ifdef DEBUG
    GA_DEBUG_PRINT("DEBUG: PCF8574 WriteByte endTransmission=");
    GA_DEBUG_PRINTLN(laatsteFout_);
#endif
  }

  uint8_t ReadByte() {
#ifdef TRACE
    GA_SERIAL.print("TRACE: PCF8574 ReadByte adres=0x");
    GA_SERIAL.println(adres, HEX);
#endif
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
      GA_DEBUG_PRINTLN("DEBUG: PCF8574 ReadByte fout");
#endif
      return 0;
    }

    laatsteFout_ = 0;
#ifdef DEBUG
    GA_DEBUG_PRINTLN("DEBUG: PCF8574 ReadByte OK");
#endif
    return Wire.read();
  }

  uint8_t lastError() const {
    return laatsteFout_;
  }

  // Chipspecifieke pin-functies (PCF8574_library-API, xreef, nagebouwd op onze eigen WriteByte()/ReadByte(): de chip zelf heeft geen richtingsregister, 
  // enkel één gedeelde byte voor alle 8 pinnen - "pinMode" zet hier enkel de schaduwwaarde zodat een INPUT-pin HIGH blijft (nodig om ze te kunnen lezen).
  void pinMode(ExtenderPins pin, uint8_t modus) {
    uint8_t bit = 1 << static_cast<uint8_t>(pin);
    if (modus == INPUT) schaduwByte_ |= bit;
    else                schaduwByte_ &= ~bit;
    WriteByte(schaduwByte_);
  }

  void digitalWrite(ExtenderPins pin, uint8_t waarde) {
    uint8_t bit = 1 << static_cast<uint8_t>(pin);
    if (waarde == HIGH) schaduwByte_ |= bit;
    else                schaduwByte_ &= ~bit;
    WriteByte(schaduwByte_);
  }

  int digitalRead(ExtenderPins pin) {
    uint8_t gelezen = ReadByte();
    return (gelezen & (1 << static_cast<uint8_t>(pin))) ? HIGH : LOW;
  }

  uint8_t digitalReadAll() {
    return ReadByte();   // alle 8 pinnen tegelijk, als één byte (bit 0 = P0, ... bit 7 = P7)
  }
};

#endif
