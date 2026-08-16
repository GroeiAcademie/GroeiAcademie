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
echo CONTROLE VEREISTE LIBRARIES
echo ============================================================
echo Deze lijst komt rechtstreeks uit library.properties en dient
echo hier manueel mee bijgewerkt te worden bij nieuwe afhankelijkheden.
echo ------------------------------------------------------------

set /P AUTO_INSTALL="Ontbrekende libraries automatisch installeren via arduino-cli? (J/N): "

set "INSTALLED_LIBS_LOG=%TEMP%\GroeiAcademieInstalledLibs_%RANDOM%.txt"
"%CLI_PATH%" lib list >"%INSTALLED_LIBS_LOG%" 2>&1

set /A MISSING_COUNT=0
set /A INSTALLED_NU=0

for %%L in ("LiquidCrystal_I2C" "Adafruit GFX Library" "Adafruit ST7735 and ST7789 Library" "Adafruit ADS1X15" "PCF8574" "IRremote") do (
    findstr /I /C:"%%~L" "!INSTALLED_LIBS_LOG!" >nul
    if errorlevel 1 (
        echo ONTBREEKT : %%~L
        set /A MISSING_COUNT+=1
        if /I "!AUTO_INSTALL!"=="J" (
            echo    Installeren: %%~L
            "%CLI_PATH%" lib install "%%~L"
            if not errorlevel 1 set /A INSTALLED_NU+=1
        )
    ) else (
        echo AANWEZIG  : %%~L
    )
)

del "%INSTALLED_LIBS_LOG%" >nul 2>&1

echo ============================================================
if !MISSING_COUNT! EQU 0 (
    echo Alle vereiste libraries zijn geinstalleerd.
    echo LET OP: PCF8574 vereist minimaal versie 0.4.0, arduino-cli lib list
    echo toont de geinstalleerde versie, controleer dit zelf indien nodig.
    popd
    pause
    exit /b 0
)

echo Er ontbraken !MISSING_COUNT! van de vereiste libraries.
if /I "!AUTO_INSTALL!"=="J" (
    echo Waarvan !INSTALLED_NU! nu automatisch geinstalleerd via arduino-cli.
    echo Voer dit script opnieuw uit om te bevestigen dat alles nu aanwezig is.
) else (
    echo Niets automatisch geinstalleerd.
    echo Installeer manueel via: arduino-cli lib install "naam van de library"
)
popd
pause
exit /b 1
