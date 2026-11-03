#include "SpiraalPatroon.h"

// Dezelfde route als in de bestaande drie MAX7219-testsketches.
static const uint8_t spiraalRoute[64][2] = {
  {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0},
  {7,1}, {7,2}, {7,3}, {7,4}, {7,5}, {7,6}, {7,7}, {6,7},
  {5,7}, {4,7}, {3,7}, {2,7}, {1,7}, {0,7}, {0,6}, {0,5},
  {0,4}, {0,3}, {0,2}, {0,1}, {1,1}, {2,1}, {3,1}, {4,1},
  {5,1}, {6,1}, {6,2}, {6,3}, {6,4}, {6,5}, {6,6}, {5,6},
  {4,6}, {3,6}, {2,6}, {1,6}, {1,5}, {1,4}, {1,3}, {1,2},
  {2,2}, {3,2}, {4,2}, {5,2}, {5,3}, {5,4}, {5,5}, {4,5},
  {3,5}, {2,5}, {2,4}, {2,3}, {3,3}, {4,3}, {4,4}, {3,4}
};

bool BerekenSpiraalPatroon(uint8_t maat, int8_t& huidigeStap, bool isUp, uint8_t stapGrootte, bool wrapAround, bool laatSpoorAchter, uint8_t beeld[8]) {
  if (maat != 8 || beeld == nullptr || stapGrootte == 0 || stapGrootte > 64) return false;
  const int16_t vorig = huidigeStap;
  if (vorig < -1 || vorig > 63) return false;
  int16_t volgende = vorig + (isUp ? static_cast<int16_t>(stapGrootte) : -static_cast<int16_t>(stapGrootte));
  bool omgeslagen = false;
  if (volgende > 63) { volgende = wrapAround ? 0 : 63; omgeslagen = wrapAround; }
  if (volgende < -1) { volgende = wrapAround ? 63 : -1; omgeslagen = wrapAround; }
  if (volgende == vorig) return false;

  if (!laatSpoorAchter) {
    if (vorig >= 0) {
      const uint8_t x=spiraalRoute[vorig][0], y=spiraalRoute[vorig][1];
      beeld[y] &= static_cast<uint8_t>(~(1u<<x));
    }
    if (volgende >= 0) {
      const uint8_t x=spiraalRoute[volgende][0], y=spiraalRoute[volgende][1];
      beeld[y] |= static_cast<uint8_t>(1u<<x);
    }
  } else if (omgeslagen && isUp) {
    for (uint8_t y=0; y<maat; ++y) beeld[y]=0;
    const uint8_t x=spiraalRoute[0][0], y=spiraalRoute[0][1];
    beeld[y] |= static_cast<uint8_t>(1u<<x);
  } else if (omgeslagen) {
    for (uint8_t y=0; y<maat; ++y) beeld[y]=0;
    for (uint8_t i=0; i<64; ++i) {
      const uint8_t x=spiraalRoute[i][0], y=spiraalRoute[i][1];
      beeld[y] |= static_cast<uint8_t>(1u<<x);
    }
  } else if (isUp) {
    for (int16_t i=vorig+1; i<=volgende; ++i) {
      const uint8_t x=spiraalRoute[i][0], y=spiraalRoute[i][1];
      beeld[y] |= static_cast<uint8_t>(1u<<x);
    }
  } else {
    for (int16_t i=vorig; i>volgende; --i) {
      const uint8_t x=spiraalRoute[i][0], y=spiraalRoute[i][1];
      beeld[y] &= static_cast<uint8_t>(~(1u<<x));
    }
  }
  huidigeStap = static_cast<int8_t>(volgende);
  return true;
}
