#include "HeartRateFromBeats.h"

bool HeartRateFromBeats::AddBeat(unsigned long beatTimeMS) {
  if (beatCount > 0) {
    if (beatTimeMS - beatTimesMS[(beatCount - 1) % 8] < 250UL) return false;   // dezelfde slag of storing
    if (beatTimeMS - beatTimesMS[(beatCount - 1) % 8] > 3000UL) beatCount = 0;   // pauze: opnieuw beginnen
  }
  beatTimesMS[beatCount % 8] = beatTimeMS;
  ++beatCount;
  if (beatCount >= 16) beatCount = 8 + beatCount % 8;   // dezelfde positie in de ring, zonder overloop van de teller
  return true;
}

bool HeartRateFromBeats::ReadBeatsPerMinute(float& beatsPerMinute) {
  beatsPerMinute = NAN;
  if (beatCount < 2 || millis() - beatTimesMS[(beatCount - 1) % 8] > 3000UL) return false;
  // Oudste slag in de ring: index 0 zolang de ring niet vol is, anders de positie na de nieuwste.
  beatsPerMinute = 60000.0f * static_cast<float>((beatCount < 8 ? beatCount : 8) - 1) /
                   static_cast<float>(beatTimesMS[(beatCount - 1) % 8] - beatTimesMS[beatCount < 8 ? 0 : beatCount % 8]);
  return true;
}

bool HeartRateFromBeats::ReadLastBeatIntervalMS(uint32_t& beatIntervalMS) {
  beatIntervalMS = 0;
  if (beatCount < 2 || millis() - beatTimesMS[(beatCount - 1) % 8] > 3000UL) return false;
  beatIntervalMS = beatTimesMS[(beatCount - 1) % 8] - beatTimesMS[(beatCount - 2) % 8];
  return true;
}
