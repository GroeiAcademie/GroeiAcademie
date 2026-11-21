@echo off
setlocal
cd /d "%~dp0"
if not exist "%~dp0Cppcheck_ALL_BOARD.ps1" (
    echo Cppcheck_ALL_BOARD.ps1 ontbreekt in extras.
    exit /b 1
)
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Cppcheck_ALL_BOARD.ps1"
if errorlevel 1 exit /b 1
exit /b 0
