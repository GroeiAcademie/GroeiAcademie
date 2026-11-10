#include "AHT10.h"
#if SENSOR_AHT10_AANTAL > 0
uint8_t AHT10::LeesStatus() {
  HardwareResourceTypeI2C* const apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (Wire.requestFrom(apparaat->adres, static_cast<uint8_t>(1)) != 1) return 0xFF;
  return static_cast<uint8_t>(Wire.read());
}

bool AHT10::Activeren() {
  if (!parent || !parent->actief) return false;
  HardwareResourceTypeI2C* const apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  metingGestart = false;
  // De officiële AHTX0-initialisatiecommando's, maar zonder de onbegrensde BUSY-lussen van begin().
  delay(20);
  Wire.beginTransmission(apparaat->adres);
  Wire.write(static_cast<uint8_t>(AHTX0_CMD_SOFTRESET));
  if (Wire.endTransmission() != 0) return false;

  delay(20);
  unsigned long starttijdWachtlus = millis();
  uint8_t sensorStatus = LeesStatus();
  while (sensorStatus != 0xFF && (sensorStatus & AHTX0_STATUS_BUSY) && millis() - starttijdWachtlus < 200UL) {
    delay(10);
    sensorStatus = LeesStatus();
  }
  if (sensorStatus == 0xFF || (sensorStatus & AHTX0_STATUS_BUSY)) return false;

  Wire.beginTransmission(apparaat->adres);
  Wire.write(static_cast<uint8_t>(AHTX0_CMD_CALIBRATE));
  Wire.write(static_cast<uint8_t>(0x08));
  Wire.write(static_cast<uint8_t>(0x00));
  (void)Wire.endTransmission();   // zoals bij Adafruit: sommige AHT20-uitvoeringen bevestigen dit niet

  starttijdWachtlus = millis();
  sensorStatus = LeesStatus();
  while (sensorStatus != 0xFF && (sensorStatus & AHTX0_STATUS_BUSY) && millis() - starttijdWachtlus < 200UL) {
    delay(10);
    sensorStatus = LeesStatus();
  }

  return sensorStatus != 0xFF && !(sensorStatus & AHTX0_STATUS_BUSY) && (sensorStatus & AHTX0_STATUS_CALIBRATED);
}

// Runtime-metingen gebruiken de bestaande I2C-toegangspoort, ook achter TCA9548A.
bool AHT10::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (parent == nullptr || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  // Het officiële triggercommando verzenden en later, zonder BUSY-wachtlus, de zes meetbytes ophalen.
  if (!metingGestart) {
    Wire.beginTransmission(apparaat->adres);
    Wire.write(static_cast<uint8_t>(AHTX0_CMD_TRIGGER));
    Wire.write(static_cast<uint8_t>(0x33));
    Wire.write(static_cast<uint8_t>(0x00));
    metingGestart = Wire.endTransmission() == 0;
    tijdstipMetingGestart = millis();
    if (!metingGestart) apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
    return false;
  }

  const uint8_t sensorStatus = LeesStatus();
  if (sensorStatus != 0xFF && (sensorStatus & AHTX0_STATUS_BUSY) && millis() - tijdstipMetingGestart < 200UL) return false;

  metingGestart = false;
  uint8_t bufferMeetgegevens[6] = {0};
  const bool gelukt = sensorStatus != 0xFF && !(sensorStatus & AHTX0_STATUS_BUSY) && Wire.requestFrom(apparaat->adres, static_cast<uint8_t>(6)) == 6;
  if (gelukt) {
    for (uint8_t byteIndex = 0; byteIndex < 6; ++byteIndex) bufferMeetgegevens[byteIndex] = static_cast<uint8_t>(Wire.read());
  }

  float relatieveVochtigheid = NAN;
  if (gelukt) {
    // Dezelfde twee 20-bit-omrekeningen als Adafruit_AHTX0::getEvent().
    const uint32_t ruweVochtigheid = (static_cast<uint32_t>(bufferMeetgegevens[1]) << 12) | (static_cast<uint32_t>(bufferMeetgegevens[2]) << 4) | (bufferMeetgegevens[3] >> 4);
    const uint32_t ruweTemperatuur = (static_cast<uint32_t>(bufferMeetgegevens[3] & 0x0F) << 16) | (static_cast<uint32_t>(bufferMeetgegevens[4]) << 8) | bufferMeetgegevens[5];
    relatieveVochtigheid = static_cast<float>(ruweVochtigheid) * 100.0f / 1048576.0f;
    waarde = static_cast<float>(ruweTemperatuur) * 200.0f / 1048576.0f - 50.0f;
  }
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) {
    WriteQueueOrStack(1, waarde);
    WriteQueueOrStack(2, relatieveVochtigheid);
  }
#endif
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
  return gelukt;
}
#endif
