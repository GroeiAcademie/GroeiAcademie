@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibrarySensorenOngeldig-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\GedeeldeBus\Sensoren\;"
set "BRON_TEST_SCRIPT=%~dp0TestLibrarySensoren-v2.0.0.cmd"

call "%~dp0TestLibraryCommonOngeldig.cmd" %*
exit /b %errorlevel%

:EXPECT_FAIL
set "TEST_FQBN=arduino:renesas_uno:minima"
set "TEST_BOARD_VERSION=BOARD_UNO_R4_MINIMA"
echo !TEST_FLAGS! | findstr /C:"BOARD_VERSION" >nul
if not errorlevel 1 set "TEST_BOARD_VERSION="
call "%~dp0TestLibraryCommonOngeldig.cmd" --VOER_DEZE_ONGELDIGE_TEST_UIT
goto :eof

:TESTLIJST
set /A TESTS=0
set /A OK=0
set /A FAIL=0

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_GSRGrove\Sensor_GSRGrove.ino"
set "TEST_NAAM=Sensor GSRGrove: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_GSRGROVE_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_GSRGROVE_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MF52A\Sensor_MF52A.ino"
set "TEST_NAAM=Sensor MF52A: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_MF52A_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_MF52A_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MF52D\Sensor_MF52D.ino"
set "TEST_NAAM=Sensor MF52D: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_MF52D_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_MF52D_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_PIEZO\Sensor_PIEZO.ino"
set "TEST_NAAM=Sensor PIEZO: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_PIEZO_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_PIEZO_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_ADXL345\Sensor_ADXL345.ino"
set "TEST_NAAM=Sensor ADXL345: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_ADXL345_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_ADXL345_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_BMP280\Sensor_BMP280.ino"
set "TEST_NAAM=Sensor BMP280: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_BMP280_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_BMP280_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_BME680\Sensor_BME680.ino"
set "TEST_NAAM=Sensor BME680: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_BME680_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_BME680_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MPU6050\Sensor_MPU6050.ino"
set "TEST_NAAM=Sensor MPU6050: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_MPU6050_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_MPU6050_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_AHT10\Sensor_AHT10.ino"
set "TEST_NAAM=Sensor AHT10: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_AHT10_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_AHT10_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_VL53L0X\Sensor_VL53L0X.ino"
set "TEST_NAAM=Sensor VL53L0X: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_VL53L0X_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_VL53L0X_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_VL6180X\Sensor_VL6180X.ino"
set "TEST_NAAM=Sensor VL6180X: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_VL6180X_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_VL6180X_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MAX30102\Sensor_MAX30102.ino"
set "TEST_NAAM=Sensor MAX30102: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_MAX30102_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_MAX30102_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_SCD41\Sensor_SCD41.ino"
set "TEST_NAAM=Sensor SCD41: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_SCD41_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_SCD41_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_BME688\Sensor_BME688.ino"
set "TEST_NAAM=Sensor BME688: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_BME688_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_BME688_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MPU6500\Sensor_MPU6500.ino"
set "TEST_NAAM=Sensor MPU6500: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_MPU6500_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_MPU6500_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_LM75A\Sensor_LM75A.ino"
set "TEST_NAAM=Sensor LM75A: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_LM75A_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_LM75A_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_SDP810125PA\Sensor_SDP810125PA.ino"
set "TEST_NAAM=Sensor SDP810125PA: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_SDP810125PA_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_SDP810125PA_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_HCSR501\Sensor_HCSR501.ino"
set "TEST_NAAM=Sensor HCSR501: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_HCSR501_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_HCSR501_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_HCSR04\Sensor_HCSR04.ino"
set "TEST_NAAM=Sensor HCSR04: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_HCSR04_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_HCSR04_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_HCSR04P\Sensor_HCSR04P.ino"
set "TEST_NAAM=Sensor HCSR04P: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_HCSR04P_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_HCSR04P_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_HYSRF05\Sensor_HYSRF05.ino"
set "TEST_NAAM=Sensor HYSRF05: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_HYSRF05_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_HYSRF05_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_HW201\Sensor_HW201.ino"
set "TEST_NAAM=Sensor HW201: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_HW201_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_HW201_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MR60BHA2\Sensor_MR60BHA2.ino"
set "TEST_NAAM=Sensor MR60BHA2: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_MR60BHA2_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_MR60BHA2_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_LD2410D\Sensor_LD2410D.ino"
set "TEST_NAAM=Sensor LD2410D: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_LD2410D_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_LD2410D_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_LD2450\Sensor_LD2450.ino"
set "TEST_NAAM=Sensor LD2450: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_LD2450_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_LD2450_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_NeuroSkyTGAM\Sensor_NeuroSkyTGAM.ino"
set "TEST_NAAM=Sensor NeuroSkyTGAM: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_NEUROSKYTGAM_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_NEUROSKYTGAM_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_HX711\Sensor_HX711.ino"
set "TEST_NAAM=Sensor HX711: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_HX711_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_HX711_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_DHTxx\Sensor_DHTxx.ino"
set "TEST_NAAM=Sensor DHT11: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_DHT11_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_DHT11_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_DHTxx\Sensor_DHTxx.ino"
set "TEST_NAAM=Sensor DHT22: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_DHT22_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_DHT22_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_AD8232\Sensor_AD8232.ino"
set "TEST_NAAM=Sensor AD8232: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_AD8232_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_AD8232_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_BioAmpEXGPill\Sensor_BioAmpEXGPill.ino"
set "TEST_NAAM=Sensor BioAmpEXGPill: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_BIOAMPEXGPILL_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_BIOAMPEXGPILL_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_EGBOPulseSensor\Sensor_EGBOPulseSensor.ino"
set "TEST_NAAM=Sensor EGBOPulseSensor: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_EGBOPULSESENSOR_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_EGBOPULSESENSOR_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MAX4466\Sensor_MAX4466.ino"
set "TEST_NAAM=Sensor MAX4466: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_MAX4466_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_MAX4466_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MAX9814\Sensor_MAX9814.ino"
set "TEST_NAAM=Sensor MAX9814: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_MAX9814_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_MAX9814_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_LM35DZ\Sensor_LM35DZ.ino"
set "TEST_NAAM=Sensor LM35DZ: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_LM35DZ_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_LM35DZ_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=REDIRECT_SERIAL bevat een onbekende redirect"
set "TEST_FLAGS=-DREDIRECT_SERIAL=0x80"
set "VERWACHTE_FOUT=REDIRECT_SERIAL bevat een onbekende redirect."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=REDIRECT_SERIAL_TO_MICROSD vereist REDIRECT_SERIAL_CONNECT_TIMEOUT_MS"
set "TEST_FLAGS=-DREDIRECT_SERIAL=REDIRECT_SERIAL_TO_MICROSD -UREDIRECT_SERIAL_CONNECT_TIMEOUT_MS -DDATA_LAYER_MICROSD_SPI_AANTAL=1"
set "VERWACHTE_FOUT=REDIRECT_SERIAL_TO_MICROSD vereist REDIRECT_SERIAL_CONNECT_TIMEOUT_MS."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MixedPins\Sensor_MixedPins.ino"
set "TEST_NAAM=DATA_LAYER_FLASH_SPI_1_CS_EXTENDER vereist DATA_LAYER_FLASH_SPI_1_CS_EXTENDER_I2C_ADDRESS"
set "TEST_FLAGS=-DDATA_LAYER_FLASH_SPI_1_CS_EXTENDER=0"
set "VERWACHTE_FOUT=DATA_LAYER_FLASH_SPI_1_CS_EXTENDER vereist DATA_LAYER_FLASH_SPI_1_CS_EXTENDER_I2C_ADDRESS."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MixedPins\Sensor_MixedPins.ino"
set "TEST_NAAM=DATA_LAYER_FLASH_SPI_2_CS_EXTENDER vereist DATA_LAYER_FLASH_SPI_2_CS_EXTENDER_I2C_ADDRESS"
set "TEST_FLAGS=-DDATA_LAYER_FLASH_SPI_2_CS_EXTENDER=0"
set "VERWACHTE_FOUT=DATA_LAYER_FLASH_SPI_2_CS_EXTENDER vereist DATA_LAYER_FLASH_SPI_2_CS_EXTENDER_I2C_ADDRESS."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MixedPins\Sensor_MixedPins.ino"
set "TEST_NAAM=DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER vereist DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER_I2C_ADD..."
set "TEST_FLAGS=-DDATA_LAYER_MICROSD_SPI_1_CS_EXTENDER=0"
set "VERWACHTE_FOUT=DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER vereist DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER_I2C_ADDRESS."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=DATA_TYPE_EEPROM_SPI en DATA_TYPE_FRAM_SPI worden nog niet ondersteund in v2.0.0"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_EEPROM_SPI"
set "VERWACHTE_FOUT=DATA_TYPE_EEPROM_SPI en DATA_TYPE_FRAM_SPI worden nog niet ondersteund in v2.0.0."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=DATA_LAYER_CONFIG bevat een onbekend datatype"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=0x0800"
set "VERWACHTE_FOUT=DATA_LAYER_CONFIG bevat een onbekend datatype."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=DATA_LAYER_CONFIG moet minstens één datatype bevatten"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=0"
set "VERWACHTE_FOUT=DATA_LAYER_CONFIG moet minstens één datatype bevatten."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=DATA_TYPE_NONE kan niet met een ander datatype worden gecombineerd"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_NONE|DATA_TYPE_EEPROM_NATIVE"
set "VERWACHTE_FOUT=DATA_TYPE_NONE kan niet met een ander datatype worden gecombineerd."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=DATA_TYPE_EEPROM_NATIVE vereist DATA_LAYER_EEPROM_NATIVE_SIZE > 0"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_EEPROM_NATIVE -DDATA_LAYER_EEPROM_NATIVE_SIZE=0"
set "VERWACHTE_FOUT=DATA_TYPE_EEPROM_NATIVE vereist DATA_LAYER_EEPROM_NATIVE_SIZE > 0."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=DATA_TYPE_EEPROM_I2C vereist DATA_LAYER_EEPROM_I2C_AANTAL >= 1"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_EEPROM_I2C -DDATA_LAYER_EEPROM_I2C_AANTAL=0"
set "VERWACHTE_FOUT=DATA_TYPE_EEPROM_I2C vereist DATA_LAYER_EEPROM_I2C_AANTAL >= 1."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=DATA_LAYER_EEPROM_I2C_AANTAL > 0 vereist DATA_TYPE_EEPROM_I2C in DATA_LAYER_CONFIG"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_EEPROM_NATIVE -DDATA_LAYER_EEPROM_I2C_AANTAL=1"
set "VERWACHTE_FOUT=DATA_LAYER_EEPROM_I2C_AANTAL > 0 vereist DATA_TYPE_EEPROM_I2C in DATA_LAYER_CONFIG."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=DATA_LAYER_EEPROM_I2C_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_EEPROM_I2C -DDATA_LAYER_EEPROM_I2C_AANTAL=3"
set "VERWACHTE_FOUT=DATA_LAYER_EEPROM_I2C_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=DATA_TYPE_FLASH_SPI vereist DATA_LAYER_FLASH_SPI_AANTAL >= 1"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_FLASH_SPI -DDATA_LAYER_FLASH_SPI_AANTAL=0"
set "VERWACHTE_FOUT=DATA_TYPE_FLASH_SPI vereist DATA_LAYER_FLASH_SPI_AANTAL >= 1."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=DATA_LAYER_FLASH_SPI_AANTAL > 0 vereist DATA_TYPE_FLASH_SPI in DATA_LAYER_CONFIG"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_EEPROM_NATIVE -DDATA_LAYER_FLASH_SPI_AANTAL=1"
set "VERWACHTE_FOUT=DATA_LAYER_FLASH_SPI_AANTAL > 0 vereist DATA_TYPE_FLASH_SPI in DATA_LAYER_CONFIG."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_FLASH\Sensor_FsrRfp_FLASH.ino"
set "TEST_NAAM=DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE mag maximaal 256 zijn (één pagina)"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_FLASH_SPI -DDATA_LAYER_FLASH_SPI_AANTAL=1 -DDATA_LAYER_FLASH_SPI_1_BUFFER_SIZE=257"
set "VERWACHTE_FOUT=DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE mag maximaal 256 zijn (één pagina)."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_FLASH\Sensor_FsrRfp_FLASH.ino"
set "TEST_NAAM=DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE mag maximaal 256 zijn (één pagina)"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_FLASH_SPI -DDATA_LAYER_FLASH_SPI_AANTAL=2 -DDATA_LAYER_FLASH_SPI_2_BUFFER_SIZE=257"
set "VERWACHTE_FOUT=DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE mag maximaal 256 zijn (één pagina)."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_FLASH\Sensor_FsrRfp_FLASH.ino"
set "TEST_NAAM=DATA_LAYER_FLASH_SPI_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_FLASH_SPI -DDATA_LAYER_FLASH_SPI_AANTAL=3"
set "VERWACHTE_FOUT=DATA_LAYER_FLASH_SPI_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_FLASH\Sensor_FsrRfp_FLASH.ino"
set "TEST_NAAM=DATA_TYPE_FRAM_I2C vereist DATA_LAYER_FRAM_I2C_AANTAL >= 1"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_FRAM_I2C -DDATA_LAYER_FRAM_I2C_AANTAL=0"
set "VERWACHTE_FOUT=DATA_TYPE_FRAM_I2C vereist DATA_LAYER_FRAM_I2C_AANTAL >= 1."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=DATA_LAYER_FRAM_I2C_AANTAL > 0 vereist DATA_TYPE_FRAM_I2C in DATA_LAYER_CONFIG"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_EEPROM_NATIVE -DDATA_LAYER_FRAM_I2C_AANTAL=1"
set "VERWACHTE_FOUT=DATA_LAYER_FRAM_I2C_AANTAL > 0 vereist DATA_TYPE_FRAM_I2C in DATA_LAYER_CONFIG."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=DATA_LAYER_FRAM_I2C_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_FRAM_I2C -DDATA_LAYER_FRAM_I2C_AANTAL=3"
set "VERWACHTE_FOUT=DATA_LAYER_FRAM_I2C_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=DATA_TYPE_MICROSD_SPI vereist DATA_LAYER_MICROSD_SPI_AANTAL >= 1"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_MICROSD_SPI -DDATA_LAYER_MICROSD_SPI_AANTAL=0"
set "VERWACHTE_FOUT=DATA_TYPE_MICROSD_SPI vereist DATA_LAYER_MICROSD_SPI_AANTAL >= 1."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=DATA_LAYER_MICROSD_SPI_AANTAL > 0 vereist DATA_TYPE_MICROSD_SPI in DATA_LAYER_CONFIG of R..."
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_EEPROM_NATIVE -DDATA_LAYER_MICROSD_SPI_AANTAL=1"
set "VERWACHTE_FOUT=DATA_LAYER_MICROSD_SPI_AANTAL > 0 vereist DATA_TYPE_MICROSD_SPI in DATA_LAYER_CONFIG of REDIRECT_SERIAL_TO_MICROSD."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=DATA_LAYER_MICROSD_SPI_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_MICROSD_SPI -DDATA_LAYER_MICROSD_SPI_AANTAL=3"
set "VERWACHTE_FOUT=DATA_LAYER_MICROSD_SPI_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MixedPins\Sensor_MixedPins.ino"
set "TEST_NAAM=EXTENDER_PCF8574_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_PCF8574_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_PCF8574_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\Data\microSD\microSD.ino"
set "TEST_NAAM=microSD met CS via een Extender: zet SD_CHIP_SELECT_MODE op 1 in SdFatConfig.h van de lib..."
set "TEST_FLAGS=-DDATA_LAYER_CONFIG=DATA_TYPE_MICROSD_SPI -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20 -DDATA_LAYER_MICROSD_SPI_1_CS_EXTENDER=GedeeldeBusComponent::DIGITAL_PINS_PCF8574 -DDATA_LAYER_MICROSD_SPI_1_CS_EXTENDER_I2C_ADDRESS=0x20 -DDATA_LAYER_MICROSD_SPI_1_CS_PIN=ExtenderPCF8574::ExtenderPins::EP_P0 -DSD_CHIP_SELECT_MODE=0"
set "VERWACHTE_FOUT=microSD met CS via een Extender: zet SD_CHIP_SELECT_MODE op 1 in SdFatConfig.h van de library "SdFat - Adafruit Fork"."
call :EXPECT_FAIL

