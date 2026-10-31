#include "OV7670.h"
#if SENSOR_OV7670_AANTAL > 0
#include <img_converters.h> // officiële Espressif camera-frameconversie: frame2bmp()
#include <stdlib.h>
#include <string.h>
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
#include "../Data/Data.h"
#endif

bool OV7670::inpluggen() {
  if (ingeplugd) return true;
  if (firstExtenderNode() != nullptr) return false;
  const int gpio[16] = {cameraConfig.pin_xclk, cameraConfig.pin_sccb_sda,
    cameraConfig.pin_sccb_scl, cameraConfig.pin_d0, cameraConfig.pin_d1,
    cameraConfig.pin_d2, cameraConfig.pin_d3, cameraConfig.pin_d4,
    cameraConfig.pin_d5, cameraConfig.pin_d6, cameraConfig.pin_d7,
    cameraConfig.pin_vsync, cameraConfig.pin_href, cameraConfig.pin_pclk,
    cameraConfig.pin_pwdn, cameraConfig.pin_reset};
  const uint8_t claims[16] = {exclusief_[0], gedeeld_[0], gedeeld_[1],
    exclusief_[1], exclusief_[2], exclusief_[3], exclusief_[4],
    exclusief_[5], exclusief_[6], exclusief_[7], exclusief_[8],
    exclusief_[9], exclusief_[10], exclusief_[11], exclusief_[12], exclusief_[13]};
  for (uint8_t n = 0; n < 16; ++n) {
    const HardwareResourcePin pin = static_cast<HardwareResourcePin>(claims[n]);
    if (gpio[n] < 0) {
      if (pin != HardwareResourcePin::NONE) return false;
    } else if (pin == HardwareResourcePin::NONE || pin == HardwareResourcePin::CUSTOM ||
               NativeArduinoPinVan(pin) != gpio[n]) return false;
  }
  if (cameraConfig.pixel_format == PIXFORMAT_JPEG) return false; // OV7670 comprimeert niet naar JPEG.
  if (fifoActief) {
    // AL422B is 393216 bytes: QVGA/RGB565 = 153600 bytes, VGA/RGB565 past NIET.
    if (cameraConfig.pixel_format != PIXFORMAT_RGB565 ||
        cameraConfig.frame_size != FRAMESIZE_QVGA) return false;
    for (uint8_t n = 14; n < 18; ++n) {
      const HardwareResourcePin pin = static_cast<HardwareResourcePin>(exclusief_[n]);
      if (pin == HardwareResourcePin::NONE || pin == HardwareResourcePin::CUSTOM) return false;
      const uint8_t gpioExtra = NativeArduinoPinVan(pin);
      if (gpioExtra == 0xFF) return false;
      for (uint8_t i = 0; i < 16; ++i)
        if (gpio[i] >= 0 && static_cast<int>(gpioExtra) == gpio[i]) return false;
      for (uint8_t i = 14; i < n; ++i)
        if (NativeArduinoPinVan(static_cast<HardwareResourcePin>(exclusief_[i])) == gpioExtra)
          return false;
    }
  }
  return Sensor::inpluggen();
}

bool OV7670::Activeren() {
  if (!ingeplugd || !parent || !parent->actief) return false;
  // De officiële Espressif-driver verzorgt XCLK, SCCB en de OV7670-registers.
  // Voor de FIFO worden de geheugenbesturingssignalen daarnaast apart ingesteld.
  if (esp_camera_init(&cameraConfig) != ESP_OK) return false;
  if (fifoActief) {
    const uint8_t wrst = NativeArduinoPinVan(static_cast<HardwareResourcePin>(exclusief_[14]));
    const uint8_t rrst = NativeArduinoPinVan(static_cast<HardwareResourcePin>(exclusief_[15]));
    const uint8_t wen  = NativeArduinoPinVan(static_cast<HardwareResourcePin>(exclusief_[16]));
    const uint8_t rclk = NativeArduinoPinVan(static_cast<HardwareResourcePin>(exclusief_[17]));
    pinMode(wrst, OUTPUT); pinMode(rrst, OUTPUT);
    pinMode(wen, OUTPUT); pinMode(rclk, OUTPUT);
    digitalWrite(wen, LOW); digitalWrite(wrst, HIGH);
    digitalWrite(rrst, HIGH); digitalWrite(rclk, LOW);
    const int gpio[8] = {cameraConfig.pin_d0, cameraConfig.pin_d1,
      cameraConfig.pin_d2, cameraConfig.pin_d3, cameraConfig.pin_d4,
      cameraConfig.pin_d5, cameraConfig.pin_d6, cameraConfig.pin_d7};
    for (uint8_t n = 0; n < 8; ++n) pinMode(gpio[n], INPUT);
    delay(1);
    digitalWrite(wrst, LOW); delayMicroseconds(1);
    digitalWrite(wrst, HIGH); delayMicroseconds(1);
    digitalWrite(rrst, LOW);
    for (uint8_t n = 0; n < 2; ++n) {
      digitalWrite(rclk, HIGH); delayMicroseconds(1);
      digitalWrite(rclk, LOW); delayMicroseconds(1);
    }
    digitalWrite(rrst, HIGH);
  }
  return true;
}

