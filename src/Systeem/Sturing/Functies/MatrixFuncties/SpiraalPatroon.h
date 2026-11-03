#ifndef SPIRAALPATROON_H
#define SPIRAALPATROON_H

#include <Arduino.h>

// Interne patroonberekening. Voorlopig gebruikt de bestaande test een 8x8-matrix.
// beeld bevat de gewenste pixeltoestand; 8 bytes, een bit per pixel.
// De berekening kent geen sturing, bus, pin of hardwareprotocol.
bool BerekenSpiraalPatroon(uint8_t maat, int8_t& huidigeStap, bool isUp, uint8_t stapGrootte, bool wrapAround, bool laatSpoorAchter, uint8_t beeld[8]);

#endif
