#include "EEGFrequencyBands.h"

bool EEGfrequencyBands::AddSample(float sampleValue) {
  if (isnan(sampleValue)) return false;
  // Boven 256 Hz: eerst sampleRateHz / 256 samples middelen tot één sample.
  if (sampleRateHz > 256) {
    decimationSum += sampleValue;
    ++decimationCount;
    if (decimationCount < sampleRateHz / 256) return true;
    sampleValue = decimationSum / decimationCount;
    decimationSum = 0;
    decimationCount = 0;
  }
  sampleWindow[sampleCount % 256] = sampleValue;
  ++sampleCount;
  if (sampleCount >= 512) sampleCount = 256 + sampleCount % 256;   // dezelfde positie in de ring, zonder overloop van de teller
  return true;
}

bool EEGfrequencyBands::ReadBandPowers(float& delta, float& theta, float& alpha, float& beta, float& gamma) {
  delta = theta = alpha = beta = gamma = NAN;
  if (sampleCount < 256 || sampleRateHz == 0) return false;
  // Gemiddelde (DC) eerst aftrekken, anders lekt het in de laagste bins.
  float meanValue = 0;
  for (uint16_t sampleIndex = 0; sampleIndex < 256; ++sampleIndex) meanValue += sampleWindow[sampleIndex];
  meanValue /= 256.0f;
  delta = theta = alpha = beta = gamma = 0;
  // De volgorde in de tijd begint bij de oudste sample (positie sampleCount % 256).
  // Frequentie van het venster: sampleRateHz, of 256 Hz als er gemiddeld werd.
  for (uint8_t frequencyBin = 1; frequencyBin < 128; ++frequencyBin) {
    const float binFrequencyHz = static_cast<float>(frequencyBin) * (sampleRateHz > 256 ? sampleRateHz / (sampleRateHz / 256) : sampleRateHz) / 256.0f;
    if (binFrequencyHz > 45.0f) break;
    float realPart = 0, imaginaryPart = 0;
    for (uint16_t sampleIndex = 0; sampleIndex < 256; ++sampleIndex) {
      const float angle = 2.0f * PI * frequencyBin * sampleIndex / 256.0f;
      const float centeredSample = sampleWindow[(sampleCount + sampleIndex) % 256] - meanValue;
      realPart += centeredSample * cosf(angle);
      imaginaryPart -= centeredSample * sinf(angle);
    }
    const float binPower = (realPart * realPart + imaginaryPart * imaginaryPart) / (256.0f * 256.0f);
    if (binFrequencyHz < 4.0f) delta += binPower;
    else if (binFrequencyHz < 8.0f) theta += binPower;
    else if (binFrequencyHz < 13.0f) alpha += binPower;
    else if (binFrequencyHz < 30.0f) beta += binPower;
    else gamma += binPower;
  }
  return true;
}
