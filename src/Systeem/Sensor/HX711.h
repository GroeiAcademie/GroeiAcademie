#ifndef SENSOR_HX711_H
#define SENSOR_HX711_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_HX711_AANTAL > 0

#include "../GedeeldeBus/GedeeldeBus.h"
#include <HX711.h>

struct SensorHX711 : Sensor {
  const HardwareResourcePin np_DOUT;
  const HardwareResourcePin np_PD_SCK;
  uint8_t exclusief_[2];
  ::HX711 adc;
  SensorHX711(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_DOUT, HardwareResourcePin np_PD_SCK)
    : Sensor(parent, {nullptr, 0, exclusief_, 2}, component, HardwareResourceToegang::EXCLUSIEF), np_DOUT(np_DOUT), np_PD_SCK(np_PD_SCK) {
    exclusief_[0] = static_cast<uint8_t>(np_DOUT);
    exclusief_[1] = static_cast<uint8_t>(np_PD_SCK);
  }
#if GEDEELDE_BUS_MIXED_PINS && EXTENDER_CD74HC4067_AANTAL >= 2
  // Via twee CD74HC4067 (twee lijnen tegelijk, één Z per lijn): DOUT op een kanaal van de parent (CD74HC4067 #1),
  // PD_SCK op een kanaal van de tweede CD74HC4067 (extender). De driver werkt op de native Z-pinnen van beide.
  GedeeldeBusNode* extenderHulp_[2] = {nullptr, nullptr};
  bool viaMixed = false;
  SensorHX711(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_DOUT, GedeeldeBusNode* extender, HardwareResourcePin np_PD_SCK)
    : Sensor(parent, {nullptr, 0, exclusief_, 2}, component, HardwareResourceToegang::EXCLUSIEF), np_DOUT(np_DOUT), np_PD_SCK(np_PD_SCK) {
    exclusief_[0] = static_cast<uint8_t>(np_DOUT);
    exclusief_[1] = static_cast<uint8_t>(np_PD_SCK);
    extenderHulp_[1] = extender;   // index 0 blijft nullptr: DOUT hoort bij de parent
    ownerClaim = extenderHulp_;
    viaMixed = true;
  }
#endif

  bool inpluggen() override;
  // Timinggevoelig native protocol: niet via een generieke GPIO-Extender (GB112).
  // Wel via twee CD74HC4067 (mixed): die schakelen DOUT en PD_SCK analoog door naar native Z-pinnen.
#if GEDEELDE_BUS_MIXED_PINS && EXTENDER_CD74HC4067_AANTAL >= 2
  bool ExtenderFysiekGeschikt(const GedeeldeBusNode* extender) const override { return viaMixed && extender == parent && extender->component == GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067; }
#else
  bool ExtenderFysiekGeschikt(const GedeeldeBusNode* extender) const override { (void)extender; return false; }
#endif

  uint8_t AantalChannels() const override { return 2; }   // meetkanalen voor Data: 1 ruwe waarde, 2 gewicht in gram
  bool Activeren() override;
  bool LeesHoofdwaarde(float& waarde);
  // Tarra: gemiddelde van SENSOR_HX711_TARE_SAMPLE_COUNT ruwe metingen als nulpunt (ongeveer 0,1 s per meting bij 10 Hz).
  // Elke meting wacht hoogstens 1 s; komt er geen meting, dan geeft Tare() false en blijft de tarra ongewijzigd.
  long tareOffset = 0;
  // true na een geslaagde Tare(); blijft true na een mislukte nieuwe poging (de vorige tarra blijft dan gelden).
  // Activeren() zet het terug op false.
  bool tarraSucceeded = false;
  bool Tare();
  // Gewicht in gram: (ruwe meting - tarra) / SENSOR_HX711_CALIBRATION_FACTOR. Niet-blokkerend: false als er nog geen nieuwe meting is.
  // Geeft ook false zolang SENSOR_HX711_CALIBRATION_FACTOR 0 is, dus zolang niet gekalibreerd werd.
  // Geeft ook false zolang er geen geslaagde Tare() was (tarraSucceeded).
  bool ReadWeightGram(float& waarde);
};

#endif

#endif
