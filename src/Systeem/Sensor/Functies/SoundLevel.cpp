#include "SoundLevel.h"

bool SoundLevel::AddSample(int32_t sampleValue, uint32_t maximumSampleGapMS) {
  // Een onderbreking langer dan maximumSampleGapMS: eerst een nieuw venster, zodat oude en nieuwe samples niet mengen.
  if (sampleCount > 0 && millis() - lastSampleMS > maximumSampleGapMS) ResetWindow();
  runningMean = isnan(runningMean) ? static_cast<float>(sampleValue) : runningMean + (static_cast<float>(sampleValue) - runningMean) / 64.0f;
  sumOfSquares += (static_cast<float>(sampleValue) - runningMean) * (static_cast<float>(sampleValue) - runningMean);
  if (sampleValue < minimumSample) minimumSample = sampleValue;
  if (sampleValue > maximumSample) maximumSample = sampleValue;
  ++sampleCount;
  lastSampleMS = millis();
  return true;
}

bool SoundLevel::ReadRootMeanSquare(float& rootMeanSquare) {
  rootMeanSquare = NAN;
  if (sampleCount == 0 || millis() - lastSampleMS > 1000UL) return false;
  rootMeanSquare = sqrtf(sumOfSquares / static_cast<float>(sampleCount));
  return true;
}

bool SoundLevel::ReadPeakToPeak(int32_t& peakToPeak) {
  peakToPeak = 0;
  if (sampleCount == 0 || millis() - lastSampleMS > 1000UL) return false;
  peakToPeak = maximumSample - minimumSample;
  return true;
}

void SoundLevel::ResetWindow() {
  sumOfSquares = 0;
  minimumSample = INT32_MAX;
  maximumSample = INT32_MIN;
  sampleCount = 0;
}
