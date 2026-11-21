// Sensor: PIEZO via ADS7953 + MAX14830SPI: signaal -> ADS7953 CH0, DO (comparator) -> MAX14830SPI GPIO0 (MIXED).
// Stroom van de hulppin fysiek controleren tegen de uitgangsstroom van de Extender.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_PIEZO_AANTAL != 1
  #error Zet SENSOR_PIEZO_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_ADS7953_AANTAL == 0
  #error Zet EXTENDER_ADS7953_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if EXTENDER_MAX14830_SPI_AANTAL == 0
  #error Zet EXTENDER_MAX14830_SPI_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if !GEDEELDE_BUS_MIXED_PINS
  #error Zet GEDEELDE_BUS_MIXED_PINS op 1 in UserConfig.h voor deze test.
#endif
#if EXTENDER_ADS7953_VOEDING_MILLIVOLT == 0
  #error Zet EXTENDER_ADS7953_VOEDING_MILLIVOLT op de werkelijke +VA (2700..5250 mV): het bereik van deze sensor vraagt 2xVREF.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
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

#if EXTENDER_MAX14830_SPI_AANTAL == 1
ExtenderMAX14830SPI extenderMAX14830SPI(
  &Native,
  GedeeldeBusComponent::UART_MAX14830,
  EXTENDER_MAX14830_SPI_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, EXTENDER_MAX14830_SPI_1_IRQ
);
#else
ExtenderMAX14830SPI extenderMAX14830SPI(
  &Native,
  GedeeldeBusComponent::UART_MAX14830,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

PIEZO* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_PIEZO_ADS7953_MAX14830SPI_MIXED"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren() &&
                      extenderMAX14830SPI.aanmelden() && extenderMAX14830SPI.controleren() && extenderMAX14830SPI.inpluggen() && extenderMAX14830SPI.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<PIEZO>(&extender, GedeeldeBusComponent::PIEZO, static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH0), &extenderMAX14830SPI, static_cast<uint8_t>(ExtenderMAX14830SPI::ExtenderPins::EP_GPIO0), HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  // Hulppin(nen) via de tweede Extender.
  {
    static unsigned long laatsteRapport = 0;
    if (millis() - laatsteRapport >= 500UL) {
      laatsteRapport = millis();
      bool toestand = false;
      if (sensor->ReadDO(toestand)) {
        GA_SERIAL.print(F("DO: "));
        GA_SERIAL.println(toestand);
      } else {
        GA_SERIAL.println(F("DO: UITLEESFOUT"));
      }
    }
  }
  int32_t waarde = 0;
  if (sensor->RawAnalogRead(waarde)) {
    GA_SERIAL.print(F("PIEZO ruwe ADC-sample (geen fysieke eenheid): "));
    GA_SERIAL.println(waarde);
  } else {
    GA_SERIAL.println(F("ADC-UITLEESFOUT: geen geldige nieuwe meetwaarde (bereik, referentie en verbinding controleren)."));
  }
  // Een geslaagde ruwe uitlezing bewijst nog geen kalibratie of correcte fysieke meetfrequentie.
}
