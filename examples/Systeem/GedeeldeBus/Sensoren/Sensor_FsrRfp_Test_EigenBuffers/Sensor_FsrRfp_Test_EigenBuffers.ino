// ============================================================================
// Sensor: FsrRfp, test eigen buffers per object (T2-9)
// ============================================================================
// Elk FsrRfp-object heeft zijn eigen buffers van de QueueOrStack, ook twee objecten van dezelfde set (B1).
// Twee objecten van set 1 op dezelfde ADS1115: A op AIN0/AIN1, B op AIN2/AIN3.
// Alleen A meet. Verwacht: de QueueOrStack van A heeft waarden (status 0 of 1),
// die van B blijft leeg (status -1). Zou B de buffers van A delen, dan zag B de waarden van A.
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
#if !((DATA_LAYER_CONFIG) & DATA_TYPE_DIRECT)
  #error Voeg DATA_TYPE_DIRECT toe aan DATA_LAYER_CONFIG voor deze test.
#endif
#if FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 == 0
  #error Zet FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 op minstens 1 voor deze test.
#endif
#if FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1 != 2
  #error Zet FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1 op 2: elk van de twee objecten gebruikt 2 ingangen van dezelfde ADS1115.
#endif

ExtenderADS1115 extenderADS1115(
  &Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD
);

struct FsrRfp* tweedeSensorFsrRfpSet1 = nullptr;   // object B

void ToonResultaat(const __FlashStringHelper* stap, bool gelukt) {
  GA_SERIAL.print(stap);
  GA_SERIAL.print(F(": "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_FsrRfp_Test_EigenBuffers"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  GA_SERIAL.println(F("=== FsrRfp test eigen buffers per object (T2-9) ==="));
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_TYPE_DIRECT);
  ToonResultaat(F("Data aanmelden/controleren/inpluggen/activeren"), Data != nullptr);
  if (Data == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }
  // Object A op AIN0 en AIN1, object B op AIN2 en AIN3 van dezelfde ADS1115 (elk claimt alleen zijn eerste 2 ingangen).
  sensorFsrRfpSet1 = GedeeldeBusNewComponent<struct FsrRfp>(&extenderADS1115, GedeeldeBusComponent::FsrRfp, Data,
      FSR_RFP_ADC_PIN_SENSOR_1_SET_1, FSR_RFP_ADC_PIN_SENSOR_2_SET_1, FSR_RFP_ADC_PIN_SENSOR_3_SET_1, FSR_RFP_ADC_PIN_SENSOR_4_SET_1,
      HardwareResourceToegang::GEDEELD, 1);
  ToonResultaat(F("Object A aanmelden/controleren/inpluggen/activeren"), sensorFsrRfpSet1 != nullptr);
  tweedeSensorFsrRfpSet1 = GedeeldeBusNewComponent<struct FsrRfp>(&extenderADS1115, GedeeldeBusComponent::FsrRfp, Data,
      FSR_RFP_ADC_PIN_SENSOR_3_SET_1, FSR_RFP_ADC_PIN_SENSOR_4_SET_1, FSR_RFP_ADC_PIN_SENSOR_1_SET_1, FSR_RFP_ADC_PIN_SENSOR_2_SET_1,
      HardwareResourceToegang::GEDEELD, 1);
  ToonResultaat(F("Object B aanmelden/controleren/inpluggen/activeren"), tweedeSensorFsrRfpSet1 != nullptr);
  if (sensorFsrRfpSet1 == nullptr || tweedeSensorFsrRfpSet1 == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  for (uint8_t teller = 0; teller < 3; teller++) sensorFsrRfpSet1->RawAnalogRead(sensorFsrRfpSet1->sensorPin[0]);   // alleen A meet

  const int8_t statusA = sensorFsrRfpSet1->ReadQueueOrStackStatus(1);
  const int8_t statusB = tweedeSensorFsrRfpSet1->ReadQueueOrStackStatus(1);
  GA_SERIAL.print(F("Status QueueOrStack A, sensor 1 (verwacht 0 of 1): "));
  GA_SERIAL.println(static_cast<int>(statusA));
  GA_SERIAL.print(F("Status QueueOrStack B, sensor 1 (verwacht -1, leeg): "));
  GA_SERIAL.println(static_cast<int>(statusB));
  GA_SERIAL.println((statusA >= 0 && statusB == -1) ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
}
