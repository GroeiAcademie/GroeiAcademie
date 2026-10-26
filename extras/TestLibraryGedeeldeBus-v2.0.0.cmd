@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryGedeeldeBus-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Toepassingsgebieden\Stimulus\Scenario1_EnkelTik\;examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\;"

call "%~dp0TestLibraryCommon.cmd" %*
exit /b %errorlevel%

:TESTLIJST
set TEST_INO="examples\Toepassingsgebieden\Stimulus\Scenario1_EnkelTik\Scenario1_EnkelTik.ino"
set TEST_NAAM="INPUT_TYPE_NONE"
set TEST_FLAGS="-DINPUT_KANAAL_CONFIG=INPUT_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set TEST_NAAM="Tik Enkele Samen Instortend Cocktail"
set TEST_FLAGS="-DINPUT_KANAAL_CONFIG=INPUT_TYPE_NONE"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

exit /b 0
