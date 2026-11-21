@echo off
setlocal
cd /d "%~dp0"
if not exist "%~dp0CppcheckOudeDataOpruimen.ps1" (
    echo CppcheckOudeDataOpruimen.ps1 ontbreekt in extras.
    exit /b 1
)
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0CppcheckOudeDataOpruimen.ps1"
set "RESULTAAT=%errorlevel%"
if not "%RESULTAAT%"=="0" (
    echo.
    echo [CCPCHECK] Opruimen afgebroken of mislukt. Code en databases niet opnieuw opgebouwd.
)
pause
exit /b %RESULTAAT%
