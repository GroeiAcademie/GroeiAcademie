@echo off
setlocal EnableDelayedExpansion

pushd "%~dp0.."

where arduino-cli >nul 2>&1
if exist "extras\LokalePaden.cmd" (
    call "extras\LokalePaden.cmd"
) else (
    where arduino-cli >nul 2>&1
    if not errorlevel 1 (
        for /f "delims=" %%P in ('where arduino-cli') do set "CLI_PATH=%%~dpnxP"
    ) else (
        echo arduino-cli niet gevonden via PATH, en extras\LokalePaden.cmd bestaat niet.
        popd
        exit /b 1
    )
)

set "FQBN=arduino:renesas_uno:minima"
set "BOARD_VERSION=BOARD_UNO_R4_MINIMA"
set /A TESTS=0
set /A OK=0
set /A FAIL=0

call :TEST "examples\Systeem\Screen\Callback_CharacterScreen\Callback_CharacterScreen.ino" "Callback CharacterScreen" "-DSCREEN_OUTPUT_CONFIG=6"
call :TEST "examples\Systeem\Screen\Callback_PixelScreen\Callback_PixelScreen.ino" "Callback PixelScreen" "-DSCREEN_OUTPUT_CONFIG=4"
call :TEST "examples\Systeem\Screen\Default_CharacterScreen\Default_CharacterScreen.ino" "CharacterScreen" "-DSCREEN_OUTPUT_CONFIG=6"
call :TEST "examples\Systeem\Screen\Default_CharacterScreen_PixelScreen\Default_CharacterScreen_PixelScreen.ino" "Default CharacterScreen + PixelScreen" "-DSCREEN_OUTPUT_CONFIG=6"
call :TEST "examples\Systeem\Screen\Default_PixelScreen\Default_PixelScreen.ino" "PixelScreen" "-DSCREEN_OUTPUT_CONFIG=4"
call :TEST "examples\Systeem\Screen\Default_CharacterScreen\Default_CharacterScreen.ino" "CharacterScreen regressie" "-DSCREEN_OUTPUT_CONFIG=2"

echo ============================================================
echo TestLibraryScreen-v2.0.0
echo Board : %FQBN%
echo Tests : %TESTS%
echo OK    : %OK%
echo FOUT  : %FAIL%
echo ============================================================

popd
if %FAIL% GTR 0 exit /b 1
exit /b 0

:TEST
set /A TESTS+=1
set "TEST_INO=%~1"
set "TEST_NAAM=%~2"
set "TEST_FLAGS=%~3"

if not exist "%TEST_INO%" (
    echo [FOUT] TEST_INO bestaat niet: %TEST_INO%
    set /A FAIL+=1
    exit /b 0
)

for %%X in ("%TEST_INO%") do set "TEST_DIR=%%~dpX"
set "TEST_DIR=!TEST_DIR:~0,-1!"

echo ------------------------------------------------------------
echo Compileren van: %TEST_INO%
echo FQBN: %FQBN%
echo BOARD_VERSION: %BOARD_VERSION%
echo %TEST_NAAM%
"%CLI_PATH%" compile --jobs 1 --fqbn "%FQBN%" --build-property "compiler.cpp.extra_flags=-DGROEIACADEMIE_IGNORE_USER_CONFIG %TEST_FLAGS% -DBOARD_VERSION=%BOARD_VERSION%" "!TEST_DIR!"
if errorlevel 1 (
    echo [FOUT] %TEST_NAAM%
    set /A FAIL+=1
) else (
    echo [OK] %TEST_NAAM%
    set /A OK+=1
)
exit /b 0
