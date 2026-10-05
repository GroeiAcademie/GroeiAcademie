@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryADC-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\ADC_Backend\;"

call "%~dp0TestLibraryCommon.cmd" %*
exit /b %errorlevel%

:TESTLIJST
set TEST_INO="examples\Systeem\ADC_Backend\ADC_Backend_Native\ADC_Backend_Native.ino"
set TEST_NAAM="ADC_NATIVE"
set TEST_FLAGS="-DADC_BACKEND=ADC_BACKEND_NATIVE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\ADC_Backend\ADC_Backend_ADS1115\ADC_Backend_ADS1115.ino"
set TEST_NAAM="ADC_ADS1115"
set TEST_FLAGS="-DADC_BACKEND=ADC_BACKEND_ADS1115"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\ADC_Backend\ADC_Backend_Native\ADC_Backend_Native.ino"
set TEST_NAAM="ADC_NATIVE + 1 x EXTENDER_ADS1115"
set TEST_FLAGS="-DADC_BACKEND=ADC_BACKEND_NATIVE -DEXTENDER_ADS1115_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\ADC_Backend\ADC_Backend_ADS1115\ADC_Backend_ADS1115.ino"
set TEST_NAAM="ADC_ADS1115 + 1 x EXTENDER_ADS1115"
set TEST_FLAGS="-DADC_BACKEND=ADC_BACKEND_ADS1115 -DEXTENDER_ADS1115_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\ADC_Backend\ADC_Backend_ADS1115\ADC_Backend_ADS1115.ino"
set TEST_NAAM="ADC_ADS1115 + 2 x EXTENDER_ADS1115"
set TEST_FLAGS="-DADC_BACKEND=ADC_BACKEND_ADS1115 -DEXTENDER_ADS1115_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

exit /b 0
