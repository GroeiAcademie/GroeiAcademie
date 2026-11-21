// Sensor: OT3686 via MAX14830SPI: DO -> GPIO0 (ingang). Geen interrupt via deze Extender: pulsen tellen hoort bij de functies.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_OT3686_AANTAL != 1
  #error Zet SENSOR_OT3686_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_MAX14830_SPI_AANTAL == 0
  #error Zet EXTENDER_MAX14830_SPI_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if EXTENDER_MAX14830_BRONKLOK_HZ == 0
  #error Stel EXTENDER_MAX14830_BRONKLOK_HZ in op de werkelijke oscillatorfrequentie van jouw MAX14830-module voordat je deze test uitvoert.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_MAX14830_SPI_AANTAL == 1
ExtenderMAX14830SPI extender(
  &Native,
  GedeeldeBusComponent::UART_MAX14830,
  EXTENDER_MAX14830_SPI_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, EXTENDER_MAX14830_SPI_1_IRQ
);
#else
ExtenderMAX14830SPI extender(
  &Native,
  GedeeldeBusComponent::UART_MAX14830,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

OT3686* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_OT3686_MAX14830SPI"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<OT3686>(&extender, GedeeldeBusComponent::OT3686, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderMAX14830SPI::ExtenderPins::EP_GPIO0)));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (!sensor) return;
  sensor->PollPulseCount();   // geen interrupt via deze Extender: zo vaak mogelijk de ingang lezen
  float venster = NAN, periode = NAN;
  uint32_t aantal = 0;
  bool toestand = false;
  sensor->LeesRPMViaTijdvenster(venster);
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250) return;
  laatsteRapport = millis();
  sensor->LeesRPMViaPeriode(periode);
  sensor->LeesAantalPulsen(aantal);
  sensor->LeesDigitaleToestand(toestand);
  GA_SERIAL.print(F("DO=")); GA_SERIAL.print(toestand);
  GA_SERIAL.print(F(" pulsen=")); GA_SERIAL.print(aantal);
  GA_SERIAL.print(F(" RPM_venster=")); GA_SERIAL.print(venster);
  GA_SERIAL.print(F(" RPM_periode=")); GA_SERIAL.println(periode);
}
