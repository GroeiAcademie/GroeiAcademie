@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST
setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibrarySturingenOngeldig-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\GedeeldeBus\Sturingen\;"
set "BRON_TEST_SCRIPT=%~dp0TestLibrarySturingen-v2.0.0.cmd"
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

set "TEST_INO=examples\Systeem\GedeeldeBus\Sturingen\Sturing_SG90\Sturing_SG90.ino"
set "TEST_NAAM=SG90 zonder PCA9685"
set "TEST_FLAGS=-DEXTENDER_PCA9685_AANTAL=0 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSTURING_SG90_AANTAL=1"
set "VERWACHTE_FOUT=Schakel de vereiste Extender in voor deze Sturing-test."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sturingen\Sturing_Relais\Sturing_Relais.ino"
set "TEST_NAAM=Relais zonder PCF8574"
set "TEST_FLAGS=-DEXTENDER_PCF8574_AANTAL=0 -DGEDEELDE_BUS_MIXED_PINS=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSTURING_RELAIS_AANTAL=1"
set "VERWACHTE_FOUT=Schakel de vereiste Extender in voor deze Sturing-test."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sturingen\Sturing_SG90\Sturing_SG90.ino"
set "TEST_NAAM=PCA9685 ondersteunt maximaal 2 instanties"
set "TEST_FLAGS=-DEXTENDER_PCA9685_AANTAL=3 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSTURING_SG90_AANTAL=1"
set "VERWACHTE_FOUT=EXTENDER_PCA9685_AANTAL ondersteunt momenteel maximaal 2 instanties."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sturingen\Sturing_Relais\Sturing_Relais.ino"
set "TEST_NAAM=Relais vereist mixed pins"
set "TEST_FLAGS=-DGEDEELDE_BUS_MIXED_PINS=0 -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSTURING_RELAIS_AANTAL=1"
set "VERWACHTE_FOUT=Relais vereist GEDEELDE_BUS_MIXED_PINS 1"
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sturingen\Sturing_ULN2003\Sturing_ULN2003.ino"
set "TEST_NAAM=ULN2003 vereist mixed pins"
set "TEST_FLAGS=-DGEDEELDE_BUS_MIXED_PINS=0 -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSTURING_ULN2003_AANTAL=1"
set "VERWACHTE_FOUT=ULN2003 vereist GEDEELDE_BUS_MIXED_PINS 1"
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sturingen\Sturing_A4988\Sturing_A4988.ino"
set "TEST_NAAM=A4988 uitgeschakeld"
set "TEST_FLAGS=-DSTURING_A4988_AANTAL=0 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=Zet STURING_A4988_AANTAL op minstens 1 voor deze test."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sturingen\Sturing_DCmotorL293D\Sturing_DCmotorL293D.ino"
set "TEST_NAAM=DCmotorL293D uitgeschakeld"
set "TEST_FLAGS=-DSTURING_DCMOTORL293D_AANTAL=0 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=Zet STURING_DCMOTORL293D_AANTAL op minstens 1 voor deze test."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sturingen\Sturing_DCmotorL298N\Sturing_DCmotorL298N.ino"
set "TEST_NAAM=DCmotorL298N uitgeschakeld"
set "TEST_FLAGS=-DSTURING_DCMOTORL298N_AANTAL=0 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=Zet STURING_DCMOTORL298N_AANTAL op minstens 1 voor deze test."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sturingen\Sturing_MG996R\Sturing_MG996R.ino"
set "TEST_NAAM=MG996R uitgeschakeld"
set "TEST_FLAGS=-DSTURING_MG996R_AANTAL=0 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=Zet STURING_MG996R_AANTAL op minstens 1 voor deze test."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sturingen\Sturing_Relais\Sturing_Relais.ino"
set "TEST_NAAM=Relais uitgeschakeld"
set "TEST_FLAGS=-DSTURING_RELAIS_AANTAL=0 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=Zet STURING_RELAIS_AANTAL op minstens 1 voor deze test."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sturingen\Sturing_RGBLED\Sturing_RGBLED.ino"
set "TEST_NAAM=RGBLED uitgeschakeld"
set "TEST_FLAGS=-DSTURING_RGBLED_AANTAL=0 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=Zet STURING_RGBLED_AANTAL op minstens 1 voor deze test."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sturingen\Sturing_SG90\Sturing_SG90.ino"
set "TEST_NAAM=SG90 uitgeschakeld"
set "TEST_FLAGS=-DSTURING_SG90_AANTAL=0 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=Zet STURING_SG90_AANTAL op minstens 1 voor deze test."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sturingen\Sturing_ULN2003\Sturing_ULN2003.ino"
set "TEST_NAAM=ULN2003 uitgeschakeld"
set "TEST_FLAGS=-DSTURING_ULN2003_AANTAL=0 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=Zet STURING_ULN2003_AANTAL op minstens 1 voor deze test."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\GedeeldeBus\Sturingen\Sturing_WS2812\Sturing_WS2812.ino"
set "TEST_NAAM=WS2812 uitgeschakeld"
set "TEST_FLAGS=-DSTURING_WS2812_AANTAL=0 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set "VERWACHTE_FOUT=Zet STURING_WS2812_AANTAL op minstens 1 voor deze test."
call :EXPECT_FAIL

echo.
echo Totaal !TESTS! - OK !OK! - FOUT !FAIL!
if !FAIL! GTR 0 exit /b 1
exit /b 0
