#include "RFP602.h"

struct RFP602* sensorRFP602 = nullptr;

RFP602::RFP602()
#if ADC_BACKEND == ADC_BACKEND_NATIVE
  : RFP602(&ADC_NATIVE, GedeeldeBusComponent::RFP602, HardwareResourceToegang::GEDEELD)
#elif ADC_BACKEND == ADC_BACKEND_ADS1115
  : RFP602(&ADC_ADS1115, GedeeldeBusComponent::RFP602, HardwareResourceToegang::GEDEELD)
#endif
{}

bool RFP602::Activeren() {
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  ADC_ADS1115.setGain(GAIN_TWOTHIRDS);
#endif
  return true;
}

int RFP602::RawAnalogRead(int sensorPin) {
  if (!actief) {
    const unsigned long nu = millis();
    if (nu - laatstePoging < SENSOR_HERAANMELDEN_NA_PAUZE_MS) return 0;
    laatstePoging = nu;

    if (!aanmelden() || !controleren() || !inpluggen() || !activeren()) {
      afmelden();
      return 0;
    }
  }

#if ADC_BACKEND == ADC_BACKEND_ADS1115
  return ADC_ADS1115.readADC(sensorPin);
#else
  return analogRead(sensorPin);
#endif
}
