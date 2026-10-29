@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryFoutinjectiesOngeldig-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\GedeeldeBus\Foutinjecties\;"
set "BRON_TEST_SCRIPT=%~dp0TestLibraryFoutinjecties-v2.0.0.cmd"

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

set "TEST_INO=examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_CS001\Hardwaretest_Foutinjectie_CS001.ino"
set "TEST_NAAM=CS001 vereist GEDEELDE_BUS_FOUTINJECTIE 1"
set "TEST_FLAGS=-DGEDEELDE_BUS_FOUTINJECTIE=0"
set "VERWACHTE_FOUT=Zet GEDEELDE_BUS_FOUTINJECTIE op 1 in UserConfig.h voor deze test (testingang)."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_ADC_OngeldigeChannel\Hardwaretest_Foutinjectie_ADC_OngeldigeChannel.ino"
set "TEST_NAAM=ADC_OngeldigeChannel vereist EXTENDER_ADS7828_AANTAL 1"
set "TEST_FLAGS=-DEXTENDER_ADS7828_AANTAL=0"
set "VERWACHTE_FOUT=Stel EXTENDER_ADS7828_AANTAL op 1 voor deze negatieve contracttest."
call :EXPECT_FAIL

echo.
echo Totaal !TESTS! - OK !OK! - FOUT !FAIL!
if !FAIL!==0 (exit /b 0) else exit /b 1
