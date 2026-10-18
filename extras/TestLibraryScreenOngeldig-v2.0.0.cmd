@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryScreenOngeldig-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\Screen\;"
set "BRON_TEST_SCRIPT=%~dp0TestLibraryScreen-v2.0.0.cmd"

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

set "TEST_INO=examples\Systeem\Screen\Callback_CharacterScreen\Callback_CharacterScreen.ino"
set "TEST_NAAM=TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED moet TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW of TTP..."
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4 -DTTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED=99"
set "VERWACHTE_FOUT=TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED moet TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW of TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH zijn."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\Screen\Callback_CharacterScreen\Callback_CharacterScreen.ino"
set "TEST_NAAM=INPUT_TYPE_PCF8574 ondersteunt alleen de gedefinieerde keypads die maximaal acht signaall..."
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE_MEMBRAAN_MATRIX_4x5=34 -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_MATRIX_4x5"
set "VERWACHTE_FOUT=INPUT_TYPE_PCF8574 ondersteunt alleen de gedefinieerde keypads die maximaal acht signaallijnen nodig hebben."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\Screen\Callback_CharacterScreen\Callback_CharacterScreen.ino"
set "TEST_NAAM=SCREEN_OUTPUT_CONFIG moet in SystemConfig.h worden gedefinieerd"
set "TEST_FLAGS=-USCREEN_OUTPUT_CONFIG"
set "VERWACHTE_FOUT=SCREEN_OUTPUT_CONFIG moet in SystemConfig.h worden gedefinieerd."
call :EXPECT_FAIL

set "TEST_INO=examples\Systeem\Screen\Callback_CharacterScreen\Callback_CharacterScreen.ino"
set "TEST_NAAM=SCREEN_OUTPUT_CONFIG bevat een onbekende uitvoerbit. Gebruik alleen SCREEN_TYPE_NONE, SCR..."
set "TEST_FLAGS=-DSCREEN_OUTPUT_CONFIG=8"
set "VERWACHTE_FOUT=SCREEN_OUTPUT_CONFIG bevat een onbekende uitvoerbit. Gebruik alleen SCREEN_TYPE_NONE, SCREEN_TYPE_SERIAL, SCREEN_TYPE_CHARACTER en SCREEN_TYPE_PIXELS."
call :EXPECT_FAIL

echo.
echo Totaal !TESTS! - OK !OK! - FOUT !FAIL!
if !FAIL!==0 (exit /b 0) else exit /b 1
