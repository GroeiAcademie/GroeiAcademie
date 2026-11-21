// Sensor: AD8232 via ADS1158.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// Analoge uitgang naar kanaal 0; bij een CD74HC4067 loopt Z via de geconfigureerde native ADC-ingang.

#include <GroeiAcademie.h>

#if SENSOR_AD8232_AANTAL != 1
  #error Zet SENSOR_AD8232_AANTAL op 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_ADS1158_AANTAL == 0
  #error Zet EXTENDER_ADS1158_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_ADS1158_AANTAL == 1
ExtenderADS1158 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1158,
  EXTENDER_ADS1158_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, EXTENDER_ADS1158_1_START, EXTENDER_ADS1158_1_RESET, EXTENDER_ADS1158_1_PWDN
);
#else
ExtenderADS1158 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1158,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

AD8232* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_AD8232_ADS1158"));
  GA_SERIAL.println(F("MEETTEST: Verbind electrodes correct; controleer variatie van ECG OUT. Geen LO+/LO- in deze route."));
  sensor = GedeeldeBusNewComponent<AD8232>(&extender, GedeeldeBusComponent::AD8232, static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN0),
    static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_GPIO0), static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_GPIO1), static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_GPIO2), HardwareResourceToegang::GEDEELD);   // LO+ GPIO0, LO- GPIO1, SDN GPIO2
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  int32_t waarde = 0;
  if (sensor->RawAnalogRead(waarde)) {
    GA_SERIAL.print(F("AD8232 ruwe ADC-sample (geen fysieke eenheid): "));
    GA_SERIAL.println(waarde);
  } else {
    GA_SERIAL.println(F("ADC-UITLEESFOUT: geen geldige nieuwe meetwaarde (bereik, referentie en verbinding controleren)."));
  }
  // Een geslaagde ruwe uitlezing bewijst nog geen kalibratie of correcte fysieke meetfrequentie.
}
