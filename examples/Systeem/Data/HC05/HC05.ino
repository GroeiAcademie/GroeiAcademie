// ============================================================================
// Data: HC-05 (Bluetooth)
// ============================================================================
// De HC-05 gedraagt zich als een seriële poort: wat je naar GA_SENSOR_SERIAL schrijft,
// komt draadloos aan op een gsm of pc (seriële Bluetooth-app of seriële monitor).
// Geen eigen klasse nodig.
// HC-05 TXD gaat naar GA_SENSOR_SERIAL_PIN_RX; HC-05 RXD gaat naar GA_SENSOR_SERIAL_PIN_TX.
// De pinnen per board staan bij GA_SENSOR_SERIAL in SystemConfig.h.
// De RXD van de HC-05 verwacht 3,3 V: op een board met 5 V-logica via een spanningsdeler.
// VCC 5 V, GND; standaardsnelheid van de HC-05: 9600 baud.
// UNO Q: bij elke reset stuurt Zephyr een opstartmelding over Serial1; die komt ook op de gsm.
// Via een UART-Extender (SC16IS752, MAX14830) kan hetzelfde met GetExtenderStream() en StartExtenderUART().
// Classic Bluetooth: werkt niet met een iPhone.
// ============================================================================

#include <GroeiAcademie.h>

#if BOARD_VERSION == BOARD_STM32F4_NUCLEO64_F401RE
// De STM32-core maakt standaard alleen Serial aan.
// Daarom maakt dit sketch Serial1 aan op de pinnen van GA_SENSOR_SERIAL.
Uart Serial1(NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_RX), NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_TX));
#endif

const unsigned long baudHC05 = 9600;
unsigned long tijdLaatsteBericht = 0;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: HC05"));
  #if defined(ARDUINO_ARCH_ESP32)
  GA_SENSOR_SERIAL.begin(baudHC05, SERIAL_8N1, NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_RX), NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_TX));
  #else
  GA_SENSOR_SERIAL.begin(baudHC05);
  #endif
  GA_SERIAL.println(F("HC-05: GA_SENSOR_SERIAL gestart"));
}

void loop() {
  // Elke seconde een bericht naar de gsm.
  if (millis() - tijdLaatsteBericht >= 1000UL) {
    tijdLaatsteBericht = millis();
    GA_SENSOR_SERIAL.print(F("GroeiAcademie: "));
    GA_SENSOR_SERIAL.println(millis() / 1000UL);
  }
  // Wat de gsm stuurt, verschijnt in de seriële monitor.
  while (GA_SENSOR_SERIAL.available()) GA_SERIAL.write(GA_SENSOR_SERIAL.read());
}
