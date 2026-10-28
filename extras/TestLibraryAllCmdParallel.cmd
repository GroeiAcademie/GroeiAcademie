@echo off
cd /d "%~dp0"

rem Opent alle negen testdomeinen als tabs in een nieuw Windows Terminal-venster.
rem Per tab eerst het Ongeldig-script, daarna het gewone script; het tabblad Foutinjecties heeft geen Ongeldig-script. Vereist Windows Terminal (wt.exe).
rem In de -Command-tekst staat \; omdat Windows Terminal een gewone ; als scheiding tussen tabs leest.

wt -w new ^
 new-tab --title "Data" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibraryDataOngeldig-v2.0.0.cmd'\; & '.\TestLibraryData-v2.0.0.cmd'" ^
 ; new-tab --title "Extenders" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibraryExtendersOngeldig-v2.0.0.cmd'\; & '.\TestLibraryExtenders-v2.0.0.cmd'" ^
 ; new-tab --title "GedeeldeBus" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibraryGedeeldeBusOngeldig-v2.0.0.cmd'\; & '.\TestLibraryGedeeldeBus-v2.0.0.cmd'" ^
 ; new-tab --title "Input" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibraryInputOngeldig-v2.0.0.cmd'\; & '.\TestLibraryInput-v2.0.0.cmd'" ^
 ; new-tab --title "Screen" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibraryScreenOngeldig-v2.0.0.cmd'\; & '.\TestLibraryScreen-v2.0.0.cmd'" ^
 ; new-tab --title "Sensoren" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibrarySensorenOngeldig-v2.0.0.cmd'\; & '.\TestLibrarySensoren-v2.0.0.cmd'" ^
 ; new-tab --title "Sturingen" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibrarySturingenOngeldig-v2.0.0.cmd'\; & '.\TestLibrarySturingen-v2.0.0.cmd'" ^
 ; new-tab --title "Tools" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibraryToolsOngeldig-v2.0.0.cmd'\; & '.\TestLibraryTools-v2.0.0.cmd'" ^
 ; new-tab --title "Foutinjecties" -d "%~dp0." powershell.exe -NoLogo -NoProfile -NoExit -Command "& '.\TestLibraryFoutinjecties-v2.0.0.cmd'"

exit /b 0
