@echo off
cls
setlocal enabledelayedexpansion

if "%~1"=="--logged" goto :MAIN_SCRIPT
if exist "%~dp0TestLibraryGedeeldeBus.txt" del /q "%~dp0TestLibraryGedeeldeBus.txt" >nul 2>&1
powershell -NoProfile -Command "& { & '%~f0' --logged %* 2>&1 | Tee-Object -FilePath '%~dp0TestLibraryGedeeldeBus.txt'; exit $LASTEXITCODE }"
set "SCRIPT_RESULT=%errorlevel%"
exit /b %SCRIPT_RESULT%

:MAIN_SCRIPT
pushd "%~dp0.."

where arduino-cli >nul 2>&1
if not errorlevel 1 (
    for /f "delims=" %%P in ('where arduino-cli') do set "CLI_PATH=%%~dpnP"
) else if exist "extras\LokalePaden.cmd" (
    call "extras\LokalePaden.cmd"
) else (
    echo arduino-cli niet gevonden via PATH, en extras\LokalePaden.cmd bestaat niet.
    if /I not "%~2"=="--no-pause" pause
    popd
    exit /b 1
)

set INCLUDE="examples\Systeem\GedeeldeBus"

set "BOARD_1_FQBN=arduino:avr:uno"
set "BOARD_1_FLAGS=-DBOARD_VERSION=BOARD_UNO_R3"
set "BOARD_1_NAAM=Arduino UNO R3"
set "BOARD_2_FQBN=arduino:renesas_uno:minima"
set "BOARD_2_FLAGS=-DBOARD_VERSION=BOARD_UNO_R4_MINIMA"
set "BOARD_2_NAAM=Arduino UNO R4 Minima"
set "BOARD_3_FQBN=arduino:renesas_uno:unor4wifi"
set "BOARD_3_FLAGS=-DBOARD_VERSION=BOARD_UNO_R4_WIFI"
set "BOARD_3_NAAM=Arduino UNO R4 WiFi"
set "BOARD_4_FQBN=arduino:zephyr:unoq"
set "BOARD_4_FLAGS=-DBOARD_VERSION=BOARD_UNO_Q"
set "BOARD_4_NAAM=Arduino UNO Q"
set "BOARD_5_FQBN=rp2040:rp2040:cytron_maker_uno_rp2040"
set "BOARD_5_FLAGS=-DBOARD_VERSION=BOARD_RP2040_CYTRON_MAKER_UNO"
set "BOARD_5_NAAM=Cytron Maker UNO RP2040"
set "BOARD_6_FQBN=STMicroelectronics:stm32:Nucleo_64:pnum=NUCLEO_F401RE"
set "BOARD_6_FLAGS=-DBOARD_VERSION=BOARD_STM32F4_NUCLEO64_F401RE"
set "BOARD_6_NAAM=STM32F4 Nucleo-F401RE"
set "BOARD_7_FQBN=esp32:esp32:d1_uno32"
set "BOARD_7_FLAGS=-DBOARD_VERSION=BOARD_ESP32_D1_UNO_R32"
set "BOARD_7_NAAM=TTGO D1 R32 / WeMos D1 R32"
set "BOARD_8_FQBN=esp32:esp32:esp32s3"
set "BOARD_8_FLAGS=-DBOARD_VERSION=BOARD_ESP32S3_ARDI32"
set "BOARD_8_NAAM=SB Components Ardi-32"
set "BOARD_9_FQBN=esp32:esp32:esp32s3"
set "BOARD_9_FLAGS=-DBOARD_VERSION=BOARD_ESP32S3_DEV"
set "BOARD_9_NAAM=Paradisetronic ESP32-S3 UNO"
set "AANTAL_BOARD_PROFIELEN=9"

set /A TESTS=0
set /A OK=0
set /A FAIL=0

