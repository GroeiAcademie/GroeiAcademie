#include "VoltageDivider.h"

bool VoltageDivider::CalculateSensorResistanceOhm(int32_t rawValue, uint32_t seriesResistorOhm, float& sensorResistanceOhm) {
  sensorResistanceOhm = NAN;
  // 0 of ADC_MAX: kortsluiting of open kring, geen betrouwbare weerstand.
  if (rawValue <= 0 || rawValue >= static_cast<int32_t>(ADC_MAX) || seriesResistorOhm == 0) return false;
  sensorResistanceOhm = static_cast<float>(seriesResistorOhm) * static_cast<float>(rawValue) / static_cast<float>(static_cast<int32_t>(ADC_MAX) - rawValue);
  return true;
}
