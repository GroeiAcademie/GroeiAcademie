#include <GroeiAcademie.h>
#if SENSOR_SDP810125PA_AANTAL != 1
#error SENSOR_SDP810125PA_AANTAL moet 1 zijn voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze test.
#endif
HardwareResourceTypeI2C apparaat(&Native, GedeeldeBusComponent::SDP810125PA, 0x25,
 HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
void setup() {
 GA_SERIAL.begin(SERIAL_BAUDRATE);
 const unsigned long startTijd = millis();
 while (!GA_SERIAL && millis()-startTijd<SERIAL_CONNECT_TIMEOUT_MS) { ; }
 bool parentOK = apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
 GA_SERIAL.println(parentOK ? F("PARENT: OK") : F("PARENT: FOUT"));
 if (!parentOK) return;
 SDP810125PA* sensor = GedeeldeBusNewComponent<SDP810125PA>(&apparaat,GedeeldeBusComponent::SDP810125PA);
 GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
 
}
void loop() {}
