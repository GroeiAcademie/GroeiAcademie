@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryToolsOngeldig-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\Tools\;"
set "BRON_TEST_SCRIPT=%~dp0TestLibraryTools-v2.0.0.cmd"

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

set "TEST_INO=examples\Systeem\Tools\I2C\I2C_DiagnoseDS2482v800\I2C_DiagnoseDS2482v800.ino"
set "TEST_NAAM=EXTENDER_DS2482_800_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_DS2482_800_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_DS2482_800_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\Tools\I2C\I2C_DiagnoseTCA9548A\I2C_DiagnoseTCA9548A.ino"
set "TEST_NAAM=EXTENDER_TCA9548A_AANTAL ondersteunt momenteel maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_TCA9548A_AANTAL=3"
set "VERWACHTE_FOUT=EXTENDER_TCA9548A_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

echo.
echo Totaal !TESTS! - OK !OK! - FOUT !FAIL!
if !FAIL!==0 (exit /b 0) else exit /b 1
