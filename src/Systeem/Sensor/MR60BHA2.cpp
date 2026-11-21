#include "MR60BHA2.h"
#if SENSOR_MR60BHA2_AANTAL > 0
// Officiele Seeed-mmWave v1.0.0: SOF=0x01, header 8 bytes,
// payload-checksum = bitwise NOT van XOR over de payload.
// TypeHeartBreath::TypeHeartRate = 0x0A15; payload = IEEE-754 float little endian.
bool MR60BHA2::inpluggen() {
  if (ingeplugd) return true;
  // Native: TX en RX moeten aangesloten zijn. Via een UART-Extender: één channel-claim.
#if EXTENDER_CD74HC4067_AANTAL > 0
  // Via een CD74HC4067: alleen de sensoruitgang (TX van de sensor) via kanaal np_RX = EP_Yn naar de native RX (Z);
  // np_TX is NONE (de sensor stuurt zelf zijn frames, geen configuratie nodig). Het kanaal blijft open na Activeren().
  const bool viaCD74HC4067 = parent != nullptr && parent->component == GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067;
  if (viaCD74HC4067 && (aangemeldePinnen.aantalExclusief != 2 || exclusief_[0] != static_cast<uint8_t>(HardwareResourcePin::NONE) ||
                  exclusief_[1] > static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y15) || nativePoort == nullptr)) return false;
  if (!viaCD74HC4067 && aangemeldePinnen.aantalExclusief == 2 && (exclusief_[0] == static_cast<uint8_t>(HardwareResourcePin::NONE) ||
#else
  if (aangemeldePinnen.aantalExclusief == 2 && (exclusief_[0] == static_cast<uint8_t>(HardwareResourcePin::NONE) ||
#endif
      exclusief_[0] == static_cast<uint8_t>(HardwareResourcePin::CUSTOM) ||
      exclusief_[1] == static_cast<uint8_t>(HardwareResourcePin::NONE) ||
      exclusief_[1] == static_cast<uint8_t>(HardwareResourcePin::CUSTOM))) return false;
  if (poort == nullptr) return false;
  if (!Sensor::inpluggen()) return false;
  // Bij een native poort configureert de driver 115200 baud. Een andere Stream
  // wordt door de UART-Extender zelf geinitialiseerd, niet door deze sensor.
  if (nativePoort != nullptr) nativePoort->begin(115200);
  return true;
}
bool MR60BHA2::Activeren() {
  framePositie = 0;
  frameLengte = 0;
  dataXor = 0;
  // Nieuwe sessie: geen ademhaling, aanwezigheid of frame uit een vorige sessie.
  lastBreathRatePerMinute = NAN;
  lastPersonPresent = -1;
  lastFrameMS = 0;
  lastBreathRateMS = 0;
  lastPersonPresentMS = 0;
  if (parent == nullptr || !parent->actief || !ingeplugd || poort == nullptr) return false;
  // Via een UART-Extender: de baud van deze Sensor (115200) op zijn channel instellen.
  return aangemeldePinnen.aantalExclusief != 1 || parent->StartExtenderUART(exclusief_[0], 115200UL);
}
bool MR60BHA2::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!actief || !poort || !parent) return false;
  if (parent->gedeeldeIdentiteiten != nullptr &&
      parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
    HardwareResourceTypeI2C* i2c = static_cast<HardwareResourceTypeI2C*>(parent);
    if (i2c->ingeplugd && !i2c->ComponentBeschikbaarTijdensRuntime()) return false;
  }
  // Niet blokkeren. Hoogstens 64 bytes verwerken per oproep, parsertoestand blijft behouden.
  uint8_t maximumBytes = 64;
  while (maximumBytes-- && poort->available() > 0) {
    const int ontvangen = poort->read();
    if (ontvangen < 0) break;
    const uint8_t byte = static_cast<uint8_t>(ontvangen);
    if (framePositie == 0) {
      if (byte != 0x01) continue;
      frameKop[0] = byte;
      framePositie = 1;
      continue;
    }
    if (framePositie < 8) {
      frameKop[framePositie++] = byte;
      if (framePositie == 8) {
        uint8_t headerXor = 0;
        for (uint8_t i = 0; i < 7; ++i) headerXor ^= frameKop[i];
        frameLengte = (static_cast<uint16_t>(frameKop[3]) << 8) | frameKop[4];
        frameType = (static_cast<uint16_t>(frameKop[5]) << 8) | frameKop[6];
        if (static_cast<uint8_t>(~headerXor) != frameKop[7] || frameLengte > 512) {
          framePositie = 0;
        }
        dataXor = 0;
      }
      continue;
    }
    if (framePositie < static_cast<uint16_t>(8 + frameLengte)) {
      const uint16_t dataIndex = framePositie - 8;
      if (frameType == 0x0A15 && frameLengte == 4 && dataIndex < 4) hartslagBytes[dataIndex] = byte;
      if (dataIndex < 4) frameDataBytes[dataIndex] = byte;
      dataXor ^= byte;
      ++framePositie;
      continue;
    }
    // Laatste byte is de checksom, ook voor een leeg datapakket.
    const bool juist = static_cast<uint8_t>(~dataXor) == byte;
    const bool hartslag = frameType == 0x0A15 && frameLengte == 4;
    framePositie = 0;
    if (juist) {
      lastFrameMS = millis();
      // Ademhalingsritme (per minuut), zoals ESPHome seeed_mr60bha2: 0 betekent geen meting.
      if (frameType == 0x0A14 && frameLengte >= 4 && sizeof(float) == 4) {
        float resultaat;
        memcpy(&resultaat, frameDataBytes, sizeof(float));
        if (isfinite(resultaat) && resultaat > 0) {
          lastBreathRatePerMinute = resultaat;
          lastBreathRateMS = millis();
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
          WriteQueueOrStack(2, resultaat);   // ademfrequentie per minuut
#endif
        }
      }
      // Aanwezigheid van een persoon.
      if (frameType == 0x0F09 && frameLengte >= 1) {
        lastPersonPresent = frameDataBytes[0] != 0 ? 1 : 0;
        lastPersonPresentMS = millis();
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
        WriteQueueOrStack(3, static_cast<int>(lastPersonPresent));   // persoon aanwezig (0 of 1)
#endif
      }
    }
    if (juist && hartslag && sizeof(float) == 4) {
      float resultaat;
      // Seeed specificeert little endian. Arduino ondersteunde borden
      // gebruiken little-endian float; geen ongecontroleerde reinterpret_cast.
      memcpy(&resultaat, hartslagBytes, sizeof(float));
      if (isfinite(resultaat) && resultaat >= 0) {   // weigert NaN en +/- oneindig
        waarde = resultaat;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
        WriteQueueOrStack(1, waarde);
#endif
        return true;
      }
    }
  }
  return false; // geen volledig gevalideerd nieuw hartslagframe ontvangen
}
#endif
