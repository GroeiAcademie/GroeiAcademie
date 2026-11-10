#include "BioAmpEXGPill.h"
#if SENSOR_BIOAMPEXGPILL_AANTAL > 0
// Upside Down Labs BioAmp EXG Pill, OUT via ADC
// Bron: https://docs.upsidedownlabs.tech/hardware/bioamp/bioamp-exg-pill/index.html
// Controleert enkel ADC-ondersteuning, NIET aanwezigheid of werking van de analoge sensor.
bool BioAmpEXGPill::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  uint16_t samplingHz = 125;
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EMG
  samplingHz = 500;
#elif SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EOG
  samplingHz = 75;
#elif SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EEG
  samplingHz = 256;
#endif
  const unsigned long nu = micros();
  const unsigned long periode = 1000000UL / samplingHz;
  if (tijdstipLaatsteSample != 0 && nu - tijdstipLaatsteSample < periode) return false;
  // Vast ritme zoals de officiële voorbeelden: het geplande sample valt precies één periode na het vorige geplande sample.
  // Meer dan 10 % te laat (of het eerste sample): opnieuw beginnen met een nieuw ritme en een gewiste filtertoestand,
  // want de officiële filtercoëfficiënten gelden alleen bij een regelmatige samplefrequentie.
  const bool opnieuwBeginnen = tijdstipLaatsteSample == 0 || nu - tijdstipLaatsteSample - periode > periode / 10;
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE != BIOAMPEXGPILL_MEETMETHODE_RAW
  // Drie keer na elkaar te laat: de keten is te traag voor de filters van deze meetmethode.
  if (tijdstipLaatsteSample != 0 && opnieuwBeginnen) { if (aantalTeLateSamplesNaElkaar < 255) ++aantalTeLateSamplesNaElkaar; }
  else if (tijdstipLaatsteSample != 0) aantalTeLateSamplesNaElkaar = 0;
  samplefrequentieNietHaalbaar = aantalTeLateSamplesNaElkaar >= 3;
#endif
  tijdstipLaatsteSample = opnieuwBeginnen ? nu : tijdstipLaatsteSample + periode;
  int32_t raw = 0;
  if (!RawAnalogRead(raw)) return false;
  float resultaat = static_cast<float>(raw);
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE != BIOAMPEXGPILL_MEETMETHODE_RAW
  if (opnieuwBeginnen) memset(filterToestand, 0, sizeof(filterToestand));
  // Vorm: {a1,a2,b0,b1,b2}, biquad/SOS in vier opeenvolgende trappen.
  // Afkomstig uit de officiële corresponderende filtervoorbeelden.
  #if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_ECG
  static const float secties[4][5] = {
    {0.70682283f,0.15621030f,0.28064917f,0.56129834f,0.28064917f},
    {0.95028224f,0.54073140f,1,2,1},
    {-1.95360385f,0.95423412f,1,-2,1},
    {-1.98048558f,0.98111344f,1,-2,1}
  };
  #elif SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EMG
  static const float secties[4][5] = {
    {0.05159732f,0.36347401f,0.01856301f,0.03712602f,0.01856301f},
    {-0.53945795f,0.39764934f,1,-2,1},
    {0.47319594f,0.70744137f,1,2,1},
    {-1.00211112f,0.74520226f,1,-2,1}
  };
  #elif SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EOG
  static const float secties[4][5] = {
    {0.02977423f,0.04296318f,0.09797471f,0.19594942f,0.09797471f},
    {0.08383952f,0.46067709f,1,2,1},
    {-1.92167271f,0.92347975f,1,-2,1},
    {-1.96758891f,0.96933514f,1,-2,1}
  };
  #elif SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EEG
  static const float secties[4][5] = {
    {-0.95391350f,0.25311356f,0.00735282f,0.01470564f,0.00735282f},
    {-1.20596630f,0.60558332f,1,2,1},
    {-1.97690645f,0.97706395f,1,-2,1},
    {-1.99071687f,0.99086813f,1,-2,1}
  };
  #else
  #error Onbekende SENSOR_BIOAMPEXGPILL_MEETMETHODE
  #endif
  for (uint8_t i = 0; i < 4; ++i) {
    float* z = filterToestand[i];
    const float x = resultaat - secties[i][0]*z[0] - secties[i][1]*z[1];
    resultaat = secties[i][2]*x + secties[i][3]*z[0] + secties[i][4]*z[1];
    z[1] = z[0]; z[0] = x;
  }
