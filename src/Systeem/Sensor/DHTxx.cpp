#include "DHTxx.h"
#if SENSOR_DHT11_AANTAL > 0 || SENSOR_DHT22_AANTAL > 0
// Official API: https://github.com/adafruit/DHT-sensor-library/blob/master/DHT.h
bool DHTxx::inpluggen() {
  if (ingeplugd) return true;
  // Deze driver gebruikt native timing/pinnen: geen GPIO- of 1-Wire-Extenderprotocol.
  if (firstExtenderNode() != nullptr) return false;
  if (this->np_DATA == HardwareResourcePin::NONE || this->np_DATA == HardwareResourcePin::CUSTOM) return false;
  if (!Sensor::inpluggen()) return false;
  dht.begin();
  return true;
}
bool DHTxx::Activeren() {
  // De DHT11 en de DHT22 hebben geen chipidentiteit; fysieke werking pas aangetoond na geldige meting.
  return parent && parent->actief && ingeplugd;
}

bool DHTxx::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!actief) return false;
  waarde = dht.readTemperature();
  const bool gelukt = !isnan(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) {
    WriteQueueOrStack(1, waarde);                         // temperatuur
    const float vocht = dht.readHumidity();               // dezelfde DHT-meting, geen fictieve waarde
    if (!isnan(vocht)) WriteQueueOrStack(2, vocht);        // relatieve vochtigheid
  }
#endif
  return gelukt;
}
#endif
