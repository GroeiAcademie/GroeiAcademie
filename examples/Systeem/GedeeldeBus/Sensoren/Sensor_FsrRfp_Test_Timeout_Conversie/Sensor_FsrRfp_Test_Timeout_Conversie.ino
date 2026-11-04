// ============================================================================
// Sensor: FsrRfp, test ophalen na verlies van de ADC (T2-5)
// ============================================================================
// Contract: een gestarte meting eindigt altijd met GetExtenderReadAnalogValue(); mislukt het ophalen,
// dan telt dat als precies één fout voor het runtime-herstel.
// Werkwijze: de sketch start een niet-blokkerende meting en wacht 5 s. Maak in die 5 s de ADS1115 los.
// Verwacht: GetAnalogRead() geeft 0 en opeenvolgende fouten stijgt met precies 1 per cyclus.
// Laat je de ADS1115 aangesloten, dan geeft GetAnalogRead() een meting en blijven de fouten 0.
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
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_FsrRfp_Test_Timeout_Conversie"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  GA_SERIAL.println(F("=== FsrRfp test ophalen na verlies van de ADC (T2-5) ==="));
  sensorFsrRfpSet1 = GedeeldeBusNewComponent<struct FsrRfp>(&extenderADS1115, GedeeldeBusComponent::FsrRfp);
  ToonResultaat(F("Sensor aanmelden/controleren/inpluggen/activeren"), sensorFsrRfpSet1 != nullptr);
  if (sensorFsrRfpSet1 == nullptr) GA_SERIAL.println(F("RESULTAAT: FOUT"));
}

void loop() {
  if (sensorFsrRfpSet1 == nullptr) return;
  ToonToestandADS1115();
  const bool gestart = sensorFsrRfpSet1->StartAnalogRead(sensorFsrRfpSet1->sensorPin[0]);
  GA_SERIAL.print(F("Start: "));
  GA_SERIAL.println(gestart ? F("gestart") : F("niet gestart (bezet, uitgeschakeld of fout)"));
  if (!gestart) { delay(1000); return; }
  GA_SERIAL.println(F("Maak nu de ADS1115 los (5 s)..."));
  delay(5000);
  const unsigned long begin = millis();
  while (!sensorFsrRfpSet1->IsAnalogReadKlaar() && millis() - begin <= 150) { yield(); }
  GA_SERIAL.print(F("GetAnalogRead (0 als de ADS1115 los is): "));
  GA_SERIAL.println(sensorFsrRfpSet1->GetAnalogRead());
  ToonToestandADS1115();
  GA_SERIAL.println();
  delay(1000);
}
