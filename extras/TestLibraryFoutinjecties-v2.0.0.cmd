@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryFoutinjecties-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\GedeeldeBus\Foutinjecties\;"

call "%~dp0TestLibraryCommon.cmd" %*
exit /b %errorlevel%

:TESTLIJST

REM 60 foutinjectie-sketches (een per foutcode) en 1 centrale interactieve sketch. Eigen flags per sketch; UserConfig.h telt niet mee.

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_CS001\Hardwaretest_Foutinjectie_CS001.ino"
set TEST_NAAM="Foutinjectie CS001 | CharacterScreen aanmelden() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_CS101\Hardwaretest_Foutinjectie_CS101.ino"
set TEST_NAAM="Foutinjectie CS101 | CharacterScreen controleren() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_CS201\Hardwaretest_Foutinjectie_CS201.ino"
set TEST_NAAM="Foutinjectie CS201 | CharacterScreen inpluggen() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_CS301\Hardwaretest_Foutinjectie_CS301.ino"
set TEST_NAAM="Foutinjectie CS301 | CharacterScreen activeren() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_CS401\Hardwaretest_Foutinjectie_CS401.ino"
set TEST_NAAM="Foutinjectie CS401 | CharacterScreen tijdens runtime niet actief"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB001\Hardwaretest_Foutinjectie_GB001.ino"
set TEST_NAAM="Foutinjectie GB001 | GEDEELD-claim zonder geldige identiteit"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB101\Hardwaretest_Foutinjectie_GB101.ino"
set TEST_NAAM="Foutinjectie GB101 | twee nodes, zelfde pin, minstens een EXCLUSIEF"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB102\Hardwaretest_Foutinjectie_GB102.ino"
set TEST_NAAM="Foutinjectie GB102 | zelfde I2C-adres op dezelfde bus"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB103\Hardwaretest_Foutinjectie_GB103.ino"
set TEST_NAAM="Foutinjectie GB103 | twee GEDEELD-claims, andere identiteit"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB104\Hardwaretest_Foutinjectie_GB104.ino"
set TEST_NAAM="Foutinjectie GB104 | parent exclusief al bezet door een andere child"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB105\Hardwaretest_Foutinjectie_GB105.ino"
set TEST_NAAM="Foutinjectie GB105 | binnen een component twee EXCLUSIEF-claims op dezelfde pin"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB106\Hardwaretest_Foutinjectie_GB106.ino"
set TEST_NAAM="Foutinjectie GB106 | binnen een component EXCLUSIEF- en GEDEELD-claim op dezelfde pin"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB107\Hardwaretest_Foutinjectie_GB107.ino"
set TEST_NAAM="Foutinjectie GB107 | binnen een component twee GEDEELD-claims, andere identiteit"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB108\Hardwaretest_Foutinjectie_GB108.ino"
set TEST_NAAM="Foutinjectie GB108 | verplichte resource ontbreekt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB109\Hardwaretest_Foutinjectie_GB109.ino"
set TEST_NAAM="Foutinjectie GB109 | I2C-adres niet toegestaan voor dit hardwaretype"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB110\Hardwaretest_Foutinjectie_GB110.ino"
set TEST_NAAM="Foutinjectie GB110 | ongeldig I2C-adres"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB111\Hardwaretest_Foutinjectie_GB111.ino"
set TEST_NAAM="Foutinjectie GB111 | parent/Extender biedt de gevraagde resource niet aan"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB201\Hardwaretest_Foutinjectie_GB201.ino"
set TEST_NAAM="Foutinjectie GB201 | inpluggen() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB202\Hardwaretest_Foutinjectie_GB202.ino"
set TEST_NAAM="Foutinjectie GB202 | inpluggen() mislukt: bus start niet"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DDEBUG -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB301\Hardwaretest_Foutinjectie_GB301.ino"
set TEST_NAAM="Foutinjectie GB301 | activeren() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB302\Hardwaretest_Foutinjectie_GB302.ino"
set TEST_NAAM="Foutinjectie GB302 | activeren() mislukt: geen antwoord op I2C-adres"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DDEBUG -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB303\Hardwaretest_Foutinjectie_GB303.ino"
set TEST_NAAM="Foutinjectie GB303 | activeren() mislukt: antwoord, maar initialisatie van de library mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DDEBUG -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB304\Hardwaretest_Foutinjectie_GB304.ino"
set TEST_NAAM="Foutinjectie GB304 | activeren() mislukt: SPI-component antwoordt niet geldig"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DDEBUG -DEXTENDER_MCP23S17_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB305\Hardwaretest_Foutinjectie_GB305.ino"
set TEST_NAAM="Foutinjectie GB305 | activeren() mislukt: I2C-busfout of timeout"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DDEBUG -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB306\Hardwaretest_Foutinjectie_GB306.ino"
set TEST_NAAM="Foutinjectie GB306 | activeren() mislukt: verkeerde chip, teruglezen klopt niet"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DDEBUG -DEXTENDER_MCP23017_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB400\Hardwaretest_Foutinjectie_GB400.ino"
set TEST_NAAM="Foutinjectie GB400 | communicatie met component mislukt tijdens runtime"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB401\Hardwaretest_Foutinjectie_GB401.ino"
set TEST_NAAM="Foutinjectie GB401 | heractiveren tijdens runtime mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB402\Hardwaretest_Foutinjectie_GB402.ino"
set TEST_NAAM="Foutinjectie GB402 | heractiveren mislukt: geen antwoord op I2C-adres"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DDEBUG -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB403\Hardwaretest_Foutinjectie_GB403.ino"
set TEST_NAAM="Foutinjectie GB403 | heractiveren mislukt: antwoord, maar initialisatie van de library mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DDEBUG -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB405\Hardwaretest_Foutinjectie_GB405.ino"
set TEST_NAAM="Foutinjectie GB405 | heractiveren mislukt: I2C-busfout of timeout"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DDEBUG -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB406\Hardwaretest_Foutinjectie_GB406.ino"
set TEST_NAAM="Foutinjectie GB406 | heractiveren mislukt: verkeerde chip, teruglezen klopt niet"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DDEBUG -DEXTENDER_MCP23017_AANTAL=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB407\Hardwaretest_Foutinjectie_GB407.ino"
set TEST_NAAM="Foutinjectie GB407 | component hersteld tijdens runtime"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB408\Hardwaretest_Foutinjectie_GB408.ino"
set TEST_NAAM="Foutinjectie GB408 | Sensor kan tijdens runtime niet meten: storing van zijn Extender"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DEXTENDER_ADS1115_AANTAL=1 -DRFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1={{1,3,5,10,20},{0UL,100UL,500UL,2000UL,10000UL},ScreenData::TYPE_FATAL,ScreenData::TYPE_CRITICAL}"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB409\Hardwaretest_Foutinjectie_GB409.ino"
set TEST_NAAM="Foutinjectie GB409 | busherstel mislukt: SDA blijft laag"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_GB410\Hardwaretest_Foutinjectie_GB410.ino"
set TEST_NAAM="Foutinjectie GB410 | meting tijdens runtime hersteld (Sensor-ladder)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DEXTENDER_ADS1115_AANTAL=1 -DRFP602_RUNTIME_DREMPELS_FOUTNIVEAU_SET_1={{1,3,5,10,20},{0UL,100UL,500UL,2000UL,10000UL},ScreenData::TYPE_FATAL,ScreenData::TYPE_CRITICAL}"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_IN001\Hardwaretest_Foutinjectie_IN001.ino"
set TEST_NAAM="Foutinjectie IN001 | InputKanalen aanmelden() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7 -DINPUT_KANAAL_CONFIG=INPUT_TYPE_HX1838"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_IN101\Hardwaretest_Foutinjectie_IN101.ino"
set TEST_NAAM="Foutinjectie IN101 | InputKanalen controleren() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7 -DINPUT_KANAAL_CONFIG=INPUT_TYPE_HX1838"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_IN201\Hardwaretest_Foutinjectie_IN201.ino"
set TEST_NAAM="Foutinjectie IN201 | InputKanalen inpluggen() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7 -DINPUT_KANAAL_CONFIG=INPUT_TYPE_HX1838"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_IN202\Hardwaretest_Foutinjectie_IN202.ino"
set TEST_NAAM="Foutinjectie IN202 | HX1838-pin niet om te zetten naar een native pin"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSCREEN_OUTPUT_CONFIG=7 -DINPUT_KANAAL_CONFIG=INPUT_TYPE_HX1838 -DHX1838_ONTVANGER_PIN=99"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_IN301\Hardwaretest_Foutinjectie_IN301.ino"
set TEST_NAAM="Foutinjectie IN301 | InputKanalen activeren() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7 -DINPUT_KANAAL_CONFIG=INPUT_TYPE_HX1838"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_IN302\Hardwaretest_Foutinjectie_IN302.ino"
set TEST_NAAM="Foutinjectie IN302 | HX1838-ontvangstlijn blijft permanent LOW"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSCREEN_OUTPUT_CONFIG=7 -DINPUT_KANAAL_CONFIG=INPUT_TYPE_HX1838"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_IN303\Hardwaretest_Foutinjectie_IN303.ino"
set TEST_NAAM="Foutinjectie IN303 | TinyIRReceiver interruptinitialisatie mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSCREEN_OUTPUT_CONFIG=7 -DINPUT_KANAAL_CONFIG=INPUT_TYPE_HX1838 -DHX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_IN304\Hardwaretest_Foutinjectie_IN304.ino"
set TEST_NAAM="Foutinjectie IN304 | HX1838-pin niet bruikbaar voor het interruptmechanisme"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSCREEN_OUTPUT_CONFIG=7 -DINPUT_KANAAL_CONFIG=INPUT_TYPE_HX1838 -DHX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_IN305\Hardwaretest_Foutinjectie_IN305.ino"
set TEST_NAAM="Foutinjectie IN305 | HX1838-receiver niet correct gestart"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSCREEN_OUTPUT_CONFIG=7 -DINPUT_KANAAL_CONFIG=INPUT_TYPE_HX1838 -DHX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE=0"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_IN401\Hardwaretest_Foutinjectie_IN401.ino"
set TEST_NAAM="Foutinjectie IN401 | InputKanalen tijdens runtime niet actief"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSCREEN_OUTPUT_CONFIG=7 -DINPUT_KANAAL_CONFIG=INPUT_TYPE_HX1838"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_IN402\Hardwaretest_Foutinjectie_IN402.ino"
set TEST_NAAM="Foutinjectie IN402 | PCF8574 niet bereikbaar op het ingestelde adres"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSCREEN_OUTPUT_CONFIG=7 -DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_PS001\Hardwaretest_Foutinjectie_PS001.ino"
set TEST_NAAM="Foutinjectie PS001 | PixelScreen aanmelden() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_PS101\Hardwaretest_Foutinjectie_PS101.ino"
set TEST_NAAM="Foutinjectie PS101 | PixelScreen controleren() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_PS201\Hardwaretest_Foutinjectie_PS201.ino"
set TEST_NAAM="Foutinjectie PS201 | PixelScreen inpluggen() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_PS301\Hardwaretest_Foutinjectie_PS301.ino"
set TEST_NAAM="Foutinjectie PS301 | PixelScreen activeren() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_PS302\Hardwaretest_Foutinjectie_PS302.ino"
set TEST_NAAM="Foutinjectie PS302 | PixelScreen koppelen (gfx) mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_PS303\Hardwaretest_Foutinjectie_PS303.ino"
set TEST_NAAM="Foutinjectie PS303 | PixelScreen afmetingencontrole mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7 -DPIXEL_SCREEN_ROTATION=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_PS304\Hardwaretest_Foutinjectie_PS304.ino"
set TEST_NAAM="Foutinjectie PS304 | PixelScreen afmetingencontrole mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7 -DPIXEL_SCREEN_ROTATION=0"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_PS305\Hardwaretest_Foutinjectie_PS305.ino"
set TEST_NAAM="Foutinjectie PS305 | tekstgrid kleiner dan het minimum"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSCREEN_OUTPUT_CONFIG=7 -DPIXEL_SCREEN_MARGIN=100"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_PS401\Hardwaretest_Foutinjectie_PS401.ino"
set TEST_NAAM="Foutinjectie PS401 | PixelScreen tijdens runtime niet actief"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_SS001\Hardwaretest_Foutinjectie_SS001.ino"
set TEST_NAAM="Foutinjectie SS001 | SerialScreen aanmelden() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_SS101\Hardwaretest_Foutinjectie_SS101.ino"
set TEST_NAAM="Foutinjectie SS101 | SerialScreen controleren() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_SS201\Hardwaretest_Foutinjectie_SS201.ino"
set TEST_NAAM="Foutinjectie SS201 | SerialScreen inpluggen() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DGEDEELDE_BUS_FOUTINJECTIE=1 -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_SS301\Hardwaretest_Foutinjectie_SS301.ino"
set TEST_NAAM="Foutinjectie SS301 | SerialScreen activeren() mislukt"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSCREEN_OUTPUT_CONFIG=7 -DINPUT_KANAAL_CONFIG=INPUT_TYPE_HX1838 -DHX1838_TOETSENINDELING=HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN -DREDIRECT_SERIAL_CONNECT_TIMEOUT_MS"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_SS302\Hardwaretest_Foutinjectie_SS302.ino"
set TEST_NAAM="Foutinjectie SS302 | SerialScreen niet beschikbaar (SIMULATIE)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Foutinjecties\Hardwaretest_Foutinjectie_Interactief\Hardwaretest_Foutinjectie_Interactief.ino"
set TEST_NAAM="Foutinjectie centraal | centrale interactieve PCF8574-sketch (geen foutcode)"
set TEST_FLAGS="-DDATA_LAYER_CONFIG=DATA_TYPE_NONE -DINPUT_KANAAL_CONFIG=INPUT_TYPE_HX1838 -DHX1838_TOETSENINDELING=HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN -DEXTENDER_PCF8574_AANTAL=1 -DI2C_ADDRESS_EXTENDER_PCF8574_1=0x20"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

exit /b 0
