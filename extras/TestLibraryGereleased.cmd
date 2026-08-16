@echo off
cls
if exist "TestLibraryStatusReport.txt" del /q "TestLibraryStatusReport.txt" >nul 2>&1
setlocal enabledelayedexpansion

:: Werk altijd vanuit de hoofdmap van de library, ook wanneer dit script vanuit extras wordt gestart.
pushd "%~dp0.."

:: Controleer of het script al via PowerShell met logging draait.
if "%~1"=="--logged" goto :MAIN_SCRIPT

:: Start het script opnieuw op, toon de uitvoer én bewaar het afzonderlijke logbestand.
:: De exitcode van de interne run wordt expliciet teruggegeven aan de aanroeper.
powershell -NoProfile -Command "& { & '%~f0' --logged %* 2>&1 | Tee-Object -FilePath '%~dp0TestLibraryGereleased.txt'; exit $LASTEXITCODE }"
set "SCRIPT_RESULT=%errorlevel%"
exit /b %SCRIPT_RESULT%

:MAIN_SCRIPT

:: Zoek arduino-cli en arduino-lint via PATH; anders via extras\LokalePaden.cmd (lokaal, niet
:: gedeeld - zie extras\LokalePaden_template.cmd om aan te maken). Geen paden van specifieke
:: personen/machines in dit gedeelde script.
where arduino-cli >nul 2>&1
if not errorlevel 1 (
    for /f "delims=" %%P in ('where arduino-cli') do set "CLI_PATH=%%~dpnP"
    where arduino-lint >nul 2>&1
    if not errorlevel 1 (
        for /f "delims=" %%Q in ('where arduino-lint') do set "LINT_PATH=%%~dpnQ"
    ) else (
        for %%R in ("%CLI_PATH%") do set "LINT_PATH=%%~dpRarduino-lint"
    )
) else if exist "extras\LokalePaden.cmd" (
    call "extras\LokalePaden.cmd"
    if not defined LINT_PATH (
        for %%R in ("%CLI_PATH%") do set "LINT_PATH=%%~dpRarduino-lint"
    )
) else (
    echo arduino-cli niet gevonden via PATH, en extras\LokalePaden.cmd bestaat niet.
    echo Kopieer extras\LokalePaden_template.cmd naar extras\LokalePaden.cmd en vul
    echo daarin het pad naar arduino-cli in, of voeg arduino-cli toe aan PATH.
    if /I not "%~2"=="--no-pause" pause
    exit /b 1
)


:: Paden die door TestLibraryNieuw.cmd getest worden dienen we hier op te geven.
:: Bij meerdere uit te sluiten directories geven we deze na elkaar op gescheiden door een spatie
:: bv: set "UITGESLOTEN_OMDAT_ZE_DEEL_UITMAKEN_VAN_NIEUW=\examples\Systeem\Input\ \examples\Systeem\andereSubdirectorie\"
set "UITGESLOTEN_OMDAT_ZE_DEEL_UITMAKEN_VAN_NIEUW=\examples\Systeem\Input\"

:: 0 = zonder DEBUG testen, 1 = met DEBUG testen
set "DEBUG_TEST=0"

:: Lijst met de vier boards die getest moeten worden
set "BOARDS=arduino:avr:uno arduino:renesas_uno:minima arduino:renesas_uno:unor4wifi esp32:esp32:d1_uno32"

set /A TESTS=0
set /A OK=0
set /A EXPECTED_MEMORY=0
set /A FAIL=0
set /A LINT_FAIL=0

