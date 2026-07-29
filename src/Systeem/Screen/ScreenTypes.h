#ifndef SCREEN_TYPES_H
#define SCREEN_TYPES_H

// SCHERM TYPES
#define SCREEN_TYPE_NONE       0x00 // geen uitvoer
#define SCREEN_TYPE_SERIAL     0x01 // gebruik seriële console
#define SCREEN_TYPE_CHARACTER  0x02 // characterscherm
#define SCREEN_TYPE_PIXELS     0x04 // pixelscherm

// ============================================================================
// SCHERMRESOLUTIES: enkel voor intern gebruik binnen de library
// ============================================================================

// Characterschermen: maximaal één keuze
#define SCREEN_LCD1602  0x0010 // 2 regels van 16 karakters
#define SCREEN_LCD1604  0x0020 // 4 regels van 16 karakters
#define SCREEN_LCD2002  0x0030 // 2 regels van 20 karakters
#define SCREEN_LCD2004  0x0040 // 4 regels van 20 karakters
#define SCREEN_LCD4002  0x0050 // 2 regels van 40 karakters

// Pixelschermen: maximaal één keuze
#define SCREEN_128X32   0x0100 // 128x32 pixels
#define SCREEN_128X64   0x0200 // 128x64 pixels
#define SCREEN_128X160  0x0300 // 128x160 pixels
#define SCREEN_240X240  0x0400 // 240x240 pixels
#define SCREEN_240X320  0x0500 // 240x320 pixels
#define SCREEN_320X480  0x0600 // 320x480 pixels
#define SCREEN_480X320  0x0700 // 480x320 pixels

#endif // SCREEN_TYPES_H
