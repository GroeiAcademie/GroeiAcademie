// ============================================================================
// Sensor: RFP602, test ADC bezet (T2-3)
// ============================================================================
// Contract: StartExtenderReadAnalog() geeft 0 (bezet) zolang een andere child zijn meting niet ophaalde.
// Twee RFP602-objecten van set 1 op dezelfde ADS1115: A op AIN0/AIN1, B op AIN2/AIN3.
// A start een niet-blokkerende meting en haalt ze nog niet op; B meet blokkerend.
// Verwacht: B geeft 0 na ongeveer 150 ms, zonder fout voor het runtime-herstel.
// Daarna haalt A zijn meting op, en B meet weer normaal en snel.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL in UserConfig.h op minstens 1: RFP602 meet via een ADS1115-Extender.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif
#if RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1 != 2
  #error Zet RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1 op 2: elk van de twee objecten gebruikt 2 ingangen van dezelfde ADS1115.
#endif

ExtenderADS1115 extenderADS1115(
  &Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD
);

struct RFP602* sensorRFP602set1tweede = nullptr;   // object B

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

  GA_SERIAL.println(F("=== RFP602 test ADC bezet (T2-3) ==="));
  // Object A op AIN0 en AIN1, object B op AIN2 en AIN3 van dezelfde ADS1115 (elk claimt alleen zijn eerste 2 ingangen).
  sensorRFP602set1 = GedeeldeBusNewComponent<struct RFP602>(&extenderADS1115, GedeeldeBusComponent::RFP602,
      RFP602_ADC_PIN_SENSOR_1_SET_1, RFP602_ADC_PIN_SENSOR_2_SET_1, RFP602_ADC_PIN_SENSOR_3_SET_1, RFP602_ADC_PIN_SENSOR_4_SET_1,
      HardwareResourceToegang::GEDEELD, 1);
  ToonResultaat(F("Object A aanmelden/controleren/inpluggen/activeren"), sensorRFP602set1 != nullptr);
  sensorRFP602set1tweede = GedeeldeBusNewComponent<struct RFP602>(&extenderADS1115, GedeeldeBusComponent::RFP602,
      RFP602_ADC_PIN_SENSOR_3_SET_1, RFP602_ADC_PIN_SENSOR_4_SET_1, RFP602_ADC_PIN_SENSOR_1_SET_1, RFP602_ADC_PIN_SENSOR_2_SET_1,
      HardwareResourceToegang::GEDEELD, 1);
  ToonResultaat(F("Object B aanmelden/controleren/inpluggen/activeren"), sensorRFP602set1tweede != nullptr);
  if (sensorRFP602set1 == nullptr || sensorRFP602set1tweede == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }
}

void loop() {
  if (sensorRFP602set1 == nullptr || sensorRFP602set1tweede == nullptr) return;
  GA_SERIAL.print(F("A start (verwacht 1): "));
  GA_SERIAL.println(sensorRFP602set1->StartAnalogRead(sensorRFP602set1->sensorPin[0]) ? 1 : 0);

  unsigned long begin = millis();
  const int waardeB = sensorRFP602set1tweede->RawAnalogRead(sensorRFP602set1tweede->sensorPin[0]);
  GA_SERIAL.print(F("B terwijl A bezet is (verwacht 0, ongeveer 150 ms): "));
  GA_SERIAL.print(waardeB);
  GA_SERIAL.print(F(" na "));
  GA_SERIAL.print(millis() - begin);
  GA_SERIAL.println(F(" ms"));
  ToonToestandADS1115();

  while (!sensorRFP602set1->IsAnalogReadKlaar()) { yield(); }
  GA_SERIAL.print(F("A haalt op (verwacht een meting): "));
  GA_SERIAL.println(sensorRFP602set1->GetAnalogRead());

  begin = millis();
  const int waardeBNaA = sensorRFP602set1tweede->RawAnalogRead(sensorRFP602set1tweede->sensorPin[0]);
  GA_SERIAL.print(F("B na A (verwacht een meting, ongeveer 10 ms): "));
  GA_SERIAL.print(waardeBNaA);
  GA_SERIAL.print(F(" na "));
  GA_SERIAL.print(millis() - begin);
  GA_SERIAL.println(F(" ms"));
  ToonToestandADS1115();
  GA_SERIAL.println();
  delay(1000);
}