echo ------------------------------------------------------------
"%LINT_PATH%" --library-manager update
if errorlevel 1 (
    set /A LINT_FAIL=1
    echo Arduino LINT is mislukt.
    <nul set /p =[PAUZE] Druk op een toets om door te gaan... & pause >nul
    echo.
) else (
    echo Arduino LINT is geslaagd.
)
echo ------------------------------------------------------------
echo [STATISCHE CONFIGURATIECONTROLES]
set "STATIC_CHECK_LOG=%TEMP%\GroeiAcademieStaticCheck_!RANDOM!_!RANDOM!.txt"
findstr /S /N /R /C:"^[ ]*#define[ ]*PIN_TOETS_[1-4][ ]*D[0-9][0-9]*" examples\*.ino >"!STATIC_CHECK_LOG!"
if not errorlevel 1 (
    echo FOUT: rechtstreekse D-pinmapping voor PIN_TOETS gevonden:
    type "!STATIC_CHECK_LOG!"
    set /A FAIL+=1
)
set "ADC_NATIVE_GEVONDEN=0"
set "ADC_ADS1115_GEVONDEN=0"
for /R "examples" %%F in (*.ino) do (
    set "AANWEZIG_IN_UITSLUITING=0"
    set "HUIDIG_PAD=%%~dpF"
    for %%U in (%UITGESLOTEN_OMDAT_ZE_DEEL_UITMAKEN_VAN_NIEUW%) do (
        if not "!HUIDIG_PAD:%%U=!"=="!HUIDIG_PAD!" set "AANWEZIG_IN_UITSLUITING=1"
    )
    if "!AANWEZIG_IN_UITSLUITING!"=="0" (
        if /I "%%~nxF"=="ADC_Validatie_Native.ino" set "ADC_NATIVE_GEVONDEN=1"
        if /I "%%~nxF"=="ADC_Validatie_ADS1115.ino" set "ADC_ADS1115_GEVONDEN=1"
    )
)
if "!ADC_NATIVE_GEVONDEN!"=="0" (
    echo FOUT: ADC_Validatie_Native.ino nergens gevonden onder examples.
    set /A FAIL+=1
)
if "!ADC_ADS1115_GEVONDEN!"=="0" (
    echo FOUT: ADC_Validatie_ADS1115.ino nergens gevonden onder examples.
    set /A FAIL+=1
)
del "!STATIC_CHECK_LOG!" >nul 2>&1
echo ------------------------------------------------------------

