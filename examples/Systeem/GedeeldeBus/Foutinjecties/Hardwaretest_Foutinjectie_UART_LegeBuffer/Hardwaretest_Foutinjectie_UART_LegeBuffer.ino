// Negatieve UART-contracttest: geen RX-data op SC16IS752-channel A.
// Verwacht: .available()==0, .read()==-1; eerst bridge fysiek aansluiten.
#include <GroeiAcademie.h>
#if EXTENDER_SC16IS752_I2C_AANTAL != 1
#error Vereist EXTENDER_SC16IS752_I2C_AANTAL=1 voor deze test.
#endif
ExtenderSC16IS752I2C extender(
  &Native, GedeeldeBusComponent::UART_SC16IS752,
  I2C_ADDRESS_EXTENDER_SC16IS752_I2C_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_SC16IS752_I2C_1_IRQ, EXTENDER_SC16IS752_I2C_1_RESET);
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  bool gelukt = extender.aanmelden();
  if (gelukt) gelukt = extender.controleren();
  if (gelukt) gelukt = extender.inpluggen();
  if (gelukt) gelukt = extender.activeren();
  if (!gelukt) { GA_SERIAL.println(F("RESULTAAT: FOUT (bridge)")); return; }
  const bool leeg = extender.StreamChannelA().available() == 0 &&
                     extender.StreamChannelA().read() == -1;
  GA_SERIAL.println(leeg ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
