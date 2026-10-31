#include "TTP223.h"
#if SENSOR_TTP223_AANTAL > 0
bool TTP223::inpluggen() {
  if (ingeplugd) return true;
  if (firstExtenderNode() != nullptr || np_OUT == HardwareResourcePin::NONE ||
      np_OUT == HardwareResourcePin::CUSTOM) return false;
  if (!Sensor::inpluggen()) return false;
  pinMode(NativeArduinoPinVan(np_OUT), INPUT);
  return true;
}
bool TTP223::Activeren() {
  return ingeplugd && parent && parent->actief;
}
bool TTP223::WerkAanraakstatusBij() {
  if (!actief || firstExtenderNode() != nullptr) return false;
  const bool nuAangeraakt = digitalRead(NativeArduinoPinVan(np_OUT)) == HIGH;
  const unsigned long nu = millis();
  aanrakingBegonnen = false;
  aanrakingLosgelaten = false;
  if (!statusBekend) {
    statusBekend = true;
    aangeraakt = nuAangeraakt;
    if (nuAangeraakt && SENSOR_TTP223_DIRECTE_MODUS) {
      aanrakingBegonnen = true;
      tijdstipEersteAanraking = nu;
      tijdstipLaatsteAanraking = nu;
      ++aantalAanrakingen;
    }
    return true;
  }
  if (nuAangeraakt != aangeraakt) {
    aangeraakt = nuAangeraakt;
    if (SENSOR_TTP223_DIRECTE_MODUS && nuAangeraakt) {
      aanrakingBegonnen = true;
      if (aantalAanrakingen == 0) tijdstipEersteAanraking = nu;
      if (heeftEerdereLoslating) {
        tijdTussenAanrakingen = nu - tijdstipLaatsteLoslaten;
        heeftAanraakInterval = true;
        if (tijdTussenAanrakingen <= SENSOR_TTP223_DUBBELE_AANRAKING_MS) ++aantalDubbeleAanrakingen;
      }
      tijdstipLaatsteAanraking = nu;
      ++aantalAanrakingen;
    } else if (SENSOR_TTP223_DIRECTE_MODUS && !nuAangeraakt) {
      aanrakingLosgelaten = true;
      duurLaatsteAanraking = nu - tijdstipLaatsteAanraking;
      totaleVoltooideAanraakduur += duurLaatsteAanraking;
      if (duurLaatsteAanraking >= SENSOR_TTP223_LANGE_AANRAKING_MS) ++aantalLangeAanrakingen;
      else ++aantalKorteAanrakingen;
      tijdstipLaatsteLoslaten = nu;
      heeftEerdereLoslating = true;
    }
  }
  return true;
}
bool TTP223::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!WerkAanraakstatusBij()) return false;
  waarde = aangeraakt ? 1.0f : 0.0f;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(1, waarde);
#endif
  return true;
}
bool TTP223::LeesAantalAanrakingen(uint32_t& waarde) {
  waarde = 0;
  if (!SENSOR_TTP223_DIRECTE_MODUS || !WerkAanraakstatusBij()) return false;
  waarde = aantalAanrakingen;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(2, waarde);
#endif
  return true;
}
bool TTP223::LeesLaatsteAanraakduur(unsigned long& waarde) {
  waarde = 0;
  if (!SENSOR_TTP223_DIRECTE_MODUS || !WerkAanraakstatusBij() || aantalAanrakingen == 0) return false;
  waarde = aangeraakt ? millis() - tijdstipLaatsteAanraking : duurLaatsteAanraking;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(3, static_cast<uint32_t>(waarde));
#endif
  return true;
}
bool TTP223::LeesTotaleAanraakduur(unsigned long& waarde) {
  waarde = 0;
  if (!SENSOR_TTP223_DIRECTE_MODUS || !WerkAanraakstatusBij()) return false;
  waarde = totaleVoltooideAanraakduur + (aangeraakt ? millis() - tijdstipLaatsteAanraking : 0);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(4, static_cast<uint32_t>(waarde));
#endif
  return true;
}
bool TTP223::LeesTijdTussenAanrakingen(unsigned long& waarde) {
  waarde = 0;
  if (!SENSOR_TTP223_DIRECTE_MODUS || !WerkAanraakstatusBij() || !heeftAanraakInterval) return false;
  waarde = tijdTussenAanrakingen;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(5, static_cast<uint32_t>(waarde));
#endif
  return true;
}
bool TTP223::LeesAanraakfrequentie(float& waarde) {
  waarde = NAN;
  if (!SENSOR_TTP223_DIRECTE_MODUS || !WerkAanraakstatusBij() || aantalAanrakingen < 2) return false;
  const unsigned long verstreken = millis() - tijdstipEersteAanraking;
  if (verstreken == 0) return false;
  waarde = static_cast<float>(aantalAanrakingen - 1) * 60000.0f / static_cast<float>(verstreken);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(6, waarde);
#endif
  return true;
}
bool TTP223::LeesAantalKorteAanrakingen(uint32_t& waarde) {
  waarde = 0;
  if (!SENSOR_TTP223_DIRECTE_MODUS || !WerkAanraakstatusBij()) return false;
  waarde = aantalKorteAanrakingen;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(7, waarde);
#endif
  return true;
}
bool TTP223::LeesAantalLangeAanrakingen(uint32_t& waarde) {
  waarde = 0;
  if (!SENSOR_TTP223_DIRECTE_MODUS || !WerkAanraakstatusBij()) return false;
  waarde = aantalLangeAanrakingen;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(8, waarde);
#endif
  return true;
}
bool TTP223::LeesAantalDubbeleAanrakingen(uint32_t& waarde) {
  waarde = 0;
  if (!SENSOR_TTP223_DIRECTE_MODUS || !WerkAanraakstatusBij()) return false;
  waarde = aantalDubbeleAanrakingen;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(9, waarde);
#endif
  return true;
}
bool TTP223::LeesTijdstipLaatsteAanraking(unsigned long& waarde) {
  waarde = 0;
  if (!SENSOR_TTP223_DIRECTE_MODUS || !WerkAanraakstatusBij() || aantalAanrakingen == 0) return false;
  waarde = tijdstipLaatsteAanraking;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(10, static_cast<uint32_t>(waarde));
#endif
  return true;
}
bool TTP223::LeesTijdstipLaatsteLoslaten(unsigned long& waarde) {
  waarde = 0;
  if (!SENSOR_TTP223_DIRECTE_MODUS || !WerkAanraakstatusBij() || !heeftEerdereLoslating) return false;
  waarde = tijdstipLaatsteLoslaten;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(11, static_cast<uint32_t>(waarde));
#endif
  return true;
}
#endif