for %%B in (%BOARDS%) do (
    set "BOARD_VERSION_TEST="
    if "%%B"=="arduino:avr:uno"               set "BOARD_VERSION_TEST=BOARD_UNO_R3"
    if "%%B"=="arduino:renesas_uno:minima"    set "BOARD_VERSION_TEST=BOARD_UNO_R4_MINIMA"
    if "%%B"=="arduino:renesas_uno:unor4wifi" set "BOARD_VERSION_TEST=BOARD_UNO_R4_WIFI"
    if "%%B"=="esp32:esp32:d1_uno32"          set "BOARD_VERSION_TEST=BOARD_ESP32_UNO"

    if not defined BOARD_VERSION_TEST (
        echo FOUT: geen BOARD_VERSION gekoppeld aan %%B.
        exit /b 1
    )

    echo ------------------------------------------------------------
    echo [AUTOMATISCHE BOARDDETECTIE ZONDER -DBOARD_VERSION]
    set /A TESTS+=1
    set "AUTO_BOARD_LOG=%TEMP%\GroeiAcademieAutoBoard_!RANDOM!_!RANDOM!.txt"
    "%CLI_PATH%" compile --jobs 1 --fqbn %%B --build-property "compiler.cpp.extra_flags=-DGROEIACADEMIE_IGNORE_USER_CONFIG -DINPUT_KANAAL_CONFIG=INPUT_TYPE_DIGITAL -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 -DSCREEN_OUTPUT_CONFIG=SCREEN_TYPE_NONE" "examples\Systeem\Input\InputkanalenDIGITAL" >"!AUTO_BOARD_LOG!" 2>&1
    set "AUTO_BOARD_RESULT=!errorlevel!"
    type "!AUTO_BOARD_LOG!"
    if "!AUTO_BOARD_RESULT!"=="0" (
        set /A OK+=1
    ) else (
        set /A FAIL+=1
        echo FOUT: automatische boarddetectie mislukt voor %%B.
    )
    del /Q "!AUTO_BOARD_LOG!" >nul 2>&1

    set "BASE_BUILD_FLAGS=-DBOARD_VERSION=!BOARD_VERSION_TEST!"

    echo ============================================================
    echo Starten met compileren voor board: %%B
    echo BOARD_VERSION=!BOARD_VERSION_TEST!
    echo ============================================================

    echo [ALLE EXAMPLES RECURSIEF TESTEN, BEHALVE UITGESLOTEN_OMDAT_ZE_DEEL_UITMAKEN_VAN_NIEUW]
    for /R "examples" %%F in (*.ino) do (
        set "AANWEZIG_IN_UITSLUITING=0"
        set "HUIDIG_PAD=%%~dpF"
        for %%U in (%UITGESLOTEN_OMDAT_ZE_DEEL_UITMAKEN_VAN_NIEUW%) do (
            if not "!HUIDIG_PAD:%%U=!"=="!HUIDIG_PAD!" set "AANWEZIG_IN_UITSLUITING=1"
        )
        if "!AANWEZIG_IN_UITSLUITING!"=="0" (
        set "EXAMPLE_FILE=%%F"
        set "EXAMPLE_DIR=%%~dpF"
        set "EXAMPLE_DIR=!EXAMPLE_DIR:~0,-1!"
        set "EXAMPLE_NAME=%%~nF"
        set "LOCAL_SCREEN_OUTPUT="
        set "LOCAL_ADC_BACKEND="
        set "IS_STIMULUS=0"

        REM Detectie SCREEN_OUTPUT_CONFIG: ondersteunt zowel het oude patroon
        REM #define SCREEN_OUTPUT_CONFIG ... rechtstreeks in de .ino, of het
        REM nieuwe #if/#error-validatiepatroon, dat geen letterlijke #define meer
        REM bevat maar SCREEN_TYPE_CHARACTER/SCREEN_TYPE_PIXELS wel nog vermeldt.
        set "HAS_CHARACTER=0"
        set "HAS_PIXELS=0"
        findstr /R /C:"^[ ]*#define[ ]*SCREEN_OUTPUT_CONFIG.*SCREEN_TYPE_CHARACTER" "%%F" >nul && set "HAS_CHARACTER=1"
        findstr /R /C:"SCREEN_TYPE_CHARACTER" "%%F" >nul && set "HAS_CHARACTER=1"
        findstr /R /C:"^[ ]*#define[ ]*SCREEN_OUTPUT_CONFIG.*SCREEN_TYPE_PIXELS" "%%F" >nul && set "HAS_PIXELS=1"
        findstr /R /C:"SCREEN_TYPE_PIXELS" "%%F" >nul && set "HAS_PIXELS=1"
        if "!HAS_CHARACTER!"=="1" if "!HAS_PIXELS!"=="1" set "LOCAL_SCREEN_OUTPUT=6"
        if "!HAS_CHARACTER!"=="1" if "!HAS_PIXELS!"=="0" set "LOCAL_SCREEN_OUTPUT=2"
        if "!HAS_CHARACTER!"=="0" if "!HAS_PIXELS!"=="1" set "LOCAL_SCREEN_OUTPUT=4"

        findstr /R /C:"^[ ]*#define[ ]*ADC_BACKEND[ ]*ADC_BACKEND_NATIVE" "%%F" >nul && set "LOCAL_ADC_BACKEND=ADC_BACKEND_NATIVE"
        findstr /R /C:"^[ ]*#define[ ]*ADC_BACKEND[ ]*ADC_BACKEND_ADS1115" "%%F" >nul && set "LOCAL_ADC_BACKEND=ADC_BACKEND_ADS1115"
        findstr /I /C:"Stimulus.h" "%%F" >nul && set "IS_STIMULUS=1"
        if "!IS_STIMULUS!"=="1" (set "SCREEN_TESTS=0 1 2 3 4 5 6 7") else set "SCREEN_TESTS=LOCAL"
        for %%C in (!SCREEN_TESTS!) do (
            set "BUILD_FLAGS=!BASE_BUILD_FLAGS!"
            set "ACTIVE_SCREEN_OUTPUT=!LOCAL_SCREEN_OUTPUT!"
            if not "%%C"=="LOCAL" set "ACTIVE_SCREEN_OUTPUT=%%C"
            if defined ACTIVE_SCREEN_OUTPUT set "BUILD_FLAGS=!BUILD_FLAGS! -DSCREEN_OUTPUT_CONFIG=!ACTIVE_SCREEN_OUTPUT!"
            if defined LOCAL_ADC_BACKEND set "BUILD_FLAGS=!BUILD_FLAGS! -DADC_BACKEND=!LOCAL_ADC_BACKEND!"
            if "!DEBUG_TEST!"=="1" set "BUILD_FLAGS=!BUILD_FLAGS! -DDEBUG"
            echo ------------------------------------------------------------
            echo Compileren van: %%F
            if defined ACTIVE_SCREEN_OUTPUT echo SCREEN_OUTPUT_CONFIG=!ACTIVE_SCREEN_OUTPUT!
            if defined LOCAL_ADC_BACKEND echo ADC_BACKEND=!LOCAL_ADC_BACKEND!
            if "!DEBUG_TEST!"=="1" echo DEBUG actief
            set /A TESTS+=1
            set "COMPILE_LOG=%TEMP%\GroeiAcademieCompile_!RANDOM!_!RANDOM!.txt"
            "%CLI_PATH%" compile --jobs 1 --fqbn %%B --build-property "compiler.cpp.extra_flags=-DGROEIACADEMIE_IGNORE_USER_CONFIG !BUILD_FLAGS!" "!EXAMPLE_DIR!" >"!COMPILE_LOG!" 2>&1
            set "COMPILE_RESULT=!errorlevel!"
            type "!COMPILE_LOG!"
            if not "!COMPILE_RESULT!"=="0" (
                set "EXPECTED_MEMORY_LIMIT=0"
                if "%%B"=="arduino:avr:uno" if /I "!EXAMPLE_NAME!"=="Tik_Enkele_Samen_Instortend_Cocktail" if defined ACTIVE_SCREEN_OUTPUT (
                    if !ACTIVE_SCREEN_OUTPUT! GEQ 4 (
                        findstr /C:"text section exceeds available space in board" "!COMPILE_LOG!" >nul
                        if not errorlevel 1 set "EXPECTED_MEMORY_LIMIT=1"
                    )
                )
                if "!EXPECTED_MEMORY_LIMIT!"=="1" (
                    set /A EXPECTED_MEMORY+=1
                    echo VERWACHTE GEHEUGENBEPERKING UNO R3: %%F
                ) else (
                    set /A FAIL+=1
                    echo.
                    echo ============================================================
                    echo COMPILATIEFOUT
                    echo Board: %%B
                    echo Voorbeeld: %%F
                    echo ============================================================
                    <nul set /p =[PAUZE] Druk op een toets om door te gaan... & pause >nul
                    echo.
                )
            ) else (
                set /A OK+=1
            )
            del /Q "!COMPILE_LOG!" >nul 2>&1
        )
    )
        )
)

