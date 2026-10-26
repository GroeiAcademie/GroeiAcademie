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
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender ADS1115\Extender ADS1115 | -DEXTENDER_ADS1115_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_ADS1115_AANTAL=1"
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
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender ADS1115 Test ExtenderPins Geweigerd\Extender ADS1115 Test ExtenderPins Geweigerd | -DEXTENDER_ADS1115_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_ADS1115_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1115_Test_ExtenderPins_Toegelaten\Extender_ADS1115_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender ADS1115 Test ExtenderPins Toegelaten\Extender ADS1115 Test ExtenderPins Toegelaten | -DEXTENDER_ADS1115_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_ADS1115_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1115_Meetkwaliteit\Extender_ADS1115_Meetkwaliteit.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender ADS1115 Meetkwaliteit\Extender ADS1115 Meetkwaliteit | -DEXTENDER_ADS1115_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_ADS1115_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1115_Meetkwaliteit\Extender_ADS1115_Meetkwaliteit.ino"
set TEST_NAAM="EXTENDER_ADS1115_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_ADS1115_AANTAL=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1158\Extender_ADS1158.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender ADS1158\Extender ADS1158 | -DEXTENDER_ADS1158_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_ADS1158_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1158\Extender_ADS1158.ino"
set TEST_NAAM="EXTENDER_ADS1158_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_ADS1158_AANTAL=2"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1158_Test_ExtenderPins_Geweigerd\Extender_ADS1158_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender ADS1158 Test ExtenderPins Geweigerd\Extender ADS1158 Test ExtenderPins Geweigerd | -DEXTENDER_ADS1158_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_ADS1158_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1158_Test_ExtenderPins_Toegelaten\Extender_ADS1158_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender ADS1158 Test ExtenderPins Toegelaten\Extender ADS1158 Test ExtenderPins Toegelaten | -DEXTENDER_ADS1158_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_ADS1158_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7828\Extender_ADS7828.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender ADS7828\Extender ADS7828 | -DEXTENDER_ADS7828_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_ADS7828_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7828\Extender_ADS7828.ino"
set TEST_NAAM="EXTENDER_ADS7828_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_ADS7828_AANTAL=2"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7828_Test_ExtenderPins_Geweigerd\Extender_ADS7828_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender ADS7828 Test ExtenderPins Geweigerd\Extender ADS7828 Test ExtenderPins Geweigerd | -DEXTENDER_ADS7828_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_ADS7828_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7828_Test_ExtenderPins_Toegelaten\Extender_ADS7828_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender ADS7828 Test ExtenderPins Toegelaten\Extender ADS7828 Test ExtenderPins Toegelaten | -DEXTENDER_ADS7828_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_ADS7828_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7953\Extender_ADS7953.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender ADS7953\Extender ADS7953 | -DEXTENDER_ADS7953_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_ADS7953_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7953\Extender_ADS7953.ino"
set TEST_NAAM="EXTENDER_ADS7953_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_ADS7953_AANTAL=2"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7953_Test_ExtenderPins_Geweigerd\Extender_ADS7953_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender ADS7953 Test ExtenderPins Geweigerd\Extender ADS7953 Test ExtenderPins Geweigerd | -DEXTENDER_ADS7953_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_ADS7953_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7953_Test_ExtenderPins_Toegelaten\Extender_ADS7953_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender ADS7953 Test ExtenderPins Toegelaten\Extender ADS7953 Test ExtenderPins Toegelaten | -DEXTENDER_ADS7953_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_ADS7953_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_CD74HC4067\Extender_CD74HC4067.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender CD74HC4067\Extender CD74HC4067 | -DEXTENDER_CD74HC4067_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_CD74HC4067_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_CD74HC4067\Extender_CD74HC4067.ino"
set TEST_NAAM="EXTENDER_CD74HC4067_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_CD74HC4067_AANTAL=2"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_CD74HC4067_Test_ExtenderPins_Geweigerd\Extender_CD74HC4067_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender CD74HC4067 Test ExtenderPins Geweigerd\Extender CD74HC4067 Test ExtenderPins Geweigerd | -DEXTENDER_CD74HC4067_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_CD74HC4067_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_CD74HC4067_Test_ExtenderPins_Toegelaten\Extender_CD74HC4067_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender CD74HC4067 Test ExtenderPins Toegelaten\Extender CD74HC4067 Test ExtenderPins Toegelaten | -DEXTENDER_CD74HC4067_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_CD74HC4067_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_DS2482v800\Extender_DS2482v800.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender DS2482v800\Extender DS2482v800 | -DEXTENDER_DS2482_800_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_DS2482_800_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_DS2482v800\Extender_DS2482v800.ino"
set TEST_NAAM="EXTENDER_DS2482_800_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_DS2482_800_AANTAL=2"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_DS2482v800_Test_ExtenderPins_Geweigerd\Extender_DS2482v800_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender DS2482v800 Test ExtenderPins Geweigerd\Extender DS2482v800 Test ExtenderPins Geweigerd | -DEXTENDER_DS2482_800_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_DS2482_800_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_DS2482v800_Test_ExtenderPins_Toegelaten\Extender_DS2482v800_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender DS2482v800 Test ExtenderPins Toegelaten\Extender DS2482v800 Test ExtenderPins Toegelaten | -DEXTENDER_DS2482_800_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_DS2482_800_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_I2C\Extender_MAX14830_I2C.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender MAX14830 I2C\Extender MAX14830 I2C | -DEXTENDER_MAX14830_I2C_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_MAX14830_I2C_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_I2C\Extender_MAX14830_I2C.ino"
set TEST_NAAM="EXTENDER_MAX14830_I2C_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_MAX14830_I2C_AANTAL=2"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_I2C_Test_ExtenderPins_Geweigerd\Extender_MAX14830_I2C_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender MAX14830 I2C Test ExtenderPins Geweigerd\Extender MAX14830 I2C Test ExtenderPins Geweigerd | -DEXTENDER_MAX14830_I2C_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_MAX14830_I2C_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_I2C_Test_ExtenderPins_Toegelaten\Extender_MAX14830_I2C_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender MAX14830 I2C Test ExtenderPins Toegelaten\Extender MAX14830 I2C Test ExtenderPins Toegelaten | -DEXTENDER_MAX14830_I2C_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_MAX14830_I2C_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_SPI\Extender_MAX14830_SPI.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender MAX14830 SPI\Extender MAX14830 SPI | -DEXTENDER_MAX14830_SPI_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_MAX14830_SPI_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_SPI\Extender_MAX14830_SPI.ino"
set TEST_NAAM="EXTENDER_MAX14830_SPI_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_MAX14830_SPI_AANTAL=2"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_SPI_Test_ExtenderPins_Geweigerd\Extender_MAX14830_SPI_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender MAX14830 SPI Test ExtenderPins Geweigerd\Extender MAX14830 SPI Test ExtenderPins Geweigerd | -DEXTENDER_MAX14830_SPI_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_MAX14830_SPI_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_SPI_Test_ExtenderPins_Toegelaten\Extender_MAX14830_SPI_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender MAX14830 SPI Test ExtenderPins Toegelaten\Extender MAX14830 SPI Test ExtenderPins Toegelaten | -DEXTENDER_MAX14830_SPI_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_MAX14830_SPI_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MCP23017\Extender_MCP23017.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender MCP23017\Extender MCP23017 | -DEXTENDER_MCP23017_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_MCP23017_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MCP23017\Extender_MCP23017.ino"
set TEST_NAAM="EXTENDER_MCP23017_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_MCP23017_AANTAL=2"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MCP23017_Test_ExtenderPins_Geweigerd\Extender_MCP23017_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender MCP23017 Test ExtenderPins Geweigerd\Extender MCP23017 Test ExtenderPins Geweigerd | -DEXTENDER_MCP23017_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_MCP23017_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MCP23017_Test_ExtenderPins_Toegelaten\Extender_MCP23017_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender MCP23017 Test ExtenderPins Toegelaten\Extender MCP23017 Test ExtenderPins Toegelaten | -DEXTENDER_MCP23017_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_MCP23017_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8574\Extender_PCF8574.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender PCF8574\Extender PCF8574 | -DEXTENDER_PCF8574_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_PCF8574_AANTAL=1"
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
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender PCF8574 Test ExtenderPins Geweigerd\Extender PCF8574 Test ExtenderPins Geweigerd | -DEXTENDER_PCF8574_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_PCF8574_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8574_Test_ExtenderPins_Toegelaten\Extender_PCF8574_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender PCF8574 Test ExtenderPins Toegelaten\Extender PCF8574 Test ExtenderPins Toegelaten | -DEXTENDER_PCF8574_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_PCF8574_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8575\Extender_PCF8575.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender PCF8575\Extender PCF8575 | -DEXTENDER_PCF8575_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_PCF8575_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8575\Extender_PCF8575.ino"
set TEST_NAAM="EXTENDER_PCF8575_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_PCF8575_AANTAL=2"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8575_Test_ExtenderPins_Geweigerd\Extender_PCF8575_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender PCF8575 Test ExtenderPins Geweigerd\Extender PCF8575 Test ExtenderPins Geweigerd | -DEXTENDER_PCF8575_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_PCF8575_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8575_Test_ExtenderPins_Toegelaten\Extender_PCF8575_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender PCF8575 Test ExtenderPins Toegelaten\Extender PCF8575 Test ExtenderPins Toegelaten | -DEXTENDER_PCF8575_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_PCF8575_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_I2C\Extender_SC16IS752_I2C.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender SC16IS752 I2C\Extender SC16IS752 I2C | -DEXTENDER_SC16IS752_I2C_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_SC16IS752_I2C_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_I2C\Extender_SC16IS752_I2C.ino"
set TEST_NAAM="EXTENDER_SC16IS752_I2C_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_SC16IS752_I2C_AANTAL=2"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_I2C_Test_ExtenderPins_Geweigerd\Extender_SC16IS752_I2C_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender SC16IS752 I2C Test ExtenderPins Geweigerd\Extender SC16IS752 I2C Test ExtenderPins Geweigerd | -DEXTENDER_SC16IS752_I2C_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_SC16IS752_I2C_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_I2C_Test_ExtenderPins_Toegelaten\Extender_SC16IS752_I2C_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender SC16IS752 I2C Test ExtenderPins Toegelaten\Extender SC16IS752 I2C Test ExtenderPins Toegelaten | -DEXTENDER_SC16IS752_I2C_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_SC16IS752_I2C_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_SPI\Extender_SC16IS752_SPI.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender SC16IS752 SPI\Extender SC16IS752 SPI | -DEXTENDER_SC16IS752_SPI_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_SC16IS752_SPI_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_SPI\Extender_SC16IS752_SPI.ino"
set TEST_NAAM="EXTENDER_SC16IS752_SPI_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_SC16IS752_SPI_AANTAL=2"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_SPI_Test_ExtenderPins_Geweigerd\Extender_SC16IS752_SPI_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender SC16IS752 SPI Test ExtenderPins Geweigerd\Extender SC16IS752 SPI Test ExtenderPins Geweigerd | -DEXTENDER_SC16IS752_SPI_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_SC16IS752_SPI_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_SPI_Test_ExtenderPins_Toegelaten\Extender_SC16IS752_SPI_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender SC16IS752 SPI Test ExtenderPins Toegelaten\Extender SC16IS752 SPI Test ExtenderPins Toegelaten | -DEXTENDER_SC16IS752_SPI_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_SC16IS752_SPI_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_TCA9548A\Extender_TCA9548A.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender TCA9548A\Extender TCA9548A | -DEXTENDER_TCA9548A_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_TCA9548A_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_TCA9548A\Extender_TCA9548A.ino"
set TEST_NAAM="EXTENDER_TCA9548A_AANTAL=2"
set TEST_FLAGS="-DEXTENDER_TCA9548A_AANTAL=2"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_TCA9548A_Test_ExtenderPins_Geweigerd\Extender_TCA9548A_Test_ExtenderPins_Geweigerd.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender TCA9548A Test ExtenderPins Geweigerd\Extender TCA9548A Test ExtenderPins Geweigerd | -DEXTENDER_TCA9548A_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_TCA9548A_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_TCA9548A_Test_ExtenderPins_Toegelaten\Extender_TCA9548A_Test_ExtenderPins_Toegelaten.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender TCA9548A Test ExtenderPins Toegelaten\Extender TCA9548A Test ExtenderPins Toegelaten | -DEXTENDER_TCA9548A_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_TCA9548A_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_MCP23S17\Extender_MCP23S17.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender MCP23S17\Extender MCP23S17 | -DEXTENDER_MCP23S17_AANTAL=1 -DCS_PIN_EXTENDER_MCP23S17_1=HardwareResourcePin::NP_D10"
set TEST_FLAGS="-DEXTENDER_MCP23S17_AANTAL=1 -DCS_PIN_EXTENDER_MCP23S17_1=HardwareResourcePin::NP_D10"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCA9685\Extender_PCA9685.ino"
set TEST_NAAM="examples\Systeem\GedeeldeBus\Extenders\Extender PCA9685\Extender PCA9685 | -DEXTENDER_PCA9685_AANTAL=1"
set TEST_FLAGS="-DEXTENDER_PCA9685_AANTAL=1"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Extenders\Extender_PCA9685_Test_OE_Pin\Extender_PCA9685_Test_OE_Pin.ino"
set TEST_NAAM="Extender PCA9685 | test OE-pin bij uitval (T2-10)"
set TEST_FLAGS="-DEXTENDER_PCA9685_AANTAL=1 -DEXTENDER_PCA9685_1_OE_PIN=ARDUINO_UNO_SHIELD_PIN_D7"
set TEST_TYPE="EXPERIMENTEEL"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT


exit /b 0
