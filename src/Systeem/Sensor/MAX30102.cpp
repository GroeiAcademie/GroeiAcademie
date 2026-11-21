#include "MAX30102.h"
#if SENSOR_MAX30102_AANTAL > 0
#include <spo2_algorithm.h>
bool MAX30102::Activeren() {
#if GEDEELDE_BUS_MIXED_PINS
  // Mixed: hulppinnen op de tweede Extender instellen vóór de library-initialisatie.
  if (viaMixed) {
    if (extenderHulp_[0] == nullptr || !extenderHulp_[0]->actief) return false;
    if (!extenderHulp_[0]->SetExtenderPinMode(np_INT, INPUT)) return false;   // INT: uitgang van de sensor
  }
#endif
  if (!parent || !parent->actief) return false;
  HardwareResourceTypeI2C* const apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  // Hardware-aanmelding via officiële library. Geen fictieve successtatus.
  const bool gevonden = max30102.begin(Wire, I2C_SPEED_STANDARD, apparaat->adres);
  Wire.setClock(I2C_SPEED_STANDARD); // SparkFun begin() verandert globale busklok; centraal standaard herstellen
  if (!gevonden) return false;
  aantalVerlorenSamples = 0;
  // Gedocumenteerde Red+IR mode. 100 samples per seconde met middeling 4 = 25 samples per seconde,
  // de frequentie waarvoor het SpO₂-algoritme van SparkFun geschreven is (zoals hun voorbeeld Example8_SPO2).
  max30102.setup(0x1F, 4, 2, 100, 411, 4096);
  bufferSampleCount = 0;
  oxygenSaturationValid = 0;
  heartRateFromAlgorithmValid = 0;
#if GEDEELDE_BUS_MIXED_PINS
  // INT (laag actief): interrupt "nieuwe sample" (PPG_RDY); lezen van de interruptstatus wist hem.
  if (viaMixed) max30102.enableDATARDY();
#endif
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
  // Nieuwe samples in de FIFO van de chip: boven 3 gaan er veelvouden van 4 verloren in de ring van de library.
  const uint8_t aantalNieuweSamples = (max30102.getWritePointer() - max30102.getReadPointer()) & 0x1F;
  if (aantalNieuweSamples > 3) {
    aantalVerlorenSamples += aantalNieuweSamples - aantalNieuweSamples % 4;
    // Het algoritme verwacht een doorlopende reeks van 25 samples per seconde.
    // Na verlies opnieuw 100 samples (4 s) verzamelen, en tot dan geen hartslag of SpO₂.
    bufferSampleCount = 0;
    heartRateFromAlgorithmValid = 0;
    oxygenSaturationValid = 0;
  }
  max30102.check();
  if (max30102.available() == 0) return false;
  // De library-ring houdt hoogstens 3 samples bij: meer dan 3 nieuwe samples tussen twee oproepen gaan verloren.
  while (max30102.available() > 0) {
    waarde = static_cast<float>(max30102.getFIFOIR()); // rauwe IR-telling, geen SpO2; de laatste blijft de hoofdwaarde
    // Hartslag en SpO₂: 100 samples (4 s) verzamelen; daarna elke 25 nieuwe samples (1 s) opnieuw berekenen over de laatste 100.
    irBuffer[bufferSampleCount] = max30102.getFIFOIR();
    redBuffer[bufferSampleCount] = max30102.getFIFORed();
    if (++bufferSampleCount == 100) {
      maxim_heart_rate_and_oxygen_saturation(irBuffer, 100, redBuffer, &oxygenSaturationPercent, &oxygenSaturationValid, &heartRateFromAlgorithm, &heartRateFromAlgorithmValid);
      lastAlgorithmMS = millis();
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
      // Nieuwe berekening: hartslag (kanaal 3) en SpO₂ (kanaal 4) naar Data, alleen als het algoritme ze geldig noemt.
      if (heartRateFromAlgorithmValid == 1) WriteQueueOrStack(3, static_cast<float>(heartRateFromAlgorithm));
      if (oxygenSaturationValid == 1) WriteQueueOrStack(4, static_cast<float>(oxygenSaturationPercent));
#endif
      memmove(irBuffer, irBuffer + 25, 75 * sizeof(irBuffer[0]));
      memmove(redBuffer, redBuffer + 25, 75 * sizeof(redBuffer[0]));
      bufferSampleCount = 75;
    }
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
// Hartslag (slagen per minuut) van de laatste berekening van het SparkFun-algoritme.
// false zolang het algoritme geen geldige waarde gaf (bv. geen vinger of te veel beweging), of als de berekening ouder is dan 3 s.
bool MAX30102::ReadHeartRateBeatsPerMinute(float& waarde) {
  waarde = NAN;
  if (!actief || heartRateFromAlgorithmValid != 1 || millis() - lastAlgorithmMS > 3000UL) return false;
  waarde = static_cast<float>(heartRateFromAlgorithm);
  return true;
}
// SpO₂ (%) van de laatste berekening.
// false zolang het algoritme geen geldige waarde gaf (bv. geen vinger of te veel beweging), of als de berekening ouder is dan 3 s.
bool MAX30102::ReadOxygenSaturationPercent(float& waarde) {
  waarde = NAN;
  if (!actief || oxygenSaturationValid != 1 || millis() - lastAlgorithmMS > 3000UL) return false;
  waarde = static_cast<float>(oxygenSaturationPercent);
  return true;
}
#endif
