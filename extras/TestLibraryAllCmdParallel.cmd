@echo off
cd /d "%~dp0"

rem Opent alle negen testdomeinen als tabs in een nieuw Windows Terminal-venster.
rem Per tab eerst het Ongeldig-script, daarna het gewone script. Vereist Windows Terminal (wt.exe).
rem In de -Command-tekst staat \; omdat Windows Terminal een gewone ; als scheiding tussen tabs leest.

wt -w new ^
 new-tab --title "1] Data" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibraryDataOngeldig-v2.0.0.cmd'\; & '.\TestLibraryData-v2.0.0.cmd'" ^
 ; new-tab --title "2] Extenders" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibraryExtendersOngeldig-v2.0.0.cmd'\; & '.\TestLibraryExtenders-v2.0.0.cmd'" ^
 ; new-tab --title "3] Foutinjecties" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibraryFoutinjectiesOngeldig-v2.0.0.cmd'\; & '.\TestLibraryFoutinjecties-v2.0.0.cmd'" ^
 ; new-tab --title "4] GedeeldeBus" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibraryGedeeldeBusOngeldig-v2.0.0.cmd'\; & '.\TestLibraryGedeeldeBus-v2.0.0.cmd'" ^
 ; new-tab --title "5] Input" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibraryInputOngeldig-v2.0.0.cmd'\; & '.\TestLibraryInput-v2.0.0.cmd'" ^
 ; new-tab --title "6] Screen" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibraryScreenOngeldig-v2.0.0.cmd'\; & '.\TestLibraryScreen-v2.0.0.cmd'" ^
 ; new-tab --title "7] Sensoren" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibrarySensorenOngeldig-v2.0.0.cmd'\; & '.\TestLibrarySensoren-v2.0.0.cmd' --split-test-per-fqbn" ^
 ; new-tab --title "8] Sturingen" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibrarySturingenOngeldig-v2.0.0.cmd'\; & '.\TestLibrarySturingen-v2.0.0.cmd'" ^
 ; new-tab --title "9] Tools" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibraryToolsOngeldig-v2.0.0.cmd'\; & '.\TestLibraryTools-v2.0.0.cmd'"

exit /b 0
