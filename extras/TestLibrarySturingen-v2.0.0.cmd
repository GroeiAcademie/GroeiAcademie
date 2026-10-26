@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibrarySturingen-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\GedeeldeBus\Sturingen\;"

call "%~dp0TestLibraryCommon.cmd" %*
exit /b %errorlevel%

:TESTLIJST

set TEST_INO="examples\Systeem\GedeeldeBus\Sturingen\Sturing_SG90\Sturing_SG90.ino"
set TEST_NAAM="Sturing SG90 | PCA9685"
set TEST_FLAGS="-DEXTENDER_PCA9685_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE" -DSTURING_SG90_AANTAL=1
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sturingen\Sturing_MG996R\Sturing_MG996R.ino"
set TEST_NAAM="Sturing MG996R | PCA9685"
set TEST_FLAGS="-DEXTENDER_PCA9685_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE" -DSTURING_MG996R_AANTAL=1
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sturingen\Sturing_DCmotorL298N\Sturing_DCmotorL298N.ino"
set TEST_NAAM="Sturing DCmotorL298N | PCA9685"
set TEST_FLAGS="-DEXTENDER_PCA9685_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE" -DSTURING_DCMOTORL298N_AANTAL=1
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sturingen\Sturing_DCmotorL293D\Sturing_DCmotorL293D.ino"
set TEST_NAAM="Sturing DCmotorL293D | PCA9685"
set TEST_FLAGS="-DEXTENDER_PCA9685_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE" -DSTURING_DCMOTORL293D_AANTAL=1
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sturingen\Sturing_Relais\Sturing_Relais.ino"
set TEST_NAAM="Sturing Relais | PCF8574"
set TEST_FLAGS="-DGEDEELDE_BUS_MIXED_PINS=1 -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE" -DSTURING_RELAIS_AANTAL=1
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sturingen\Sturing_RGBLED\Sturing_RGBLED.ino"
set TEST_NAAM="Sturing RGBLED | PCA9685"
set TEST_FLAGS="-DEXTENDER_PCA9685_AANTAL=1 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE" -DSTURING_RGBLED_AANTAL=1
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sturingen\Sturing_WS2812\Sturing_WS2812.ino"
set TEST_NAAM="Sturing WS2812 | native datapin | Adafruit NeoPixel"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE" -DSTURING_WS2812_AANTAL=1
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sturingen\Sturing_ULN2003\Sturing_ULN2003.ino"
set TEST_NAAM="Sturing ULN2003 | vier digitale pinnen"
set TEST_FLAGS="-DGEDEELDE_BUS_MIXED_PINS=1 -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20 -DDATA_LAYER_CONFIG=DATA_TYPE_NONE" -DSTURING_ULN2003_AANTAL=1
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Sturingen\Sturing_A4988\Sturing_A4988.ino"
set TEST_NAAM="Sturing A4988 | native STEP DIR"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE" -DSTURING_A4988_AANTAL=1
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

exit /b 0
