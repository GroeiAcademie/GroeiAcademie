#include "LD2450.h"
#if SENSOR_LD2450_AANTAL > 0
// Hi-Link HLK-LD2450 Instruction Manual, sectie 6:
// https://device.report/manual/14806950
// Datagram: AA FF 03 00 + 24 bytes doelinformatie + 55 CC (30 bytes totaal).
// Decodeert pas bij een volledig radarframe een nieuwe positie.
bool LD2450::inpluggen() {
  if (ingeplugd) return true;
  // Native: TX en RX moeten aangesloten zijn. Via een UART-Extender: één channel-claim.
  if (aangemeldePinnen.aantalExclusief == 2 && (exclusief_[0] == static_cast<uint8_t>(HardwareResourcePin::NONE) ||
      exclusief_[0] == static_cast<uint8_t>(HardwareResourcePin::CUSTOM) ||
      exclusief_[1] == static_cast<uint8_t>(HardwareResourcePin::NONE) ||
      exclusief_[1] == static_cast<uint8_t>(HardwareResourcePin::CUSTOM))) return false;
  if (poort == nullptr) return false;
  return Sensor::inpluggen();
}
bool LD2450::Activeren() {
  // Alleen interfacecontrole; een geldig frame moet bij het uitlezen worden gecontroleerd.
  if (parent == nullptr || !parent->actief || !ingeplugd) return false;
  // Via een UART-Extender: de baud van deze Sensor (256000) op zijn channel instellen.
  return aangemeldePinnen.aantalExclusief != 1 || parent->StartExtenderUART(exclusief_[0], 256000UL);
}

// Een byte uit een bestaande Stream is geen geinterpreteerde sensormeting.
bool LD2450::LeesOntvangenByte(uint8_t& waarde) {
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

// HLK-LD2450: 4-byte header, 3 x 8 bytes target, 2-byte terminator.
// Niet-blokkerend; de waarden worden pas geldig na het volledige frame.
bool LD2450::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!actief || !parent) return false;
  if (parent->gedeeldeIdentiteiten &&
      parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
    HardwareResourceTypeI2C* i2c = static_cast<HardwareResourceTypeI2C*>(parent);
    if (i2c->ingeplugd && !i2c->ComponentBeschikbaarTijdensRuntime()) return false;
  }
  const uint8_t kop[4] = {0xAA, 0xFF, 0x03, 0x00};
  uint8_t maximaal = 96;
  while (maximaal-- && poort->available() > 0) {
    const int gelezen = poort->read();
    if (gelezen < 0) break;
    const uint8_t byte = static_cast<uint8_t>(gelezen);
    if (framePositie < 4) {
      if (byte == kop[framePositie]) frame[framePositie++] = byte;
      else { framePositie = byte == kop[0] ? 1 : 0; if (framePositie) frame[0] = byte; }
      continue;
    }
    frame[framePositie++] = byte;
    if (framePositie != 30) continue;
    framePositie = 0;
    if (frame[28] != 0x55 || frame[29] != 0xCC) continue;
    positieGeldig = false;
    // Het protocol sorteert de drie doelen niet op afstand: alle geldige doelen vergelijken en het dichtstbijzijnde houden.
    bool gevonden = false;
    float kleinsteAfstand = 0;
    for (uint8_t doel = 0; doel < 3; ++doel) {
      const uint8_t* t = frame + 4 + doel * 8;
      bool leeg = true;
      for (uint8_t j = 0; j < 8; ++j) if (t[j]) leeg = false;
      if (leeg) continue;
      const uint16_t rawX = static_cast<uint16_t>(t[0]) | (static_cast<uint16_t>(t[1]) << 8);
      const uint16_t rawY = static_cast<uint16_t>(t[2]) | (static_cast<uint16_t>(t[3]) << 8);
      // Hi-Link: bit 15=1 is positief, 0 is negatief.
      const float x = (rawX & 0x8000) ? static_cast<float>(rawX & 0x7FFF) : -static_cast<float>(rawX);
      const float y = (rawY & 0x8000) ? static_cast<float>(rawY & 0x7FFF) : -static_cast<float>(rawY);
      const float afstand = sqrtf(x * x + y * y);
      if (gevonden && afstand >= kleinsteAfstand) continue;
      gevonden = true;
      kleinsteAfstand = afstand;
      laatsteX = x;
      laatsteY = y;
    }
    if (gevonden) {
      positieGeldig = true;
      waarde = kleinsteAfstand;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
      {
        WriteQueueOrStack(1, waarde);      // afstand dichtstbijzijnde doel
        WriteQueueOrStack(2, laatsteX);    // X-positie
        WriteQueueOrStack(3, laatsteY);    // Y-positie
      }
#endif
      return true;
    }
    // Geldig leeg rapport: geen doel, dus geen verzonnen positie.
    laatsteX = NAN; laatsteY = NAN;
    waarde = 0;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    WriteQueueOrStack(1, waarde);
#endif
    return true;
  }
  return false;
}
bool LD2450::LeesPositie(float& x, float& y) {
  x = positieGeldig ? laatsteX : NAN;
  y = positieGeldig ? laatsteY : NAN;
  return positieGeldig;
}
#endif
