@echo off
cd /d "%~dp0"
set "BEPERK_TEST_TOT_FQBN=arduino:renesas_uno:minima"

rem Opent alle negen testdomeinen als tabs in een nieuw Windows Terminal-venster.
rem De gewone scripts compileren alleen op de FQBN in BEPERK_TEST_TOT_FQBN.
rem De Ongeldig-scripts compileren al op hun eigen TEST_FQBN.
rem Elke tab zet BEPERK_TEST_TOT_FQBN zelf, omdat Windows Terminal de omgeving niet altijd doorgeeft aan nieuwe tabs.
rem Per tab eerst het Ongeldig-script, daarna het gewone script. Vereist Windows Terminal (wt.exe).
rem In de -Command-tekst staat \; omdat Windows Terminal een gewone ; als scheiding tussen tabs leest.

wt -w new ^
 new-tab --title "1] Data" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "$env:BEPERK_TEST_TOT_FQBN='%BEPERK_TEST_TOT_FQBN%'\; & '.\TestLibraryDataOngeldig-v2.0.0.cmd'\; & '.\TestLibraryData-v2.0.0.cmd'" ^
 ; new-tab --title "2] Extenders" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "$env:BEPERK_TEST_TOT_FQBN='%BEPERK_TEST_TOT_FQBN%'\; & '.\TestLibraryExtendersOngeldig-v2.0.0.cmd'\; & '.\TestLibraryExtenders-v2.0.0.cmd'" ^
 ; new-tab --title "3] Foutinjecties" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "$env:BEPERK_TEST_TOT_FQBN='%BEPERK_TEST_TOT_FQBN%'\; & '.\TestLibraryFoutinjectiesOngeldig-v2.0.0.cmd'\; & '.\TestLibraryFoutinjecties-v2.0.0.cmd'" ^
 ; new-tab --title "4] GedeeldeBus" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "$env:BEPERK_TEST_TOT_FQBN='%BEPERK_TEST_TOT_FQBN%'\; & '.\TestLibraryGedeeldeBusOngeldig-v2.0.0.cmd'\; & '.\TestLibraryGedeeldeBus-v2.0.0.cmd'" ^
 ; new-tab --title "5] Input" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "$env:BEPERK_TEST_TOT_FQBN='%BEPERK_TEST_TOT_FQBN%'\; & '.\TestLibraryInputOngeldig-v2.0.0.cmd'\; & '.\TestLibraryInput-v2.0.0.cmd'" ^
 ; new-tab --title "6] Screen" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "$env:BEPERK_TEST_TOT_FQBN='%BEPERK_TEST_TOT_FQBN%'\; & '.\TestLibraryScreenOngeldig-v2.0.0.cmd'\; & '.\TestLibraryScreen-v2.0.0.cmd'" ^
 ; new-tab --title "7] Sensoren" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "$env:BEPERK_TEST_TOT_FQBN='%BEPERK_TEST_TOT_FQBN%'\; & '.\TestLibrarySensorenOngeldig-v2.0.0.cmd'\; & '.\TestLibrarySensoren-v2.0.0.cmd'" ^
 ; new-tab --title "8] Sturingen" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "$env:BEPERK_TEST_TOT_FQBN='%BEPERK_TEST_TOT_FQBN%'\; & '.\TestLibrarySturingenOngeldig-v2.0.0.cmd'\; & '.\TestLibrarySturingen-v2.0.0.cmd'" ^
 ; new-tab --title "9] Tools" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "$env:BEPERK_TEST_TOT_FQBN='%BEPERK_TEST_TOT_FQBN%'\; & '.\TestLibraryToolsOngeldig-v2.0.0.cmd'\; & '.\TestLibraryTools-v2.0.0.cmd'"

exit /b 0
