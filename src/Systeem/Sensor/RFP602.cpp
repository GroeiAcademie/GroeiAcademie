#include "RFP602.h"
#if ((defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)) || (defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && (defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)))) || (defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || (defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)))
  #include "../Screen/Screen.h"   // ScreenData in de waarde van de override-defines
#endif

struct RFP602* sensorRFP602set1 = nullptr;
#if defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
struct RFP602* sensorRFP602set2 = nullptr;   // 1b: set 2
#endif

// V5: RFP602 meet via de analoge functies van zijn parent (V8) en kent het type ADC niet meer.
// Het meetbereik (6144 mV, vroeger ADS1X15_GAIN_6144MV) gaat mee bij elke meting; de ADC kiest zelf zijn versterking.
bool RFP602::Activeren() {
  // Bereik 0 = alleen nagaan of de parent een ondersteunde ADC-Extender is: geen I2C, geen meting (R1).
  return parent != nullptr && parent->StartExtenderReadAnalog(static_cast<uint8_t>(sensorPin[0]), 0) == 1;
}

// Runtime-herstel (D042): de override-drempels per set (alleen gecompileerd als de define bestaat).
#if defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsRFP602set1 = RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1;
static_assert(DrempelsFoutniveauGeldig(drempelsRFP602set1), "RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsRFP602set2 = RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2;
static_assert(DrempelsFoutniveauGeldig(drempelsRFP602set2), "RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if ((defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)) || (defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && (defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2))))
#if defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsRFP602channel1set1 = RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1;
static_assert(DrempelsFoutniveauGeldig(drempelsRFP602channel1set1), "RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsRFP602channel2set1 = RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1;
static_assert(DrempelsFoutniveauGeldig(drempelsRFP602channel2set1), "RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsRFP602channel3set1 = RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1;
static_assert(DrempelsFoutniveauGeldig(drempelsRFP602channel3set1), "RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsRFP602channel4set1 = RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1;
static_assert(DrempelsFoutniveauGeldig(drempelsRFP602channel4set1), "RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
#if defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsRFP602channel1set2 = RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2;
static_assert(DrempelsFoutniveauGeldig(drempelsRFP602channel1set2), "RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsRFP602channel2set2 = RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2;
static_assert(DrempelsFoutniveauGeldig(drempelsRFP602channel2set2), "RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsRFP602channel3set2 = RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2;
static_assert(DrempelsFoutniveauGeldig(drempelsRFP602channel3set2), "RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsRFP602channel4set2 = RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2;
static_assert(DrempelsFoutniveauGeldig(drempelsRFP602channel4set2), "RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#endif
// Volgorde per kanaal: eigen kanaal, dan de sensor, dan de ADC-Extender (exemplaar, type), dan algemeen.
// nullptr = geen kanaal- en geen sensor-override: de drempels van de ADC-Extender, bepaald bij de meting.
#if defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
  #define RFP602_DREMPELS_ZONDER_KANAAL &drempelsRFP602set1
#else
  #define RFP602_DREMPELS_ZONDER_KANAAL nullptr
#endif
static const DrempelsFoutniveauTijdensRuntime* const drempelsRFP602perChannelSet1[4] = {
#if defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
  &drempelsRFP602channel1set1,
#else
  RFP602_DREMPELS_ZONDER_KANAAL,
#endif
#if defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
  &drempelsRFP602channel2set1,
#else
  RFP602_DREMPELS_ZONDER_KANAAL,
#endif
#if defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
  &drempelsRFP602channel3set1,
#else
  RFP602_DREMPELS_ZONDER_KANAAL,
#endif
#if defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
  &drempelsRFP602channel4set1
#else
  RFP602_DREMPELS_ZONDER_KANAAL
#endif
};
#undef RFP602_DREMPELS_ZONDER_KANAAL
#if defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
#if defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
  #define RFP602_DREMPELS_ZONDER_KANAAL &drempelsRFP602set2
#else
  #define RFP602_DREMPELS_ZONDER_KANAAL nullptr
#endif
static const DrempelsFoutniveauTijdensRuntime* const drempelsRFP602perChannelSet2[4] = {
#if defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
  &drempelsRFP602channel1set2,
#else
  RFP602_DREMPELS_ZONDER_KANAAL,
#endif
#if defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
  &drempelsRFP602channel2set2,
#else
  RFP602_DREMPELS_ZONDER_KANAAL,
#endif
#if defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
  &drempelsRFP602channel3set2,
#else
  RFP602_DREMPELS_ZONDER_KANAAL,
#endif
#if defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
  &drempelsRFP602channel4set2
#else
  RFP602_DREMPELS_ZONDER_KANAAL
#endif
};
#undef RFP602_DREMPELS_ZONDER_KANAAL
#endif
#endif

// Na elke meting het ene resultaat op één effectieve ladder (D042): kanaal-override, anders Sensor-override,
// anders de ladder van de ADC-Extender zelf (exemplaar, type, algemeen). Nooit op twee ladders tegelijk. 1b: de drempels van de eigen set.
void RFP602::MetingTijdensRuntimeVerwerken(int sensorPin, bool gelukt) {
  // Runtime-herstel (D042) bestaat alleen voor I2C: een ADC-Extender op SPI heeft geen ladder.
  HardwareResourceTypeI2C* ads1115 = (parent != nullptr && parent->gedeeldeIdentiteiten != nullptr && parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA)
      ? static_cast<HardwareResourceTypeI2C*>(parent) : nullptr;
  if (ads1115 == nullptr) return;
#if ((defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)) || (defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && (defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2))))
#if defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
  for (uint8_t kanaal = 0; kanaal < (set == 2 ? RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 : RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1); kanaal++) {
    if (this->sensorPin[kanaal] == sensorPin) {
      if ((set == 2 ? drempelsRFP602perChannelSet2 : drempelsRFP602perChannelSet1)[kanaal] != nullptr) {
        MetingSensorTijdensRuntimeVerwerken(toestandTijdensRuntimePerChannel[kanaal], gelukt, *(set == 2 ? drempelsRFP602perChannelSet2 : drempelsRFP602perChannelSet1)[kanaal], ads1115, kanaal + 1);
        return;
      }
      break;
    }
  }
#else
  for (uint8_t kanaal = 0; kanaal < RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1; kanaal++) {
    if (this->sensorPin[kanaal] == sensorPin) {
      if (drempelsRFP602perChannelSet1[kanaal] != nullptr) {
        MetingSensorTijdensRuntimeVerwerken(toestandTijdensRuntimePerChannel[kanaal], gelukt, *drempelsRFP602perChannelSet1[kanaal], ads1115, kanaal + 1);
        return;
      }
      break;
    }
  }
#endif
  ads1115->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
#elif (defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || (defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)))
  (void)sensorPin;
#if defined(RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
  if (set == 2) { MetingSensorTijdensRuntimeVerwerken(toestandTijdensRuntime, gelukt, drempelsRFP602set2, ads1115, 0); return; }
#endif
#if defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
  if (set != 2) { MetingSensorTijdensRuntimeVerwerken(toestandTijdensRuntime, gelukt, drempelsRFP602set1, ads1115, 0); return; }
#endif
  ads1115->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);   // deze set heeft geen eigen sensor-override
#else
  (void)sensorPin;
  ads1115->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
#endif
}

// Het herstelpad van deze sensor, gedeeld door RawAnalogRead() en StartAnalogRead() (D042: elke sensor die tijdens runtime
// uitgeschakeld kan worden, heeft zo'n pad nodig). true = actief, eventueel na een nieuwe aanmelding.
bool RFP602::OpnieuwAanmeldenNaPauze() {
  if (actief) return true;
  const unsigned long nu = millis();
  if (nu - laatstePoging < SENSOR_HERAANMELDEN_NA_PAUZE_MS) return false;
  laatstePoging = nu;

  if (!aanmelden() || !controleren() || !inpluggen() || !activeren()) {
    if (!heraangemeldConflictGetoond) GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(this);
    else GedeeldeBusVerwijderTijdelijkeConflicten();
    heraangemeldConflictGetoond = true;
    afmelden();
    return false;
  }
  heraangemeldConflictGetoond = false;
  return true;
}

int RFP602::RawAnalogRead(int sensorPin) {
  if (parent == nullptr) return 0;
  HardwareResourceTypeI2C* i2c = (parent != nullptr && parent->gedeeldeIdentiteiten != nullptr && parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA)
      ? static_cast<HardwareResourceTypeI2C*>(parent) : nullptr;
  // Runtime-herstel (D042): ADC uitgeschakeld en nog in de pauze: niet meten. Na herstel heractiveert de ADC deze sensor zelf.
  // Nog niet ingeplugd (bv. na een mislukte aanmelding): het herstelpad hieronder regelt de levenscyclus.
  if (i2c != nullptr && i2c->ingeplugd && !i2c->ComponentBeschikbaarTijdensRuntime()) return 0;
  if (!OpnieuwAanmeldenNaPauze()) return 0;

  // Blokkerend: starten, wachten (met dezelfde grens als ADS1X15::readADC(), maximaal 138 ms), waarde ophalen.
  // De 150 ms gelden voor de volledige aanvraag: wachten op een vrije ADC en wachten op de eigen conversie samen.
  // R1: de vier resultaten van Start uitdrukkelijk behandelen, nooit als bool.
  const unsigned long start = millis();
  int8_t gestart = parent->StartExtenderReadAnalog(static_cast<uint8_t>(sensorPin), 6144);
  while (gestart == 0 && millis() - start <= 150) { yield(); gestart = parent->StartExtenderReadAnalog(static_cast<uint8_t>(sensorPin), 6144); }
  if (gestart == 0 || gestart == -2) return 0;   // bezet of niet ondersteund: geen meting, geen fout voor D042
  bool gelukt = false;
  int32_t waarde = 0;
  if (gestart == 1) {
    bool klaar = parent->IsExtenderReadAnalogReady();
    while (!klaar && millis() - start <= 150) { yield(); klaar = parent->IsExtenderReadAnalogReady(); }
    // R2: Get altijd aanroepen, ook na een time-out: dat geeft de ADC vrij.
    const bool opgehaald = parent->GetExtenderReadAnalogValue(waarde);
    gelukt = klaar && opgehaald;
  }
  const int ruweData = static_cast<int>(waarde);
  MetingTijdensRuntimeVerwerken(sensorPin, gelukt);   // gestart == -1 of time-out: hardwarefout
  if (!gelukt) return 0;

#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  // lastValue en QueueOrStack van het channel van deze sensor: sensorNummer zoeken in sensorPin[].
  for (uint8_t sensorNummer = 1; sensorNummer <= numberOfChannels; sensorNummer++) {   // 1b: aantal van de eigen set
    if (this->sensorPin[sensorNummer - 1] == sensorPin) { WriteQueueOrStack(sensorNummer, ruweData); break; }
  }
#endif
  return ruweData;
}

// Niet-blokkerend: StartAnalogRead(), IsAnalogReadKlaar() en GetAnalogRead() samen zijn één meting = één resultaat (D042).
bool RFP602::StartAnalogRead(int sensorPin) {
  if (parent == nullptr) return false;
  HardwareResourceTypeI2C* i2c = (parent != nullptr && parent->gedeeldeIdentiteiten != nullptr && parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA)
      ? static_cast<HardwareResourceTypeI2C*>(parent) : nullptr;
  if (i2c != nullptr && i2c->ingeplugd && !i2c->ComponentBeschikbaarTijdensRuntime()) return false;
  if (!OpnieuwAanmeldenNaPauze()) return false;
  // R1: de vier resultaten van Start uitdrukkelijk behandelen, nooit als bool.
  const int8_t gestart = parent->StartExtenderReadAnalog(static_cast<uint8_t>(sensorPin), 6144);
  if (gestart == 1) {
    laatstePin = sensorPin;   // alleen een echt gestarte meting onthouden
    return true;              // contract (R2, R3): deze meting afsluiten met GetAnalogRead()
  }
  if (gestart == -1) {
    // Hardwarefout: de meting eindigt hier, meteen het ene resultaat doorgeven.
    MetingTijdensRuntimeVerwerken(sensorPin, false);
  }
  return false;   // 0 = bezet, -2 = niet ondersteund: geen fout voor D042
}

bool RFP602::IsAnalogReadKlaar() {
  // Bij een fout geeft de ADC-Extender true (niet blijven wachten); GetAnalogRead() geeft dan 0 en geeft het ene resultaat door.
  if (parent == nullptr) return true;
  return parent->IsExtenderReadAnalogReady();
}

int RFP602::GetAnalogRead() {
  if (parent == nullptr) { laatsteWaarde = 0; return 0; }
  int32_t waarde = 0;
  const bool gelukt = parent->GetExtenderReadAnalogValue(waarde);
  laatsteWaarde = static_cast<int>(waarde);
  // Het ene resultaat van deze meting.
  MetingTijdensRuntimeVerwerken(laatstePin, gelukt);
  if (!gelukt) { laatsteWaarde = 0; return 0; }

#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  // lastValue en QueueOrStack van het channel van deze sensor: sensorNummer zoeken in sensorPin[].
  for (uint8_t sensorNummer = 1; sensorNummer <= numberOfChannels; sensorNummer++) {   // 1b: aantal van de eigen set
    if (sensorPin[sensorNummer - 1] == laatstePin) { WriteQueueOrStack(sensorNummer, laatsteWaarde); break; }
  }
#endif
  return laatsteWaarde;
}
