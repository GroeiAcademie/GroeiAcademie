#ifndef GROEIACADEMIE_H
#define GROEIACADEMIE_H

#include "Screen.h"
#include "Stimulus.h"
#include "Input.h"
#include "Data.h"


#if STURING_A4988_AANTAL > 0
  #include "Systeem/Sturing/A4988.h"
#endif
#if STURING_L293D_AANTAL > 0
  #include "Systeem/Sturing/L293D.h"
#endif
#if STURING_L298N_AANTAL > 0
  #include "Systeem/Sturing/L298N.h"
#endif
#if STURING_MAX7219_AANTAL > 0
  #include "Systeem/Sturing/MAX7219.h"
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

#if STURING_A4988_AANTAL > 0 || STURING_ULN2003_AANTAL > 0
  #include "Systeem/Sturing/Functies/Elektromotoren/StappenmotorBesturing.h"
#endif
#if STURING_SG90_AANTAL > 0 || STURING_MG996R_AANTAL > 0
  #include "Systeem/Sturing/Functies/Elektromotoren/ServomotorBesturing.h"
#endif
#if STURING_L293D_AANTAL > 0 || STURING_L298N_AANTAL > 0
  #include "Systeem/Sturing/Functies/Elektromotoren/DCmotorBesturing.h"
#endif


#if SENSOR_INMP441_AANTAL > 0
  #include "Systeem/Sensor/INMP441.h"
#endif
#if SENSOR_OV7670_AANTAL > 0
  #include "Systeem/Sensor/OV7670.h"
#endif

#if SENSOR_GSRGROVE_AANTAL > 0
  #include "Systeem/Sensor/GSRGrove.h"
#endif
#if SENSOR_MF52A_AANTAL > 0 || SENSOR_MF52D_AANTAL > 0
  #include "Systeem/Sensor/MF52x.h"
#endif
#if SENSOR_PIEZO_AANTAL > 0
  #include "Systeem/Sensor/PIEZO.h"
#endif
#if SENSOR_WATER_AANTAL > 0
  #include "Systeem/Sensor/WATER.h"
#endif

#if SENSOR_AD8232_AANTAL > 0
  #include "Systeem/Sensor/AD8232.h"
#endif
#if SENSOR_BIOAMPEXGPILL_AANTAL > 0
  #include "Systeem/Sensor/BioAmpEXGPill.h"
#endif
#if SENSOR_EGBOPULSESENSOR_AANTAL > 0
  #include "Systeem/Sensor/EGBOPulseSensor.h"
#endif
#if SENSOR_MAX4466_AANTAL > 0 || SENSOR_MAX9814_AANTAL > 0
  #include "Systeem/Sensor/MAX4466_9814.h"
#endif
#if SENSOR_LM35DZ_AANTAL > 0
  #include "Systeem/Sensor/LM35DZ.h"
#endif


// I2C-sensoren (alleen actief indien geconfigureerd).
#if SENSOR_ADXL345_AANTAL > 0
#include "Systeem/Sensor/ADXL345.h"
#endif
#if SENSOR_BMP280_AANTAL > 0
#include "Systeem/Sensor/BMP280.h"
#endif
#if SENSOR_BME680_AANTAL > 0 || SENSOR_BME688_AANTAL > 0
#include "Systeem/Sensor/BME68x.h"
#endif
#if SENSOR_MPU6050_AANTAL > 0
#include "Systeem/Sensor/MPU6050.h"
#endif
#if SENSOR_AHT10_AANTAL > 0
#include "Systeem/Sensor/AHT10.h"
#endif
#if SENSOR_VL53L0X_AANTAL > 0
#include "Systeem/Sensor/VL53L0X.h"
#endif

#if SENSOR_VL6180X_AANTAL > 0
#include "Systeem/Sensor/VL6180X.h"
#endif
#if SENSOR_MAX30102_AANTAL > 0
#include "Systeem/Sensor/MAX30102.h"
#endif

#if SENSOR_SCD41_AANTAL > 0
#include "Systeem/Sensor/SCD41.h"
#endif
#if SENSOR_MPU6500_AANTAL > 0
#include "Systeem/Sensor/MPU6500.h"
#endif
#if SENSOR_LM75A_AANTAL > 0
#include "Systeem/Sensor/LM75A.h"
#endif

#if SENSOR_SDP810125PA_AANTAL > 0
#include "Systeem/Sensor/SDP810125PA.h"
#endif
#if SENSOR_DS18B20_AANTAL > 0
#include "Systeem/Sensor/DS18B20.h"
#endif
#if SENSOR_DS3231_AANTAL > 0
#include "Systeem/Sensor/DS3231.h"
#endif
#if SENSOR_TTP223_AANTAL > 0
#include "Systeem/Sensor/TTP223.h"
#endif
#if SENSOR_OT3686_AANTAL > 0
#include "Systeem/Sensor/OT3686.h"
#endif
#if SENSOR_MCU6701_AANTAL > 0
#include "Systeem/Sensor/MCU6701.h"
#endif
#if SENSOR_HCSR501_AANTAL > 0
#include "Systeem/Sensor/HCSR501.h"
#endif

#if SENSOR_HCSR04_AANTAL > 0 || SENSOR_HCSR04P_AANTAL > 0 || SENSOR_HYSRF05_AANTAL > 0
#include "Systeem/Sensor/HSR0x.h"
#endif

#if SENSOR_HX711_AANTAL > 0
#include "Systeem/Sensor/HX711.h"
#endif
#if SENSOR_DHT11_AANTAL > 0 || SENSOR_DHT22_AANTAL > 0
#include "Systeem/Sensor/DHTxx.h"
#endif

#if SENSOR_AS608_AANTAL > 0
#include "Systeem/Sensor/AS608.h"
#endif
#if SENSOR_NEUROSKYTGAM_AANTAL > 0
#include "Systeem/Sensor/NeuroSkyTGAM.h"
#endif

#if SENSOR_LD2450_AANTAL > 0
#include "Systeem/Sensor/LD2450.h"
#endif

#if SENSOR_LD2410D_AANTAL > 0
#include "Systeem/Sensor/LD2410D.h"
#endif

#if SENSOR_MR60BHA2_AANTAL > 0
#include "Systeem/Sensor/MR60BHA2.h"
#endif

#if SENSOR_HW201_AANTAL > 0
#include "Systeem/Sensor/HW201.h"
#endif

#endif
