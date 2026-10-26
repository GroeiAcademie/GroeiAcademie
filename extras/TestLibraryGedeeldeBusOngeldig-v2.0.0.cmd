@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryGedeeldeBusOngeldig-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Toepassingsgebieden\Stimulus\Scenario1_EnkelTik\;examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\;"
set "BRON_TEST_SCRIPT=%~dp0TestLibraryGedeeldeBus-v2.0.0.cmd"

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
set "TEST_NAAM=ONGELDIG RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1"
set "TEST_FLAGS=-DRFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1=3"
set "VERWACHTE_FOUT=RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1 moet 2 of 4 zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=RFP602 ZONDER ADS1115-EXTENDER"
set "TEST_FLAGS=-DEXTENDER_ADS1115_AANTAL=0"
set "VERWACHTE_FOUT=Zet EXTENDER_ADS1115_AANTAL in UserConfig.h op minstens 1: RFP602 meet via een ADS1115-Extender."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=KEYPAD_GENERIEK_AANTAL_PINNEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_..."
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_USER_DEFINED_DIRECT -DKEYPAD_GENERIEK_PINNEN={0,1,2,3} -DKEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED=LOW -DKEYPAD_GENERIEK_KEY_LAYOUT={{_LABEL_OPSCHRIFT_1,_LABEL_OPSCHRIFT_1},{_LABEL_OPSCHRIFT_2,_LABEL_OPSCHRIFT_2},{_LABEL_OPSCHRIFT_3,_LABEL_OPSCHRIFT_3},{_LABEL_OPSCHRIFT_4,_LABEL_OPSCHRIFT_4}}"
set "VERWACHTE_FOUT=KEYPAD_GENERIEK_AANTAL_PINNEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_DIRECT."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=KEYPAD_GENERIEK_PINNEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED..."
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_USER_DEFINED_DIRECT -DKEYPAD_GENERIEK_AANTAL_PINNEN=4 -DKEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED=LOW -DKEYPAD_GENERIEK_KEY_LAYOUT={{_LABEL_OPSCHRIFT_1,_LABEL_OPSCHRIFT_1},{_LABEL_OPSCHRIFT_2,_LABEL_OPSCHRIFT_2},{_LABEL_OPSCHRIFT_3,_LABEL_OPSCHRIFT_3},{_LABEL_OPSCHRIFT_4,_LABEL_OPSCHRIFT_4}}"
set "VERWACHTE_FOUT=KEYPAD_GENERIEK_PINNEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_DIRECT."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED moet ingesteld worden in UserConfig.h bij K..."
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_USER_DEFINED_DIRECT -DKEYPAD_GENERIEK_AANTAL_PINNEN=4 -DKEYPAD_GENERIEK_PINNEN={0,1,2,3} -DKEYPAD_GENERIEK_KEY_LAYOUT={{_LABEL_OPSCHRIFT_1,_LABEL_OPSCHRIFT_1},{_LABEL_OPSCHRIFT_2,_LABEL_OPSCHRIFT_2},{_LABEL_OPSCHRIFT_3,_LABEL_OPSCHRIFT_3},{_LABEL_OPSCHRIFT_4,_LABEL_OPSCHRIFT_4}}"
set "VERWACHTE_FOUT=KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_DIRECT."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=KEYPAD_GENERIEK_KEY_LAYOUT moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEF..."
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_USER_DEFINED_DIRECT -DKEYPAD_GENERIEK_AANTAL_PINNEN=4 -DKEYPAD_GENERIEK_PINNEN={0,1,2,3} -DKEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED=LOW"
set "VERWACHTE_FOUT=KEYPAD_GENERIEK_KEY_LAYOUT moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_DIRECT."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=KEYPAD_GENERIEK_RIJ_PINNEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEF..."
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_USER_DEFINED_MATRIX -DKEYPAD_GENERIEK_AANTAL_RIJEN=2 -DKEYPAD_GENERIEK_KOLOM_PINNEN={2,3} -DKEYPAD_GENERIEK_AANTAL_KOLOMMEN=2 -DKEYPAD_GENERIEK_KEY_LAYOUT={{_LABEL_OPSCHRIFT_1,_LABEL_OPSCHRIFT_1},{_LABEL_OPSCHRIFT_2,_LABEL_OPSCHRIFT_2},{_LABEL_OPSCHRIFT_3,_LABEL_OPSCHRIFT_3},{_LABEL_OPSCHRIFT_4,_LABEL_OPSCHRIFT_4}}"
set "VERWACHTE_FOUT=KEYPAD_GENERIEK_RIJ_PINNEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_MATRIX."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=KEYPAD_GENERIEK_AANTAL_RIJEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_D..."
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_USER_DEFINED_MATRIX -DKEYPAD_GENERIEK_RIJ_PINNEN={0,1} -DKEYPAD_GENERIEK_KOLOM_PINNEN={2,3} -DKEYPAD_GENERIEK_AANTAL_KOLOMMEN=2 -DKEYPAD_GENERIEK_KEY_LAYOUT={{_LABEL_OPSCHRIFT_1,_LABEL_OPSCHRIFT_1},{_LABEL_OPSCHRIFT_2,_LABEL_OPSCHRIFT_2},{_LABEL_OPSCHRIFT_3,_LABEL_OPSCHRIFT_3},{_LABEL_OPSCHRIFT_4,_LABEL_OPSCHRIFT_4}}"
set "VERWACHTE_FOUT=KEYPAD_GENERIEK_AANTAL_RIJEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_MATRIX."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=KEYPAD_GENERIEK_KOLOM_PINNEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_D..."
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_USER_DEFINED_MATRIX -DKEYPAD_GENERIEK_RIJ_PINNEN={0,1} -DKEYPAD_GENERIEK_AANTAL_RIJEN=2 -DKEYPAD_GENERIEK_AANTAL_KOLOMMEN=2 -DKEYPAD_GENERIEK_KEY_LAYOUT={{_LABEL_OPSCHRIFT_1,_LABEL_OPSCHRIFT_1},{_LABEL_OPSCHRIFT_2,_LABEL_OPSCHRIFT_2},{_LABEL_OPSCHRIFT_3,_LABEL_OPSCHRIFT_3},{_LABEL_OPSCHRIFT_4,_LABEL_OPSCHRIFT_4}}"
set "VERWACHTE_FOUT=KEYPAD_GENERIEK_KOLOM_PINNEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_MATRIX."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=KEYPAD_GENERIEK_AANTAL_KOLOMMEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USE..."
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_USER_DEFINED_MATRIX -DKEYPAD_GENERIEK_RIJ_PINNEN={0,1} -DKEYPAD_GENERIEK_AANTAL_RIJEN=2 -DKEYPAD_GENERIEK_KOLOM_PINNEN={2,3} -DKEYPAD_GENERIEK_KEY_LAYOUT={{_LABEL_OPSCHRIFT_1,_LABEL_OPSCHRIFT_1},{_LABEL_OPSCHRIFT_2,_LABEL_OPSCHRIFT_2},{_LABEL_OPSCHRIFT_3,_LABEL_OPSCHRIFT_3},{_LABEL_OPSCHRIFT_4,_LABEL_OPSCHRIFT_4}}"
set "VERWACHTE_FOUT=KEYPAD_GENERIEK_AANTAL_KOLOMMEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_MATRIX."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=Ongeldige KEYPAD_TYPE voor INPUT_TYPE_PCF8574"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=99"
set "VERWACHTE_FOUT=INPUT_TYPE_PCF8574 ondersteunt alleen de gedefinieerde keypads die maximaal acht signaallijnen nodig hebben."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED moet TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW of TTP..."
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4 -DTTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED=99"
set "VERWACHTE_FOUT=TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED moet TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW of TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=INPUT_TYPE_PCF8574 ondersteunt alleen de gedefinieerde keypads die maximaal acht signaall..."
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE_MEMBRAAN_MATRIX_4x5=34 -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_MATRIX_4x5"
set "VERWACHTE_FOUT=INPUT_TYPE_PCF8574 ondersteunt alleen de gedefinieerde keypads die maximaal acht signaallijnen nodig hebben."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=Ongeldige KEYPAD_TYPE voor AANTAL_KEYPAD_TOETSEN"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=99"
set "VERWACHTE_FOUT=Ongeldige KEYPAD_TYPE voor AANTAL_KEYPAD_TOETSEN."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set "TEST_NAAM=SCREEN_OUTPUT_CONFIG bevat een onbekende uitvoerbit. Gebruik alleen SCREEN_TYPE_NONE, SCR..."
set "TEST_FLAGS=-DSCREEN_OUTPUT_CONFIG=8"
set "VERWACHTE_FOUT=SCREEN_OUTPUT_CONFIG bevat een onbekende uitvoerbit. Gebruik alleen SCREEN_TYPE_NONE, SCREEN_TYPE_SERIAL, SCREEN_TYPE_CHARACTER en SCREEN_TYPE_PIXELS."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Scenario1_EnkelTik\Scenario1_EnkelTik.ino"
set "TEST_NAAM=Input: onbekend invoertype"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=0x80"
set "VERWACHTE_FOUT=INPUT_KANAAL_CONFIG bevat een onbekend invoertype."
call :EXPECT_FAIL

set "TEST_INO=examples\Toepassingsgebieden\Stimulus\Scenario1_EnkelTik\Scenario1_EnkelTik.ino"
set "TEST_NAAM=Input: ongeldige invoertype-combinatie"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=0x80"
set "VERWACHTE_FOUT=Geldige INPUT_KANAAL_CONFIG: INPUT_TYPE_NONE, INPUT_TYPE_PCF8574, INPUT_TYPE_HX1838 of INPUT_TYPE_PCF8574 | INPUT_TYPE_HX1838."
call :EXPECT_FAIL

echo.
set "VERWACHTE_FOUT=SCREEN_OUTPUT_CONFIG moet in SystemConfig.h worden gedefinieerd."

echo Totaal !TESTS! - OK !OK! - FOUT !FAIL!
if !FAIL!==0 (exit /b 0) else exit /b 1
