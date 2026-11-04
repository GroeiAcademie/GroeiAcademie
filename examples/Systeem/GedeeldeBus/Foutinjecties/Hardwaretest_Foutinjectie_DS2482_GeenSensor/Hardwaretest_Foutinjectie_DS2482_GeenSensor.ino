// ============================================================================
// Foutinjectie: DS2482_GeenSensor
// ============================================================================
// Negatieve One-Wire-contracttest: DS2482-800 is aangesloten, maar het
// opgegeven 1-Wire-channel heeft GEEN DS18B20. Verwijder de DS18B20 voor setup.
// Verwacht: bridge actief, DS18B20::Activeren() geeft false.
// ============================================================================

#include <GroeiAcademie.h>
#if EXTENDER_DS2482_800_AANTAL != 1 || SENSOR_DS18B20_AANTAL == 0
#error Vereist EXTENDER_DS2482_800_AANTAL=1 en SENSOR_DS18B20_AANTAL=1.
#endif
ExtenderDS2482v800 bridge(
  &Native, GedeeldeBusComponent::ONE_WIRE_DS2482v800,
  I2C_ADDRESS_EXTENDER_DS2482_800_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);
DS18B20 sensor(&bridge, GedeeldeBusComponent::DS18B20,
                static_cast<HardwareResourcePin>(0));
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Hardwaretest_Foutinjectie_DS2482_GeenSensor"));
  bool gelukt = bridge.aanmelden();
  if (gelukt) gelukt = bridge.controleren();
  if (gelukt) gelukt = bridge.inpluggen();
  if (gelukt) gelukt = bridge.activeren();
  if (!gelukt) { GA_SERIAL.println(F("RESULTAAT: FOUT (bridge)")); return; }
  const bool geenSensor = !sensor.Activeren();
  GA_SERIAL.println(geenSensor ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