echo.
echo ============================================================
echo TESTRESULTAAT
echo ============================================================
echo Totaal getest                         : !TESTS!
echo OK bevonden                           : !OK!
echo Verwachte UNO R3-geheugenbeperkingen : !EXPECTED_MEMORY!
echo Onverwacht mislukt                    : !FAIL!
if !LINT_FAIL!==0 (
    echo Arduino LINT                         : GESLAAGD
) else (
    echo Arduino LINT                         : MISLUKT
)
echo ============================================================

set /A TOTAL_FAIL=FAIL+LINT_FAIL
if !TOTAL_FAIL!==0 (
    echo Alle onverwachte controles zijn OK bevonden.
    if !EXPECTED_MEMORY! GTR 0 echo !EXPECTED_MEMORY! gekende UNO R3-geheugenbeperkingen zijn afzonderlijk geregistreerd.
) else (
    echo !TESTS! compilaties uitgevoerd, waarvan !OK! OK, !EXPECTED_MEMORY! verwacht beperkt en !FAIL! onverwacht mislukt.
)

echo ============================================================
popd
echo === TEST VOLLEDIG AFGEROND ===
if /I not "%~2"=="--no-pause" (
    <nul set /p =[EINDE] Testen voltooid. Druk op een toets om af te sluiten... & pause >nul
    echo.
)
exit /b !TOTAL_FAIL!