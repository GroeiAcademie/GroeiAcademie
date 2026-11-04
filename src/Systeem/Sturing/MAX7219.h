#ifndef MAX7219_H
#define MAX7219_H

#include <Arduino.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "../GedeeldeBus/GedeeldeBus.h"

// MAX7219 8x8-matrix: hardware-SPI met CS native of via PCF8574, of alle drie signalen via PCF8574.
struct MAX7219 : GedeeldeBusNode {
  uint8_t gedeeld_[3];
  uint8_t exclusief_[3];
  MD_MAX72XX matrix;
  MD_MAX72XX::moduleType_t hardwareType;
  uint8_t din, clk, cs;
  bool viaExtender;
#if GEDEELDE_BUS_MIXED_PINS && EXTENDER_PCF8574_AANTAL > 0
  GedeeldeBusNode* extenderCS_[1] = {nullptr};
  bool viaMixed = false;
#endif
  uint8_t buffer_[8] = {0,0,0,0,0,0,0,0};

  // Hardware-SPI; bestaande GedeeldeBus-pinclaming met gedeelde MOSI/SCK en eigen CS.
  MAX7219(GedeeldeBusNode* parent, HardwareResourcePin pinCS, MD_MAX72XX::moduleType_t type)
    : GedeeldeBusNode(parent,{gedeeld_,3,exclusief_,1},GedeeldeBusComponent::MAX7219),
      gedeeld_{static_cast<uint8_t>(HardwareResourcePin::NP_SCK),static_cast<uint8_t>(HardwareResourcePin::NONE),static_cast<uint8_t>(HardwareResourcePin::NP_MOSI)},
      exclusief_{static_cast<uint8_t>(pinCS),static_cast<uint8_t>(HardwareResourcePin::NONE),static_cast<uint8_t>(HardwareResourcePin::NONE)},
      matrix(type,static_cast<int8_t>(NativeArduinoPinVan(pinCS)),1),
      hardwareType(type),din(0),clk(0),cs(NativeArduinoPinVan(pinCS)),viaExtender(false) {
    gedeeldeIdentiteiten = GedeeldeBusIdentiteitSPI;
  }

  // Alle signaalpinnen van dezelfde PCF8574-parent; exclusieve extenderclaims.
  // pinDIN, pinCLK en pinCS zijn PCF8574-pinnummers P0..P7, geen Arduino-nummers.
  MAX7219(GedeeldeBusNode* parent, uint8_t pinDIN, uint8_t pinCLK, uint8_t pinCS, MD_MAX72XX::moduleType_t type)
    : GedeeldeBusNode(parent,{nullptr,0,exclusief_,3},GedeeldeBusComponent::MAX7219),
      gedeeld_{0,0,0},exclusief_{pinDIN,pinCLK,pinCS},
      matrix(type,static_cast<int8_t>(-1),1),
      hardwareType(type),din(pinDIN),clk(pinCLK),cs(pinCS),viaExtender(true) {}

#if GEDEELDE_BUS_MIXED_PINS && EXTENDER_PCF8574_AANTAL > 0
  // Hardware-SPI voor DIN en CLK; alleen LOAD/CS is een PCF8574-pin.
  MAX7219(GedeeldeBusNode* parent, GedeeldeBusNode* extender, uint8_t pinCS, MD_MAX72XX::moduleType_t type)
    : GedeeldeBusNode(parent,{gedeeld_,3,exclusief_,1},GedeeldeBusComponent::MAX7219),
      gedeeld_{static_cast<uint8_t>(HardwareResourcePin::NP_SCK),static_cast<uint8_t>(HardwareResourcePin::NONE),static_cast<uint8_t>(HardwareResourcePin::NP_MOSI)},
      exclusief_{pinCS,static_cast<uint8_t>(HardwareResourcePin::NONE),static_cast<uint8_t>(HardwareResourcePin::NONE)},
      matrix(type,static_cast<int8_t>(-1),1),
      hardwareType(type),din(0),clk(0),cs(pinCS),viaExtender(true) {
    gedeeldeIdentiteiten = GedeeldeBusIdentiteitSPI;
    extenderCS_[0] = extender;
    ownerClaim = extenderCS_;
    viaMixed = true;
  }
#endif

