#include "MCU6701.h"
#if SENSOR_MCU6701_AANTAL > 0
bool MCU6701::inpluggen() {
  if (ingeplugd) return true;
  if (methode == UitleesMethode::ANALOOG) return Sensor::inpluggen();
  if (firstExtenderNode() != nullptr || np_CS == HardwareResourcePin::NONE ||
      np_CS == HardwareResourcePin::CUSTOM) return false;
  if (!Sensor::inpluggen()) return false;
  pinMode(NativeArduinoPinVan(np_CS), OUTPUT);
  digitalWrite(NativeArduinoPinVan(np_CS), HIGH);
  InitialiserenGedeeldeBus(GedeeldeBusType::SPI);
  return true;
}
bool MCU6701::Activeren() {
  if (!ingeplugd || !parent || !parent->actief) return false;
  if (methode == UitleesMethode::SPI) return true; // MCP3201 heeft geen chip-ID: niet als hardwarebewijs interpreteren
  return parent->StartExtenderReadAnalog(sensorPin, 0) == 1;
}
bool MCU6701::StartAnalogRead() {
  laatsteStartresultaat = -1;
  if (metingGestart || !actief || !parent) return false;
  if (methode == UitleesMethode::SPI) {
    laatsteStartresultaat = 1;
    metingGestart = true;
    return true;
  }
  if (parent->gedeeldeIdentiteiten &&
      parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
    HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
    if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  }
  laatsteStartresultaat = parent->StartExtenderReadAnalog(sensorPin, SENSOR_MCU6701_ADC_BEREIK_MILLIVOLT);
  if (laatsteStartresultaat == 1) { metingGestart = true; return true; }
  if (laatsteStartresultaat == -1 && parent->gedeeldeIdentiteiten &&
      parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA)
    static_cast<HardwareResourceTypeI2C*>(parent)->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
  return false;
}
bool MCU6701::IsAnalogReadKlaar() {
  if (methode == UitleesMethode::SPI) return true;
  return !metingGestart || parent == nullptr || parent->IsExtenderReadAnalogReady();
}
int MCU6701::GetAnalogRead() {
  if (!metingGestart) return 0;
  if (methode == UitleesMethode::SPI) {
    metingGestart = false;
    int32_t waarde = 0;
    const bool gelukt = RawAnalogRead(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    if (gelukt) WriteQueueOrStack(1, static_cast<int>(waarde));
#endif
    return gelukt ? static_cast<int>(waarde) : 0;
  }
  metingGestart = false;
  if (!parent) return 0;
  int32_t waarde = 0;
  const bool gelukt = parent->GetExtenderReadAnalogValue(waarde);
  if (parent->gedeeldeIdentiteiten && parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA)
    static_cast<HardwareResourceTypeI2C*>(parent)->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) WriteQueueOrStack(1, static_cast<int>(waarde));
#endif

  return gelukt ? static_cast<int>(waarde) : 0;
}
bool MCU6701::RawAnalogRead(int32_t& waarde) {
  waarde = 0;
  if (!actief) return false;
  if (methode == UitleesMethode::SPI) {
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(NativeArduinoPinVan(np_CS), LOW);
    const uint8_t boven = SPI.transfer(0);
    const uint8_t onder = SPI.transfer(0);
    digitalWrite(NativeArduinoPinVan(np_CS), HIGH);
    SPI.endTransaction();
    // MCP3201, Fig. 5-1: één HI-Z, één NULL, 12 databits en twee HI-Z bij 16 klokpulsen.
    const uint16_t ruw = (static_cast<uint16_t>(boven & 0x3F) << 6) | (onder >> 2);
    waarde = static_cast<int32_t>((static_cast<uint32_t>(ruw) * ADC_MAX) / 4095UL);
    return true; // SPI-interface kan zonder chip-ID geen fysiek ontbrekend device detecteren
  }
  const unsigned long begin = millis();
  bool gestart = StartAnalogRead();
  while (!gestart && laatsteStartresultaat == 0 && millis() - begin <= 150) {
    yield();
    gestart = StartAnalogRead();
  }
  if (!metingGestart || !parent) return false;
  while (!IsAnalogReadKlaar() && millis() - begin <= 150) yield();
  const bool klaar = IsAnalogReadKlaar();
  metingGestart = false;
  const bool gelukt = parent->GetExtenderReadAnalogValue(waarde);
  if (parent->gedeeldeIdentiteiten && parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA)
    static_cast<HardwareResourceTypeI2C*>(parent)->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
  if (!klaar || !gelukt) { waarde = 0; return false; }
  return true;
}
bool MCU6701::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  int32_t ruw = 0;
  if (!RawAnalogRead(ruw)) return false;
  waarde = static_cast<float>(ruw);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(1, waarde);
#endif
  return true;
}
bool MCU6701::afmelden() {
  if (metingGestart && methode == UitleesMethode::ANALOOG) (void)GetAnalogRead();
  if (methode == UitleesMethode::SPI && ingeplugd) digitalWrite(NativeArduinoPinVan(np_CS), HIGH);
  return Sensor::afmelden();
}
#endif
