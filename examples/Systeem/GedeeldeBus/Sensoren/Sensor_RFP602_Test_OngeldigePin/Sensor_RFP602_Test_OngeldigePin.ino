// ============================================================================
// Sensor: RFP602, test ongeldige pin (T2-2)
// ============================================================================
// Contract: StartExtenderReadAnalog() geeft -2 (niet ondersteund) voor een pin die de ADC niet heeft.
// RawAnalogRead(7) vraagt ingang 7; de ADS1115 heeft alleen AIN0 tot AIN3.
// Verwacht: waarde 0, geen fout voor het runtime-herstel (opeenvolgende fouten blijft 0),
// en de meting op een geldige ingang werkt daarna gewoon.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL in UserConfig.h op minstens 1: RFP602 meet via een ADS1115-Extender.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

ExtenderADS1115 extenderADS1115(
  &Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD
);

void ToonResultaat(const __FlashStringHelper* stap, bool gelukt) {
  GA_SERIAL.print(stap);
  GA_SERIAL.print(F(": "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
}

// Toont de toestand van het runtime-herstel (D042) van de ADS1115.
void ToonToestandADS1115() {
  GA_SERIAL.print(F("  ADS1115 actief: "));
  GA_SERIAL.print(extenderADS1115.actief ? F("ja") : F("nee"));
  GA_SERIAL.print(F(", opeenvolgende fouten: "));
  GA_SERIAL.print(static_cast<int>(extenderADS1115.toestandTijdensRuntime.aantalOpeenvolgendeFouten));
  GA_SERIAL.print(F(", foutniveau: "));
  GA_SERIAL.println(static_cast<int>(extenderADS1115.toestandTijdensRuntime.huidigFoutniveau));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  GA_SERIAL.println(F("=== RFP602 test ongeldige pin (T2-2) ==="));
  sensorRFP602set1 = GedeeldeBusNewComponent<struct RFP602>(&extenderADS1115, GedeeldeBusComponent::RFP602);
  ToonResultaat(F("Sensor aanmelden/controleren/inpluggen/activeren"), sensorRFP602set1 != nullptr);
  if (sensorRFP602set1 == nullptr) GA_SERIAL.println(F("RESULTAAT: FOUT"));
}

void loop() {
  if (sensorRFP602set1 == nullptr) return;
  GA_SERIAL.print(F("Ongeldige ingang 7 (verwacht 0): "));
  GA_SERIAL.println(sensorRFP602set1->RawAnalogRead(7));
  ToonToestandADS1115();
  GA_SERIAL.print(F("Geldige ingang sensor 1 (verwacht een meting): "));
  GA_SERIAL.println(sensorRFP602set1->RawAnalogRead(sensorRFP602set1->sensorPin[0]));
  ToonToestandADS1115();
  GA_SERIAL.println();
  delay(1000);
}
