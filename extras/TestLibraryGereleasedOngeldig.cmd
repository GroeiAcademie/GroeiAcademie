@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryGereleasedOngeldig"
set "CONTROLE_MODUS=EXCLUDE"
set "CONTROLE_PADEN=examples\Systeem\Input\;examples\Systeem\GedeeldeBus\;"
set "BRON_TEST_SCRIPT=%~dp0TestLibraryGereleased.cmd"

call "%~dp0TestLibraryCommonOngeldig.cmd" %*
exit /b %errorlevel%

:EXPECT_FAIL
set "TEST_FQBN=arduino:avr:uno"
set "TEST_BOARD_VERSION=BOARD_UNO_R3"
echo !TEST_FLAGS! | findstr /C:"BOARD_VERSION" >nul
if not errorlevel 1 set "TEST_BOARD_VERSION="
call "%~dp0TestLibraryCommonOngeldig.cmd" --VOER_DEZE_ONGELDIGE_TEST_UIT
goto :eof

:TESTLIJST
set /A TESTS=0
set /A OK=0
set /A FAIL=0

echo.
echo ============================================================
echo GERELEASEDE ONGELDIGE CONFIGURATIES: SCREEN
echo ============================================================
set "EXAMPLE=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail"

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=ONBEKENDE SCREEN-BIT"
set "TEST_FLAGS=-DSCREEN_OUTPUT_CONFIG=8"
set "VERWACHTE_FOUT=SCREEN_OUTPUT_CONFIG bevat een onbekende uitvoerbit."
call :EXPECT_FAIL
set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=ONGELDIG PIXELSCHERM"
set "TEST_FLAGS=-DSCREEN_OUTPUT_CONFIG=SCREEN_TYPE_PIXELS -DACTIEF_PIXEL_SCREEN=99"
set "VERWACHTE_FOUT=ACTIEF_PIXEL_SCREEN bevat geen ondersteunde pixelschermresolutie."
call :EXPECT_FAIL
set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=ONGELDIGE PIXEL ROTATIE"
set "TEST_FLAGS=-DSCREEN_OUTPUT_CONFIG=SCREEN_TYPE_PIXELS -DPIXEL_SCREEN_ROTATION=4"
set "VERWACHTE_FOUT=PIXEL_SCREEN_ROTATION moet 0, 1, 2 of 3 zijn."
call :EXPECT_FAIL
set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=ONGELDIGE CHARACTER I2C-MODUS"
set "TEST_FLAGS=-DSCREEN_OUTPUT_CONFIG=SCREEN_TYPE_CHARACTER -DCHARACTERSCREEN_I2C_ADRES_MODUS=3"
set "VERWACHTE_FOUT=CHARACTERSCREEN_I2C_ADRES_MODUS moet 0, 1 of 2 zijn."
call :EXPECT_FAIL

echo.
echo ============================================================
echo GERELEASEDE ONGELDIGE CONFIGURATIES: SYSTEMCONFIG
echo ============================================================
set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=ONGELDIGE BOARD_VERSION"
set "TEST_FLAGS=-DBOARD_VERSION=99"
set "VERWACHTE_FOUT=Selecteer een geldige BOARD_VERSION."
call :EXPECT_FAIL
set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=MEERDERE TALEN GESELECTEERD"
set "TEST_FLAGS=-DLANGUAGE_NL -DLANGUAGE_EN"
set "VERWACHTE_FOUT=Selecteer slechts een taal in UserConfig.h of SystemConfig.h."
call :EXPECT_FAIL
set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=ONGELDIG AANTAL_SENSOREN_AANWEZIG"
set "TEST_FLAGS=-DAANTAL_SENSOREN_AANWEZIG=3"
set "VERWACHTE_FOUT=AANTAL_SENSOREN_AANWEZIG moet 2 of 4 zijn."
call :EXPECT_FAIL
set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=ONGELDIGE ADC_BACKEND"
set "TEST_FLAGS=-DADC_BACKEND=99"
set "VERWACHTE_FOUT=Selecteer een geldige ADC_BACKEND."
call :EXPECT_FAIL

echo.
echo Totaal !TESTS! - OK !OK! - FOUT !FAIL!
if !FAIL!==0 (exit /b 0) else exit /b 1

