// ============================================================================
// Sensor: FsrRfp, test ongeldige pin (T2-2)
// ============================================================================
// Contract: StartExtenderReadAnalog() geeft -2 (niet ondersteund) voor een pin die de ADC niet heeft.
// RawAnalogRead(7) vraagt ingang 7; de ADS1115 heeft alleen AIN0 tot AIN3.
// Verwacht: waarde 0, geen fout voor het runtime-herstel (opeenvolgende fouten blijft 0),
// en de meting op een geldige ingang werkt daarna gewoon.
// Aansluiting gebruikte I2C-parent/Extender: SDA -> board-SDA, SCL -> board-SCL; GND -> board-GND.
// UNO R4 Minima/WiFi: fysieke SDA- en SCL-header (bij R4 Minima ook A4=SDA, A5=SCL); UNO Q: board-SDA/SCL.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22 (bestaand FrameWork-boardprofiel).
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; DEV: SDA=GPIO8, SCL=GPIO9 (FrameWork-profiel).
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21 (ook Maker/Grove 6).
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8 (bestaand FrameWork-boardprofiel).
// Voeding VCC en GND volgens het concrete modulebord; controleer I2C-logicaniveau, pull-ups en eventuele levelshifting.
// Bij een analoge sensor is dit de bus van de ADC-Extender, NIET een I2C-aansluiting op de sensor zelf.
// Sluit de analoge sensoruitgang op het in de code/configuratie gebruikte ADC-kanaal aan.
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
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_FsrRfp_Test_OngeldigePin"));

  GA_SERIAL.println(F("=== FsrRfp test ongeldige pin (T2-2) ==="));
  sensorFsrRfpSet1 = GedeeldeBusNewComponent<struct FsrRfp>(&extenderADS1115, GedeeldeBusComponent::FsrRfp);
  ToonResultaat(F("Sensor aanmelden/controleren/inpluggen/activeren"), sensorFsrRfpSet1 != nullptr);
  if (sensorFsrRfpSet1 == nullptr) GA_SERIAL.println(F("RESULTAAT: FOUT"));
}

void loop() {
  if (sensorFsrRfpSet1 == nullptr) return;
  GA_SERIAL.print(F("Ongeldige ingang 7 (verwacht 0): "));
  GA_SERIAL.println(sensorFsrRfpSet1->RawAnalogRead(7));
  ToonToestandADS1115();
  GA_SERIAL.print(F("Geldige ingang sensor 1 (verwacht een meting): "));
  GA_SERIAL.println(sensorFsrRfpSet1->RawAnalogRead(sensorFsrRfpSet1->sensorPin[0]));
  ToonToestandADS1115();
  GA_SERIAL.println();
  delay(1000);
}
