// Sensor: MR60BHA2 via MAX14830SPI.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// UART-kanaal 0. De bestaande Sensor-Activeren() stelt de kanaalbaudrate in. Controleer werkelijke moduleklok en RX/TX.

#include <GroeiAcademie.h>

#if SENSOR_MR60BHA2_AANTAL != 1
  #error Zet SENSOR_MR60BHA2_AANTAL op 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_MAX14830_BRONKLOK_HZ == 0
  #error Stel EXTENDER_MAX14830_BRONKLOK_HZ in op de werkelijke oscillatorfrequentie van jouw MAX14830-module voordat je deze test uitvoert.
#endif

#if EXTENDER_MAX14830_SPI_AANTAL == 0
  #error Zet EXTENDER_MAX14830_SPI_AANTAL in UserConfig.h op minstens 1 voor deze test.
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

MR60BHA2* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MR60BHA2_MAX14830SPI"));
  GA_SERIAL.println(F("MEETTEST: Controleer ontvangen radarmeting, niet een zelf afgeleide hartslag."));
  sensor = GedeeldeBusNewComponent<MR60BHA2>(&extender, GedeeldeBusComponent::MR60BHA2, static_cast<uint8_t>(ExtenderMAX14830SPI::ExtenderPins::EP_UART0));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  float waarde = NAN;
  if (sensor->LeesHoofdwaarde(waarde)) {
    GA_SERIAL.print(F("Radarwaarde (eenheid volgens firmware): "));
    GA_SERIAL.println(waarde);

  }
  // Geen volledig ontvangen/valide frame is geen fout in deze loop; volgende iteratie probeert opnieuw.
}
