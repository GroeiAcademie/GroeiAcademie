#include "GedeeldeBus.h"
#include <Wire.h>
#include <SPI.h>
#include "../../Configuratie/SystemConfig.h"

static bool i2cGeinitialiseerd = false;
static bool spiGeinitialiseerd = false;

void GedeeldeBusInitialiseren(GedeeldeBusType busType) {
  if (busType == GedeeldeBusType::I2C) {
    if (!i2cGeinitialiseerd) {
      // Board-specifieke SDA/SCL-logica, ongewijzigd overgenomen uit de eerdere, aparte implementaties in Screen.cpp en Input.cpp.
      #if BOARD_VERSION == BOARD_ARDI32
        Wire.begin(ARDUINO_UNO_SHIELD_PIN_SDA, ARDUINO_UNO_SHIELD_PIN_SCL);
      #else
        Wire.begin();
      #endif
      i2cGeinitialiseerd = true;
    }
  } else if (busType == GedeeldeBusType::SPI) {
    if (!spiGeinitialiseerd) {
      SPI.begin();
      spiGeinitialiseerd = true;
    }
  }
}

bool GedeeldeBusIsGeinitialiseerd(GedeeldeBusType busType) {
  if (busType == GedeeldeBusType::I2C) return i2cGeinitialiseerd;
  if (busType == GedeeldeBusType::SPI) return spiGeinitialiseerd;
  return false;
}

void GedeeldeBusResetten() {
  i2cGeinitialiseerd = false;
  spiGeinitialiseerd = false;
}
