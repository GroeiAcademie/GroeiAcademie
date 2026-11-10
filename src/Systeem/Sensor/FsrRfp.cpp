#include "FsrRfp.h"
#if ((defined(FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)) || (defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && (defined(FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)))) || (defined(FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || (defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && defined(FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)))
  #include "../Screen/Screen.h"   // ScreenData in de waarde van de override-defines
#endif

struct FsrRfp* sensorFsrRfpSet1 = nullptr;
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
struct FsrRfp* sensorFsrRfpSet2 = nullptr;   // 1b: set 2
#endif

// V5: FsrRfp meet via de analoge functies van zijn parent en kent het type ADC niet meer.
// Het meetbereik (6144 mV, vroeger ADS1X15_GAIN_6144MV) gaat mee bij elke meting; de ADC kiest zelf zijn versterking.
bool FsrRfp::Activeren() {
 // Bereik 0 = alleen nagaan of de parent een ondersteunde ADC-Extender is: geen I2C, geen meting.
  return parent != nullptr && parent->StartExtenderReadAnalog(static_cast<uint8_t>(sensorPin[0]), 0) == 1;
}

// Runtime-herstel: de override-drempels per set (alleen gecompileerd als de define bestaat).
#if defined(FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsFsrRfpSet1 = FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1;
static_assert(DrempelsFoutniveauGeldig(drempelsFsrRfpSet1), "FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && defined(FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsFsrRfpSet2 = FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2;
static_assert(DrempelsFoutniveauGeldig(drempelsFsrRfpSet2), "FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if ((defined(FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)) || (defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && (defined(FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2))))
#if defined(FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsFsrRfpChannel1Set1 = FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1;
static_assert(DrempelsFoutniveauGeldig(drempelsFsrRfpChannel1Set1), "FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsFsrRfpChannel2Set1 = FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1;
static_assert(DrempelsFoutniveauGeldig(drempelsFsrRfpChannel2Set1), "FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsFsrRfpChannel3Set1 = FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1;
static_assert(DrempelsFoutniveauGeldig(drempelsFsrRfpChannel3Set1), "FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsFsrRfpChannel4Set1 = FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1;
static_assert(DrempelsFoutniveauGeldig(drempelsFsrRfpChannel4Set1), "FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
#if defined(FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsFsrRfpChannel1Set2 = FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2;
static_assert(DrempelsFoutniveauGeldig(drempelsFsrRfpChannel1Set2), "FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsFsrRfpChannel2Set2 = FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2;
static_assert(DrempelsFoutniveauGeldig(drempelsFsrRfpChannel2Set2), "FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsFsrRfpChannel3Set2 = FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2;
static_assert(DrempelsFoutniveauGeldig(drempelsFsrRfpChannel3Set2), "FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsFsrRfpChannel4Set2 = FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2;
static_assert(DrempelsFoutniveauGeldig(drempelsFsrRfpChannel4Set2), "FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#endif
// Volgorde per kanaal: eigen kanaal, dan de sensor, dan de ADC-Extender (exemplaar, type), dan algemeen.
// nullptr = geen kanaal- en geen sensor-override: de drempels van de ADC-Extender, bepaald bij de meting.
#if defined(FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
  #define FSR_RFP_DREMPELS_ZONDER_KANAAL &drempelsFsrRfpSet1
#else
  #define FSR_RFP_DREMPELS_ZONDER_KANAAL nullptr
#endif
static const DrempelsFoutniveauTijdensRuntime* const drempelsFsrRfpPerChannelSet1[4] = {
#if defined(FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
  &drempelsFsrRfpChannel1Set1,
#else
  FSR_RFP_DREMPELS_ZONDER_KANAAL,
#endif
#if defined(FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
  &drempelsFsrRfpChannel2Set1,
#else
  FSR_RFP_DREMPELS_ZONDER_KANAAL,
#endif
#if defined(FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
  &drempelsFsrRfpChannel3Set1,
#else
  FSR_RFP_DREMPELS_ZONDER_KANAAL,
#endif
#if defined(FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
  &drempelsFsrRfpChannel4Set1
#else
  FSR_RFP_DREMPELS_ZONDER_KANAAL
#endif
};
#undef FSR_RFP_DREMPELS_ZONDER_KANAAL
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
#if defined(FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
  #define FSR_RFP_DREMPELS_ZONDER_KANAAL &drempelsFsrRfpSet2
#else
  #define FSR_RFP_DREMPELS_ZONDER_KANAAL nullptr
#endif
static const DrempelsFoutniveauTijdensRuntime* const drempelsFsrRfpPerChannelSet2[4] = {
#if defined(FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
  &drempelsFsrRfpChannel1Set2,
#else
  FSR_RFP_DREMPELS_ZONDER_KANAAL,
#endif
#if defined(FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
  &drempelsFsrRfpChannel2Set2,
#else
  FSR_RFP_DREMPELS_ZONDER_KANAAL,
#endif
#if defined(FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
  &drempelsFsrRfpChannel3Set2,
#else
  FSR_RFP_DREMPELS_ZONDER_KANAAL,
#endif
#if defined(FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
  &drempelsFsrRfpChannel4Set2
#else
  FSR_RFP_DREMPELS_ZONDER_KANAAL
#endif
};
#undef FSR_RFP_DREMPELS_ZONDER_KANAAL
#endif
#endif

// Na elke meting het ene resultaat op één effectieve ladder: kanaal-override, anders Sensor-override,
// anders de ladder van de ADC-Extender zelf (exemplaar, type, algemeen). Nooit op twee ladders tegelijk. 1b: de drempels van de eigen set.
void FsrRfp::MetingTijdensRuntimeVerwerken(int sensorPin, bool gelukt) {
  // Runtime-herstel bestaat alleen voor I2C: een ADC-Extender op SPI heeft geen ladder.
  HardwareResourceTypeI2C* ads1115 = (parent != nullptr && parent->gedeeldeIdentiteiten != nullptr && parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA)
      ? static_cast<HardwareResourceTypeI2C*>(parent) : nullptr;
  if (ads1115 == nullptr) return;
#if ((defined(FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || defined(FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)) || (defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && (defined(FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(FSR_RFP_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(FSR_RFP_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2) || defined(FSR_RFP_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2))))
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
  for (uint8_t kanaal = 0; kanaal < (set == 2 ? FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 : FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1); kanaal++) {
    if (this->sensorPin[kanaal] == sensorPin) {
      if ((set == 2 ? drempelsFsrRfpPerChannelSet2 : drempelsFsrRfpPerChannelSet1)[kanaal] != nullptr) {
        MetingSensorTijdensRuntimeVerwerken(toestandTijdensRuntimePerChannel[kanaal], gelukt, *(set == 2 ? drempelsFsrRfpPerChannelSet2 : drempelsFsrRfpPerChannelSet1)[kanaal], ads1115, kanaal + 1);
        return;
      }
      break;
    }
  }
#else
  for (uint8_t kanaal = 0; kanaal < FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1; kanaal++) {
    if (this->sensorPin[kanaal] == sensorPin) {
      if (drempelsFsrRfpPerChannelSet1[kanaal] != nullptr) {
        MetingSensorTijdensRuntimeVerwerken(toestandTijdensRuntimePerChannel[kanaal], gelukt, *drempelsFsrRfpPerChannelSet1[kanaal], ads1115, kanaal + 1);
        return;
      }
      break;
    }
  }
#endif
  ads1115->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
#elif (defined(FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1) || (defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && defined(FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)))
  (void)sensorPin;
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && defined(FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_2)
  if (set == 2) { MetingSensorTijdensRuntimeVerwerken(toestandTijdensRuntime, gelukt, drempelsFsrRfpSet2, ads1115, 0); return; }
#endif
#if defined(FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1)
  if (set != 2) { MetingSensorTijdensRuntimeVerwerken(toestandTijdensRuntime, gelukt, drempelsFsrRfpSet1, ads1115, 0); return; }
#endif
  ads1115->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);   // deze set heeft geen eigen sensor-override
#else
  (void)sensorPin;
  ads1115->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
#endif
}

// Het herstelpad van deze sensor, gedeeld door RawAnalogRead() en StartAnalogRead() (elke sensor die tijdens runtime
// uitgeschakeld kan worden, heeft zo'n pad nodig). true = actief, eventueel na een nieuwe aanmelding.
bool FsrRfp::OpnieuwAanmeldenNaPauze() {
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

int FsrRfp::RawAnalogRead(int sensorPin) {
  int32_t waarde = 0;
  return RawAnalogRead(sensorPin, waarde) ? static_cast<int>(waarde) : 0;
}

bool FsrRfp::RawAnalogRead(int sensorPin, int32_t& waarde) {
  waarde = 0;
  if (parent == nullptr) return false;
  HardwareResourceTypeI2C* i2c = (parent->gedeeldeIdentiteiten != nullptr && parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA)
      ? static_cast<HardwareResourceTypeI2C*>(parent) : nullptr;
  // Runtime-herstel: ADC uitgeschakeld en nog in de pauze: niet meten. Na herstel heractiveert de ADC deze sensor zelf.
  // Nog niet ingeplugd (bv. na een mislukte aanmelding): het herstelpad hieronder regelt de levenscyclus.
  if (i2c != nullptr && i2c->ingeplugd && !i2c->ComponentBeschikbaarTijdensRuntime()) return false;
  if (!OpnieuwAanmeldenNaPauze()) return false;

  // Blokkerend: starten, wachten (met dezelfde grens als ADS1X15::readADC(), maximaal 138 ms), waarde ophalen.
  // De 150 ms gelden voor de volledige aanvraag: wachten op een vrije ADC en wachten op de eigen conversie samen.
  // de vier resultaten van Start uitdrukkelijk behandelen, nooit als bool.
  const unsigned long start = millis();
  int8_t gestart = parent->StartExtenderReadAnalog(static_cast<uint8_t>(sensorPin), FSR_RFP_ADC_BEREIK_MILLIVOLT);
  while (gestart == 0 && millis() - start <= 150) { yield(); gestart = parent->StartExtenderReadAnalog(static_cast<uint8_t>(sensorPin), FSR_RFP_ADC_BEREIK_MILLIVOLT); }
  if (gestart == 0 || gestart == -2) return false;   // bezet of niet ondersteund: geen meting, geen fout voor D042
  bool gelukt = false;
  if (gestart == 1) {
    bool klaar = parent->IsExtenderReadAnalogReady();
    while (!klaar && millis() - start <= 150) { yield(); klaar = parent->IsExtenderReadAnalogReady(); }
    // Get altijd aanroepen, ook na een time-out: dat geeft de ADC vrij.
    const bool opgehaald = parent->GetExtenderReadAnalogValue(waarde);
    gelukt = klaar && opgehaald;
  }
  const int ruweData = static_cast<int>(waarde);
  MetingTijdensRuntimeVerwerken(sensorPin, gelukt);   // gestart == -1 of time-out: hardwarefout
  if (!gelukt) { waarde = 0; return false; }

#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  // lastValue en QueueOrStack van het channel van deze sensor: sensorNummer zoeken in sensorPin[].
  for (uint8_t sensorNummer = 1; sensorNummer <= numberOfChannels; sensorNummer++) {   // 1b: aantal van de eigen set
    if (this->sensorPin[sensorNummer - 1] == sensorPin) { WriteQueueOrStack(sensorNummer, ruweData); break; }
  }
#endif
  return true;
}

// Niet-blokkerend: StartAnalogRead(), IsAnalogReadKlaar() en GetAnalogRead() samen zijn één meting = één resultaat.
bool FsrRfp::StartAnalogRead(int sensorPin) {
  if (parent == nullptr) return false;
  HardwareResourceTypeI2C* i2c = (parent->gedeeldeIdentiteiten != nullptr && parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA)
      ? static_cast<HardwareResourceTypeI2C*>(parent) : nullptr;
  if (i2c != nullptr && i2c->ingeplugd && !i2c->ComponentBeschikbaarTijdensRuntime()) return false;
  if (!OpnieuwAanmeldenNaPauze()) return false;
  // de vier resultaten van Start uitdrukkelijk behandelen, nooit als bool.
  const int8_t gestart = parent->StartExtenderReadAnalog(static_cast<uint8_t>(sensorPin), FSR_RFP_ADC_BEREIK_MILLIVOLT);
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

bool FsrRfp::IsAnalogReadKlaar() {
  // Bij een fout geeft de ADC-Extender true (niet blijven wachten); GetAnalogRead() geeft dan 0 en geeft het ene resultaat door.
  if (parent == nullptr) return true;
  return parent->IsExtenderReadAnalogReady();
}

int FsrRfp::GetAnalogRead() {
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
