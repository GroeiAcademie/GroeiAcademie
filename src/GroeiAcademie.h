#ifndef GROEIACADEMIE_H
#define GROEIACADEMIE_H

#include "Screen.h"
#include "Stimulus.h"
#include "Input.h"
#include "Data.h"


#if STURING_A4988_AANTAL > 0
  #include "Systeem/Sturing/A4988.h"
#endif
#if STURING_DCMOTORL293D_AANTAL > 0
  #include "Systeem/Sturing/DCmotorL293D.h"
#endif
#if STURING_DCMOTORL298N_AANTAL > 0
  #include "Systeem/Sturing/DCmotorL298N.h"
#endif
#if STURING_MG996R_AANTAL > 0
  #include "Systeem/Sturing/MG996R.h"
#endif
#if STURING_RELAIS_AANTAL > 0
  #include "Systeem/Sturing/Relais.h"
#endif
#if STURING_RGBLED_AANTAL > 0
  #include "Systeem/Sturing/RGBLED.h"
#endif
#if STURING_SG90_AANTAL > 0
  #include "Systeem/Sturing/SG90.h"
#endif
#if STURING_ULN2003_AANTAL > 0
  #include "Systeem/Sturing/ULN2003.h"
#endif
#if STURING_WS2812_AANTAL > 0
  #include "Systeem/Sturing/WS2812.h"
#endif

#endif
