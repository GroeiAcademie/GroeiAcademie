// ============================================================================
// Sensor: FsrRfp, test ADC bezet (T2-3)
// ============================================================================
// Contract: StartExtenderReadAnalog() geeft 0 (bezet) zolang een andere child zijn meting niet ophaalde.
// Twee FsrRfp-objecten van set 1 op dezelfde ADS1115: A op AIN0/AIN1, B op AIN2/AIN3.
// A start een niet-blokkerende meting en haalt ze nog niet op; B meet blokkerend.
// Verwacht: B geeft 0 na ongeveer 150 ms, zonder fout voor het runtime-herstel.
// Daarna haalt A zijn meting op, en B meet weer normaal en snel.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL in UserConfig.h op minstens 1: FsrRfp meet via een ADS1115-Extender.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
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
  GA_SERIAL.println(F("TEST: Sensor_FsrRfp_Test_ADC_Bezet"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  GA_SERIAL.println(F("=== FsrRfp test ADC bezet (T2-3) ==="));
  // Object A op AIN0 en AIN1, object B op AIN2 en AIN3 van dezelfde ADS1115 (elk claimt alleen zijn eerste 2 ingangen).
  sensorFsrRfpSet1 = GedeeldeBusNewComponent<struct FsrRfp>(&extenderADS1115, GedeeldeBusComponent::FsrRfp,
      FSR_RFP_ADC_PIN_SENSOR_1_SET_1, FSR_RFP_ADC_PIN_SENSOR_2_SET_1, FSR_RFP_ADC_PIN_SENSOR_3_SET_1, FSR_RFP_ADC_PIN_SENSOR_4_SET_1,
      HardwareResourceToegang::GEDEELD, 1);
  ToonResultaat(F("Object A aanmelden/controleren/inpluggen/activeren"), sensorFsrRfpSet1 != nullptr);
  tweedeSensorFsrRfpSet1 = GedeeldeBusNewComponent<struct FsrRfp>(&extenderADS1115, GedeeldeBusComponent::FsrRfp,
      FSR_RFP_ADC_PIN_SENSOR_3_SET_1, FSR_RFP_ADC_PIN_SENSOR_4_SET_1, FSR_RFP_ADC_PIN_SENSOR_1_SET_1, FSR_RFP_ADC_PIN_SENSOR_2_SET_1,
      HardwareResourceToegang::GEDEELD, 1);
  ToonResultaat(F("Object B aanmelden/controleren/inpluggen/activeren"), tweedeSensorFsrRfpSet1 != nullptr);
  if (sensorFsrRfpSet1 == nullptr || tweedeSensorFsrRfpSet1 == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }
}

void loop() {
  if (sensorFsrRfpSet1 == nullptr || tweedeSensorFsrRfpSet1 == nullptr) return;
  GA_SERIAL.print(F("A start (verwacht 1): "));
  GA_SERIAL.println(sensorFsrRfpSet1->StartAnalogRead(sensorFsrRfpSet1->sensorPin[0]) ? 1 : 0);

  unsigned long begin = millis();
  const int waardeB = tweedeSensorFsrRfpSet1->RawAnalogRead(tweedeSensorFsrRfpSet1->sensorPin[0]);
  GA_SERIAL.print(F("B terwijl A bezet is (verwacht 0, ongeveer 150 ms): "));
  GA_SERIAL.print(waardeB);
  GA_SERIAL.print(F(" na "));
  GA_SERIAL.print(millis() - begin);
  GA_SERIAL.println(F(" ms"));
  ToonToestandADS1115();

  while (!sensorFsrRfpSet1->IsAnalogReadKlaar()) { yield(); }
  GA_SERIAL.print(F("A haalt op (verwacht een meting): "));
  GA_SERIAL.println(sensorFsrRfpSet1->GetAnalogRead());

  begin = millis();
  const int waardeBNaA = tweedeSensorFsrRfpSet1->RawAnalogRead(tweedeSensorFsrRfpSet1->sensorPin[0]);
  GA_SERIAL.print(F("B na A (verwacht een meting, ongeveer 10 ms): "));
  GA_SERIAL.print(waardeBNaA);
  GA_SERIAL.print(F(" na "));
  GA_SERIAL.print(millis() - begin);
  GA_SERIAL.println(F(" ms"));
  ToonToestandADS1115();
  GA_SERIAL.println();
  delay(1000);
}
