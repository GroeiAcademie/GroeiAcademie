@echo off
setlocal
cd /d "%~dp0"
if exist "LokalePaden.cmd" call "LokalePaden.cmd"
if not defined CCPCHECK_DATA_PATH (
    echo CCPCHECK_DATA_PATH ontbreekt in extras\LokalePaden.cmd.
    exit /b 1
)

:: Bestaande parameters blijven rechtstreeks ondersteund.
:: Alleen -CacheResponsbestanden zonder andere parameters doorloopt de volledige CMD-keten.
if not "%~1"=="" (
    if /I "%~1"=="-CacheResponsbestanden" (
        if not "%~2"=="" (
            powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0CppcheckBijwerken.ps1" %*
            if errorlevel 1 exit /b 1
            exit /b 0
        )
    ) else (
        powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0CppcheckBijwerken.ps1" %*
        if errorlevel 1 exit /b 1
        exit /b 0
    )
)

:: Een CMD: alle negen gewone testdomeinen, samenvoegen, daarna Cppcheck.
if not defined CLI_PATH (
    where arduino-cli.exe >nul 2>&1
    if errorlevel 1 (
        echo Arduino CLI niet gevonden via CLI_PATH of PATH.
        exit /b 1
    )
    for /f "delims=" %%P in ('where arduino-cli.exe') do if not defined CLI_PATH set "CLI_PATH=%%P"
)
if not defined CPPCHECK_PATH (
    echo CPPCHECK_PATH ontbreekt in extras\LokalePaden.cmd.
    exit /b 1
)
if not exist "%CPPCHECK_PATH%" (
    echo Cppcheck niet gevonden: "%CPPCHECK_PATH%"
    exit /b 1
)

if /I "%~1"=="-CacheResponsbestanden" (
    powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0CppcheckBijwerken.ps1" -UpdateAll -CliPath "%CLI_PATH%" -CacheResponsbestanden
) else (
    powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0CppcheckBijwerken.ps1" -UpdateAll -CliPath "%CLI_PATH%"
)
if errorlevel 1 exit /b 1

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0CppcheckBijwerken.ps1" -Finalize
if errorlevel 1 exit /b 1

if not exist "%CCPCHECK_DATA_PATH%\compile_commands.json" (
    echo De gezamenlijke compilatiedatabase ontbreekt. Geen Cppcheck-scan.
    exit /b 1
)
if not exist "%CCPCHECK_DATA_PATH%\CppcheckCache" mkdir "%CCPCHECK_DATA_PATH%\CppcheckCache"
if errorlevel 1 exit /b 1

echo [CCPCHECK] Alle configuraties verzameld en gecontroleerd. Start Cppcheck-scan.
"%CPPCHECK_PATH%" --project="%CCPCHECK_DATA_PATH%\compile_commands.json" --cppcheck-build-dir="%CCPCHECK_DATA_PATH%\CppcheckCache" --platform=unix32 --enable=all --check-level=exhaustive --inline-suppr --xml --xml-version=2 --output-file="%CCPCHECK_DATA_PATH%\CppcheckBijwerken.xml" 1>"%CCPCHECK_DATA_PATH%\CppcheckBijwerken.stdout.log"
if errorlevel 1 (
    echo Cppcheck heeft een foutstatus gemeld. Controleer de uitvoerbestanden.
    exit /b 1
)
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0CppcheckBijwerken.ps1" -RapportInoRegels "%CCPCHECK_DATA_PATH%\CppcheckBijwerken.xml" -OutputDatabase "%CCPCHECK_DATA_PATH%\compile_commands.json"
if errorlevel 1 exit /b 1
echo [CCPCHECK] Cppcheck voltooid. Rapport: "%CCPCHECK_DATA_PATH%\CppcheckBijwerken.xml"
exit /b 0
