#ifndef WS2812_H
#define WS2812_H
#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"
#include "../GedeeldeBus/GedeeldeBus.h"
#include <Adafruit_NeoPixel.h>
// Native datapin. NeoPixel-protocol kan niet via PCF8574 of PCA9685.
// Parent: native/root node. Aparte geschikte 5V-voeding en signaalniveau vereist.
struct WS2812 : GedeeldeBusNode {
  uint8_t exclusief_[1],pin,width,height;
  Adafruit_NeoPixel pixels;
#if defined(STURING_WS2812_WIDTH) && defined(STURING_WS2812_HEIGHT)
  WS2812(GedeeldeBusNode* parent,HardwareResourcePin dataPin)
    : WS2812(parent,dataPin,STURING_WS2812_WIDTH,STURING_WS2812_HEIGHT) {}
#endif
  WS2812(GedeeldeBusNode* parent,HardwareResourcePin dataPin,uint8_t matrixWidth,uint8_t matrixHeight)
    :GedeeldeBusNode(parent,{nullptr,0,exclusief_,1},GedeeldeBusComponent::WS2812),
     exclusief_{static_cast<uint8_t>(dataPin)},pin(NativeArduinoPinVan(dataPin)),width(matrixWidth),height(matrixHeight),pixels(uint16_t(matrixWidth)*matrixHeight,NativeArduinoPinVan(dataPin),NEO_GRB+NEO_KHZ800){}
  bool Activeren() override {if (!width || !height || pin==static_cast<uint8_t>(HardwareResourcePin::NONE)) return false;pixels.begin();pixels.clear();pixels.show();return true;}
  bool setPixelColor(uint8_t x,uint8_t y,uint32_t color){if(!actief||x>=width||y>=height)return false;pixels.setPixelColor(uint16_t(y)*width+x,color);pixels.show();return true;}
  // Monochroom patroon met een gekozen RGB-kleur; een enkele show() per gewijzigd beeld.
  bool ToonPatroon(uint8_t maat, const uint8_t beeld[8], uint32_t kleur) {
    if (!actief || maat!=8 || beeld==nullptr || width<maat || height<maat) return false;
    bool gewijzigd=false;
    for (uint8_t y=0; y<maat; ++y) for (uint8_t x=0; x<maat; ++x) {
      const uint16_t pixel=uint16_t(y)*width+x;
      const uint32_t gewenst=(beeld[y] & (1u<<x)) ? kleur : 0;
      if (pixels.getPixelColor(pixel)!=gewenst) {
        pixels.setPixelColor(pixel,gewenst);
        gewijzigd=true;
      }
    }
    if (gewijzigd) pixels.show();
    return true;
  }
  void StopSturing() override {if(!actief)return;pixels.clear();pixels.show();}   // niet actief: begin() liep niet
};

#endif
