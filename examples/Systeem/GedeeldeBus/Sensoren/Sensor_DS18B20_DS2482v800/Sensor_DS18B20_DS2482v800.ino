// Sensor: DS18B20 via DS2482v800, 1-Wire-kanaal IO0.
// Officieel Adafruit_DS248x-bridgecontract wordt binnen de bestaande FrameWork-driver gebruikt.
// DS18B20 DQ naar IO0, volgens de officiële DS18B20-voedings- en pull-up-eisen.
// Een fysieke Sensor op IO0 is noodzakelijk voor een geslaagde Activeren().

#include <GroeiAcademie.h>

#if SENSOR_DS18B20_AANTAL != 1
  #error Zet SENSOR_DS18B20_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_DS2482_800_AANTAL == 0
  #error Zet EXTENDER_DS2482_800_AANTAL op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_DS2482_800_AANTAL == 1
ExtenderDS2482v800 extender(&Native, GedeeldeBusComponent::ONE_WIRE_DS2482v800, I2C_ADDRESS_EXTENDER_DS2482_800_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);
#else
ExtenderDS2482v800 extender(&Native, GedeeldeBusComponent::ONE_WIRE_DS2482v800, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0);
#endif

DS18B20* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_DS18B20_DS2482v800"));
  GA_SERIAL.println(F("MEETTEST: Verwarm de probe licht en controleer temperatuurevolutie."));
  const bool parentOK = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(parentOK ? F("EXTENDER: OK") : F("EXTENDER: FOUT"));
  if (!parentOK) return;
  sensor = GedeeldeBusNewComponent<DS18B20>(&extender, GedeeldeBusComponent::DS18B20, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderDS2482v800::ExtenderPins::EP_IO0)));
  GA_SERIAL.println(sensor ? F("SENSOR: OK") : F("SENSOR: FOUT"));
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
