// Sensor: MAX4466_9814_ADS1158_Verwerking
// Verwerking: geluidsniveau (RMS en piek-tot-piek) op het vaste ritme.
// Dit zijn twee alternatieve fysieke modules, nooit tegelijk op dezelfde Sensor-ingang.
// Stel in UserConfig.h exact één van SENSOR_MAX4466_AANTAL en SENSOR_MAX9814_AANTAL op 1.
// Analoge OUT -> ADC-kanaal 0; voedingsspanning en ADC-referentie controleren.

#include <GroeiAcademie.h>

#if (SENSOR_MAX4466_AANTAL + SENSOR_MAX9814_AANTAL) != 1
  #error Kies precies een module: SENSOR_MAX4466_AANTAL=1 OF SENSOR_MAX9814_AANTAL=1.
#endif
#if EXTENDER_ADS1158_AANTAL == 0
  #error Zet EXTENDER_ADS1158_AANTAL op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE && DATA_LAYER_CONFIG != DATA_TYPE_DIRECT
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE of DATA_TYPE_DIRECT voor deze test.
#endif

#if EXTENDER_ADS1158_AANTAL == 1
ExtenderADS1158 extender(
  &Native, GedeeldeBusComponent::ADC_ADS1158, EXTENDER_ADS1158_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, EXTENDER_ADS1158_1_START, EXTENDER_ADS1158_1_RESET, EXTENDER_ADS1158_1_PWDN
);
#else
ExtenderADS1158 extender(
  &Native, GedeeldeBusComponent::ADC_ADS1158, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

MAX4466_9814* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MAX4466_9814_ADS1158_Verwerking"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
  GA_SERIAL.println(F("MEETTEST: Produceer geluid en controleer RMS en piek-tot-piek (ADC-waarde, geen dB)."));
#if SENSOR_MAX4466_AANTAL == 1
  sensor = GedeeldeBusNewComponent<MAX4466_9814>(&extender, GedeeldeBusComponent::MAX4466, static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN0), HardwareResourceToegang::GEDEELD);
#else
  sensor = GedeeldeBusNewComponent<MAX4466_9814>(&extender, GedeeldeBusComponent::MAX9814, static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN0), static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_GPIO0), static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_GPIO1), HardwareResourceToegang::GEDEELD);   // GAIN GPIO0, AR GPIO1
#endif
  GA_SERIAL.println(sensor ? F("SENSOR: OK") : F("SENSOR: FOUT"));
}
void loop() {
  if (sensor == nullptr) return;
  // Zo vaak mogelijk oproepen: de functie neemt zelf een sample op het vaste ritme (SENSOR_MAX4466_SAMPLE_RATE_HZ of SENSOR_MAX9814_SAMPLE_RATE_HZ).
  float rootMeanSquare = NAN;
  const bool gelukt = sensor->ReadSoundLevelRootMeanSquare(rootMeanSquare);
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250UL) return;
  laatsteRapport = millis();
  int32_t peakToPeak = 0;
  if (!gelukt || !sensor->ReadSoundLevelPeakToPeak(peakToPeak)) return;
  GA_SERIAL.print(F("RMS (ADC-waarde): ")); GA_SERIAL.print(rootMeanSquare);
  GA_SERIAL.print(F("  Piek-tot-piek: ")); GA_SERIAL.println(peakToPeak);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  GA_SERIAL.print(F("Via Data, RMS (ADC-waarde): "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(2));
  GA_SERIAL.print(F("Via Data, piek-tot-piek: "));
  GA_SERIAL.println(sensor->ReadQueueOrStackValue(3));
#endif
}
