#include "RFP602.h"

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
  const int ruweData = ADC_ADS1115.readADC(sensorPin);
#else
  const int ruweData = analogRead(sensorPin);
#endif

#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data != nullptr) Data->SensorWrite(ruweData);
#endif
  return ruweData;
}
