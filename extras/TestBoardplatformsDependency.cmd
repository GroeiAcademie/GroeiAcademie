@echo off
cls
setlocal enabledelayedexpansion
pushd "%~dp0.."

:: Bepaal welk pad bestaat
:: Zoek arduino-cli via PATH; anders via extras\LokalePaden.cmd (lokaal, niet gedeeld -
:: zie extras\LokalePaden_template.cmd om aan te maken). Geen paden van specifieke
:: personen/machines in dit gedeelde script.
where arduino-cli >nul 2>&1
if not errorlevel 1 (
    for /f "delims=" %%P in ('where arduino-cli') do set "CLI_PATH=%%~dpnP"
) else if exist "extras\LokalePaden.cmd" (
    call "extras\LokalePaden.cmd"
) else (
    echo arduino-cli niet gevonden via PATH, en extras\LokalePaden.cmd bestaat niet.
    echo Kopieer extras\LokalePaden_template.cmd naar extras\LokalePaden.cmd en vul
    echo daarin het pad naar arduino-cli in, of voeg arduino-cli toe aan PATH.
        pause
        popd
        exit /b 1
)



echo ============================================================
echo CONTROLE VEREISTE BOARDPLATFORMS
echo ============================================================
echo Deze lijst komt rechtstreeks uit docs/HARDWARE_SUPPORT.md en dient
echo hier manueel mee bijgewerkt te worden bij nieuwe ondersteunde boards.
echo ------------------------------------------------------------

set /P AUTO_INSTALL="Ontbrekende boardplatforms automatisch installeren via arduino-cli? (J/N): "

set "INSTALLED_CORES_LOG=%TEMP%\GroeiAcademieInstalledCores_%RANDOM%.txt"
"%CLI_PATH%" core list >"%INSTALLED_CORES_LOG%" 2>&1

set /A MISSING_COUNT=0
set /A INSTALLED_NU=0

set "BOARD_1_NAME=Arduino UNO R3"
set "BOARD_1_FQBN=arduino:avr:uno"
set "BOARD_1_PLATFORM=arduino:avr"
set "BOARD_1_PACKAGE=Arduino AVR Boards by Arduino"

set "BOARD_2_NAME=Arduino UNO R4 Minima"
set "BOARD_2_FQBN=arduino:renesas_uno:minima"
set "BOARD_2_PLATFORM=arduino:renesas_uno"
set "BOARD_2_PACKAGE=Arduino UNO R4 Boards by Arduino"

set "BOARD_3_NAME=Arduino UNO R4 WiFi"
set "BOARD_3_FQBN=arduino:renesas_uno:unor4wifi"
set "BOARD_3_PLATFORM=arduino:renesas_uno"
set "BOARD_3_PACKAGE=Arduino UNO R4 Boards by Arduino"

set "BOARD_4_NAME=Arduino UNO Q"
set "BOARD_4_FQBN=arduino:zephyr:unoq"
set "BOARD_4_PLATFORM=arduino:zephyr"
set "BOARD_4_PACKAGE=Arduino Zephyr Boards by Arduino"

set "BOARD_5_NAME=ESP32-WROOM-32U"
set "BOARD_5_FQBN=esp32:esp32:d1_uno32"
set "BOARD_5_PLATFORM=esp32:esp32"
set "BOARD_5_PACKAGE=esp32 by Espressif Systems"

set "BOARD_6_NAME=Cytron Maker UNNO RP2040"
set "BOARD_6_FQBN=rp2040:rp2040:cytron_maker_uno_rp2040"
set "BOARD_6_PLATFORM=rp2040:rp2040"
set "BOARD_6_PACKAGE=Raspberry Pi Pico/RP2040/RP2350 by Earle F. Philhower, III"

set "BOARD_7_NAME=STM32F4 Nucleo-F401RE"
set "BOARD_7_FQBN=STMicroelectronics:stm32:Nucleo_64:pnum=NUCLEO_F401RE"
set "BOARD_7_PLATFORM=STMicroelectronics:stm32"
set "BOARD_7_PACKAGE=STM32 MCU based boards by STMicroelectronics"

set "BOARD_8_NAME=ESP32-S3-WROOM-1"
set "BOARD_8_FQBN=esp32:esp32:esp32s3"
set "BOARD_8_PLATFORM=esp32:esp32"
set "BOARD_8_PACKAGE=esp32 by Espressif Systems"

for /L %%N in (1,1,8) do (
    set "BOARD_NAME=!BOARD_%%N_NAME!"
    set "BOARD_FQBN=!BOARD_%%N_FQBN!"
    set "BOARD_PLATFORM=!BOARD_%%N_PLATFORM!"
    set "BOARD_PACKAGE=!BOARD_%%N_PACKAGE!"
    set "PACKAGE_KEY=!BOARD_PACKAGE: =_!"
    set "PACKAGE_KEY=!PACKAGE_KEY:/=_!"
    set "PACKAGE_KEY=!PACKAGE_KEY:,=_!"
    set "PACKAGE_KEY=!PACKAGE_KEY:.=_!"

    findstr /I /B /C:"!BOARD_PLATFORM! " "!INSTALLED_CORES_LOG!" >nul
    if errorlevel 1 (
        echo ONTBREEKT : !BOARD_NAME!, FQBN: !BOARD_FQBN!, Boards Manager pakket: !BOARD_PACKAGE!
        if not defined GETELD_!PACKAGE_KEY! (
            set /A MISSING_COUNT+=1
            set "GETELD_!PACKAGE_KEY!=1"
        )
        if /I "!AUTO_INSTALL!"=="J" (
            set "THIRDPARTY_URL="
            if /I "!BOARD_PLATFORM!"=="rp2040:rp2040" set "THIRDPARTY_URL=https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json"
            if /I "!BOARD_PLATFORM!"=="STMicroelectronics:stm32" set "THIRDPARTY_URL=https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json"
            if defined THIRDPARTY_URL (
                echo    Package-index toevoegen: !THIRDPARTY_URL!
                "%CLI_PATH%" config add board_manager.additional_urls "!THIRDPARTY_URL!"
                "%CLI_PATH%" core update-index
            )
            echo    Installeren: !BOARD_PLATFORM! ^(!BOARD_PACKAGE!^)
            "%CLI_PATH%" core install "!BOARD_PLATFORM!"
            if not errorlevel 1 (
                set /A INSTALLED_NU+=1
                "%CLI_PATH%" core list >"!INSTALLED_CORES_LOG!" 2>&1
            )
        )
    ) else (
        echo AANWEZIG  : !BOARD_NAME!, FQBN: !BOARD_FQBN!, Boards Manager pakket: !BOARD_PACKAGE!
    )
)

del "%INSTALLED_CORES_LOG%" >nul 2>&1

echo ============================================================
if !MISSING_COUNT! EQU 0 (
    echo Alle vereiste boardplatforms zijn geinstalleerd.
    popd
    pause
    exit /b 0
)

echo Er ontbraken !MISSING_COUNT! van de vereiste boardplatforms.
if /I "!AUTO_INSTALL!"=="J" (
    echo Waarvan !INSTALLED_NU! nu automatisch geinstalleerd via arduino-cli.
    echo Voer dit script opnieuw uit om te bevestigen dat alles nu aanwezig is.
) else (
    echo Niets automatisch geinstalleerd.
    echo Installeer manueel via Boards Manager met de hierboven vermelde pakketnaam.
)
popd
pause
exit /b 1
