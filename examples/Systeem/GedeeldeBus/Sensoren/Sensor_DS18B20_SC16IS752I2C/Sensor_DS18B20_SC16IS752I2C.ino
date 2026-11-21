// Sensor: DS18B20 via SC16IS752I2C: 1-Wire via UART-channel CHANNEL_A (Maxim AN214: reset 9600 baud, bits 115200 baud).
// Bedrading: TX en RX van het channel samen op DQ via een open-drain schakeling (bv. diode van TX naar DQ), pull-up 4,7 kohm op DQ.
// Eén DS18B20 per channel: Activeren zoekt met READ ROM.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_DS18B20_AANTAL != 1
  #error Zet SENSOR_DS18B20_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_SC16IS752_I2C_AANTAL == 0
  #error Zet EXTENDER_SC16IS752_I2C_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_SC16IS752_I2C_AANTAL == 1
ExtenderSC16IS752I2C extender(
  &Native,
  GedeeldeBusComponent::UART_SC16IS752,
  I2C_ADDRESS_EXTENDER_SC16IS752_I2C_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_SC16IS752_I2C_1_IRQ, EXTENDER_SC16IS752_I2C_1_RESET
);
#else
ExtenderSC16IS752I2C extender(
  &Native,
  GedeeldeBusComponent::UART_SC16IS752,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

DS18B20* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_DS18B20_SC16IS752I2C"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<DS18B20>(&extender, GedeeldeBusComponent::DS18B20, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_CHANNEL_A)));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  if (!sensor->temperatuurmetingGestart && !sensor->StartTemperatuurmeting()) {
    GA_SERIAL.println(F("Temperatuurmeting starten: FOUT"));
    return;
  }
  if (!sensor->IsTemperatuurmetingKlaar()) return;
  float waarde = NAN;
  if (sensor->LeesHoofdwaarde(waarde)) {
    GA_SERIAL.print(F("Temperatuur (C): "));
    GA_SERIAL.println(waarde);
  } else {
    GA_SERIAL.println(F("Temperatuur-uitlezing: FOUT"));
  }
}
