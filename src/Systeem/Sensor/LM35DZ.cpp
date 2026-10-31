#include "LM35DZ.h"
#if SENSOR_LM35DZ_AANTAL > 0

void LM35DZ::MeetwaardeVerwerken(int32_t waarde, bool gelukt) {
  laatsteMetingGeldig = gelukt;
  laatsteEffectiefBereikMillivolt = gelukt && parent != nullptr ? parent->GetExtenderReadAnalogEffectiefBereikMillivolt() : 0;
  laatsteMeetwaarde = gelukt ? static_cast<int>(waarde) : 0;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) {
    WriteQueueOrStack(1, laatsteMeetwaarde);   // ADC-waarde
    WriteQueueOrStack(2, TemperatuurC());      // afgeleide temperatuur (°C)
  }
#endif
}

float LM35DZ::TemperatuurC() {
  if (!laatsteMetingGeldig || laatsteEffectiefBereikMillivolt == 0) return NAN;
  const int32_t waarde = laatsteMeetwaarde;
  // LM35: 10 mV / graad C. Gebruik de werkelijke ADC-fullscale,
  // niet het gevraagde maximale sensorsignaal (bv. 1500 versus 2048 mV).
  return (static_cast<float>(waarde) * laatsteEffectiefBereikMillivolt / static_cast<float>(ADC_MAX)) / 10.0f;
}

#endif
