#include "LD2410D.h"
#if SENSOR_LD2410D_AANTAL > 0
// Fabrikant: Hi-Link HLK-LD2410D Serial Communication Protocol V1.01 (2025-05-21), 1.3.1.
// F4 F3 F2 F1 + uint16 little-endian lengte + [type AA ... 55 00] + F8 F7 F6 F5.
// Beperkt RAM: geen dynamische allocatie; per sensor een vast buffer van 48 bytes.
// Geldige rapportframes worden tijdens runtime gedecodeerd naar aanwezigheid (0..3).
bool LD2410D::inpluggen() {
  if (ingeplugd) return true;
  // Native: TX en RX moeten aangesloten zijn. Via een UART-Extender: één channel-claim.
  if (aangemeldePinnen.aantalExclusief == 2 && (exclusief_[0] == static_cast<uint8_t>(HardwareResourcePin::NONE) ||
      exclusief_[0] == static_cast<uint8_t>(HardwareResourcePin::CUSTOM) ||
      exclusief_[1] == static_cast<uint8_t>(HardwareResourcePin::NONE) ||
      exclusief_[1] == static_cast<uint8_t>(HardwareResourcePin::CUSTOM))) return false;
  if (poort == nullptr) return false;
  return Sensor::inpluggen();
}
bool LD2410D::Activeren() {
  // Alleen interfacecontrole; een geldig frame moet bij het uitlezen worden gecontroleerd.
  if (parent == nullptr || !parent->actief || !ingeplugd) return false;
  // Via een UART-Extender: de baud van deze Sensor (115200) op zijn channel instellen.
  return aangemeldePinnen.aantalExclusief != 1 || parent->StartExtenderUART(exclusief_[0], 115200UL);
}

// Een byte uit een bestaande Stream is geen geinterpreteerde sensormeting.
bool LD2410D::LeesOntvangenByte(uint8_t& waarde) {
  waarde = 0;
  if (!actief || !parent) return false;
  if (parent->gedeeldeIdentiteiten != nullptr &&
      parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
    HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
    if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  }
  if (poort->available() <= 0) return false;
  const int ontvangen = poort->read();
  if (ontvangen < 0) return false;
  waarde = static_cast<uint8_t>(ontvangen);
  return true;
}

// HLK-LD2410D reporting protocol, normal and engineering frames.
// Zonder volledig frame geen nieuwe aanwezigheid rapporteren.
bool LD2410D::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!actief || !parent) return false;
  if (parent->gedeeldeIdentiteiten &&
      parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
    HardwareResourceTypeI2C* i2c = static_cast<HardwareResourceTypeI2C*>(parent);
    if (i2c->ingeplugd && !i2c->ComponentBeschikbaarTijdensRuntime()) return false;
  }
  const uint8_t kop[4] = {0xF4, 0xF3, 0xF2, 0xF1};
  const uint8_t einde[4] = {0xF8, 0xF7, 0xF6, 0xF5};
  uint8_t maximaal = 96;
  while (maximaal-- && poort->available() > 0) {
    const int gelezen = poort->read();
    if (gelezen < 0) break;
    const uint8_t byte = static_cast<uint8_t>(gelezen);
    if (framePositie < 4) {
      framePositie = byte == kop[framePositie] ? framePositie + 1 : (byte == kop[0] ? 1 : 0);
      continue;
    }
    if (framePositie == 4) { // payloadlengte LE
      verwachteLengte = byte;
      framePositie = verwachteLengte >= 13 && verwachteLengte <= sizeof(frame) ? 5 : 0;
      continue;
    }
    if (framePositie == 5) { // hoog byte; buffer maximaal 48
      if (byte != 0) { framePositie = 0; continue; }
      framePositie = 6;
      continue;
    }
    if (framePositie < 6 + verwachteLengte) {
      frame[framePositie++ - 6] = byte;
      continue;
    }
    const uint8_t staartIndex = framePositie - 6 - verwachteLengte;
    if (staartIndex >= 4 || byte != einde[staartIndex]) { framePositie = 0; continue; }
    ++framePositie;
    if (staartIndex == 3) {
      framePositie = 0;
      // type 01=engineering, 02=basic, 0xAA start en 0x55 00 sluit af
      if ((frame[0] == 0x01 || frame[0] == 0x02) && frame[1] == 0xAA &&
          frame[verwachteLengte-2] == 0x55 && frame[verwachteLengte-1] == 0x00 &&
          frame[2] <= 3) {
        waarde = static_cast<float>(frame[2]);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
        WriteQueueOrStack(1, waarde);
#endif
        return true;
      }
    }
  }
  return false;
}
#endif
