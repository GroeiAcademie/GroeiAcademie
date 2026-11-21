// Sensor: LD2450 via MAX14830I2C.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// UART-kanaal 0. De bestaande Sensor-Activeren() stelt de kanaalbaudrate in. Controleer werkelijke moduleklok en RX/TX.

#include <GroeiAcademie.h>

#if SENSOR_LD2450_AANTAL != 1
  #error Zet SENSOR_LD2450_AANTAL op 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_MAX14830_BRONKLOK_HZ == 0
  #error Stel EXTENDER_MAX14830_BRONKLOK_HZ in op de werkelijke oscillatorfrequentie van jouw MAX14830-module voordat je deze test uitvoert.
#endif

#if EXTENDER_MAX14830_I2C_AANTAL == 0
  #error Zet EXTENDER_MAX14830_I2C_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_MAX14830_I2C_AANTAL == 1
ExtenderMAX14830I2C extender(
  &Native,
  GedeeldeBusComponent::UART_MAX14830,
  I2C_ADDRESS_EXTENDER_MAX14830_I2C_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_MAX14830_I2C_1_IRQ
);
#else
ExtenderMAX14830I2C extender(
  &Native,
  GedeeldeBusComponent::UART_MAX14830,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

LD2450* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_LD2450_MAX14830I2C"));
  GA_SERIAL.println(F("MEETTEST: Beweeg en controleer X, Y en afstand uit geldige radarframes."));
  sensor = GedeeldeBusNewComponent<LD2450>(&extender, GedeeldeBusComponent::LD2450, static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_UART0));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  float waarde = NAN;
  if (sensor->LeesHoofdwaarde(waarde)) {
    GA_SERIAL.print(F("Afstand dichtstbijzijnde doel (mm): "));
    GA_SERIAL.println(waarde);
    float x = NAN;
    float y = NAN;
    if (sensor->LeesPositie(x, y)) {
      GA_SERIAL.print(F("X (mm): ")); GA_SERIAL.println(x);
      GA_SERIAL.print(F("Y (mm): ")); GA_SERIAL.println(y);
    } else {
      GA_SERIAL.println(F("Positie: GEEN GELDIG DOEL"));
    }
  }
  // Geen volledig ontvangen/valide frame is geen fout in deze loop; volgende iteratie probeert opnieuw.
}
