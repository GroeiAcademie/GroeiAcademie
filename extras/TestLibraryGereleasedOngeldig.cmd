@echo off
cls
if exist "%~dp0TestLibraryStatusReport.txt" del /q "%~dp0TestLibraryStatusReport.txt" >nul 2>&1
:: Controleer of het script al via PowerShell met logging draait.
if "%~1"=="--logged" goto :MAIN_SCRIPT

:: Start het script opnieuw op, toon de uitvoer én bewaar het afzonderlijke logbestand.
:: De exitcode van de interne run wordt expliciet teruggegeven aan de aanroeper.
if exist "%~dp0TestLibraryGereleasedOngeldig.txt" del /q "%~dp0TestLibraryGereleasedOngeldig.txt" >nul 2>&1
powershell -NoProfile -Command "& { & '%~f0' --logged %* 2>&1 | Tee-Object -FilePath '%~dp0TestLibraryGereleasedOngeldig.txt'; exit $LASTEXITCODE }"
set "SCRIPT_RESULT=%errorlevel%"
exit /b %SCRIPT_RESULT%

:MAIN_SCRIPT
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
        if /I not "%~2"=="--no-pause" pause
        exit /b 1
)



goto :MAIN


:EXPECT_FAIL
set /A TESTS+=1
set "EXTRA_FLAGS=%~2"
set "VERWACHTE_FOUT=%~3"
echo !EXTRA_FLAGS! | findstr /C:"BOARD_VERSION" >nul
if errorlevel 1 set "EXTRA_FLAGS=!EXTRA_FLAGS! -DBOARD_VERSION=BOARD_UNO_R3"
"%CLI_PATH%" compile --jobs 1 --fqbn %BOARD% --build-property "compiler.cpp.extra_flags=-DGROEIACADEMIE_IGNORE_USER_CONFIG !EXTRA_FLAGS!" "%EXAMPLE%" >"%TEMP%\GA_ReleasedInvalid.txt" 2>&1
set "COMPILE_RESULT=!errorlevel!"
if "!COMPILE_RESULT!"=="0" (
    echo [FOUT] %~1 compileerde onverwacht.
    set /A FAIL+=1
) else (
    findstr /C:"!VERWACHTE_FOUT!" "%TEMP%\GA_ReleasedInvalid.txt" >nul
    if errorlevel 1 (
        echo [FOUT] %~1 faalde om een andere reden dan verwacht.
        type "%TEMP%\GA_ReleasedInvalid.txt"
        set /A FAIL+=1
    ) else (
        echo [OK] %~1 correct geweigerd: !VERWACHTE_FOUT!
        set /A OK+=1
    )
)
del "%TEMP%\GA_ReleasedInvalid.txt" >nul 2>&1
goto :eof

:MAIN
set "BOARD=arduino:avr:uno"
set /A TESTS=0
set /A OK=0
set /A FAIL=0

echo.
echo ============================================================
echo GERELEASEDE ONGELDIGE CONFIGURATIES: SCREEN
echo ============================================================
set "EXAMPLE=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail"

call :EXPECT_FAIL "ONBEKENDE SCREEN-BIT" "-DSCREEN_OUTPUT_CONFIG=8" "SCREEN_OUTPUT_CONFIG bevat een onbekende uitvoerbit."
call :EXPECT_FAIL "ONGELDIG PIXELSCHERM" "-DSCREEN_OUTPUT_CONFIG=SCREEN_TYPE_PIXELS -DACTIEF_PIXEL_SCREEN=99" "ACTIEF_PIXEL_SCREEN bevat geen ondersteunde pixelschermresolutie."
call :EXPECT_FAIL "ONGELDIGE PIXEL ROTATIE" "-DSCREEN_OUTPUT_CONFIG=SCREEN_TYPE_PIXELS -DPIXEL_SCREEN_ROTATION=4" "PIXEL_SCREEN_ROTATION moet 0, 1, 2 of 3 zijn."
call :EXPECT_FAIL "ONGELDIGE CHARACTER I2C-MODUS" "-DSCREEN_OUTPUT_CONFIG=SCREEN_TYPE_CHARACTER -DCHARACTERSCREEN_I2C_ADRES_MODUS=3" "CHARACTERSCREEN_I2C_ADRES_MODUS moet 0, 1 of 2 zijn."

echo.
echo ============================================================
echo GERELEASEDE ONGELDIGE CONFIGURATIES: SYSTEMCONFIG
echo ============================================================
call :EXPECT_FAIL "ONGELDIGE BOARD_VERSION" "-DBOARD_VERSION=99" "Selecteer een geldige BOARD_VERSION."
call :EXPECT_FAIL "MEERDERE TALEN GESELECTEERD" "-DLANGUAGE_NL -DLANGUAGE_EN" "Selecteer slechts een taal in UserConfig.h of SystemConfig.h."
call :EXPECT_FAIL "ONGELDIG AANTAL_SENSOREN_AANWEZIG" "-DAANTAL_SENSOREN_AANWEZIG=3" "AANTAL_SENSOREN_AANWEZIG moet 2 of 4 zijn."
call :EXPECT_FAIL "ONGELDIGE ADC_BACKEND" "-DADC_BACKEND=99" "Selecteer een geldige ADC_BACKEND."

echo.
echo Totaal !TESTS! - OK !OK! - FOUT !FAIL!
popd
echo === TEST VOLLEDIG AFGEROND ===
if /I not "%~2"=="--no-pause" (
    <nul set /p =[EINDE] Testen voltooid. Druk op een toets om af te sluiten... & pause >nul
    echo.
)
if !FAIL!==0 (exit /b 0) else exit /b 1
