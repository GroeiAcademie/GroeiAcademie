@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibrarySensoren-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\GedeeldeBus\Sensoren\;"

call "%~dp0TestLibraryCommon.cmd" %*
exit /b %errorlevel%

:TESTLIJST

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_TestOpDubbeleComponentcodes\Sensor_TestOpDubbeleComponentcodes.ino"
set TEST_NAAM="Sensor componentcodes uniek"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set TEST_NAAM="Sensor FsrRfp | ExtenderADS1115"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set TEST_NAAM="Sensor FsrRfp | ExtenderADS1115 | 4 sensoren"
set TEST_FLAGS="-DFSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1=4 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_DIRECT\Sensor_FsrRfp_DIRECT.ino"
set TEST_NAAM="Sensor FsrRfp | DIRECT"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_DIRECT"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_FLASH\Sensor_FsrRfp_FLASH.ino"
set TEST_NAAM="Sensor FsrRfp | FLASH"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_FLASH_SPI"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_MICROSD\Sensor_FsrRfp_MICROSD.ino"
set TEST_NAAM="Sensor FsrRfp | MICROSD"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_MICROSD_SPI"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_TweeSetsOpTweeADS1115\Sensor_FsrRfp_TweeSetsOpTweeADS1115.ino"
set TEST_NAAM="Sensor FsrRfp | twee sets op twee ExtenderADS1115"
set TEST_FLAGS="-DFSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2=4 -DEXTENDER_ADS1115_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_Test_OngeldigePin\Sensor_FsrRfp_Test_OngeldigePin.ino"
set TEST_NAAM="Sensor FsrRfp | test ongeldige pin (T2-2)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_Test_ADC_Bezet\Sensor_FsrRfp_Test_ADC_Bezet.ino"
set TEST_NAAM="Sensor FsrRfp | test ADC bezet (T2-3)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DFSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_Test_Hardwarefout\Sensor_FsrRfp_Test_Hardwarefout.ino"
set TEST_NAAM="Sensor FsrRfp | test hardwarefout (T2-4)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_Test_Timeout_Conversie\Sensor_FsrRfp_Test_Timeout_Conversie.ino"
set TEST_NAAM="Sensor FsrRfp | test ophalen na verlies van de ADC (T2-5)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_Test_ZonderGet\Sensor_FsrRfp_Test_ZonderGet.ino"
set TEST_NAAM="Sensor FsrRfp | test start zonder ophalen (T2-6)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DFSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_Test_VerkeerdeParent\Sensor_FsrRfp_Test_VerkeerdeParent.ino"
set TEST_NAAM="Sensor FsrRfp | test verkeerde parent (T2-7)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_Test_EigenBuffers\Sensor_FsrRfp_Test_EigenBuffers.ino"
set TEST_NAAM="Sensor FsrRfp | test eigen buffers per object (T2-9)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_DIRECT -DFSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1=8 -DFSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_MixedPins\Sensor_MixedPins.ino"
set TEST_NAAM="Sensor Mixed Pins | 2 board + 2 PCF8574"
set TEST_FLAGS="-DGEDEELDE_BUS_MIXED_PINS=1 -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_GSRGrove\Sensor_GSRGrove.ino"
set TEST_NAAM="Sensor GSRGrove | ADC Extender"
set TEST_FLAGS="-DSENSOR_GSRGROVE_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_MF52A\Sensor_MF52A.ino"
set TEST_NAAM="Sensor MF52A | ADC Extender"
set TEST_FLAGS="-DSENSOR_MF52A_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_MF52D\Sensor_MF52D.ino"
set TEST_NAAM="Sensor MF52D | ADC Extender"
set TEST_FLAGS="-DSENSOR_MF52D_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_PIEZO\Sensor_PIEZO.ino"
set TEST_NAAM="Sensor PIEZO | ADC Extender"
set TEST_FLAGS="-DSENSOR_PIEZO_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_WATER\Sensor_WATER.ino"
set TEST_NAAM="Sensor WATER | ADC Extender"
set TEST_FLAGS="-DSENSOR_WATER_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_ADXL345\Sensor_ADXL345.ino"
set TEST_NAAM="Sensor ADXL345 | I2C device-ID lifecycle"
set TEST_FLAGS="-DSENSOR_ADXL345_AANTAL=1 -DI2C_ADDRESS_SENSOR_ADXL345_1=0x53 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_BMP280\Sensor_BMP280.ino"
set TEST_NAAM="Sensor BMP280 | I2C device-ID lifecycle"
set TEST_FLAGS="-DSENSOR_BMP280_AANTAL=1 -DI2C_ADDRESS_SENSOR_BMP280_1=0x77 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_BME680\Sensor_BME680.ino"
set TEST_NAAM="Sensor BME680 | I2C device-ID lifecycle"
set TEST_FLAGS="-DSENSOR_BME680_AANTAL=1 -DI2C_ADDRESS_SENSOR_BME680_1=0x77 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_MPU6050\Sensor_MPU6050.ino"
set TEST_NAAM="Sensor MPU6050 | I2C device-ID lifecycle"
set TEST_FLAGS="-DSENSOR_MPU6050_AANTAL=1 -DI2C_ADDRESS_SENSOR_MPU6050_1=0x68 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_AHT10\Sensor_AHT10.ino"
set TEST_NAAM="Sensor AHT10 | officiële library init"
set TEST_FLAGS="-DSENSOR_AHT10_AANTAL=1 -DI2C_ADDRESS_SENSOR_AHT10_1=0x38 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_VL53L0X\Sensor_VL53L0X.ino"
set TEST_NAAM="Sensor VL53L0X | officiële library init"
set TEST_FLAGS="-DSENSOR_VL53L0X_AANTAL=1 -DI2C_ADDRESS_SENSOR_VL53L0X_1=0x29 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_VL6180X\Sensor_VL6180X.ino"
set TEST_NAAM="Sensor VL6180X | officiële library init"
set TEST_FLAGS="-DSENSOR_VL6180X_AANTAL=1 -DI2C_ADDRESS_SENSOR_VL6180X_1=0x29 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_MAX30102\Sensor_MAX30102.ino"
set TEST_NAAM="Sensor MAX30102 | officiële library init"
set TEST_FLAGS="-DSENSOR_MAX30102_AANTAL=1 -DI2C_ADDRESS_SENSOR_MAX30102_1=0x57 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT
set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_SCD41\Sensor_SCD41.ino"
set TEST_NAAM="Sensor SCD41 | Sensirion serial identity"
set TEST_FLAGS="-DSENSOR_SCD41_AANTAL=1 -DI2C_ADDRESS_SENSOR_SCD41_1=0x62 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_BME688\Sensor_BME688.ino"
set TEST_NAAM="Sensor BME688 | I2C lifecycle"
set TEST_FLAGS="-DSENSOR_BME688_AANTAL=1 -DI2C_ADDRESS_SENSOR_BME688_1=0x77 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_MPU6500\Sensor_MPU6500.ino"
set TEST_NAAM="Sensor MPU6500 | I2C lifecycle"
set TEST_FLAGS="-DSENSOR_MPU6500_AANTAL=1 -DI2C_ADDRESS_SENSOR_MPU6500_1=0x68 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_LM75A\Sensor_LM75A.ino"
set TEST_NAAM="Sensor LM75A | I2C lifecycle"
set TEST_FLAGS="-DSENSOR_LM75A_AANTAL=1 -DI2C_ADDRESS_SENSOR_LM75A_1=0x48 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_SDP810125PA\Sensor_SDP810125PA.ino"
set TEST_NAAM="Sensor SDP810125PA | lifecycle"
set TEST_FLAGS="-DSENSOR_SDP810125PA_AANTAL=1 -DI2C_ADDRESS_SENSOR_SDP810125PA_1=0x25 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_DS18B20\Sensor_DS18B20.ino"
set TEST_NAAM="Sensor DS18B20 | lifecycle"
set TEST_FLAGS="-DSENSOR_DS18B20_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_HW201\Sensor_HW201.ino"
set TEST_NAAM="Sensor HW201 | lifecycle"
set TEST_FLAGS="-DSENSOR_HW201_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_HCSR501\Sensor_HCSR501.ino"
set TEST_NAAM="Sensor HCSR501 | lifecycle"
set TEST_FLAGS="-DSENSOR_HCSR501_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_HCSR04\Sensor_HCSR04.ino"
set TEST_NAAM="Sensor HCSR04 | lifecycle"
set TEST_FLAGS="-DSENSOR_HCSR04_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_HCSR04P\Sensor_HCSR04P.ino"
set TEST_NAAM="Sensor HCSR04P | lifecycle"
set TEST_FLAGS="-DSENSOR_HCSR04P_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_HYSRF05\Sensor_HYSRF05.ino"
set TEST_NAAM="Sensor HYSRF05 | lifecycle"
set TEST_FLAGS="-DSENSOR_HYSRF05_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_HX711\Sensor_HX711.ino"
set TEST_NAAM="Sensor HX711 | lifecycle"
set TEST_FLAGS="-DSENSOR_HX711_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_DHTxx\Sensor_DHTxx.ino"
set TEST_NAAM="Sensor DHT11 | lifecycle"
set TEST_FLAGS="-DSENSOR_DHT11_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_DHTxx\Sensor_DHTxx.ino"
set TEST_NAAM="Sensor DHT22 | lifecycle"
set TEST_FLAGS="-DSENSOR_DHT22_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_AD8232\Sensor_AD8232.ino"
set TEST_NAAM="Sensor AD8232 | ADC lifecycle"
set TEST_FLAGS="-DSENSOR_AD8232_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_BioAmpEXGPill\Sensor_BioAmpEXGPill.ino"
set TEST_NAAM="Sensor BioAmpEXGPill | ADC lifecycle"
set TEST_FLAGS="-DSENSOR_BIOAMPEXGPILL_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_EGBOPulseSensor\Sensor_EGBOPulseSensor.ino"
set TEST_NAAM="Sensor EGBOPulseSensor | ADC lifecycle"
set TEST_FLAGS="-DSENSOR_EGBOPULSESENSOR_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_MAX4466\Sensor_MAX4466.ino"
set TEST_NAAM="Sensor MAX4466 | ADC lifecycle"
set TEST_FLAGS="-DSENSOR_MAX4466_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_MAX9814\Sensor_MAX9814.ino"
set TEST_NAAM="Sensor MAX9814 | ADC lifecycle"
set TEST_FLAGS="-DSENSOR_MAX9814_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_LM35DZ\Sensor_LM35DZ.ino"
set TEST_NAAM="Sensor LM35DZ | ADC Extender"
set TEST_FLAGS="-DSENSOR_LM35DZ_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT
set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_DS3231\Sensor_DS3231.ino"
set TEST_NAAM="Sensor DS3231 | I2C klok"
set TEST_FLAGS="-DSENSOR_DS3231_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_XYSTICK\Sensor_XYSTICK.ino"
set TEST_NAAM="Sensor XY-STICK | ADC-contract ADS1115 en native knop"
set TEST_FLAGS="-DEXTENDER_ADS1115_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_MS3625\Sensor_MS3625.ino"
set TEST_NAAM="Sensor MS3625 | I2S via INMP441, alleen ESP32-S3"
set TEST_FLAGS="-DSENSOR_INMP441_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_EXTENDED_FQBN"
set WELKE_FQBN_SKIPPEN="esp32:esp32:d1_uno32,rp2040:rp2040:cytron_maker_uno_rp2040,STMicroelectronics:stm32:Nucleo_64:pnum=NUCLEO_F401RE"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_BioAmpEXGPill_ECG_Hartslag\Sensor_BioAmpEXGPill_ECG_Hartslag.ino"
set TEST_NAAM="Sensor BioAmpEXGPill | ECG hartslag"
set TEST_FLAGS="-DSENSOR_BIOAMPEXGPILL_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSENSOR_BIOAMPEXGPILL_MEETMETHODE=BIOAMPEXGPILL_MEETMETHODE_ECG"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_BioAmpEXGPill_EMG_Envelop\Sensor_BioAmpEXGPill_EMG_Envelop.ino"
set TEST_NAAM="Sensor BioAmpEXGPill | EMG envelop"
set TEST_FLAGS="-DSENSOR_BIOAMPEXGPILL_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSENSOR_BIOAMPEXGPILL_MEETMETHODE=BIOAMPEXGPILL_MEETMETHODE_EMG"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_BioAmpEXGPill_EOG_Knipperen\Sensor_BioAmpEXGPill_EOG_Knipperen.ino"
set TEST_NAAM="Sensor BioAmpEXGPill | EOG knipperen"
set TEST_FLAGS="-DSENSOR_BIOAMPEXGPILL_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSENSOR_BIOAMPEXGPILL_MEETMETHODE=BIOAMPEXGPILL_MEETMETHODE_EOG"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT


exit /b 0
