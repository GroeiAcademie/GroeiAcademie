@echo off
setlocal
cd /d "%~dp0"
pushd "%~dp0.."

if exist "extras\LokalePaden.cmd" call "extras\LokalePaden.cmd"
if not defined CCPCHECK_DATA_PATH (
    echo CCPCHECK_DATA_PATH ontbreekt in extras\LokalePaden.cmd.
    popd
    exit /b 1
)

:: Alleen samenvoegen en na succesvolle controle verouderde hashmappen opruimen.
:: De negen gewone test-CMD's worden niet uitgevoerd of gewijzigd.
powershell -NoProfile -ExecutionPolicy Bypass -File "extras\CppcheckBijwerken.ps1" -Finalize
set "RESULTAAT=%errorlevel%"
popd
exit /b %RESULTAAT%