for /L %%N in (1,1,!AANTAL_BOARD_PROFIELEN!) do (
    call set "BOARD_FQBN=%%BOARD_%%N_FQBN%%"
    call set "BOARD_FLAGS=%%BOARD_%%N_FLAGS%%"
    call set "BOARD_NAAM=%%BOARD_%%N_NAAM%%"
    echo.
    echo ============================================================
    echo !BOARD_NAAM! - !BOARD_FQBN!
    echo ============================================================
    for %%I in (%INCLUDE%) do (
        if exist "%%~I" (
            for /R "%%~I" %%F in (*.ino) do call :COMPILE_EXAMPLE "%%F"
        ) else (
            echo [FOUT] INCLUDE bestaat niet: %%~I
            set /A FAIL+=1
        )
    )
)

echo.
echo ============================================================
echo TESTRESULTAAT
echo ============================================================
echo Totaal getest : !TESTS!
echo OK             : !OK!
echo FOUT           : !FAIL!
echo ============================================================
popd
echo === TEST VOLLEDIG AFGEROND ===
if /I not "%~2"=="--no-pause" pause
if !FAIL!==0 (exit /b 0) else exit /b 1

:COMPILE_EXAMPLE
set "EXAMPLE_FILE=%~1"
set "EXAMPLE_DIR=%~dp1"
set "EXAMPLE_DIR=!EXAMPLE_DIR:~0,-1!"
set "EXAMPLE_NAAM=%~nx1"
set "EXTRA_FLAGS=-DGROEIACADEMIE_IGNORE_USER_CONFIG !BOARD_FLAGS! -DGEDEELDE_BUS_PROTOTYPE"
if /I "!EXAMPLE_NAAM!"=="Default_CharacterScreen.ino" set "EXTRA_FLAGS=!EXTRA_FLAGS! -DSCREEN_OUTPUT_CONFIG=SCREEN_TYPE_CHARACTER"
if /I "!EXAMPLE_NAAM!"=="Default_PixelScreen.ino" set "EXTRA_FLAGS=!EXTRA_FLAGS! -DSCREEN_OUTPUT_CONFIG=SCREEN_TYPE_PIXELS"
if /I "!EXAMPLE_NAAM!"=="Default_CharacterScreen_PixelScreen.ino" set "EXTRA_FLAGS=!EXTRA_FLAGS! -DSCREEN_OUTPUT_CONFIG=6"
if /I "!EXAMPLE_NAAM!"=="InputkanalenDIGITAL.ino" set "EXTRA_FLAGS=!EXTRA_FLAGS! -DINPUT_KANAAL_CONFIG=INPUT_TYPE_DIGITAL -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4"
if /I "!EXAMPLE_NAAM!"=="InputkanalenPCF8574.ino" set "EXTRA_FLAGS=!EXTRA_FLAGS! -DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4"
if /I "!EXAMPLE_NAAM!"=="InputkanalenHX1838.ino" set "EXTRA_FLAGS=!EXTRA_FLAGS! -DINPUT_KANAAL_CONFIG=INPUT_TYPE_HX1838 -DHX1838_BRON_CODES=HX1838_BRON_CODES_DEFINE -DHX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE=1 -DHX1838_TOETSENINDELING=HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN"
set /A TESTS+=1
echo ------------------------------------------------------------
echo Compileren van: !EXAMPLE_FILE!
"%CLI_PATH%" compile --jobs 1 --fqbn "!BOARD_FQBN!" --build-property "compiler.cpp.extra_flags=!EXTRA_FLAGS!" "!EXAMPLE_DIR!" >"%TEMP%\GA_Compile.txt" 2>&1
set "RESULT=!errorlevel!"
type "%TEMP%\GA_Compile.txt"
if "!RESULT!"=="0" (
    set /A OK+=1
) else (
    echo [FOUT] !BOARD_NAAM! ^| !EXAMPLE_NAAM!
    set /A FAIL+=1
)
del "%TEMP%\GA_Compile.txt" >nul 2>&1
goto :eof
