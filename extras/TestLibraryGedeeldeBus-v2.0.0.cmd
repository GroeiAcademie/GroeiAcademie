@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryGedeeldeBus-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\GedeeldeBus\Default_CharacterScreen\;examples\Systeem\GedeeldeBus\Default_CharacterScreen_PixelScreen\;examples\Systeem\GedeeldeBus\Default_PixelScreen\;examples\Systeem\GedeeldeBus\InputkanalenDIGITAL\;examples\Systeem\GedeeldeBus\InputkanalenHX1838\;examples\Systeem\GedeeldeBus\InputkanalenPCF8574\;examples\Systeem\GedeeldeBus\Tik_Enkele_Samen_Instortend_Cocktail\;"

call "%~dp0TestLibraryCommon.cmd" %*
exit /b %errorlevel%

:TESTLIJST
set TEST_INO="examples\Systeem\GedeeldeBus\Default_CharacterScreen\Default_CharacterScreen.ino"
set TEST_NAAM="Default_CharacterScreen"
set TEST_FLAGS=""
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Default_CharacterScreen_PixelScreen\Default_CharacterScreen_PixelScreen.ino"
set TEST_NAAM="Default_CharacterScreen_PixelScreen"
set TEST_FLAGS=""
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Default_PixelScreen\Default_PixelScreen.ino"
set TEST_NAAM="Default_PixelScreen"
set TEST_FLAGS=""
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\InputkanalenDIGITAL\InputkanalenDIGITAL.ino"
set TEST_NAAM="InputkanalenDIGITAL"
set TEST_FLAGS=""
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\InputkanalenHX1838\InputkanalenHX1838.ino"
set TEST_NAAM="InputkanalenHX1838"
set TEST_FLAGS=""
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\InputkanalenPCF8574\InputkanalenPCF8574.ino"
set TEST_NAAM="InputkanalenPCF8574"
set TEST_FLAGS=""
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\GedeeldeBus\Tik_Enkele_Samen_Instortend_Cocktail\Tik_Enkele_Samen_Instortend_Cocktail.ino"
set TEST_NAAM="Tik_Enkele_Samen_Instortend_Cocktail"
set TEST_FLAGS=""
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

exit /b 0