  bool Activeren() override {
    if (!parent || !parent->actief) return false;
    if (!viaExtender) {
      if (cs==static_cast<uint8_t>(HardwareResourcePin::NONE)) return false;
      InitialiserenGedeeldeBus(GedeeldeBusType::SPI);
      return matrix.begin();
    }
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF && EXTENDER_PCF8574_AANTAL > 0
#if GEDEELDE_BUS_MIXED_PINS
    if (viaMixed) {
      if (parent!=&Native || extenderCS_[0]==nullptr || !extenderCS_[0]->actief || cs>7 ||
          extenderCS_[0]->component!=GedeeldeBusComponent::DIGITAL_PINS_PCF8574) return false;
      extenderCS_[0]->WriteExtenderPin(cs,HIGH);
      extenderCS_[0]->SetExtenderPinMode(cs,OUTPUT);
      InitialiserenGedeeldeBus(GedeeldeBusType::SPI);
    } else
#endif
    {
      if (parent!=firstExtenderNode() || parent->component!=GedeeldeBusComponent::DIGITAL_PINS_PCF8574 ||
          din>7 || clk>7 || cs>7 || din==clk || din==cs || clk==cs) return false;
      parent->WriteExtenderPin(cs,HIGH); parent->SetExtenderPinMode(cs,OUTPUT);
      parent->WriteExtenderPin(clk,LOW); parent->SetExtenderPinMode(clk,OUTPUT);
      parent->WriteExtenderPin(din,LOW); parent->SetExtenderPinMode(din,OUTPUT);
    }

    for (uint8_t i=0;i<8;i++) buffer_[i]=0;
    schrijfRegister(0x0F,0); // display test uit
    schrijfRegister(0x09,0); // decode mode uit
    schrijfRegister(0x0B,7); // acht digits
    schrijfRegister(0x0A,4); // testhelderheid
    for (uint8_t i=0;i<8;i++) schrijfRegister(i+1,0);
    schrijfRegister(0x0C,1); // normaal bedrijf
    return true; // De GPIO-interface heeft geen afzonderlijke foutstatus voor de hardwareoverdracht.
#else
    return false;
#endif
  }

  bool setPixel(uint8_t x,uint8_t y,bool aan) {
    if (!actief || x>=8 || y>=8) return false;
    if (!viaExtender) return matrix.setPoint(y,x,aan);
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF && EXTENDER_PCF8574_AANTAL > 0
    // Identieke row/column mapping als de officiële MD_MAX72XX::setPoint().
    const bool digitsAlsRijen = hardwareType==MD_MAX72XX::FC16_HW || hardwareType==MD_MAX72XX::PAROLA_HW ||
      hardwareType==MD_MAX72XX::ICSTATION_HW || hardwareType==MD_MAX72XX::DR1CR0RR0_HW ||
      hardwareType==MD_MAX72XX::DR1CR0RR1_HW || hardwareType==MD_MAX72XX::DR1CR1RR0_HW ||
      hardwareType==MD_MAX72XX::DR1CR1RR1_HW;
    const bool kolomOmgekeerd = hardwareType==MD_MAX72XX::GENERIC_HW || hardwareType==MD_MAX72XX::PAROLA_HW ||
      hardwareType==MD_MAX72XX::ICSTATION_HW || hardwareType==MD_MAX72XX::DR0CR1RR0_HW ||
      hardwareType==MD_MAX72XX::DR0CR1RR1_HW || hardwareType==MD_MAX72XX::DR1CR1RR0_HW ||
      hardwareType==MD_MAX72XX::DR1CR1RR1_HW;
    const bool rijOmgekeerd = hardwareType==MD_MAX72XX::ICSTATION_HW ||
      hardwareType==MD_MAX72XX::DR0CR0RR1_HW || hardwareType==MD_MAX72XX::DR0CR1RR1_HW ||
      hardwareType==MD_MAX72XX::DR1CR0RR1_HW || hardwareType==MD_MAX72XX::DR1CR1RR1_HW;
    const uint8_t rij = rijOmgekeerd ? (7-(digitsAlsRijen ? y : x)) : (digitsAlsRijen ? y : x);
    const uint8_t kolom = kolomOmgekeerd ? (7-(digitsAlsRijen ? x : y)) : (digitsAlsRijen ? x : y);
    const uint8_t digit = rij;
    const uint8_t bit = kolom;
    const uint8_t masker=static_cast<uint8_t>(1u<<bit);
    const uint8_t waarde=aan ? static_cast<uint8_t>(buffer_[digit]|masker) : static_cast<uint8_t>(buffer_[digit]&static_cast<uint8_t>(~masker));
    schrijfRegister(digit+1,waarde);
    buffer_[digit]=waarde;
    return true; // Schrijfopdracht uitgevoerd; geen ACK van de MAX7219.
#else
    return false;
#endif
  }

