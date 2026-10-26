@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibrarySensoren-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\GedeeldeBus\Sensoren\;"

call "%~dp0TestLibraryCommon.cmd" %*
exit /b %errorlevel%

:TESTLIJST

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_RFP602\Sensor_RFP602.ino"
set TEST_NAAM="Sensor RFP602 | ExtenderADS1115"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_RFP602\Sensor_RFP602.ino"
set TEST_NAAM="Sensor RFP602 | ExtenderADS1115 | 4 sensoren"
set TEST_FLAGS="-DRFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1=4 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_RFP602_DIRECT\Sensor_RFP602_DIRECT.ino"
set TEST_NAAM="Sensor RFP602 | DIRECT"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_DIRECT"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_RFP602_FLASH\Sensor_RFP602_FLASH.ino"
set TEST_NAAM="Sensor RFP602 | FLASH"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_FLASH_SPI"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_RFP602_MICROSD\Sensor_RFP602_MICROSD.ino"
set TEST_NAAM="Sensor RFP602 | MICROSD"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_MICROSD_SPI"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_RFP602_TweeSetsOpTweeADS1115\Sensor_RFP602_TweeSetsOpTweeADS1115.ino"
set TEST_NAAM="Sensor RFP602 | twee sets op twee ExtenderADS1115"
set TEST_FLAGS="-DRFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2=4 -DEXTENDER_ADS1115_AANTAL=2 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_RFP602_Test_OngeldigePin\Sensor_RFP602_Test_OngeldigePin.ino"
set TEST_NAAM="Sensor RFP602 | test ongeldige pin (T2-2)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_RFP602_Test_ADC_Bezet\Sensor_RFP602_Test_ADC_Bezet.ino"
set TEST_NAAM="Sensor RFP602 | test ADC bezet (T2-3)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DRFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_RFP602_Test_Hardwarefout\Sensor_RFP602_Test_Hardwarefout.ino"
set TEST_NAAM="Sensor RFP602 | test hardwarefout (T2-4)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_RFP602_Test_Timeout_Conversie\Sensor_RFP602_Test_Timeout_Conversie.ino"
set TEST_NAAM="Sensor RFP602 | test ophalen na verlies van de ADC (T2-5)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_RFP602_Test_ZonderGet\Sensor_RFP602_Test_ZonderGet.ino"
set TEST_NAAM="Sensor RFP602 | test start zonder ophalen (T2-6)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DRFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_RFP602_Test_VerkeerdeParent\Sensor_RFP602_Test_VerkeerdeParent.ino"
set TEST_NAAM="Sensor RFP602 | test verkeerde parent (T2-7)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_RFP602_Test_EigenBuffers\Sensor_RFP602_Test_EigenBuffers.ino"
set TEST_NAAM="Sensor RFP602 | test eigen buffers per object (T2-9)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_DIRECT -DRFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1=8 -DRFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_MixedPins\Sensor_MixedPins.ino"
set TEST_NAAM="Sensor Mixed Pins | 2 board + 2 PCF8574"
set TEST_FLAGS="-DGEDEELDE_BUS_MIXED_PINS=1 -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

exit /b 0
