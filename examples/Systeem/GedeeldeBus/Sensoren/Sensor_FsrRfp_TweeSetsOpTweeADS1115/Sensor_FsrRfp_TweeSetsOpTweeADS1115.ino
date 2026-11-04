// ============================================================================
// Sensor: FsrRfp, twee sets op twee ADS1115's
// ============================================================================
// Lifecycletest voor twee FsrRfp-sets zonder Data-class (1b).
// Set 1 meet via ADS1115 #1, set 2 via ADS1115 #2: elke set heeft een eigen ADS1115 (G2).
// Nodig in je configuratie (of als compileervlag):
//   EXTENDER_ADS1115_AANTAL 2, I2C_ADDRESS_EXTENDER_ADS1115_2 (bv. 0x49)
//   FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 (2 of 4)
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

#if EXTENDER_ADS1115_AANTAL < 2
  #error Zet EXTENDER_ADS1115_AANTAL op 2: elke FsrRfp-set meet via een eigen ADS1115.
#endif

#if !defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
  #error Zet FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 op 2 of 4 in ConfigFsrRfp.h.
#endif

#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

ExtenderADS1115 extenderADS1115set1(
  &Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD
);

ExtenderADS1115 extenderADS1115set2(
  &Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_2,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_ADS1115_2_ALERT_RDY,
  HardwareResourceToegang::GEDEELD
);


void ToonResultaat(const __FlashStringHelper* stap, bool gelukt) {
  GA_SERIAL.print(stap);
  GA_SERIAL.print(F(": "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_FsrRfp_TweeSetsOpTweeADS1115"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  GA_SERIAL.println(F("=== FsrRfp twee sets op twee ADS1115's ==="));

  // Set 1: standaardkanalen FSR_RFP_ADC_PIN_SENSOR_n_SET_1, set = 1.
  sensorFsrRfpSet1 = GedeeldeBusNewComponent<struct FsrRfp>(
      &extenderADS1115set1, GedeeldeBusComponent::FsrRfp,
      HardwareResourceToegang::GEDEELD, 1);
  ToonResultaat(F("Set 1 aanmelden/controleren/inpluggen/activeren"), sensorFsrRfpSet1 != nullptr);

  // Set 2: standaardkanalen FSR_RFP_ADC_PIN_SENSOR_n_SET_2, set = 2.
  sensorFsrRfpSet2 = GedeeldeBusNewComponent<struct FsrRfp>(
      &extenderADS1115set2, GedeeldeBusComponent::FsrRfp,
      HardwareResourceToegang::GEDEELD, 2);
  ToonResultaat(F("Set 2 aanmelden/controleren/inpluggen/activeren"), sensorFsrRfpSet2 != nullptr);

  if (sensorFsrRfpSet1 == nullptr || sensorFsrRfpSet2 == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }
  GA_SERIAL.println(F("RESULTAAT: OK"));
}

void loop() {
  if (sensorFsrRfpSet1 == nullptr || sensorFsrRfpSet2 == nullptr) return;
  for (uint8_t sensorNummer = 0; sensorNummer < FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1; sensorNummer++) {
    GA_SERIAL.print(F("Set 1 sensor "));
    GA_SERIAL.print(sensorNummer + 1);
    GA_SERIAL.print(F(": "));
    GA_SERIAL.println(sensorFsrRfpSet1->RawAnalogRead(sensorFsrRfpSet1->sensorPin[sensorNummer]));
  }
  for (uint8_t sensorNummer = 0; sensorNummer < FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2; sensorNummer++) {
    GA_SERIAL.print(F("Set 2 sensor "));
    GA_SERIAL.print(sensorNummer + 1);
    GA_SERIAL.print(F(": "));
    GA_SERIAL.println(sensorFsrRfpSet2->RawAnalogRead(sensorFsrRfpSet2->sensorPin[sensorNummer]));
  }
  GA_SERIAL.println();
  delay(250);
}
