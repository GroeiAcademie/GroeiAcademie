@echo off
setlocal
cd /d "%~dp0"
pushd "%~dp0.."

:: Zelfstandige Cppcheck-update. De bestaande test-CMD's worden niet uitgevoerd of gewijzigd.
:: Gebruik dezelfde lokale Arduino CLI-configuratie als TestLibraryCommon.cmd.
where arduino-cli >nul 2>&1
if exist "extras\LokalePaden.cmd" (
    call "extras\LokalePaden.cmd"
) else (
    where arduino-cli >nul 2>&1
    if not errorlevel 1 (
        for /f "delims=" %%P in ('where arduino-cli') do set "CLI_PATH=%%~dpnP"
    ) else (
        echo arduino-cli niet gevonden via PATH, en extras\LokalePaden.cmd bestaat niet.
        popd
        exit /b 1
    )
)

if not defined CCPCHECK_DATA_PATH (
    echo CCPCHECK_DATA_PATH ontbreekt in extras\LokalePaden.cmd.
    popd
    exit /b 1
)

if not defined CLI_PATH (
    echo Arduino CLI niet gevonden: CLI_PATH is leeg.
    popd
    exit /b 1
)

wt -w new ^
 new-tab --title "Data" -d "%~dp0.." powershell.exe -NoLogo -NoProfile -NoExit -ExecutionPolicy Bypass -File "extras\CppcheckBijwerken.ps1" -TestScript "Data" -SnapshotRoot "%CCPCHECK_DATA_PATH%\Databases" -CliPath "%CLI_PATH%" ^
 ; new-tab --title "Extenders" -d "%~dp0.." powershell.exe -NoLogo -NoProfile -NoExit -ExecutionPolicy Bypass -File "extras\CppcheckBijwerken.ps1" -TestScript "Extenders" -SnapshotRoot "%CCPCHECK_DATA_PATH%\Databases" -CliPath "%CLI_PATH%" ^
 ; new-tab --title "GedeeldeBus" -d "%~dp0.." powershell.exe -NoLogo -NoProfile -NoExit -ExecutionPolicy Bypass -File "extras\CppcheckBijwerken.ps1" -TestScript "GedeeldeBus" -SnapshotRoot "%CCPCHECK_DATA_PATH%\Databases" -CliPath "%CLI_PATH%" ^
 ; new-tab --title "Input" -d "%~dp0.." powershell.exe -NoLogo -NoProfile -NoExit -ExecutionPolicy Bypass -File "extras\CppcheckBijwerken.ps1" -TestScript "Input" -SnapshotRoot "%CCPCHECK_DATA_PATH%\Databases" -CliPath "%CLI_PATH%" ^
 ; new-tab --title "Screen" -d "%~dp0.." powershell.exe -NoLogo -NoProfile -NoExit -ExecutionPolicy Bypass -File "extras\CppcheckBijwerken.ps1" -TestScript "Screen" -SnapshotRoot "%CCPCHECK_DATA_PATH%\Databases" -CliPath "%CLI_PATH%" ^
 ; new-tab --title "Sensoren" -d "%~dp0.." powershell.exe -NoLogo -NoProfile -NoExit -ExecutionPolicy Bypass -File "extras\CppcheckBijwerken.ps1" -TestScript "Sensoren" -SnapshotRoot "%CCPCHECK_DATA_PATH%\Databases" -CliPath "%CLI_PATH%" ^
 ; new-tab --title "Sturingen" -d "%~dp0.." powershell.exe -NoLogo -NoProfile -NoExit -ExecutionPolicy Bypass -File "extras\CppcheckBijwerken.ps1" -TestScript "Sturingen" -SnapshotRoot "%CCPCHECK_DATA_PATH%\Databases" -CliPath "%CLI_PATH%" ^
 ; new-tab --title "Tools" -d "%~dp0.." powershell.exe -NoLogo -NoProfile -NoExit -ExecutionPolicy Bypass -File "extras\CppcheckBijwerken.ps1" -TestScript "Tools" -SnapshotRoot "%CCPCHECK_DATA_PATH%\Databases" -CliPath "%CLI_PATH%" ^
 ; new-tab --title "Foutinjecties" -d "%~dp0.." powershell.exe -NoLogo -NoProfile -NoExit -ExecutionPolicy Bypass -File "extras\CppcheckBijwerken.ps1" -TestScript "Foutinjecties" -SnapshotRoot "%CCPCHECK_DATA_PATH%\Databases" -CliPath "%CLI_PATH%"
set "RESULTAAT=%errorlevel%"
popd
exit /b %RESULTAAT%
