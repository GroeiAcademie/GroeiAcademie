#include "HX711.h"
#if SENSOR_HX711_AANTAL > 0
// Official: https://github.com/bogde/HX711/blob/master/src/HX711.h
bool SensorHX711::inpluggen() {
  if (ingeplugd) return true;
  // Deze driver gebruikt native timing/pinnen: geen GPIO- of 1-Wire-Extenderprotocol (uitzondering: twee CD74HC4067).
  if (firstExtenderNode() != nullptr && !ExtenderFysiekGeschikt(firstExtenderNode())) return false;
  if (this->np_DOUT == HardwareResourcePin::NONE || this->np_DOUT == HardwareResourcePin::CUSTOM) return false;
  if (this->np_PD_SCK == HardwareResourcePin::NONE || this->np_PD_SCK == HardwareResourcePin::CUSTOM) return false;
#if GEDEELDE_BUS_MIXED_PINS && EXTENDER_CD74HC4067_AANTAL >= 2
  if (viaMixed) {
    if (extenderHulp_[1] == nullptr || extenderHulp_[1]->component != GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067) return false;
    if (!Sensor::inpluggen()) return false;
    adc.begin(NativeArduinoPinVan(static_cast<ExtenderCD74HC4067*>(parent)->np_Z),
              NativeArduinoPinVan(static_cast<ExtenderCD74HC4067*>(extenderHulp_[1])->np_Z));
    return true;
  }
#endif
  if (!Sensor::inpluggen()) return false;
  adc.begin(NativeArduinoPinVan(this->np_DOUT), NativeArduinoPinVan(this->np_PD_SCK));
  return true;
}
bool SensorHX711::Activeren() {
  if (!parent || !parent->actief || !ingeplugd) return false;
#if GEDEELDE_BUS_MIXED_PINS && EXTENDER_CD74HC4067_AANTAL >= 2
  // De parent (CD74HC4067 #1) koos het DOUT-kanaal; het PD_SCK-kanaal op de tweede CD74HC4067 hier zelf kiezen.
  if (viaMixed && (extenderHulp_[1] == nullptr || !extenderHulp_[1]->actief || !extenderHulp_[1]->SelectAccessForChild(this))) return false;
#endif
  // Na (opnieuw) activeren kan de belasting veranderd zijn: eerst een nieuwe tarra.
  tarraSucceeded = false;
  // Interface voorbereid; fysieke respons wordt later gecontroleerd.
  return true;
}

bool SensorHX711::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!actief || !adc.is_ready()) return false;
#if GEDEELDE_BUS_MIXED_PINS && EXTENDER_CD74HC4067_AANTAL >= 2
  // Mixed: PD_SCK loopt via de tweede CD74HC4067; is die niet actief, dan geen meting.
  if (viaMixed && (extenderHulp_[1] == nullptr || !extenderHulp_[1]->actief)) return false;
#endif
  waarde = static_cast<float>(adc.read()); // ruwe brugwaarde, niet gekalibreerd
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(1, waarde);
#endif
  return true;
}
bool SensorHX711::Tare() {
  if (!actief) return false;
#if GEDEELDE_BUS_MIXED_PINS && EXTENDER_CD74HC4067_AANTAL >= 2
  // Mixed: PD_SCK loopt via de tweede CD74HC4067; is die niet actief, dan geen meting.
  if (viaMixed && (extenderHulp_[1] == nullptr || !extenderHulp_[1]->actief)) return false;
#endif
  long sumOfReadings = 0;
  for (uint8_t sampleIndex = 0; sampleIndex < SENSOR_HX711_TARE_SAMPLE_COUNT; sampleIndex++) {
    // Elke meting wacht hoogstens 1 s, zodat een ontbrekende module de sketch niet blokkeert.
    if (!adc.wait_ready_timeout(1000)) return false;
    sumOfReadings += adc.read();
  }
  tareOffset = sumOfReadings / SENSOR_HX711_TARE_SAMPLE_COUNT;
  tarraSucceeded = true;
  return true;
}

bool SensorHX711::ReadWeightGram(float& waarde) {
  waarde = NAN;
  if (!actief || !tarraSucceeded || !adc.is_ready() || SENSOR_HX711_CALIBRATION_FACTOR == 0) return false;
#if GEDEELDE_BUS_MIXED_PINS && EXTENDER_CD74HC4067_AANTAL >= 2
  // Mixed: PD_SCK loopt via de tweede CD74HC4067; is die niet actief, dan geen meting.
  if (viaMixed && (extenderHulp_[1] == nullptr || !extenderHulp_[1]->actief)) return false;
#endif
  waarde = static_cast<float>(adc.read() - tareOffset) / SENSOR_HX711_CALIBRATION_FACTOR;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(2, waarde);   // gewicht (gram)
#endif
  return true;
}
#endif
