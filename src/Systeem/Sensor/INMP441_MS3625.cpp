#include "INMP441_MS3625.h"
#if SENSOR_INMP441_AANTAL > 0 || SENSOR_MS3625_AANTAL > 0
bool INMP441_MS3625::inpluggen() {
  if (ingeplugd) return true;
  if (firstExtenderNode() != nullptr || np_BCLK == HardwareResourcePin::NONE ||
      np_WS == HardwareResourcePin::NONE || np_SD == HardwareResourcePin::NONE ||
      np_BCLK == HardwareResourcePin::CUSTOM || np_WS == HardwareResourcePin::CUSTOM ||
      np_SD == HardwareResourcePin::CUSTOM || np_BCLK == np_WS || np_BCLK == np_SD || np_WS == np_SD)
    return false;
  return Sensor::inpluggen();
}

bool INMP441_MS3625::Activeren() {
  if (!ingeplugd || !parent || !parent->actief) return false;
  // Nieuwe sessie: geen geluidsvenster uit een vorige sessie.
  soundLevel.ResetWindow();
#if BOARD_VERSION == BOARD_RP2040_CYTRON_MAKER_UNO
  // arduino-pico legt WS altijd op de pin direct na BCLK.
  if (NativeArduinoPinVan(np_WS) != NativeArduinoPinVan(np_BCLK) + 1) return false;
#endif
  uint8_t sdPin = NativeArduinoPinVan(np_SD);
#if GEDEELDE_BUS_MIXED_PINS && EXTENDER_CD74HC4067_AANTAL > 0
  if (viaMixed) {
    // SD via de CD74HC4067: eerst het kanaal kiezen (blijft open), dan I2S op de native Z-pin.
    if (extenderHulp_[2] == nullptr || extenderHulp_[2]->component != GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067 ||
        !extenderHulp_[2]->actief || !extenderHulp_[2]->SelectAccessForChild(this)) return false;
    sdPin = NativeArduinoPinVan(static_cast<ExtenderCD74HC4067*>(extenderHulp_[2])->np_Z);
  }
#endif
#if BOARD_VERSION == BOARD_RP2040_CYTRON_MAKER_UNO
  // 32-bit slots, zoals op de ESP32: de 24-bit microfoondata staat links uitgelijnd.
  if (!I2Sinput.setBCLK(NativeArduinoPinVan(np_BCLK)) || !I2Sinput.setDATA(sdPin) || !I2Sinput.setBitsPerSample(32)) return false;
  return I2Sinput.begin(component == GedeeldeBusComponent::MS3625 ? SENSOR_MS3625_SAMPLEFREQUENTIE_HZ : SENSOR_INMP441_SAMPLEFREQUENTIE_HZ);
#else
  I2S.setPins(NativeArduinoPinVan(np_BCLK), NativeArduinoPinVan(np_WS), -1, sdPin);
  return I2S.begin(I2S_MODE_STD, (component == GedeeldeBusComponent::MS3625 ? SENSOR_MS3625_SAMPLEFREQUENTIE_HZ : SENSOR_INMP441_SAMPLEFREQUENTIE_HZ),
                   I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO);
#endif
}

size_t INMP441_MS3625::LeesSamples(int32_t* samples, size_t aantal) {
  if (!actief || samples == nullptr || aantal == 0 || aantal > (static_cast<size_t>(-1) / sizeof(int32_t))) return 0;
#if GEDEELDE_BUS_MIXED_PINS && EXTENDER_CD74HC4067_AANTAL > 0
  // Mixed: SD loopt via de CD74HC4067; is die niet actief, dan geen samples.
  if (viaMixed && (extenderHulp_[2] == nullptr || !extenderHulp_[2]->actief)) return 0;
#endif
#if BOARD_VERSION == BOARD_RP2040_CYTRON_MAKER_UNO
  // arduino-pico: read32() leest telkens één volledig stereopaar, zodat links en rechts in de juiste volgorde blijven.
  // read32() wacht tot het paar er is; een maximale wachttijd is niet aangetoond.
  size_t gelezen = 0;
  while (gelezen + 2 <= aantal) {
    int32_t links = 0;
    int32_t rechts = 0;
    if (!I2Sinput.read32(&links, &rechts)) break;
    samples[gelezen++] = links;
    samples[gelezen++] = rechts;
  }
#else
  // Officiële API levert het aantal gelezen bytes, niet het aantal samples.
  const size_t gelezen = I2S.readBytes(reinterpret_cast<char*>(samples), aantal * sizeof(int32_t)) / sizeof(int32_t);
#endif
  // Geluidsniveau: alleen het linkerslot (even posities), 24-bit waarde links uitgelijnd in het 32-bit woord.
  // Onderbreking: de standaardgrens van AddSample() (250 ms), want de DMA buffert de samples tussen twee aanroepen.
  for (size_t index = 0; index < gelezen; index += 2) {
    if (soundLevel.sampleCount >= (component == GedeeldeBusComponent::MS3625 ? SENSOR_MS3625_SAMPLEFREQUENTIE_HZ : SENSOR_INMP441_SAMPLEFREQUENTIE_HZ) / 4) soundLevel.ResetWindow();
    soundLevel.AddSample(samples[index] >> 8);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    // Venster net vol: RMS (kanaal 2) en piek-tot-piek (kanaal 3) van dit venster naar Data, op het moment van afsluiten.
    // Het venster blijft voor de leesfuncties beschikbaar tot de volgende sample het wist.
    if (soundLevel.sampleCount == (component == GedeeldeBusComponent::MS3625 ? SENSOR_MS3625_SAMPLEFREQUENTIE_HZ : SENSOR_INMP441_SAMPLEFREQUENTIE_HZ) / 4) {
      float rootMeanSquare = NAN;
      int32_t peakToPeak = 0;
      if (soundLevel.ReadRootMeanSquare(rootMeanSquare)) WriteQueueOrStack(2, rootMeanSquare);
      if (soundLevel.ReadPeakToPeak(peakToPeak)) WriteQueueOrStack(3, static_cast<int>(peakToPeak));
    }
#endif
  }
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  for (size_t index = 0; index < gelezen; ++index)
    WriteQueueOrStack(1, static_cast<int>(samples[index])); // ESP32 en RP2040: 32-bit int, geen conversieverlies
#endif
  return gelezen;
}

bool INMP441_MS3625::afmelden() {
#if BOARD_VERSION == BOARD_RP2040_CYTRON_MAKER_UNO
  if (actief) I2Sinput.end();
#else
  if (actief) I2S.end();
#endif
  return Sensor::afmelden();
}
#endif
