// ============================================================================
// Data: HC-05 (Bluetooth)
// ============================================================================
// De HC-05 gedraagt zich als een seriële poort: wat je naar Serial1 schrijft, komt draadloos aan
// op een gsm of pc (seriële Bluetooth-app of seriële monitor). Geen eigen klasse nodig.
// Aansluiten op de UNO R4: HC-05 TXD naar D0 (RX1), HC-05 RXD naar D1 (TX1) via een spanningsdeler
// (de RXD van de HC-05 verwacht 3,3 V), VCC 5 V, GND. Standaardsnelheid van de HC-05: 9600 baud.
// Via een UART-Extender (SC16IS752, MAX14830) kan hetzelfde met GetExtenderStream() en StartExtenderUART().
// Classic Bluetooth: werkt niet met een iPhone.
// ============================================================================

#include <GroeiAcademie.h>

#if !defined(ARDUINO_ARCH_RENESAS)
  #error Deze test gebruikt Serial1 op D0/D1 van de UNO R4.
#endif

const unsigned long baudHC05 = 9600;
unsigned long tijdLaatsteBericht = 0;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: HC05"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  Serial1.begin(baudHC05);
  GA_SERIAL.println(F("HC-05: Serial1 gestart"));
}

void loop() {
  // Elke seconde een bericht naar de gsm.
  if (millis() - tijdLaatsteBericht >= 1000UL) {
    tijdLaatsteBericht = millis();
    Serial1.print(F("GroeiAcademie: "));
    Serial1.println(millis() / 1000UL);
  }
  // Wat de gsm stuurt, verschijnt in de seriële monitor.
  while (Serial1.available()) GA_SERIAL.write(Serial1.read());
}
