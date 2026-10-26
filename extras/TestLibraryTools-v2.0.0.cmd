@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryTools-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\Tools\;"

call "%~dp0TestLibraryCommon.cmd" %*
exit /b %errorlevel%

:TESTLIJST
set TEST_INO="examples\Systeem\Tools\I2C\I2C_DiagnoseAangemeldeDevices\I2C_DiagnoseAangemeldeDevices.ino"
set TEST_NAAM="I2C Diagnose Aangemelde Devices"
set TEST_FLAGS=""
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Tools\I2C\I2C_DiagnoseDS2482v800\I2C_DiagnoseDS2482v800.ino"
set TEST_NAAM="I2C Diagnose DS2482v800"
set TEST_FLAGS="-DEXTENDER_DS2482_800_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Tools\I2C\I2C_DiagnoseTCA9548A\I2C_DiagnoseTCA9548A.ino"
set TEST_NAAM="I2C Diagnose TCA9548A"
set TEST_FLAGS="-DEXTENDER_TCA9548A_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Tools\I2C\I2C_MetenHerstelEnBlokkeertijd\I2C_MetenHerstelEnBlokkeertijd.ino"
set TEST_NAAM="I2C Meten Herstel En Blokkeertijd"
set TEST_FLAGS="-DEXTENDER_ADS1115_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Tools\I2C\I2C_MetenReactietijdEnBetrouwbaarheid\I2C_MetenReactietijdEnBetrouwbaarheid.ino"
set TEST_NAAM="I2C Meten Reactietijd En Betrouwbaarheid"
set TEST_FLAGS=""
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Tools\I2C\I2C_OpsporenNietAangemeldeDevices\I2C_OpsporenNietAangemeldeDevices.ino"
set TEST_NAAM="I2C Opsporen Niet Aangemelde Devices"
set TEST_FLAGS=""
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Tools\I2C\I2C_Scan\I2C_Scan.ino"
set TEST_NAAM="I2C Scan"
set TEST_FLAGS=""
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Tools\I2C\I2C_TestenKloksnelhedenAangemeldeDevices\I2C_TestenKloksnelhedenAangemeldeDevices.ino"
set TEST_NAAM="I2C Testen Kloksnelheden Aangemelde Devices"
set TEST_FLAGS=""
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Tools\I2C\I2C_TotaleDiagnoseAlleDevices\I2C_TotaleDiagnoseAlleDevices.ino"
set TEST_NAAM="I2C Totale Diagnose Alle Devices"
set TEST_FLAGS=""
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

exit /b 0
