#include "BeatPeakDetector.h"

bool BeatPeakDetector::AddSample(float sampleValue, unsigned long sampleTimeMS, bool& beatDetected) {
  beatDetected = false;
  if (isnan(sampleValue)) return false;
  // Basislijn: voortschrijdend gemiddelde met factor 0,02 (ongeveer de laatste 50 samples).
  baselineValue = isnan(baselineValue) ? sampleValue : baselineValue + (sampleValue - baselineValue) * 0.02f;
  const float deviationFromBaseline = sampleValue - baselineValue;
  adaptiveThreshold = deviationFromBaseline > adaptiveThreshold ? deviationFromBaseline : adaptiveThreshold * 0.995f;
  // Lokaal maximum: het vorige sample lag hoger dan dit sample en boven 60 % van de drempel.
  if (previousSampleValue > 0 && previousSampleValue > 0.6f * adaptiveThreshold && deviationFromBaseline < previousSampleValue &&
      (lastBeatTimeMS == 0 || sampleTimeMS - lastBeatTimeMS >= refractoryPeriodMS)) {
    beatDetected = true;
    lastBeatTimeMS = sampleTimeMS;
  }
  previousSampleValue = deviationFromBaseline;
  return true;
}
