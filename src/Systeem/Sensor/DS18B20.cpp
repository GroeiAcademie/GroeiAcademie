#include "DS18B20.h"
#if SENSOR_DS18B20_AANTAL > 0
// Officieel libraryvoorbeeld: github.com/milesburton/Arduino-Temperature-Control-Library/examples/Single
bool DS18B20::inpluggen() {
  if (ingeplugd) return true;
  if (np_DQ == HardwareResourcePin::NONE || np_DQ == HardwareResourcePin::CUSTOM) return false;
#if EXTENDER_DS2482_800_AANTAL > 0
  // Bij een DS2482-parent is np_DQ het 1-Wire-kanaal IO0..IO7 (0..7),
  // niet een pin van de Arduino. De bridge beheert de timing.
  if (parent != nullptr && parent->component == GedeeldeBusComponent::ONE_WIRE_DS2482v800) {
    if (static_cast<uint8_t>(np_DQ) > 7) return false;
    return Sensor::inpluggen();
  }
#endif
  // Een andere Extender is geen ondersteunde 1-Wire-bridge.
  if (firstExtenderNode() != nullptr) return false;
  if (!Sensor::inpluggen()) return false;
  oneWire.begin(NativeArduinoPinVan(np_DQ));
  return true;
}
bool DS18B20::Activeren() {
  if (!parent || !parent->actief) return false;
#if EXTENDER_DS2482_800_AANTAL > 0
  if (parent->component == GedeeldeBusComponent::ONE_WIRE_DS2482v800) {
    // De bridge zelf kan op een TCA9548A-kanaal zitten. Herselecteer dat
    // kanaal vóór iedere I2C-actie die de 1-Wire-zoekopdracht uitvoert.
    if (!static_cast<HardwareResourceTypeI2C*>(parent)->ComponentBeschikbaarTijdensRuntime()) return false;
    if (!parent->ExtenderOneWireSelectChannel(static_cast<uint8_t>(np_DQ))) return false;
    if (!parent->ExtenderOneWireSearchReset()) return false;
    DeviceAddress adres;
    uint8_t gevonden = 0;
    romGevonden = false;
    while (parent->ExtenderOneWireSearch(adres)) {
      if (adres[0] != 0x28) continue;
      ++gevonden;
      if (romExpliciet) {
        bool gelijk = true;
        for (uint8_t i = 0; i < 8; ++i) if (adres[i] != geselecteerdeRom[i]) gelijk = false;
        if (gelijk) romGevonden = true;
      } else if (gevonden == 1) {
        for (uint8_t i = 0; i < 8; ++i) geselecteerdeRom[i] = adres[i];
      }
    }
    // Zonder expliciete ROM alleen bruikbaar bij exact één DS18B20 op het channel.
    if (!romExpliciet) romGevonden = gevonden == 1;
    return romGevonden;
  }
#endif
  temperatuur.begin();
  temperatuur.setWaitForConversion(false);
  DeviceAddress adres;
  return temperatuur.getDeviceCount() == 1 && temperatuur.getAddress(adres, 0) && adres[0] == 0x28;
}

// Officieel DS2482-800_8_channel_read.ino als protocolbasis: 1-Wire reset, MATCH ROM (55), 44, MATCH ROM (55), BE.
// De conversiewachttijd van 750 ms gebeurt met millis(), niet met delay().
bool DS18B20::StartTemperatuurmeting() {
  if (!actief || !parent || temperatuurmetingGestart) return false;
#if EXTENDER_DS2482_800_AANTAL > 0
  if (parent->component == GedeeldeBusComponent::ONE_WIRE_DS2482v800) {
    HardwareResourceTypeI2C* bridge = static_cast<HardwareResourceTypeI2C*>(parent);   // I2C-1-Wire-Extender: runtimeherstel via I2C, meting via het 1-Wire-contract
    if (!bridge->ComponentBeschikbaarTijdensRuntime() ||
        !bridge->ExtenderOneWireSelectChannel(static_cast<uint8_t>(np_DQ)) ||
        !romGevonden || !bridge->ExtenderOneWireReset() ||
        !bridge->ExtenderOneWireWriteByte(0x55)) { // MATCH ROM; SKIP ROM mag niet bij multi-drop
      bridge->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
      return false;
    }
    for (uint8_t i = 0; i < 8; ++i) {
      if (!bridge->ExtenderOneWireWriteByte(geselecteerdeRom[i])) {
        bridge->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
        return false;
      }
    }
    if (!bridge->ExtenderOneWireWriteByte(0x44)) { // CONVERT T
      bridge->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
      return false;
    }
    bridge->ResultaatComponentToegangTijdensRuntimeVerwerken(true);
  } else
#endif
  {
    temperatuur.requestTemperatures();
  }
  tijdstipTemperatuurmetingStart = millis();
  temperatuurmetingGestart = true;
  return true;
}
bool DS18B20::IsTemperatuurmetingKlaar() {
  return temperatuurmetingGestart && millis() - tijdstipTemperatuurmetingStart >= 750UL;
}
bool DS18B20::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!IsTemperatuurmetingKlaar() || !parent) return false;
  temperatuurmetingGestart = false;
#if EXTENDER_DS2482_800_AANTAL > 0
  if (parent->component == GedeeldeBusComponent::ONE_WIRE_DS2482v800) {
    HardwareResourceTypeI2C* bridge = static_cast<HardwareResourceTypeI2C*>(parent);   // I2C-1-Wire-Extender: runtimeherstel via I2C, meting via het 1-Wire-contract
    if (!bridge->ComponentBeschikbaarTijdensRuntime() ||
        !bridge->ExtenderOneWireSelectChannel(static_cast<uint8_t>(np_DQ)) ||
        !romGevonden || !bridge->ExtenderOneWireReset() ||
        !bridge->ExtenderOneWireWriteByte(0x55)) { // MATCH ROM, alleen gekozen sensor
      bridge->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
      return false;
    }
    for (uint8_t i = 0; i < 8; ++i) {
      if (!bridge->ExtenderOneWireWriteByte(geselecteerdeRom[i])) {
        bridge->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
        return false;
      }
    }
    if (!bridge->ExtenderOneWireWriteByte(0xBE)) {
      bridge->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
      return false;
    }
    uint8_t scratchpad[9];
    for (uint8_t i = 0; i < 9; ++i) {
      if (!bridge->ExtenderOneWireReadByte(&scratchpad[i])) {
        bridge->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
        return false;
      }
    }
    // Dallas/Maxim CRC-8, polynoom 0x31 in gereflecteerde vorm 0x8C.
    uint8_t crc = 0;
    for (uint8_t i = 0; i < 8; ++i) {
      uint8_t data = scratchpad[i];
      for (uint8_t bit = 0; bit < 8; ++bit) {
        const uint8_t mix = (crc ^ data) & 1;
        crc >>= 1;
        if (mix) crc ^= 0x8C;
        data >>= 1;
      }
    }
    const bool gelukt = crc == scratchpad[8];
    bridge->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
    if (!gelukt) return false;
    const int16_t raw = static_cast<int16_t>(
        static_cast<uint16_t>(scratchpad[0]) |
        (static_cast<uint16_t>(scratchpad[1]) << 8));
    waarde = raw / 16.0f;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    WriteQueueOrStack(1, waarde);
#endif
    return true;
  }
#endif
  waarde = temperatuur.getTempCByIndex(0);
  const bool gelukt = waarde > DEVICE_DISCONNECTED_C && !isnan(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) WriteQueueOrStack(1, waarde);
#endif
  return gelukt;
}
#endif
