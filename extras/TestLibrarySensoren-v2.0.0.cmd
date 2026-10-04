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
set TEST_NAAM="Sensor RFP602 | ADC_BACKEND_NATIVE"
set TEST_FLAGS="-DADC_BACKEND=ADC_BACKEND_NATIVE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_BASIC_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sensoren\Sensor_RFP602\Sensor_RFP602.ino"
set TEST_NAAM="Sensor RFP602 | ADC_BACKEND_ADS1115"
set TEST_FLAGS="-DADC_BACKEND=ADC_BACKEND_ADS1115"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_BASIC_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

exit /b 0
