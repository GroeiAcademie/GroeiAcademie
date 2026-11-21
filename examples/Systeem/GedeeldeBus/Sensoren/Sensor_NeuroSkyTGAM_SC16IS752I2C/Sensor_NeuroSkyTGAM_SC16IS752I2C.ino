// Sensor: NeuroSkyTGAM via SC16IS752I2C.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// UART-kanaal 0. De bestaande Sensor-Activeren() stelt de kanaalbaudrate in. Controleer werkelijke moduleklok en RX/TX.

#include <GroeiAcademie.h>

#if SENSOR_NEUROSKYTGAM_AANTAL != 1
  #error Zet SENSOR_NEUROSKYTGAM_AANTAL op 1 voor deze test.
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

NeuroSkyTGAM* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_NeuroSkyTGAM_SC16IS752I2C"));
  GA_SERIAL.println(F("MEETTEST: Controleer valide attention, meditation en EEG-banden bij goed contact."));
  sensor = GedeeldeBusNewComponent<NeuroSkyTGAM>(&extender, GedeeldeBusComponent::NeuroSkyTGAM, static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_CHANNEL_A));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  float waarde = NAN;
  if (sensor->LeesHoofdwaarde(waarde)) {
    GA_SERIAL.print(F("Attention (0-100): "));
    GA_SERIAL.println(waarde);
    if (sensor->LeesMeditatie(waarde)) {
      GA_SERIAL.print(F("Meditation (0-100): ")); GA_SERIAL.println(waarde);
    }
    uint32_t banden[8] = {};
    if (sensor->LeesEEGBanden(banden)) {
      GA_SERIAL.print(F("EEG delta/theta/lowalpha/highalpha/lowbeta/highbeta/lowgamma/midgamma: "));
      for (uint8_t i = 0; i < 8; ++i) { GA_SERIAL.print(banden[i]); GA_SERIAL.print(' '); }
      GA_SERIAL.println();
    }
  }
  // Geen volledig ontvangen/valide frame is geen fout in deze loop; volgende iteratie probeert opnieuw.
}
