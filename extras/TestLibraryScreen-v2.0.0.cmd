@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryScreen-v2.0.0"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\Screen\;"

call "%~dp0TestLibraryCommon.cmd" %*
exit /b %errorlevel%

:TESTLIJST
set TEST_INO="examples\Systeem\Screen\Callback_CharacterScreen\Callback_CharacterScreen.ino"
set TEST_NAAM="Callback CharacterScreen"
set TEST_FLAGS="-DSCREEN_OUTPUT_CONFIG=6"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Screen\Callback_PixelScreen\Callback_PixelScreen.ino"
set TEST_NAAM="Callback PixelScreen"
set TEST_FLAGS="-DSCREEN_OUTPUT_CONFIG=4"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Screen\Default_CharacterScreen\Default_CharacterScreen.ino"
set TEST_NAAM="CharacterScreen"
set TEST_FLAGS="-DSCREEN_OUTPUT_CONFIG=6"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Screen\Default_CharacterScreen_PixelScreen\Default_CharacterScreen_PixelScreen.ino"
set TEST_NAAM="Default CharacterScreen + PixelScreen"
set TEST_FLAGS="-DSCREEN_OUTPUT_CONFIG=7"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Screen\Default_PixelScreen\Default_PixelScreen.ino"
set TEST_NAAM="PixelScreen"
set TEST_FLAGS="-DSCREEN_OUTPUT_CONFIG=4"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Screen\Default_CharacterScreen\Default_CharacterScreen.ino"
set TEST_NAAM="CharacterScreen regressie"
set TEST_FLAGS="-DSCREEN_OUTPUT_CONFIG=2"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT


set TEST_INO="examples\Systeem\Screen\Default_CharacterScreen\Default_CharacterScreen.ino"
set TEST_NAAM="SCREEN_OUTPUT_CONFIG=1"
set TEST_FLAGS="-DSCREEN_OUTPUT_CONFIG=1"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Screen\Default_CharacterScreen\Default_CharacterScreen.ino"
set TEST_NAAM="SCREEN_OUTPUT_CONFIG=3"
set TEST_FLAGS="-DSCREEN_OUTPUT_CONFIG=3"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT

set TEST_INO="examples\Systeem\Screen\Default_PixelScreen\Default_PixelScreen.ino"
set TEST_NAAM="SCREEN_OUTPUT_CONFIG=5"
set TEST_FLAGS="-DSCREEN_OUTPUT_CONFIG=5"
set TEST_TYPE="NIEUW"
set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
call "%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT
exit /b 0
