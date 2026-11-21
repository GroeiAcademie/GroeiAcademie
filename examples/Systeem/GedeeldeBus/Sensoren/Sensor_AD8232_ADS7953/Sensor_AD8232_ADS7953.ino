// Sensor: AD8232 via ADS7953.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// Analoge uitgang naar kanaal 0; bij een CD74HC4067 loopt Z via de geconfigureerde native ADC-ingang.

#include <GroeiAcademie.h>

#if EXTENDER_ADS7953_VOEDING_MILLIVOLT == 0
  #error Zet EXTENDER_ADS7953_VOEDING_MILLIVOLT op de werkelijke +VA (2700..5250 mV): het bereik van deze sensor vraagt 2xVREF.
#endif

#if (SENSOR_AD8232_ADC_BEREIK_MILLIVOLT > EXTENDER_ADS7953_REFERENTIE_MILLIVOLT && (SENSOR_AD8232_ADC_BEREIK_MILLIVOLT > 2 * EXTENDER_ADS7953_REFERENTIE_MILLIVOLT || EXTENDER_ADS7953_VOEDING_MILLIVOLT < 2 * EXTENDER_ADS7953_REFERENTIE_MILLIVOLT)) || \
    (EXTENDER_ADS7953_VOEDING_MILLIVOLT != 0 && EXTENDER_ADS7953_VOEDING_MILLIVOLT < EXTENDER_ADS7953_REFERENTIE_MILLIVOLT)
  #error SENSOR_AD8232_ADC_BEREIK_MILLIVOLT is groter dan wat de ADS7953 meet: het dubbele bereik (2 x EXTENDER_ADS7953_REFERENTIE_MILLIVOLT) vraagt EXTENDER_ADS7953_VOEDING_MILLIVOLT van minstens 2 x de referentie; stel beide in volgens je hardware, of verlaag het bereik tot de werkelijke uitgangsspanning van de sensor.
#endif
#if SENSOR_AD8232_AANTAL != 1
  #error Zet SENSOR_AD8232_AANTAL op 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_ADS7953_AANTAL == 0
  #error Zet EXTENDER_ADS7953_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_ADS7953_AANTAL == 1
ExtenderADS7953 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7953,
  EXTENDER_ADS7953_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI
);
#else
ExtenderADS7953 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7953,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

AD8232* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_AD8232_ADS7953"));
  GA_SERIAL.println(F("MEETTEST: Verbind electrodes correct; controleer variatie van ECG OUT. Geen LO+/LO- in deze route."));
  sensor = GedeeldeBusNewComponent<AD8232>(&extender, GedeeldeBusComponent::AD8232, static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH0), HardwareResourcePin::NP_D2, HardwareResourcePin::NP_D3, HardwareResourceToegang::GEDEELD);   // LO+ native D2, LO- native D3
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