  // Volledig gewenst monochroom patroon. De 8 is voorlopig de bestaande matrixmaat.
  // Bij native SPI: kleine wijzigingen rechtstreeks, grote wijzigingen gebundeld.
  // Bij PCF8574: alleen gewijzigde MAX7219-registers versturen.
  bool ToonPatroon(uint8_t maat, const uint8_t beeld[8]) {
    if (!actief || maat != 8 || beeld == nullptr) return false;
    if (!viaExtender) {
      uint8_t wijzigingen=0;
      for (uint8_t y=0; y<8; ++y)
        for (uint8_t x=0; x<8; ++x)
          if (matrix.getPoint(y,x) != ((beeld[y] & (1u<<x)) != 0)) ++wijzigingen;
      if (!wijzigingen) return true;
      // Bij veel pixelwijzigingen is één gebundelde flush efficiënter.
      const bool bundelen=wijzigingen>8;
      if (bundelen) matrix.update(MD_MAX72XX::OFF);
      bool gelukt=true;
      for (uint8_t y=0; y<8; ++y)
        for (uint8_t x=0; x<8; ++x) {
          const bool aan=(beeld[y] & (1u<<x)) != 0;
          if (matrix.getPoint(y,x) != aan && !matrix.setPoint(y,x,aan)) gelukt=false;
        }

      if (bundelen) { matrix.update(); matrix.update(MD_MAX72XX::ON); }
      return gelukt;
    }
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF && EXTENDER_PCF8574_AANTAL > 0
    // Identieke orientatiemapping als de bestaande setPixel().
    const bool digitsAlsRijen = hardwareType==MD_MAX72XX::FC16_HW || hardwareType==MD_MAX72XX::PAROLA_HW ||
      hardwareType==MD_MAX72XX::ICSTATION_HW || hardwareType==MD_MAX72XX::DR1CR0RR0_HW ||
      hardwareType==MD_MAX72XX::DR1CR0RR1_HW || hardwareType==MD_MAX72XX::DR1CR1RR0_HW ||
      hardwareType==MD_MAX72XX::DR1CR1RR1_HW;
    const bool kolomOmgekeerd = hardwareType==MD_MAX72XX::GENERIC_HW || hardwareType==MD_MAX72XX::PAROLA_HW ||
      hardwareType==MD_MAX72XX::ICSTATION_HW || hardwareType==MD_MAX72XX::DR0CR1RR0_HW ||
      hardwareType==MD_MAX72XX::DR0CR1RR1_HW || hardwareType==MD_MAX72XX::DR1CR1RR0_HW ||
      hardwareType==MD_MAX72XX::DR1CR1RR1_HW;
    const bool rijOmgekeerd = hardwareType==MD_MAX72XX::ICSTATION_HW ||
      hardwareType==MD_MAX72XX::DR0CR0RR1_HW || hardwareType==MD_MAX72XX::DR0CR1RR1_HW ||
      hardwareType==MD_MAX72XX::DR1CR0RR1_HW || hardwareType==MD_MAX72XX::DR1CR1RR1_HW;
    uint8_t nieuw[8]={0,0,0,0,0,0,0,0};
    for (uint8_t y=0; y<8; ++y) for (uint8_t x=0; x<8; ++x) {
      if (!(beeld[y] & (1u<<x))) continue;
      const uint8_t rij=rijOmgekeerd ? (7-(digitsAlsRijen ? y : x)) : (digitsAlsRijen ? y : x);
      const uint8_t kolom=kolomOmgekeerd ? (7-(digitsAlsRijen ? x : y)) : (digitsAlsRijen ? x : y);
      nieuw[rij] |= static_cast<uint8_t>(1u<<kolom);
    }

    for (uint8_t i=0; i<8; ++i) if (nieuw[i]!=buffer_[i]) {
      schrijfRegister(i+1,nieuw[i]);
      buffer_[i]=nieuw[i];
    }
    return true; // Geen hardware-ACK beschikbaar.
#else
    return false;
#endif
  }

  bool clear() {
    if (!actief) return false;
    if (!viaExtender) {matrix.clear();return true;}
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF && EXTENDER_PCF8574_AANTAL > 0
    for(uint8_t i=0;i<8;i++) {schrijfRegister(i+1,0);buffer_[i]=0;}
    return true;
#else
    return false;
#endif
  }

  bool setIntensity(uint8_t waarde) {
    if (!actief || waarde>15) return false;
    if (!viaExtender) return matrix.control(0,MD_MAX72XX::INTENSITY,waarde);
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF && EXTENDER_PCF8574_AANTAL > 0
    schrijfRegister(0x0A,waarde);return true;
#else
    return false;
#endif
  }

  void StopSturing() override {
    if (!actief) return;
    if (!viaExtender) {
      matrix.clear();matrix.control(MD_MAX72XX::SHUTDOWN,MD_MAX72XX::ON);
      return;
    }
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF && EXTENDER_PCF8574_AANTAL > 0
    if (parent && parent->actief) schrijfRegister(0x0C,0);
#endif
  }

private:
  void schrijfRegister(uint8_t reg,uint8_t data) {
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF && EXTENDER_PCF8574_AANTAL > 0
    // MAX7219: 16 bits MSB-first, LOAD/CS blijft laag tot na beide bytes.
#if GEDEELDE_BUS_MIXED_PINS
    if (viaMixed) {
      SPI.beginTransaction(SPISettings(1000000,MSBFIRST,SPI_MODE0));
      extenderCS_[0]->WriteExtenderPin(cs,LOW);
      SPI.transfer(reg);
      SPI.transfer(data);
      extenderCS_[0]->WriteExtenderPin(cs,HIGH);
      SPI.endTransaction();
      return;
    }
#endif
    const uint16_t pakket=(static_cast<uint16_t>(reg)<<8)|data;
    parent->WriteExtenderPin(cs,LOW);
    for(int8_t i=15;i>=0;--i) {
      parent->WriteExtenderPin(din,(pakket&(static_cast<uint16_t>(1u)<<i))?HIGH:LOW);
      parent->WriteExtenderPin(clk,HIGH);
      parent->WriteExtenderPin(clk,LOW);
    }
    parent->WriteExtenderPin(cs,HIGH);
#else
    (void)reg;(void)data;
#endif
  }
};

#endif
