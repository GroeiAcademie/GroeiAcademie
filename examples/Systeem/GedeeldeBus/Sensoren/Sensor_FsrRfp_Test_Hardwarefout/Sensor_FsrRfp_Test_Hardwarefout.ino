// ============================================================================
// Sensor: FsrRfp, test hardwarefout (T2-4)
// ============================================================================
// Contract: StartExtenderReadAnalog() geeft -1 bij een I2C-fout; dat telt als fout voor het runtime-herstel.
// Werkwijze: laat de sketch lopen, maak tijdens het meten de voeding of SDA van de ADS1115 los,
// wacht tot de ADS1115 uitgeschakeld is (actief: nee), en sluit hem daarna terug aan.
// Verwacht: opeenvolgende fouten en foutniveau lopen op volgens de drempels, de ADS1115 gaat uit op het
// uitschakelniveau, en na het terugplaatsen herstelt hij (actief: ja, fouten terug 0) en meet FsrRfp weer.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL in UserConfig.h op minstens 1: FsrRfp meet via een ADS1115-Extender.
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

// Toont de toestand van het runtime-herstel van de ADS1115.
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
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_FsrRfp_Test_Hardwarefout"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  GA_SERIAL.println(F("=== FsrRfp test hardwarefout (T2-4) ==="));
  GA_SERIAL.println(F("Maak tijdens het meten de ADS1115 los en sluit hem daarna terug aan."));
  sensorFsrRfpSet1 = GedeeldeBusNewComponent<struct FsrRfp>(&extenderADS1115, GedeeldeBusComponent::FsrRfp);
  ToonResultaat(F("Sensor aanmelden/controleren/inpluggen/activeren"), sensorFsrRfpSet1 != nullptr);
  if (sensorFsrRfpSet1 == nullptr) GA_SERIAL.println(F("RESULTAAT: FOUT"));
}

void loop() {
  if (sensorFsrRfpSet1 == nullptr) return;
  GA_SERIAL.print(millis());
  GA_SERIAL.print(F(" ms"));
  for (uint8_t sensorNummer = 0; sensorNummer < FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1; sensorNummer++) {
    GA_SERIAL.print(F(" | sensor "));
    GA_SERIAL.print(sensorNummer + 1);
    GA_SERIAL.print(F(": "));
    GA_SERIAL.print(sensorFsrRfpSet1->RawAnalogRead(sensorFsrRfpSet1->sensorPin[sensorNummer]));
  }
  GA_SERIAL.println();
  ToonToestandADS1115();
  delay(200);
}
