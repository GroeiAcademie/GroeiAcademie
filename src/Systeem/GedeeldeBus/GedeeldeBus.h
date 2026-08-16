#ifndef GEDEELDE_BUS_H
#define GEDEELDE_BUS_H

#include <Arduino.h>

// ============================================================================
// GedeeldeBus: neutrale, gedeelde Wire(I2C)/SPI-initialisatie.
//
// AANLEIDING (v1.1.1-patch): Screen.cpp, Input.cpp (PCF8574-tak) en
// Stimulus.cpp (InitialiseerADS1115()) riepen elk onafhankelijk Wire.begin()
// aan, met identieke, maar apart onderhouden board-specifieke logica
// (ARDI32 gebruikt aparte SDA/SCL-pinnen, andere boards de standaard
// Wire.begin()). Bij gelijktijdig gebruik van bijvoorbeeld Input met PCF8574
// én een I2C-scherm was er geen garantie dat dit conflictvrij bleef.
//
// GEEN OVERKOEPELENDE KERNEL: Input, Screen en Stimulus blijven bewust
// onafhankelijke, los te testen subsystemen. GedeeldeBus kent geen van drie;
// ze regelt uitsluitend businitialisatie, verder niets.
//
// GEDRAGSBEHOUDEND: dit is een PATCH (v1.1.0 → v1.1.1), geen API-wijziging.
// De board-specifieke ARDI32-logica die voorheen apart in Screen.cpp en
// Input.cpp stond, is hier samengevoegd, met exact hetzelfde gedrag.
//
// GEBRUIK: elk subsysteem dat I2C nodig heeft, roept dit aan in plaats van
// zelf Wire.begin() aan te roepen. Meerdere aanroepen zijn veilig: enkel de
// eerste aanroep initialiseert de bus effectief.
// ============================================================================

enum class GedeeldeBusType : byte {
  I2C = 0,
  SPI = 1
};

// Initialiseert de opgegeven bus, enkel bij de eerste aanroep voor dat type.
// Veilig om meermaals aan te roepen vanuit verschillende subsystemen.
void GedeeldeBusInitialiseren(GedeeldeBusType busType);

// Enkel voor testdoeleinden/diagnose: geeft terug of een bus al geïnitialiseerd is.
bool GedeeldeBusIsGeinitialiseerd(GedeeldeBusType busType);

// Reset de interne "al geïnitialiseerd"-vlaggen. Beïnvloedt de fysieke bus
// zelf niet; enkel bedoeld voor testscenario's.
void GedeeldeBusResetten();

#endif // GEDEELDE_BUS_H
