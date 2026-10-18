#include "RFP602.h"
#if ADC_BACKEND == ADC_BACKEND_ADS1115 && (defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU) || defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU) || defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU) || defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU)) || ADC_BACKEND == ADC_BACKEND_ADS1115 && defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU)
  #include "../Screen/Screen.h"   // ScreenData in de waarde van de override-defines
#endif

struct RFP602* sensorRFP602 = nullptr;

RFP602::RFP602()
#if DATA_LAYER_CONFIG == DATA_TYPE_NONE
  #if ADC_BACKEND == ADC_BACKEND_NATIVE
    : RFP602(&ADC_NATIVE, GedeeldeBusComponent::RFP602, HardwareResourceToegang::GEDEELD)
  #elif ADC_BACKEND == ADC_BACKEND_ADS1115
    : RFP602(&ADC_ADS1115, GedeeldeBusComponent::RFP602, HardwareResourceToegang::GEDEELD)
  #endif
#else
  : RFP602(::Data)
#endif
{}

#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
RFP602::RFP602(struct Data* Data)
#if ADC_BACKEND == ADC_BACKEND_NATIVE
  : RFP602(&ADC_NATIVE, GedeeldeBusComponent::RFP602, Data, HardwareResourceToegang::GEDEELD)
#elif ADC_BACKEND == ADC_BACKEND_ADS1115
  : RFP602(&ADC_ADS1115, GedeeldeBusComponent::RFP602, Data, HardwareResourceToegang::GEDEELD)
#endif
{}
#endif

bool RFP602::Activeren() {
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  ADC_ADS1115.setGain(ADS1X15_GAIN_6144MV);
#endif
  return true;
}

