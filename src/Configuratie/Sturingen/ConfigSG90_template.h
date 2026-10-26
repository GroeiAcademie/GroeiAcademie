#ifndef CONFIG_SG90_H
#define CONFIG_SG90_H
// STURING: SG90
// Kopieer dit bestand naar ConfigSG90.h en stel alleen benodigde waarden in.
// 0 = niet opgenomen; groter dan 0 = opgenomen; het aantal objecten wordt niet begrensd.
#ifndef STURING_SG90_AANTAL
// #define STURING_SG90_AANTAL 0
#endif
// Pulsgrenzen eerst kalibreren op het fysieke exemplaar en de PCA9685-frequentie.
#ifndef STURING_SG90_MIN_PWM
// #define STURING_SG90_MIN_PWM 205
#endif
#ifndef STURING_SG90_MAX_PWM
// #define STURING_SG90_MAX_PWM 410
#endif
#endif