#endif
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EMG
  // EMG-envelop bijwerken met elk gefilterd sample (opnieuw beginnen: envelop ook opnieuw).
  if (opnieuwBeginnen) { memset(envelopBuffer, 0, sizeof(envelopBuffer)); envelopSom = 0; envelopIndex = 0; }
  const float absoluut = fabsf(resultaat);
  envelopSom += absoluut - envelopBuffer[envelopIndex];
  envelopBuffer[envelopIndex] = absoluut;
  envelopIndex = static_cast<uint16_t>((envelopIndex + 1) % SENSOR_BIOAMPEXGPILL_ENVELOP_VENSTER);
  laatsteEnvelop = (envelopSom / SENSOR_BIOAMPEXGPILL_ENVELOP_VENSTER) * 2.0f;
#endif
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_ECG || SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EOG
  // Piekdetectie met elk gefilterd sample (opnieuw beginnen: venster ook opnieuw).
  if (opnieuwBeginnen) {
    memset(piekData, 0, sizeof(piekData)); memset(piekGemiddelde, 0, sizeof(piekGemiddelde)); memset(piekAfwijking, 0, sizeof(piekAfwijking));
    piekIndex = 0;
  }
  if (resultaat - piekGemiddelde[piekIndex] > piekFactor * piekAfwijking[piekIndex]) {
    piekData[piekIndex] = resultaat + piekData[piekIndex];
    laatstePiek = true;
  } else {
    piekData[piekIndex] = resultaat;
    laatstePiek = false;
  }
  float som = 0;
  for (uint8_t n = 0; n < piekVenster; ++n) som += piekData[n];
  const float gemiddelde = som / piekVenster;
  float kwadraten = 0;
  for (uint8_t n = 0; n < piekVenster; ++n) kwadraten += (piekData[n] - gemiddelde) * (piekData[n] - gemiddelde);
  piekGemiddelde[piekIndex] = gemiddelde;
  piekAfwijking[piekIndex] = sqrtf(kwadraten / piekVenster);
  piekIndex = static_cast<uint8_t>((piekIndex + 1) % piekVenster);
  piekBekend = true;
#endif
  if (samplefrequentieNietHaalbaar) return false;
  waarde = resultaat;
  return true;
}
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EMG
// Laatste EMG-envelop; wordt bijgewerkt door LeesHoofdwaarde().
bool BioAmpEXGPill::LeesEMGEnvelop(float& waarde) {
  waarde = laatsteEnvelop;
  return actief && !samplefrequentieNietHaalbaar && !isnan(waarde);
}
#endif
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_ECG
// Piek bij het laatste ECG-sample; wordt bijgewerkt door LeesHoofdwaarde().
bool BioAmpEXGPill::LeesHartslagPiek(bool& piek) {
  piek = laatstePiek;
  return actief && !samplefrequentieNietHaalbaar && piekBekend;
}
#endif
#if SENSOR_BIOAMPEXGPILL_MEETMETHODE == BIOAMPEXGPILL_MEETMETHODE_EOG
// Knipper (piek) bij het laatste EOG-sample, volgens EyeBlinkDetection ("Experimental!"); wordt bijgewerkt door LeesHoofdwaarde().
bool BioAmpEXGPill::LeesKnipperen(bool& knipper) {
  knipper = laatstePiek;
  return actief && !samplefrequentieNietHaalbaar && piekBekend;
}
#endif
#endif
