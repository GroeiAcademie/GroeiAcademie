// Sensor: LD2410D via SC16IS752I2C.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// UART-kanaal 0. De bestaande Sensor-Activeren() stelt de kanaalbaudrate in. Controleer werkelijke moduleklok en RX/TX.

#include <GroeiAcademie.h>

#if SENSOR_LD2410D_AANTAL != 1
  #error Zet SENSOR_LD2410D_AANTAL op 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_SC16IS752_I2C_AANTAL == 0
  #error Zet EXTENDER_SC16IS752_I2C_AANTAL in UserConfig.h op minstens 1 voor deze test.
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

LD2410D* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_LD2410D_SC16IS752I2C"));
  GA_SERIAL.println(F("MEETTEST: Beweeg, sta stil en verlaat het bereik; controleer geldige radarframes."));
  sensor = GedeeldeBusNewComponent<LD2410D>(&extender, GedeeldeBusComponent::LD2410D, static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_CHANNEL_A), static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_GPIO0));   // OUT op GPIO0
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  // OUT (aanwezigheid) via GPIO0 van dezelfde Extender.
  {
    static unsigned long laatsteRapport = 0;
    if (millis() - laatsteRapport >= 500UL) {
      laatsteRapport = millis();
      bool toestand = false;
      if (sensor->ReadOUT(toestand)) {
        GA_SERIAL.print(F("OUT: "));
        GA_SERIAL.println(toestand);
      } else {
        GA_SERIAL.println(F("OUT: UITLEESFOUT"));
      }
    }
  }
  float waarde = NAN;
  if (sensor->LeesHoofdwaarde(waarde)) {
    GA_SERIAL.print(F("Aanwezigheid (0=geen, 1=beweging, 2=stil, 3=beide): "));
    GA_SERIAL.println(waarde);

  }
  // Geen volledig ontvangen/valide frame is geen fout in deze loop; volgende iteratie probeert opnieuw.
}