#if ADC_BACKEND == ADC_BACKEND_ADS1115
// Runtime-herstel (D042): de override-drempels van deze sensor (alleen gecompileerd als de define bestaat).
#if defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsRFP602 = RFP602_RUNTIME_DREMPELS_FOUTNIVEAU;
static_assert(DrempelsFoutniveauGeldig(drempelsRFP602), "RFP602_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if ADC_BACKEND == ADC_BACKEND_ADS1115 && (defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU) || defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU) || defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU) || defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU))
#if defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsRFP602kanaal1 = RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU;
static_assert(DrempelsFoutniveauGeldig(drempelsRFP602kanaal1), "RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsRFP602kanaal2 = RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU;
static_assert(DrempelsFoutniveauGeldig(drempelsRFP602kanaal2), "RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsRFP602kanaal3 = RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU;
static_assert(DrempelsFoutniveauGeldig(drempelsRFP602kanaal3), "RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
#if defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU)
static constexpr DrempelsFoutniveauTijdensRuntime drempelsRFP602kanaal4 = RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU;
static_assert(DrempelsFoutniveauGeldig(drempelsRFP602kanaal4), "RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
#endif
// Volgorde per kanaal: eigen kanaal, dan de sensor, dan de ADS1115 (exemplaar, type), dan algemeen.
// nullptr = geen kanaal- en geen sensor-override: de drempels van de ADS1115, bepaald bij de meting.
#if defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU)
  #define RFP602_DREMPELS_ZONDER_KANAAL &drempelsRFP602
#else
  #define RFP602_DREMPELS_ZONDER_KANAAL nullptr
#endif
static const DrempelsFoutniveauTijdensRuntime* const drempelsRFP602PerKanaal[4] = {
#if defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU)
  &drempelsRFP602kanaal1,
#else
  RFP602_DREMPELS_ZONDER_KANAAL,
#endif
#if defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU)
  &drempelsRFP602kanaal2,
#else
  RFP602_DREMPELS_ZONDER_KANAAL,
#endif
#if defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU)
  &drempelsRFP602kanaal3,
#else
  RFP602_DREMPELS_ZONDER_KANAAL,
#endif
#if defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU)
  &drempelsRFP602kanaal4
#else
  RFP602_DREMPELS_ZONDER_KANAAL
#endif
};
#undef RFP602_DREMPELS_ZONDER_KANAAL
#endif

// Na elke meting het ene resultaat op één effectieve ladder (D042): kanaal-override, anders Sensor-override,
// anders de ladder van de ADS1115 zelf (exemplaar, type, algemeen). Nooit op twee ladders tegelijk.
void RFP602::MetingTijdensRuntimeVerwerken(int sensorPin, bool gelukt) {
#if ADC_BACKEND == ADC_BACKEND_ADS1115 && (defined(RFP602_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU) || defined(RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU) || defined(RFP602_CHANNEL_3_RUNTIME_DREMPELS_FOUTNIVEAU) || defined(RFP602_CHANNEL_4_RUNTIME_DREMPELS_FOUTNIVEAU))
  for (uint8_t kanaal = 0; kanaal < AANTAL_SENSOREN_AANWEZIG; kanaal++) {
    if (this->sensorPin[kanaal] == sensorPin) {
      if (drempelsRFP602PerKanaal[kanaal] != nullptr) {
        MetingSensorTijdensRuntimeVerwerken(toestandTijdensRuntimePerKanaal[kanaal], gelukt, *drempelsRFP602PerKanaal[kanaal], &ADC_ADS1115, kanaal + 1);
        return;
      }
      break;
    }
  }
  ADC_ADS1115.ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
#elif ADC_BACKEND == ADC_BACKEND_ADS1115 && defined(RFP602_RUNTIME_DREMPELS_FOUTNIVEAU)
  (void)sensorPin;
  MetingSensorTijdensRuntimeVerwerken(toestandTijdensRuntime, gelukt, drempelsRFP602, &ADC_ADS1115, 0);
#else
  (void)sensorPin;
  ADC_ADS1115.ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
#endif
}
#endif

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
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  // Runtime-herstel (D042): ADS1115 uitgeschakeld en nog in de pauze: niet meten. Na herstel heractiveert de ADS1115 deze sensor zelf.
  // Nog niet ingeplugd (bv. na een mislukte aanmelding): het herstelpad hieronder regelt de levenscyclus.
  if (ADC_ADS1115.ingeplugd && !ADC_ADS1115.ComponentBeschikbaarTijdensRuntime()) return 0;
#endif
  if (!OpnieuwAanmeldenNaPauze()) return 0;

#if ADC_BACKEND == ADC_BACKEND_ADS1115
  const int ruweData = ADC_ADS1115.readADC(sensorPin);
  const bool gelukt = ADC_ADS1115.getError() == ADS1X15_OK;
  MetingTijdensRuntimeVerwerken(sensorPin, gelukt);
  if (!gelukt) return 0;
#else
  const int ruweData = analogRead(sensorPin);
#endif

#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  // lastValue en QueueOrStack van het channel van deze sensor: sensorNummer zoeken in sensorPin[].
  for (uint8_t sensorNummer = 1; sensorNummer <= AANTAL_SENSOREN_AANWEZIG; sensorNummer++) {
    if (this->sensorPin[sensorNummer - 1] == sensorPin) { WriteQueueOrStack(sensorNummer, ruweData); break; }
  }
#endif
  return ruweData;
}

// Niet-blokkerend: StartAnalogRead(), IsAnalogReadKlaar() en GetAnalogRead() samen zijn één meting = één resultaat (D042).
bool RFP602::StartAnalogRead(int sensorPin) {
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  if (ADC_ADS1115.ingeplugd && !ADC_ADS1115.ComponentBeschikbaarTijdensRuntime()) return false;
#endif
  if (!OpnieuwAanmeldenNaPauze()) return false;
  laatstePin = sensorPin;
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  ADC_ADS1115.requestADC(sensorPin);
  nietBlokkerendeMetingMislukt = false;
  if (ADC_ADS1115.getError() != ADS1X15_OK) {
    // De meting eindigt hier: meteen het ene resultaat doorgeven.
    MetingTijdensRuntimeVerwerken(sensorPin, false);
    return false;
  }
#else
  laatsteWaarde = analogRead(sensorPin);
#endif
  return true;
}

bool RFP602::IsAnalogReadKlaar() {
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  // Bij een fout: niet blijven wachten (true). GetAnalogRead() geeft dan 0 en geeft het ene resultaat door.
  if (nietBlokkerendeMetingMislukt) return true;
  const bool klaar = ADC_ADS1115.isReady();
  if (ADC_ADS1115.getError() != ADS1X15_OK) { nietBlokkerendeMetingMislukt = true; return true; }
  return klaar;
#else
  return true;
#endif
}

int RFP602::GetAnalogRead() {
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  bool gelukt = !nietBlokkerendeMetingMislukt;
  nietBlokkerendeMetingMislukt = false;
  if (gelukt) {
    laatsteWaarde = ADC_ADS1115.getValue();
    gelukt = ADC_ADS1115.getError() == ADS1X15_OK;
  }
  // Het ene resultaat van deze meting.
  MetingTijdensRuntimeVerwerken(laatstePin, gelukt);
  if (!gelukt) { laatsteWaarde = 0; return 0; }
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  // lastValue en QueueOrStack van het channel van deze sensor: sensorNummer zoeken in sensorPin[].
  for (uint8_t sensorNummer = 1; sensorNummer <= AANTAL_SENSOREN_AANWEZIG; sensorNummer++) {
    if (sensorPin[sensorNummer - 1] == laatstePin) { WriteQueueOrStack(sensorNummer, laatsteWaarde); break; }
  }
#endif
  return laatsteWaarde;
}
