@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryExtenders-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\GedeeldeBus\Extenders\;"

call "%~dp0TestLibraryCommon.cmd" %*
exit /b %errorlevel%

:TESTLIJST
set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1115\Extender_ADS1115.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_ADS1115_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1115\Extender_ADS1115.ino"
set TEST_NAAM="EXTENDER_ADS1115_LIBRARY_ROB_TILLAART"
set TEST_FLAGS="-DEXTENDER_ADS1115_AANTAL=1 -DEXTENDER_ADS1115_LIBRARY=EXTENDER_ADS1115_LIBRARY_ROB_TILLAART"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1115\Extender_ADS1115.ino"
set TEST_NAAM="EXTENDER_ADS1115_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_ADS1115_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1115_Test_ExtenderPins_Geweigerd\Extender_ADS1115_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_ADS1115_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1115_Test_ExtenderPins_Toegelaten\Extender_ADS1115_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_ADS1115_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1158\Extender_ADS1158.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_ADS1158_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1158\Extender_ADS1158.ino"
set TEST_NAAM="EXTENDER_ADS1158_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_ADS1158_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1158_Test_ExtenderPins_Geweigerd\Extender_ADS1158_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_ADS1158_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1158_Test_ExtenderPins_Toegelaten\Extender_ADS1158_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_ADS1158_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7828\Extender_ADS7828.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_ADS7828_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7828\Extender_ADS7828.ino"
set TEST_NAAM="EXTENDER_ADS7828_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_ADS7828_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7828_Test_ExtenderPins_Geweigerd\Extender_ADS7828_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_ADS7828_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7828_Test_ExtenderPins_Toegelaten\Extender_ADS7828_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_ADS7828_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7953\Extender_ADS7953.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_ADS7953_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7953\Extender_ADS7953.ino"
set TEST_NAAM="EXTENDER_ADS7953_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_ADS7953_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7953_Test_ExtenderPins_Geweigerd\Extender_ADS7953_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_ADS7953_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7953_Test_ExtenderPins_Toegelaten\Extender_ADS7953_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_ADS7953_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_CD74HC4067\Extender_CD74HC4067.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_CD74HC4067_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_CD74HC4067\Extender_CD74HC4067.ino"
set TEST_NAAM="EXTENDER_CD74HC4067_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_CD74HC4067_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_CD74HC4067_Test_ExtenderPins_Geweigerd\Extender_CD74HC4067_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_CD74HC4067_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_CD74HC4067_Test_ExtenderPins_Toegelaten\Extender_CD74HC4067_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_CD74HC4067_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_DS2482v800\Extender_DS2482v800.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_DS2482_800_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_DS2482v800\Extender_DS2482v800.ino"
set TEST_NAAM="EXTENDER_DS2482_800_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_DS2482_800_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_DS2482v800_Test_ExtenderPins_Geweigerd\Extender_DS2482v800_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_DS2482_800_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_DS2482v800_Test_ExtenderPins_Toegelaten\Extender_DS2482v800_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_DS2482_800_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_I2C\Extender_MAX14830_I2C.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_MAX14830_I2C_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_I2C\Extender_MAX14830_I2C.ino"
set TEST_NAAM="EXTENDER_MAX14830_I2C_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_MAX14830_I2C_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_I2C_Test_ExtenderPins_Geweigerd\Extender_MAX14830_I2C_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_MAX14830_I2C_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_I2C_Test_ExtenderPins_Toegelaten\Extender_MAX14830_I2C_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_MAX14830_I2C_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_SPI\Extender_MAX14830_SPI.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_MAX14830_SPI_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_SPI\Extender_MAX14830_SPI.ino"
set TEST_NAAM="EXTENDER_MAX14830_SPI_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_MAX14830_SPI_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_SPI_Test_ExtenderPins_Geweigerd\Extender_MAX14830_SPI_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_MAX14830_SPI_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_SPI_Test_ExtenderPins_Toegelaten\Extender_MAX14830_SPI_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_MAX14830_SPI_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MCP23017\Extender_MCP23017.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_MCP23017_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MCP23017\Extender_MCP23017.ino"
set TEST_NAAM="EXTENDER_MCP23017_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_MCP23017_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MCP23017_Test_ExtenderPins_Geweigerd\Extender_MCP23017_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_MCP23017_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MCP23017_Test_ExtenderPins_Toegelaten\Extender_MCP23017_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_MCP23017_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8574\Extender_PCF8574.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_PCF8574_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8574\Extender_PCF8574.ino"
set TEST_NAAM="EXTENDER_PCF8574_LIBRARY_ROB_TILLAART"
set TEST_FLAGS="-DEXTENDER_PCF8574_AANTAL=1 -DEXTENDER_PCF8574_LIBRARY=EXTENDER_PCF8574_LIBRARY_ROB_TILLAART"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8574\Extender_PCF8574.ino"
set TEST_NAAM="EXTENDER_PCF8574_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_PCF8574_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8574_Test_ExtenderPins_Geweigerd\Extender_PCF8574_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_PCF8574_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8574_Test_ExtenderPins_Toegelaten\Extender_PCF8574_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_PCF8574_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8575\Extender_PCF8575.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_PCF8575_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8575\Extender_PCF8575.ino"
set TEST_NAAM="EXTENDER_PCF8575_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_PCF8575_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8575_Test_ExtenderPins_Geweigerd\Extender_PCF8575_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_PCF8575_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8575_Test_ExtenderPins_Toegelaten\Extender_PCF8575_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_PCF8575_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_I2C\Extender_SC16IS752_I2C.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_SC16IS752_I2C_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_I2C\Extender_SC16IS752_I2C.ino"
set TEST_NAAM="EXTENDER_SC16IS752_I2C_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_SC16IS752_I2C_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_I2C_Test_ExtenderPins_Geweigerd\Extender_SC16IS752_I2C_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_SC16IS752_I2C_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_I2C_Test_ExtenderPins_Toegelaten\Extender_SC16IS752_I2C_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_SC16IS752_I2C_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_SPI\Extender_SC16IS752_SPI.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_SC16IS752_SPI_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_SPI\Extender_SC16IS752_SPI.ino"
set TEST_NAAM="EXTENDER_SC16IS752_SPI_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_SC16IS752_SPI_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_SPI_Test_ExtenderPins_Geweigerd\Extender_SC16IS752_SPI_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_SC16IS752_SPI_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_SPI_Test_ExtenderPins_Toegelaten\Extender_SC16IS752_SPI_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_SC16IS752_SPI_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_TCA9548A\Extender_TCA9548A.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_TCA9548A_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_TCA9548A\Extender_TCA9548A.ino"
set TEST_NAAM="EXTENDER_TCA9548A_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_TCA9548A_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_TCA9548A_Test_ExtenderPins_Geweigerd\Extender_TCA9548A_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_TCA9548A_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_TCA9548A_Test_ExtenderPins_Toegelaten\Extender_TCA9548A_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM=""
set TEST_FLAGS="-DEXTENDER_TCA9548A_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

exit /b 0
