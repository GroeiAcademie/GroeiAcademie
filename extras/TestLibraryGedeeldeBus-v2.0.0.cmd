@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryGedeeldeBus-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\Screen\Default_CharacterScreen\;examples\Systeem\Screen\Default_CharacterScreen_PixelScreen\;examples\Systeem\Screen\Default_PixelScreen\;examples\Systeem\Input\InputkanalenDIGITAL\;examples\Systeem\Input\InputkanalenHX1838\;examples\Systeem\Input\InputkanalenPCF8574\;examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\;"

call "%~dp0TestLibraryCommon.cmd" %*
exit /b %errorlevel%

:TESTLIJST
set TEST_INO="examples\Systeem\Screen\Default_CharacterScreen\Default_CharacterScreen.ino"
set TEST_NAAM="Default_CharacterScreen"
set TEST_FLAGS="-DSCREEN_OUTPUT_CONFIG=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Screen\Default_CharacterScreen_PixelScreen\Default_CharacterScreen_PixelScreen.ino"
set TEST_NAAM="Default_CharacterScreen_PixelScreen"
set TEST_FLAGS="-DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Screen\Default_PixelScreen\Default_PixelScreen.ino"
set TEST_NAAM="Default_PixelScreen"
set TEST_FLAGS="-DSCREEN_OUTPUT_CONFIG=4"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Input\InputkanalenDIGITAL\InputkanalenDIGITAL.ino"
set TEST_NAAM="InputkanalenDIGITAL"
set TEST_FLAGS="-DINPUT_KANAAL_CONFIG=INPUT_TYPE_DIGITAL -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Input\InputkanalenHX1838\InputkanalenHX1838.ino"
set TEST_NAAM="InputkanalenHX1838"
set TEST_FLAGS="-DINPUT_KANAAL_CONFIG=INPUT_TYPE_HX1838 -DHX1838_BRON_CODES=HX1838_BRON_CODES_DEFINE -DHX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE=1 -DHX1838_TOETSENINDELING=HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Input\InputkanalenPCF8574\InputkanalenPCF8574.ino"
set TEST_NAAM="InputkanalenPCF8574"
set TEST_FLAGS="-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set TEST_NAAM="Tik_Enkele_Samen_Instortend_Cocktail"
set TEST_FLAGS="-DADC_BACKEND=ADC_BACKEND_NATIVE -DINPUT_KANAAL_CONFIG=INPUT_TYPE_DIGITAL -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 -DSCREEN_OUTPUT_CONFIG=SCREEN_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

exit /b 0