echo.
set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=FsrRfp set 2: ongeldig aantal"
set "TEST_FLAGS=-DFSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2=3"
set "VERWACHTE_FOUT=FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 moet 2 of 4 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=FsrRfp set 2 vereist tweede ADS1115"
set "TEST_FLAGS=-DFSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2=2 -DEXTENDER_ADS1115_AANTAL=1"
set "VERWACHTE_FOUT=FsrRfp set 2 vraagt een eigen ADS1115: zet EXTENDER_ADS1115_AANTAL op 2."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=microSD: ongeldig flushinterval"
set "TEST_FLAGS=-DDATA_LAYER_MICROSD_SPI_FLUSH_NA_AANTAL_REGEL=0"
set "VERWACHTE_FOUT=DATA_LAYER_MICROSD_SPI_FLUSH_NA_AANTAL_REGEL moet tussen 1 en 255 liggen."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=ADC: ongeldige resolutie"
set "TEST_FLAGS=-DADC_BITS=9"
set "VERWACHTE_FOUT=ADC_BITS moet 10, 12, 14 of 15 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_DS18B20\Sensor_DS18B20.ino"
set "TEST_NAAM=SENSOR_DS18B20_AANTAL: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_DS18B20_AANTAL=-1"
set "VERWACHTE_FOUT=SENSOR_DS18B20_AANTAL mag niet negatief zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=FSR_RFP_ADC_BEREIK_MILLIVOLT: ongeldige configuratie"
set "TEST_FLAGS=-DFSR_RFP_ADC_BEREIK_MILLIVOLT=0"
set "VERWACHTE_FOUT=FSR_RFP_ADC_BEREIK_MILLIVOLT moet groter dan 0 zijn (de maximale spanning van de sensoruitgang in mV)."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=FSR_RFP_PROBE_SET_1: ongeldige configuratie"
set "TEST_FLAGS=-DFSR_RFP_PROBE_SET_1=999"
set "VERWACHTE_FOUT=FSR_RFP_PROBE_SET_1 moet FSR_RFP_PROBE_RFP602, FSR_RFP_PROBE_FSR400 of FSR_RFP_PROBE_FSR402 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_GSRGrove\Sensor_GSRGrove.ino"
set "TEST_NAAM=SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT=0"
set "VERWACHTE_FOUT=SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT moet groter dan 0 zijn (de maximale spanning van de sensoruitgang in mV)."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=SENSOR_INMP441_AANTAL: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_INMP441_AANTAL=2"
set "VERWACHTE_FOUT=SENSOR_INMP441_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=SENSOR_MS3625_AANTAL: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_MS3625_AANTAL=2"
set "VERWACHTE_FOUT=SENSOR_MS3625_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MCU6701_Analoog\Sensor_MCU6701_Analoog.ino"
set "TEST_NAAM=SENSOR_MCU6701_AANTAL: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_MCU6701_AANTAL=2"
set "VERWACHTE_FOUT=SENSOR_MCU6701_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MF52A\Sensor_MF52A.ino"
set "TEST_NAAM=SENSOR_MF52A_ADC_BEREIK_MILLIVOLT: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_MF52A_ADC_BEREIK_MILLIVOLT=0"
set "VERWACHTE_FOUT=SENSOR_MF52A_ADC_BEREIK_MILLIVOLT moet groter dan 0 zijn (de maximale spanning van de sensoruitgang in mV)."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MF52D\Sensor_MF52D.ino"
set "TEST_NAAM=SENSOR_MF52D_ADC_BEREIK_MILLIVOLT: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_MF52D_ADC_BEREIK_MILLIVOLT=0"
set "VERWACHTE_FOUT=SENSOR_MF52D_ADC_BEREIK_MILLIVOLT moet groter dan 0 zijn (de maximale spanning van de sensoruitgang in mV)."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_PIEZO\Sensor_PIEZO.ino"
set "TEST_NAAM=SENSOR_PIEZO_ADC_BEREIK_MILLIVOLT: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_PIEZO_ADC_BEREIK_MILLIVOLT=0"
set "VERWACHTE_FOUT=SENSOR_PIEZO_ADC_BEREIK_MILLIVOLT moet groter dan 0 zijn (de maximale spanning van de sensoruitgang in mV)."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_TTP223_Metingen\Sensor_TTP223_Metingen.ino"
set "TEST_NAAM=SENSOR_TTP223_AANTAL: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_TTP223_AANTAL=2"
set "VERWACHTE_FOUT=SENSOR_TTP223_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_AD8232\Sensor_AD8232.ino"
set "TEST_NAAM=EXTENDER_ADS1115_DATARATE: ongeldige configuratie (sensoren)"
set "TEST_FLAGS=-DEXTENDER_ADS1115_DATARATE=8"
set "VERWACHTE_FOUT=EXTENDER_ADS1115_DATARATE moet tussen 0 en 7 liggen."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_DS3231\Sensor_DS3231.ino"
set "TEST_NAAM=Sensor DS3231: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_DS3231_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_DS3231_AANTAL moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_DS3231\Sensor_DS3231.ino"
set "TEST_NAAM=I2C_ADDRESS_SENSOR_DS3231_1: ongeldig adres"
set "TEST_FLAGS=-DSENSOR_DS3231_AANTAL=1 -DI2C_ADDRESS_SENSOR_DS3231_1=0x69 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=I2C_ADDRESS_SENSOR_DS3231_1 moet 0x68 zijn: het adres van de DS3231 ligt vast in de chip."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_BioAmpEXGPill_ECG_Hartslag\Sensor_BioAmpEXGPill_ECG_Hartslag.ino"
set "TEST_NAAM=SENSOR_BIOAMPEXGPILL_ENVELOP_VENSTER: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_BIOAMPEXGPILL_ENVELOP_VENSTER=0 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_BIOAMPEXGPILL_ENVELOP_VENSTER moet tussen 1 en 1024 liggen."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MS3625\Sensor_MS3625.ino"
set "TEST_NAAM=SENSOR_INMP441_SAMPLEFREQUENTIE_HZ: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_INMP441_SAMPLEFREQUENTIE_HZ=7999 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_INMP441_SAMPLEFREQUENTIE_HZ moet tussen 8000 en 48000 Hz liggen."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MS3625\Sensor_MS3625.ino"
set "TEST_NAAM=SENSOR_MS3625_SAMPLEFREQUENTIE_HZ: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_MS3625_SAMPLEFREQUENTIE_HZ=7999 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_MS3625_SAMPLEFREQUENTIE_HZ moet tussen 8000 en 48000 Hz liggen."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_BioAmpEXGPill\Sensor_BioAmpEXGPill.ino"
set "TEST_NAAM=SENSOR_BIOAMPEXGPILL_MEETMETHODE: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_BIOAMPEXGPILL_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DSENSOR_BIOAMPEXGPILL_MEETMETHODE=99 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=Onbekende SENSOR_BIOAMPEXGPILL_MEETMETHODE"
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MS3625\Sensor_MS3625.ino"
set "TEST_NAAM=MS3625 via I2S: UNO R4 niet ondersteund"
set "TEST_FLAGS=-DSENSOR_MS3625_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=INMP441 en MS3625 vereisen I2S; de RA4M1 van de UNO R4 heeft geen I2S-interface."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MAX4466\Sensor_MAX4466.ino"
set "TEST_NAAM=Sensor_MAX4466: EXTENDER_ADS1115_DATARATE te traag"
set "TEST_FLAGS=-DSENSOR_MAX4466_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DEXTENDER_ADS1115_DATARATE=4 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=EXTENDER_ADS1115_DATARATE is te traag voor SENSOR_MAX4466_SAMPLE_RATE_HZ; verhoog EXTENDER_ADS1115_DATARATE of verlaag de samplefrequentie."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MAX9814\Sensor_MAX9814.ino"
set "TEST_NAAM=Sensor_MAX9814: EXTENDER_ADS1115_DATARATE te traag"
set "TEST_FLAGS=-DSENSOR_MAX9814_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DEXTENDER_ADS1115_DATARATE=4 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=EXTENDER_ADS1115_DATARATE is te traag voor SENSOR_MAX9814_SAMPLE_RATE_HZ; verhoog EXTENDER_ADS1115_DATARATE of verlaag de samplefrequentie."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_BioAmpEXGPill_EMG_Envelop\Sensor_BioAmpEXGPill_EMG_Envelop.ino"
set "TEST_NAAM=Sensor_BioAmpEXGPill_EMG_Envelop: EXTENDER_ADS1115_DATARATE te traag"
set "TEST_FLAGS=-DSENSOR_BIOAMPEXGPILL_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DEXTENDER_ADS1115_DATARATE=4 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSENSOR_BIOAMPEXGPILL_MEETMETHODE=BIOAMPEXGPILL_MEETMETHODE_EMG"
set "VERWACHTE_FOUT=EXTENDER_ADS1115_DATARATE is te traag voor de vaste EMG-frequentie van 500 Hz; verhoog EXTENDER_ADS1115_DATARATE."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_BioAmpEXGPill_EOG_Knipperen\Sensor_BioAmpEXGPill_EOG_Knipperen.ino"
set "TEST_NAAM=Sensor_BioAmpEXGPill_EOG_Knipperen: EXTENDER_ADS1115_DATARATE te traag"
set "TEST_FLAGS=-DSENSOR_BIOAMPEXGPILL_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DEXTENDER_ADS1115_DATARATE=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSENSOR_BIOAMPEXGPILL_MEETMETHODE=BIOAMPEXGPILL_MEETMETHODE_EOG"
set "VERWACHTE_FOUT=EXTENDER_ADS1115_DATARATE is te traag voor de vaste EOG-frequentie van 75 Hz; verhoog EXTENDER_ADS1115_DATARATE."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_BioAmpEXGPill_ECG_Hartslag\Sensor_BioAmpEXGPill_ECG_Hartslag.ino"
set "TEST_NAAM=Sensor_BioAmpEXGPill_ECG_Hartslag: EXTENDER_ADS1115_DATARATE te traag"
set "TEST_FLAGS=-DSENSOR_BIOAMPEXGPILL_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DEXTENDER_ADS1115_DATARATE=3 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSENSOR_BIOAMPEXGPILL_MEETMETHODE=BIOAMPEXGPILL_MEETMETHODE_ECG"
set "VERWACHTE_FOUT=EXTENDER_ADS1115_DATARATE is te traag voor de vaste ECG-frequentie van 125 Hz; verhoog EXTENDER_ADS1115_DATARATE."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_BioAmpEXGPill\Sensor_BioAmpEXGPill.ino"
set "TEST_NAAM=Sensor_BioAmpEXGPill: EXTENDER_ADS1115_DATARATE te traag"
set "TEST_FLAGS=-DSENSOR_BIOAMPEXGPILL_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DEXTENDER_ADS1115_DATARATE=3 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=EXTENDER_ADS1115_DATARATE is te traag voor de samplefrequentie van de gekozen SENSOR_BIOAMPEXGPILL_MEETMETHODE; verhoog EXTENDER_ADS1115_DATARATE."
call :EXPECT_FAIL

