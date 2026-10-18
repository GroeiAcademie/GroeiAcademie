@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryADCOngeldig-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\ADC_Backend\;"
set "BRON_TEST_SCRIPT=%~dp0TestLibraryADC-v2.0.0.cmd"

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

set "TEST_INO=examples\Systeem\ADC_Backend\ADC_Backend_ADS1115\ADC_Backend_ADS1115.ino"
set "TEST_NAAM=EXTENDER_ADS1115_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_ADS1115_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_ADS1115_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

echo.
echo Totaal !TESTS! - OK !OK! - FOUT !FAIL!
if !FAIL!==0 (exit /b 0) else exit /b 1
