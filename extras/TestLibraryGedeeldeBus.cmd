@echo off
if /I "%~1"=="--TESTLIJST" goto :TESTLIJST

setlocal EnableDelayedExpansion
set "BESTANDSNAAM=TestLibraryGedeeldeBus"
set "CONTROLE_MODUS=INCLUDE"
set "CONTROLE_PADEN=examples\Systeem\GedeeldeBus\;"

set "EXTRA_FLAGS=-DGEDEELDE_BUS_PROTOTYPE"

call "%~dp0TestLibraryCommon.cmd" %*
exit /b %errorlevel%

:TESTLIJST
REM Eerste uitvoering genereert TestLibraryGedeeldeBus.ino met de ontbrekende invulblokken.

exit /b 0