:: De twee volgende tests controleren alleen de #error; ze compileren met de UNO R4-core en een andere BOARD_VERSION, niet met de core van dat board.
set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MS3625\Sensor_MS3625.ino"
set "TEST_NAAM=MS3625 via I2S: UNO Q niet ondersteund"
set "TEST_FLAGS=-DSENSOR_MS3625_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DBOARD_VERSION=BOARD_UNO_Q"
set "VERWACHTE_FOUT=INMP441 en MS3625 worden op de UNO Q momenteel niet ondersteund: er is nog geen werkende I2S-implementatie aangetoond voor de gebruikte Arduino-core."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_MS3625\Sensor_MS3625.ino"
set "TEST_NAAM=MS3625 via I2S: Nucleo F401RE niet ondersteund"
set "TEST_FLAGS=-DSENSOR_MS3625_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DBOARD_VERSION=BOARD_STM32F4_NUCLEO64_F401RE"
set "VERWACHTE_FOUT=INMP441 en MS3625 worden op de Nucleo F401RE niet ondersteund: de STM32-core heeft geen standaard I2S-library."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_LM35DZ\Sensor_LM35DZ.ino"
set "TEST_NAAM=SENSOR_LM35DZ_ADC_BEREIK_MILLIVOLT: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_LM35DZ_AANTAL=1 -DEXTENDER_ADS1115_AANTAL=1 -DSENSOR_LM35DZ_ADC_BEREIK_MILLIVOLT=0 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_LM35DZ_ADC_BEREIK_MILLIVOLT moet groter dan 0 zijn (de maximale spanning van de sensoruitgang in mV)."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_OT3686_Metingen\Sensor_OT3686_Metingen.ino"
set "TEST_NAAM=Sensor OT3686: maximaal een exemplaar"
set "TEST_FLAGS=-DSENSOR_OT3686_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_OT3686_AANTAL moet 0 of 1 zijn (één native interrupt per Sensor)."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_OT3686_Metingen\Sensor_OT3686_Metingen.ino"
set "TEST_NAAM=SENSOR_OT3686_AANTAL_SLEUVEN: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_OT3686_AANTAL=1 -DSENSOR_OT3686_AANTAL_SLEUVEN=0 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_OT3686_AANTAL_SLEUVEN moet minstens 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_OT3686_Metingen\Sensor_OT3686_Metingen.ino"
set "TEST_NAAM=SENSOR_OT3686_MEETINTERVAL_MS: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_OT3686_AANTAL=1 -DSENSOR_OT3686_MEETINTERVAL_MS=5 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_OT3686_MEETINTERVAL_MS moet minstens 10 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_TTP223_Metingen\Sensor_TTP223_Metingen.ino"
set "TEST_NAAM=SENSOR_TTP223_DIRECTE_MODUS: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_TTP223_AANTAL=1 -DSENSOR_TTP223_DIRECTE_MODUS=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_TTP223_DIRECTE_MODUS moet 0 of 1 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_ADS7828\Sensor_FsrRfp_ADS7828.ino"
set "TEST_NAAM=EXTENDER_ADS7828_REFERENTIE_MILLIVOLT: ongeldige configuratie"
set "TEST_FLAGS=-DEXTENDER_ADS7828_AANTAL=1 -DEXTENDER_ADS7828_REFERENTIE_MILLIVOLT=6000 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=EXTENDER_ADS7828_REFERENTIE_MILLIVOLT moet binnen 1..5250 mV liggen."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_AD8232_ADS7953\Sensor_AD8232_ADS7953.ino"
set "TEST_NAAM=EXTENDER_ADS7953_VOEDING_MILLIVOLT: ongeldige configuratie"
set "TEST_FLAGS=-DEXTENDER_ADS7953_AANTAL=1 -DSENSOR_AD8232_AANTAL=1 -DEXTENDER_ADS7953_VOEDING_MILLIVOLT=1000 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=EXTENDER_ADS7953_VOEDING_MILLIVOLT moet 0 of 2700..5250 mV zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp_CD74HC4067\Sensor_FsrRfp_CD74HC4067.ino"
set "TEST_NAAM=EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT: ongeldige configuratie"
set "TEST_FLAGS=-DEXTENDER_CD74HC4067_AANTAL=1 -DEXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT=6000 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT moet 0 of maximaal 5500 mV zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_HX711\Sensor_HX711.ino"
set "TEST_NAAM=SENSOR_HX711_TARE_SAMPLE_COUNT: ongeldige configuratie"
set "TEST_FLAGS=-DSENSOR_HX711_AANTAL=1 -DSENSOR_HX711_TARE_SAMPLE_COUNT=0 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=SENSOR_HX711_TARE_SAMPLE_COUNT moet tussen 1 en 255 liggen."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=NATIVE_ADC_RESOLUTIE_BITS buiten 1 tot 16"
set "TEST_FLAGS=-DNATIVE_ADC_RESOLUTIE_BITS=0"
set "VERWACHTE_FOUT=NATIVE_ADC_RESOLUTIE_BITS moet tussen 1 en 16 liggen."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=FSR_RFP_PROBE_SET_2 met een onbekende probe"
set "TEST_FLAGS=-DFSR_RFP_PROBE_SET_2=99"
set "VERWACHTE_FOUT=FSR_RFP_PROBE_SET_2 moet FSR_RFP_PROBE_RFP602, FSR_RFP_PROBE_FSR400 of FSR_RFP_PROBE_FSR402 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1 met dalende aantallen"
set "TEST_FLAGS=-DEXTENDER_ADS1115_AANTAL=1 -DFSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1={{5,3,5,10,20},{0UL,100UL,500UL,2000UL,10000UL},ScreenData::TYPE_FATAL,ScreenData::TYPE_CRITICAL}"
set "VERWACHTE_FOUT=FSR_RFP_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sensoren\Sensor_FsrRfp\Sensor_FsrRfp.ino"
set "TEST_NAAM=FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1 met dalende aantallen"
set "TEST_FLAGS=-DEXTENDER_ADS1115_AANTAL=1 -DFSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1={{5,3,5,10,20},{0UL,100UL,500UL,2000UL,10000UL},ScreenData::TYPE_FATAL,ScreenData::TYPE_CRITICAL}"
set "VERWACHTE_FOUT=FSR_RFP_CHANNEL_1_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL."
call :EXPECT_FAIL

echo Totaal !TESTS! - OK !OK! - FOUT !FAIL!
if !FAIL!==0 (exit /b 0) else exit /b 1