bool OV7670::LeesBeeld(camera_fb_t*& beeld) {
  beeld = nullptr;
  if (!actief || fifoActief) return false;
  beeld = esp_camera_fb_get();
  return beeld != nullptr;
  // De aanroeper moet het buffer na gebruik teruggeven via esp_camera_fb_return().
}

bool OV7670::BewaarBeeldOpMicroSD(const char* naam) {
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI)
  if (!actief || naam == nullptr || naam[0] == '\0' ||
      ::Data == nullptr || !(::Data->typesActief & DATA_TYPE_MICROSD_SPI))
    return false;
  DataMICROSDviaSPI* kanaal = (parent != nullptr &&
      parent->component == GedeeldeBusComponent::DATA_MICROSD)
      ? static_cast<DataMICROSDviaSPI*>(parent) : ::Data->MICROSDviaSPI[0];
  if (kanaal == nullptr || !kanaal->actief ||
      (kanaal->Extender == HardwareResourceToegang::EXCLUSIEF &&
       !kanaal->IsRuntimeEigenaar(this))) return false;
  const size_t lengte = strlen(naam);
  if (lengte < 4 || strcmp(naam + lengte - 4, ".bmp") != 0) return false;

  if (!fifoActief) {
    // Bestaande cameramodule ZONDER FIFO, bestaande Espressif-conversie.
    camera_fb_t* beeld = nullptr;
    if (!LeesBeeld(beeld)) return false;
    uint8_t* buf = nullptr;
    size_t buf_len = 0;
    const bool omgezet = frame2bmp(beeld, &buf, &buf_len);
    esp_camera_fb_return(beeld);
    if (!omgezet || buf == nullptr) { free(buf); return false; }
    if (!kanaal->ReserveerRuntime(this)) { free(buf); return false; }
    File32 bestand = kanaal->open(naam, O_WRONLY | O_CREAT | O_TRUNC);
    const bool gelukt = bestand && bestand.write(buf, buf_len) == buf_len;
    if (bestand) { bestand.flush(); bestand.close(); }
    if (kanaal->Extender != HardwareResourceToegang::EXCLUSIEF)
      kanaal->VrijgevenRuntime(this);
    free(buf);
    return gelukt;
  }

  // FIFO-module OV7670/AL422B. WCK is fysiek aan de OV7670-PCLK gekoppeld,
  // /OE is laag en WEN is actief hoog (ArduCAM OV7670FIFO-schema).
  // QVGA RGB565: 320 * 240 * 2 = 153600 bytes < 393216 bytes FIFO.
  const uint8_t wrst = NativeArduinoPinVan(static_cast<HardwareResourcePin>(exclusief_[14]));
  const uint8_t rrst = NativeArduinoPinVan(static_cast<HardwareResourcePin>(exclusief_[15]));
  const uint8_t wen  = NativeArduinoPinVan(static_cast<HardwareResourcePin>(exclusief_[16]));
  const uint8_t rclk = NativeArduinoPinVan(static_cast<HardwareResourcePin>(exclusief_[17]));
  const int vsync = cameraConfig.pin_vsync;

  // Eerste VSYNC: begin frame, schrijfpointer resetten, WEN aan.
  digitalWrite(wen, LOW);
  if (pulseIn(vsync, LOW, 1000000UL) == 0) return false;
  digitalWrite(wrst, LOW); delayMicroseconds(6);
  digitalWrite(wrst, HIGH); delayMicroseconds(6);
  digitalWrite(wen, HIGH);
  // Volgende VSYNC: einde van één volledig opgenomen frame.
  const unsigned long frameSync = pulseIn(vsync, LOW, 1000000UL);
  digitalWrite(wen, LOW);
  if (frameSync == 0) return false;
  delay(2);

  if (!kanaal->ReserveerRuntime(this)) return false;
  File32 bestand = kanaal->open(naam, O_WRONLY | O_CREAT | O_TRUNC);
  if (!bestand) {
    if (kanaal->Extender != HardwareResourceToegang::EXCLUSIEF)
      kanaal->VrijgevenRuntime(this);
    return false;
  }

  // BMP24, top-down (negatieve hoogte): direct per rij naar microSD,
  // zonder het gehele beeld in RAM te laden.
  const uint32_t breedte = 320, hoogte = 240;
  const uint32_t beeldbytes = breedte * hoogte * 3;
  uint8_t kop[54] = {};
  kop[0] = 'B'; kop[1] = 'M';
  const uint32_t bestandsgrootte = 54 + beeldbytes;
  kop[2] = static_cast<uint8_t>(bestandsgrootte);
  kop[3] = static_cast<uint8_t>(bestandsgrootte >> 8);
  kop[4] = static_cast<uint8_t>(bestandsgrootte >> 16);
  kop[5] = static_cast<uint8_t>(bestandsgrootte >> 24);
  kop[10] = 54; kop[14] = 40;
  kop[18] = static_cast<uint8_t>(breedte);
  kop[19] = static_cast<uint8_t>(breedte >> 8);
  const int32_t topDown = -static_cast<int32_t>(hoogte);
  kop[22] = static_cast<uint8_t>(topDown);
  kop[23] = static_cast<uint8_t>(topDown >> 8);
  kop[24] = static_cast<uint8_t>(topDown >> 16);
  kop[25] = static_cast<uint8_t>(topDown >> 24);
  kop[26] = 1; kop[28] = 24;
  kop[34] = static_cast<uint8_t>(beeldbytes);
  kop[35] = static_cast<uint8_t>(beeldbytes >> 8);
  kop[36] = static_cast<uint8_t>(beeldbytes >> 16);
  kop[37] = static_cast<uint8_t>(beeldbytes >> 24);

  bool gelukt = bestand.write(kop, sizeof(kop)) == sizeof(kop);
  if (gelukt) {
    // ArduCAM: RRST laag tijdens drie RCLK-pulsen, daarna pointer lezen.
    digitalWrite(rrst, LOW);
    for (uint8_t i = 0; i < 3; ++i) {
      digitalWrite(rclk, HIGH); delayMicroseconds(1);
      digitalWrite(rclk, LOW); delayMicroseconds(1);
    }
    digitalWrite(rrst, HIGH);
    const int gpio[8] = {cameraConfig.pin_d0, cameraConfig.pin_d1,
      cameraConfig.pin_d2, cameraConfig.pin_d3, cameraConfig.pin_d4,
      cameraConfig.pin_d5, cameraConfig.pin_d6, cameraConfig.pin_d7};
    uint8_t rij[320 * 3];
    for (uint32_t y = 0; y < hoogte && gelukt; ++y) {
      for (uint32_t x = 0; x < breedte; ++x) {
        uint16_t pixel = 0;
        for (uint8_t deel = 0; deel < 2; ++deel) {
          digitalWrite(rclk, HIGH); delayMicroseconds(1);
          digitalWrite(rclk, LOW); delayMicroseconds(1);
          uint8_t octet = 0;
          for (uint8_t bit = 0; bit < 8; ++bit)
            if (digitalRead(gpio[bit])) octet |= static_cast<uint8_t>(1U << bit);
          pixel = static_cast<uint16_t>((pixel << 8) | octet);
        }
        // OV7670 RGB565 -> BMP24 B,G,R.
        rij[x * 3] = static_cast<uint8_t>((pixel & 0x1F) << 3);
        rij[x * 3 + 1] = static_cast<uint8_t>(((pixel >> 5) & 0x3F) << 2);
        rij[x * 3 + 2] = static_cast<uint8_t>(((pixel >> 11) & 0x1F) << 3);
      }
      gelukt = bestand.write(rij, sizeof(rij)) == sizeof(rij);
    }
  }
  if (gelukt) bestand.flush();
  bestand.close();
  if (!gelukt) kanaal->remove(naam);  // geen gedeeltelijk BMP laten staan
  if (kanaal->Extender != HardwareResourceToegang::EXCLUSIEF)
    kanaal->VrijgevenRuntime(this);
  return gelukt;
#else
  (void)naam;
  return false;
#endif
}

bool OV7670::afmelden() {
  if (actief) esp_camera_deinit();
  return Sensor::afmelden();
}
#endif
