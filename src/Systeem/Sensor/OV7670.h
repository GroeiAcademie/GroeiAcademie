#ifndef SENSOR_OV7670_H
#define SENSOR_OV7670_H
#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"
#if SENSOR_OV7670_AANTAL > 0
#if BOARD_VERSION != BOARD_ESP32S3_ARDI32 && BOARD_VERSION != BOARD_ESP32S3_DEV
#error OV7670 is in het FrameWork uitsluitend voor ESP32-S3 ARDI32 of DEV voorzien.
#endif
#include <esp_camera.h>
#include "../GedeeldeBus/GedeeldeBus.h"

// pinnen[0..15]: XCLK,SCCB_SDA,SCCB_SCL,D0..D7,VSYNC,HREF,PCLK,PWDN,RESET.
// Bij de AL422B-uitvoering: pinnen[16..19] = WRST,RRST,WEN,RCLK.
// D0..D7 zijn in die uitvoering de FIFO-uitgangen; de sensor-PCLK stuurt de
// FIFO-WCK fysiek aan (geen softwarematige WCK-pin). OE moet laag zijn.
struct OV7670 : Sensor {
  camera_config_t cameraConfig;
  const bool fifoActief;
  uint8_t gedeeld_[2];
  uint8_t exclusief_[18];

private:
  OV7670(GedeeldeBusNode* parent, GedeeldeBusComponent component,
         const camera_config_t& cameraConfig, const HardwareResourcePin* pinnen,
         bool fifoActief)
    : Sensor(parent, {gedeeld_, 2, exclusief_, 18}, component, HardwareResourceToegang::GEDEELD),
      cameraConfig(cameraConfig), fifoActief(fifoActief) {
    gedeeldeIdentiteiten = GedeeldeBusIdentiteitI2C;
    gedeeld_[0] = static_cast<uint8_t>(pinnen[1]);
    gedeeld_[1] = static_cast<uint8_t>(pinnen[2]);
    exclusief_[0] = static_cast<uint8_t>(pinnen[0]);
    for (uint8_t n = 3; n < 16; ++n) exclusief_[n - 2] = static_cast<uint8_t>(pinnen[n]);
    for (uint8_t n = 14; n < 18; ++n)
      exclusief_[n] = static_cast<uint8_t>(HardwareResourcePin::NONE);
  }

public:
  // Bestaande constructor: ongewijzigd voor OV7670 zonder FIFO.
  OV7670(GedeeldeBusNode* parent, GedeeldeBusComponent component,
         const camera_config_t& cameraConfig, const HardwareResourcePin (&pinnen)[16],
         bool fifoActief = false)
    : OV7670(parent, component, cameraConfig, &pinnen[0], fifoActief) {}

  // FIFO-variant: dezelfde pinnen-array, uitgebreid met 4 besturingspinnen.
  OV7670(GedeeldeBusNode* parent, GedeeldeBusComponent component,
         const camera_config_t& cameraConfig, const HardwareResourcePin (&pinnen)[20])
    : OV7670(parent, component, cameraConfig, &pinnen[0], true) {
    for (uint8_t n = 0; n < 4; ++n)
      exclusief_[14 + n] = static_cast<uint8_t>(pinnen[16 + n]);
  }

  bool inpluggen() override;
  bool Activeren() override;
  bool afmelden() override;
  bool LeesBeeld(camera_fb_t*& beeld);
  bool BewaarBeeldOpMicroSD(const char* naam);
};
#endif
#endif
