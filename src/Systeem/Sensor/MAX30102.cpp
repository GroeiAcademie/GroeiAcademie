#include "MAX30102.h"
#if SENSOR_MAX30102_AANTAL > 0
bool MAX30102::Activeren() {
  if (!parent || !parent->actief) return false;
  HardwareResourceTypeI2C* const apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  // Hardware-aanmelding via officiële library. Geen fictieve successtatus.
  const bool gevonden = max30102.begin(Wire, I2C_SPEED_STANDARD, apparaat->adres);
  Wire.setClock(I2C_SPEED_STANDARD); // SparkFun begin() verandert globale busklok; centraal standaard herstellen
  if (!gevonden) return false;
  max30102.setup(0x1F, 4, 2, 400, 411, 4096); // gedocumenteerde Red+IR mode
  return true;
}

// Runtime-metingen gebruiken de bestaande I2C-toegangspoort, ook achter TCA9548A.
bool MAX30102::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (parent == nullptr || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  // check() geeft het aantal nieuwe samples; 0 betekent ook "nog niets nieuw" en is dus geen storing.
  // Geen nieuwe sample: geen meetwaarde en geen melding aan de foutladder (de chip zelf is al gecontroleerd hierboven).
  max30102.check();
  if (max30102.available() == 0) return false;
  // Alle opgehaalde samples verwerken: de buffer van de library is klein en overschrijft anders de oudste.
  while (max30102.available() > 0) {
    waarde = static_cast<float>(max30102.getFIFOIR()); // rauwe IR-telling, geen SpO2; de laatste blijft de hoofdwaarde
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    {
      WriteQueueOrStack(1, static_cast<uint32_t>(max30102.getFIFOIR()));
      WriteQueueOrStack(2, static_cast<uint32_t>(max30102.getFIFORed()));
    }
#endif
    max30102.nextSample();
  }
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(true);
  return true;
}
#endif
