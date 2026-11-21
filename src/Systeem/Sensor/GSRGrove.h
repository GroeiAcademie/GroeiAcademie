#ifndef SENSOR_GSRGrove_H
#define SENSOR_GSRGrove_H

#include <Arduino.h>
#include "SensorAnaloog.h"
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_GSRGROVE_AANTAL > 0

// GSRGrove: Grove GSR analoge uitgang, uitsluitend ADC-waarde.
// Eén component voor één fysiek apparaat, via de bestaande ADC-Extender.
// De meetcode zit in SensorAnaloog; deze klasse geeft alleen de pin en het bereik door.
struct GSRGrove : SensorAnaloog {
  GSRGrove(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin,
      HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : SensorAnaloog(parent, component, sensorPin, SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT, Extender) {}
  uint8_t AantalChannels() const override { return 3; }   // meetkanalen voor Data: 1 ruwe ADC-waarde, 2 huidgeleiding, 3 huidweerstand
  // Huidweerstand volgens de Seeed-formule: ((1024 + 2 x meting) x 10000) / (kalibratie - meting), met 10-bit metingen bij de voedingsspanning.
  // De ADC-waarde wordt eerst herschaald naar die 10-bit schaal (SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT = voedingsspanning van de module).
  // false: geen meting, of meting niet onder SENSOR_GSRGROVE_CALIBRATION_VALUE (geen huidcontact of verkeerde kalibratie).
  bool ReadSkinResistanceOhm(float& waarde) {
    waarde = NAN;
    int32_t rawValue = 0;
    if (!actief || parent == nullptr || !RawAnalogRead(rawValue)) return false;
    const float serialPortReading = static_cast<float>(rawValue) * parent->GetExtenderReadAnalogEffectiefBereikMillivolt() / bereikMillivolt * 1023.0f / static_cast<float>(ADC_MAX);
    if (serialPortReading >= SENSOR_GSRGROVE_CALIBRATION_VALUE) return false;
    waarde = ((1024.0f + 2.0f * serialPortReading) * 10000.0f) / (SENSOR_GSRGROVE_CALIBRATION_VALUE - serialPortReading);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    WriteQueueOrStack(3, waarde);   // huidweerstand (Ohm)
#endif
    return true;
  }
  // Huidgeleiding in microsiemens: 1 000 000 / huidweerstand (Ohm).
  bool ReadSkinConductanceMicrosiemens(float& waarde) {
    if (!ReadSkinResistanceOhm(waarde) || waarde <= 0) { waarde = NAN; return false; }
    waarde = 1000000.0f / waarde;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    WriteQueueOrStack(2, waarde);   // huidgeleiding (microsiemens)
#endif
    return true;
  }
};

#endif

#endif
