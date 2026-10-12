@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryData-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\Data\;"

call "%~dp0TestLibraryCommon.cmd" %*
exit /b %errorlevel%

:TESTLIJST
set TEST_INO="examples\Systeem\Data\EEPROM\EEPROM.ino"
set TEST_NAAM="Data EEPROM"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_EEPROM"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Data\FLASH\FLASH.ino"
set TEST_NAAM="Data FLASH"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_FLASH"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Data\microSD\microSD.ino"
set TEST_NAAM="Data microSD"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_MICROSD"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_RFP602\Sensor_RFP602.ino"
set TEST_NAAM="Data Serial redirect | DATA_TYPE_NONE | REDIRECT_SERIAL_TO_MICROSD"
set TEST_FLAGS="-DADC_BACKEND=ADC_BACKEND_NATIVE -DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DREDIRECT_SERIAL=REDIRECT_SERIAL_TO_MICROSD"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

exit /b 0
