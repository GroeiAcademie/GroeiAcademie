@echo off
setlocal
cd /d "%~dp0"
if exist "LokalePaden.cmd" call "LokalePaden.cmd"
if not defined CCPCHECK_DATA_PATH (
    echo CCPCHECK_DATA_PATH ontbreekt in extras\LokalePaden.cmd.
    exit /b 1
)
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0CppcheckBijwerken.ps1" %*
exit /b %errorlevel%
