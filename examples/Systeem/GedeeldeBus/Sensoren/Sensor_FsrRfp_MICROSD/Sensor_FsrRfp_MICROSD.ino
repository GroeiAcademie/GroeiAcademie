// ============================================================================
// Sensor: FsrRfp microSD
// ============================================================================
// FsrRfp via Data RECORDING naar microSD.
// De metingen komen in SENSOR.DAT.
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

ExtenderADS1115 extenderADS1115(
  &Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD
);


#if !((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  #error Voeg DATA_TYPE_MICROSD_SPI toe aan DATA_LAYER_CONFIG voor deze test.
#endif

void ToonResultaat(const __FlashStringHelper* stap, bool gelukt) {
  GA_SERIAL.print(stap);
  GA_SERIAL.print(F(": "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_FsrRfp_MICROSD"));

  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_TYPE_MICROSD_SPI);
  ToonResultaat(F("Data aanmelden/controleren/inpluggen/activeren"), Data != nullptr);

  if (Data == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  sensorFsrRfpSet1 = GedeeldeBusNewComponent<struct FsrRfp>(&extenderADS1115, GedeeldeBusComponent::FsrRfp, Data);
  const bool gelukt = sensorFsrRfpSet1 != nullptr;
  ToonResultaat(F("Sensor aanmelden/controleren/inpluggen/activeren"), gelukt);

  if (!gelukt) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  Data->RecordingStart();
  GA_SERIAL.println(F("RESULTAAT: OK"));
}

void loop() {
  if (sensorFsrRfpSet1 == nullptr || Data == nullptr) return;
  for (uint8_t sensorNummer = 0; sensorNummer < FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1; sensorNummer++) {
    const int ruweData = sensorFsrRfpSet1->RawAnalogRead(sensorFsrRfpSet1->sensorPin[sensorNummer]);
    GA_SERIAL.print(F("Sensor "));
    GA_SERIAL.print(sensorNummer + 1);
    GA_SERIAL.print(F(": "));
    GA_SERIAL.println(ruweData);
  }

  GA_SERIAL.println();
  delay(250);
}
