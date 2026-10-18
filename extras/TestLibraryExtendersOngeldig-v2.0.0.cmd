@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryExtendersOngeldig-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\GedeeldeBus\Extenders\;"
set "BRON_TEST_SCRIPT=%~dp0TestLibraryExtenders-v2.0.0.cmd"

call "%~dp0TestLibraryCommonOngeldig.cmd" %*
exit /b %errorlevel%

:EXPECT_FAIL
set "TEST_FQBN=arduino:renesas_uno:minima"
set "TEST_BOARD_VERSION=BOARD_UNO_R4_MINIMA"
call "%~dp0TestLibraryCommonOngeldig.cmd" --VOER_DEZE_ONGELDIGE_TEST_UIT
goto :eof

:TESTLIJST
set /A TESTS=0
set /A OK=0
set /A FAIL=0

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1115\Extender_ADS1115.ino"
set "TEST_NAAM=EXTENDER_ADS1115_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_ADS1115_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_ADS1115_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_ADS1158\Extender_ADS1158.ino"
set "TEST_NAAM=EXTENDER_ADS1158_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_ADS1158_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_ADS1158_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7828\Extender_ADS7828.ino"
set "TEST_NAAM=EXTENDER_ADS7828_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_ADS7828_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_ADS7828_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_ADS7953\Extender_ADS7953.ino"
set "TEST_NAAM=EXTENDER_ADS7953_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_ADS7953_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_ADS7953_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_CD74HC4067\Extender_CD74HC4067.ino"
set "TEST_NAAM=EXTENDER_CD74HC4067_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_CD74HC4067_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_CD74HC4067_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_DS2482v800\Extender_DS2482v800.ino"
set "TEST_NAAM=EXTENDER_DS2482_800_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_DS2482_800_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_DS2482_800_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_I2C\Extender_MAX14830_I2C.ino"
set "TEST_NAAM=EXTENDER_MAX14830_I2C_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_MAX14830_I2C_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_MAX14830_I2C_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_MAX14830_SPI\Extender_MAX14830_SPI.ino"
set "TEST_NAAM=EXTENDER_MAX14830_SPI_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_MAX14830_SPI_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_MAX14830_SPI_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_MCP23017\Extender_MCP23017.ino"
set "TEST_NAAM=EXTENDER_MCP23017_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_MCP23017_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_MCP23017_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_MCP23S17\Extender_MCP23S17.ino"
set "TEST_NAAM=EXTENDER_MCP23S17_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_MCP23S17_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_MCP23S17_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_PCA9685\Extender_PCA9685.ino"
set "TEST_NAAM=EXTENDER_PCA9685_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_PCA9685_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_PCA9685_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8574\Extender_PCF8574.ino"
set "TEST_NAAM=EXTENDER_PCF8574_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_PCF8574_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_PCF8574_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_PCF8575\Extender_PCF8575.ino"
set "TEST_NAAM=EXTENDER_PCF8575_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_PCF8575_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_PCF8575_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_I2C\Extender_SC16IS752_I2C.ino"
set "TEST_NAAM=EXTENDER_SC16IS752_I2C_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_SC16IS752_I2C_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_SC16IS752_I2C_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_SC16IS752_SPI\Extender_SC16IS752_SPI.ino"
set "TEST_NAAM=EXTENDER_SC16IS752_SPI_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_SC16IS752_SPI_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_SC16IS752_SPI_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Extenders\Extender_TCA9548A\Extender_TCA9548A.ino"
set "TEST_NAAM=EXTENDER_TCA9548A_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_TCA9548A_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_TCA9548A_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

echo.
echo Totaal !TESTS! - OK !OK! - FOUT !FAIL!
if !FAIL!==0 (exit /b 0) else exit /b 1
