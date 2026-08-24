@echo off
cls
if exist "%~dp0TestLibraryStatusReport.txt" del /q "%~dp0TestLibraryStatusReport.txt" >nul 2>&1
setlocal enabledelayedexpansion

:: Controleer of het script al via PowerShell met logging draait.
if "%~1"=="--logged" goto :MAIN_SCRIPT

:: Start het script opnieuw op, toon de uitvoer én bewaar het afzonderlijke logbestand.
:: De exitcode van de interne run wordt expliciet teruggegeven aan de aanroeper.
powershell -NoProfile -Command "& { & '%~f0' --logged %* 2>&1 | Tee-Object -FilePath '%~dp0TestLibraryGereleased.txt'; exit $LASTEXITCODE }"
set "SCRIPT_RESULT=%errorlevel%"
exit /b %SCRIPT_RESULT%

:MAIN_SCRIPT

:: Alleen de echte hoofd-run wijzigt de werkmap. De buitenste logging-wrapper laat de werkmap van de aanroeper ongemoeid.
pushd "%~dp0.."

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
    popd
    exit /b 1
)


:: Paden die door TestLibraryNieuw.cmd getest worden dienen we hier op te geven.
:: Bij meerdere uit te sluiten directories geven we deze na elkaar op gescheiden door een spatie
:: bv: set "UITGESLOTEN_OMDAT_ZE_DEEL_UITMAKEN_VAN_NIEUW=\examples\Systeem\Input\ \examples\Systeem\andereSubdirectorie\"
set "UITGESLOTEN_OMDAT_ZE_DEEL_UITMAKEN_VAN_NIEUW=\examples\Systeem\Input\"

:: 0 = zonder DEBUG testen, 1 = met DEBUG testen
set "DEBUG_TEST=0"

:: v1.1.1 teststrategie:
:: - Arduino UNO R4 Minima is het referentieboard voor de volledige gereleasete regressiematrix.
:: - De andere officieel ondersteunde boards krijgen een minimale gerichte regressietest.
:: - De experimentele acceptatieboards krijgen dezelfde minimale test zonder release-impact.
set "BOARDS=arduino:renesas_uno:minima"
set "MINIMALE_OFFICIELE_BOARDS=arduino:avr:uno arduino:renesas_uno:unor4wifi esp32:esp32:d1_uno32"
set ACCEPTATIE_BOARDS="rp2040:rp2040:cytron_maker_uno_rp2040" "STMicroelectronics:stm32:Nucleo_64:pnum=NUCLEO_F401RE" "esp32:esp32:esp32s3"

set /A TESTS=0
set /A OK=0
set /A EXPECTED_MEMORY=0
set /A FAIL=0
set /A LINT_FAIL=0
set /A ACCEPTATIE_TESTS=0
set /A ACCEPTATIE_OK=0
set /A ACCEPTATIE_FAIL=0
set /A ACCEPTATIE_EXPECTED_DEPENDENCY=0

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

findstr /C:"PCF8574" library.properties >nul || (
    echo FOUT: PCF8574 dependency ontbreekt in library.properties.
    set /A FAIL+=1
)
powershell -NoProfile -Command "Select-String -Path 'src\Systeem\Input\Input.cpp' -Pattern 'PrintToScreen\(\x22','Serial\.print\(\x22','Serial\.println\(\x22' | Where-Object { $_.Line -notmatch 'Serial\.(?:print|println)\(\x22\s*\x22\)' } | ForEach-Object { '{0}:{1}' -f $_.LineNumber,$_.Line }" >"%TEMP%\GA_InputHardcoded.txt"
set "HARDCODED_SIZE=0"
for %%A in ("%TEMP%\GA_InputHardcoded.txt") do set "HARDCODED_SIZE=%%~zA"
if !HARDCODED_SIZE! GTR 0 (
    echo FOUT: Hardcoded gebruikerstekst gevonden in Input.cpp.
    type "%TEMP%\GA_InputHardcoded.txt"
    set /A FAIL+=1
)
del "%TEMP%\GA_InputHardcoded.txt" >nul 2>&1
for %%K in (InputConfigureren OpvragenHuidigeToetsAanslag OpvragenHuidigeToetsAanslagen OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag InputResultaat InputResultaten InputKanaal StatusOpvragenToetsAanslagen MAX_AANTAL_SIMULTANE_TOETSAANSLAGEN INPUT_TYPE_DIGITAL INPUT_TYPE_PCF8574) do (
    findstr /B /C:"%%K	" keywords.txt >nul || (
        echo FOUT: keywords.txt mist %%K
        set /A FAIL+=1
    )
)
echo ------------------------------------------------------------

for %%B in (%BOARDS%) do (
    set "BOARD_VERSION_TEST="
    if "%%B"=="arduino:avr:uno"               set "BOARD_VERSION_TEST=BOARD_UNO_R3"
    if "%%B"=="arduino:renesas_uno:minima"    set "BOARD_VERSION_TEST=BOARD_UNO_R4_MINIMA"
    if "%%B"=="arduino:renesas_uno:unor4wifi" set "BOARD_VERSION_TEST=BOARD_UNO_R4_WIFI"
    if "%%B"=="esp32:esp32:d1_uno32"          set "BOARD_VERSION_TEST=BOARD_ESP32_UNO"

    if not defined BOARD_VERSION_TEST (
        echo FOUT: geen BOARD_VERSION gekoppeld aan %%B.
        popd
        exit /b 1
    )

    echo ------------------------------------------------------------
    echo [AUTOMATISCHE BOARDDETECTIE ZONDER -DBOARD_VERSION]
    set /A TESTS+=1
    set "AUTO_BOARD_LOG=%TEMP%\GroeiAcademieAutoBoard_!RANDOM!_!RANDOM!.txt"
    echo Compileren van: examples\Systeem\Input\InputkanalenDIGITAL\InputkanalenDIGITAL.ino
    echo INPUT_TYPE_DIGITAL ^| KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 ^| SCREEN_OUTPUT_CONFIG=SCREEN_TYPE_NONE ^| AUTOMATISCHE BOARDDETECTIE ZONDER -DBOARD_VERSION
    "%CLI_PATH%" compile --jobs 1 --fqbn %%B --build-property "compiler.cpp.extra_flags=-DGROEIACADEMIE_IGNORE_USER_CONFIG -DINPUT_KANAAL_CONFIG=INPUT_TYPE_DIGITAL -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 -DSCREEN_OUTPUT_CONFIG=SCREEN_TYPE_NONE" "examples\Systeem\Input\InputkanalenDIGITAL" >"!AUTO_BOARD_LOG!" 2>&1
    set "AUTO_BOARD_RESULT=!errorlevel!"
    type "!AUTO_BOARD_LOG!"
    if "!AUTO_BOARD_RESULT!"=="0" (
        set "GEHEUGEN_BOARD=%%B"
        set "GEHEUGEN_BESTAND=InputkanalenDIGITAL.ino"
        set "GEHEUGEN_TEST=INPUT_TYPE_DIGITAL | KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 | SCREEN_OUTPUT_CONFIG=SCREEN_TYPE_NONE | AUTOMATISCHE BOARDDETECTIE ZONDER -DBOARD_VERSION"
        set "GEHEUGEN_STATUS=OK"
        call :REGISTREER_GEHEUGENGEBRUIK "!AUTO_BOARD_LOG!"
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
            set "GEHEUGEN_BOARD=%%B"
            set "GEHEUGEN_BESTAND=%%~nxF"
            set "GEHEUGEN_TEST=-"
            if defined ACTIVE_SCREEN_OUTPUT set "GEHEUGEN_TEST=SCREEN_OUTPUT_CONFIG=!ACTIVE_SCREEN_OUTPUT!"
            if defined LOCAL_ADC_BACKEND (
                if "!GEHEUGEN_TEST!"=="-" (
                    set "GEHEUGEN_TEST=ADC_BACKEND=!LOCAL_ADC_BACKEND!"
                ) else (
                    set "GEHEUGEN_TEST=!GEHEUGEN_TEST! | ADC_BACKEND=!LOCAL_ADC_BACKEND!"
                )
            )
            if "!DEBUG_TEST!"=="1" (
                if "!GEHEUGEN_TEST!"=="-" (
                    set "GEHEUGEN_TEST=DEBUG"
                ) else (
                    set "GEHEUGEN_TEST=!GEHEUGEN_TEST! | DEBUG"
                )
            )
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
                    set "GEHEUGEN_STATUS=VERWACHTE_GEHEUGENBEPERKING"
                    call :REGISTREER_GEHEUGENGEBRUIK "!COMPILE_LOG!"
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
                set "GEHEUGEN_STATUS=OK"
                call :REGISTREER_GEHEUGENGEBRUIK "!COMPILE_LOG!"
                set /A OK+=1
            )
            del /Q "!COMPILE_LOG!" >nul 2>&1
        )
    )
        )
)

echo.
echo ============================================================
echo GERELEASETE INPUT-MATRIX OP REFERENTIEBOARD: UNO R4 MINIMA
echo ============================================================
call :TEST_STABIELE_INPUT_VOLLEDIG

echo.
echo ============================================================
echo MINIMALE REGRESSIE OP OVERIGE OFFICIELE BOARDS
echo ============================================================
for %%B in (%MINIMALE_OFFICIELE_BOARDS%) do call :TEST_MINIMAAL_BOARD "%%~B" OFFICIEEL

echo.
echo ============================================================
echo MINIMALE REGRESSIE OP ACCEPTATIEBOARDS
echo ============================================================
for %%B in (%ACCEPTATIE_BOARDS%) do call :TEST_MINIMAAL_BOARD "%%~B" ACCEPTATIE

echo.
echo ============================================================
echo TESTRESULTAAT
echo ============================================================
echo Totaal getest                         : !TESTS!
echo OK bevonden                           : !OK!
echo Verwachte UNO R3-geheugenbeperkingen : !EXPECTED_MEMORY!
echo Onverwacht mislukt                    : !FAIL!
echo Acceptatietests                         : !ACCEPTATIE_TESTS!
echo Acceptatietests OK                      : !ACCEPTATIE_OK!
echo Acceptatietests verwachte dependencybeperking: !ACCEPTATIE_EXPECTED_DEPENDENCY!
echo Acceptatietests FOUT                    : !ACCEPTATIE_FAIL! ^(geen release-impact^)
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


:TEST_STABIELE_INPUT_VOLLEDIG
set "BOARD=arduino:renesas_uno:minima"
set "MODE=OFFICIEEL"
set "BOARD_FLAGS=-DBOARD_VERSION=BOARD_UNO_R4_MINIMA"
set "TEST_NAAM=INPUT_TYPE_NONE"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_NONE"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_DIGITAL | KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_DIGITAL -DKEYPAD_TYPE=KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_DIGITAL | KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_DIGITAL -DKEYPAD_TYPE=KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_DIGITAL | KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_DIGITAL -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_DIGITAL | KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_DIGITAL -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_DIGITAL | KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_DIGITAL -DKEYPAD_TYPE=KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4 | TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4 -DTTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED=TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4 | TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4 -DTTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED=TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | KEYPAD_TYPE_USER_DEFINED_DIRECT"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_USER_DEFINED_DIRECT -DKEYPAD_GENERIEK_AANTAL_PINNEN=4 -DKEYPAD_GENERIEK_PINNEN={PCF8574_PIN_P0,PCF8574_PIN_P1,PCF8574_PIN_P2,PCF8574_PIN_P3} -DKEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED=KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW -DKEYPAD_GENERIEK_KEY_LAYOUT={{_LABEL_OPSCHRIFT_1,LABEL_TOETS_1},{_LABEL_OPSCHRIFT_2,LABEL_TOETS_2},{_LABEL_OPSCHRIFT_3,LABEL_TOETS_3},{_LABEL_OPSCHRIFT_4,LABEL_TOETS_4}}"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | KEYPAD_TYPE_USER_DEFINED_MATRIX"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_USER_DEFINED_MATRIX -DKEYPAD_GENERIEK_AANTAL_RIJEN=3 -DKEYPAD_GENERIEK_AANTAL_KOLOMMEN=3 -DKEYPAD_GENERIEK_RIJ_PINNEN={PCF8574_PIN_P0,PCF8574_PIN_P1,PCF8574_PIN_P2} -DKEYPAD_GENERIEK_KOLOM_PINNEN={PCF8574_PIN_P3,PCF8574_PIN_P4,PCF8574_PIN_P5} -DKEYPAD_GENERIEK_KEY_LAYOUT={{_LABEL_OPSCHRIFT_1,LABEL_TOETS_1},{_LABEL_OPSCHRIFT_2,LABEL_TOETS_2},{_LABEL_OPSCHRIFT_3,LABEL_TOETS_3},{_LABEL_OPSCHRIFT_4,LABEL_TOETS_4},{_LABEL_OPSCHRIFT_5,LABEL_TOETS_5},{_LABEL_OPSCHRIFT_6,LABEL_TOETS_6},{_LABEL_OPSCHRIFT_7,LABEL_TOETS_7},{_LABEL_OPSCHRIFT_8,LABEL_TOETS_8},{_LABEL_OPSCHRIFT_9,LABEL_TOETS_9}}"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_DIGITAL | UITGEBREIDE NON-BLOCKING EVENTS"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_DIGITAL -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 -DINPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID -DINPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS=1000UL"
call :COMPILE_STABIELE_INPUT
set "TEST_NAAM=INPUT_TYPE_PCF8574 | UITGEBREIDE NON-BLOCKING EVENTS"
set "TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4 -DINPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID -DINPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS=1000UL"
call :COMPILE_STABIELE_INPUT
call :COMPILE_STABIELE_INPUT_STIMULUS
goto :eof

:COMPILE_STABIELE_INPUT
set "EXAMPLE_METARGUMENTEN="
if not "!TEST_FLAGS:INPUT_TYPE_NONE=!"=="!TEST_FLAGS!" (
    set "EXAMPLE=examples\Toepassingsgebieden\Stimulus\Scenario1_EnkelTik"
) else if not "!TEST_FLAGS:KEYPAD_TYPE_USER_DEFINED_DIRECT=!"=="!TEST_FLAGS!" (
    set "EXAMPLE=examples\Systeem\Input\InputkanalenPCF8574UserDefinedDirect"
) else if not "!TEST_FLAGS:KEYPAD_TYPE_USER_DEFINED_MATRIX=!"=="!TEST_FLAGS!" (
    set "EXAMPLE=examples\Systeem\Input\InputkanalenPCF8574UserDefinedMatrix"
) else if not "!TEST_FLAGS:INPUT_TYPE_PCF8574=!"=="!TEST_FLAGS!" (
    set "EXAMPLE=examples\Systeem\Input\InputkanalenPCF8574"
    set "EXAMPLE_METARGUMENTEN=examples\Systeem\Input\InputkanalenPCF8574metArgumenten"
) else if not "!TEST_FLAGS:INPUT_TYPE_DIGITAL=!"=="!TEST_FLAGS!" (
    set "EXAMPLE=examples\Systeem\Input\InputkanalenDIGITAL"
    set "EXAMPLE_METARGUMENTEN=examples\Systeem\Input\InputkanalenDIGITALmetArgumenten"
)

for %%X in ("!EXAMPLE!") do set "EXAMPLE_BESTAND=%%~nxX.ino"
echo ------------------------------------------------------------
echo Compileren van: !EXAMPLE!\!EXAMPLE_BESTAND!
echo !TEST_NAAM!
set /A TESTS+=1
"%CLI_PATH%" compile --jobs 1 --fqbn "%BOARD%" --build-property "compiler.cpp.extra_flags=-DGROEIACADEMIE_IGNORE_USER_CONFIG !TEST_FLAGS! !BOARD_FLAGS!" "!EXAMPLE!" >"%TEMP%\GA_ReleasedInputCompile.txt" 2>&1
set "COMPILE_RESULT=!errorlevel!"
type "%TEMP%\GA_ReleasedInputCompile.txt"
if "!COMPILE_RESULT!"=="0" (
    set "GEHEUGEN_BOARD=%BOARD%"
    set "GEHEUGEN_BESTAND=!EXAMPLE_BESTAND!"
    set "GEHEUGEN_TEST=!TEST_NAAM!"
    set "GEHEUGEN_STATUS=OK"
    call :REGISTREER_GEHEUGENGEBRUIK "%TEMP%\GA_ReleasedInputCompile.txt"
    set /A OK+=1
) else (
    echo [FOUT][OFFICIEEL] %BOARD% ^| !TEST_NAAM!
    set /A FAIL+=1
)
del "%TEMP%\GA_ReleasedInputCompile.txt" >nul 2>&1

if defined EXAMPLE_METARGUMENTEN (
    for %%X in ("!EXAMPLE_METARGUMENTEN!") do set "EXAMPLE_METARGUMENTEN_BESTAND=%%~nxX.ino"
    echo ------------------------------------------------------------
    echo Compileren van: !EXAMPLE_METARGUMENTEN!\!EXAMPLE_METARGUMENTEN_BESTAND!
    echo !TEST_NAAM! ^| metArgumenten
    set /A TESTS+=1
    "%CLI_PATH%" compile --jobs 1 --fqbn "%BOARD%" --build-property "compiler.cpp.extra_flags=-DGROEIACADEMIE_IGNORE_USER_CONFIG !TEST_FLAGS! !BOARD_FLAGS!" "!EXAMPLE_METARGUMENTEN!" >"%TEMP%\GA_ReleasedInputCompileMetArgumenten.txt" 2>&1
    set "COMPILE_RESULT=!errorlevel!"
    type "%TEMP%\GA_ReleasedInputCompileMetArgumenten.txt"
    if "!COMPILE_RESULT!"=="0" (
        set "GEHEUGEN_BOARD=%BOARD%"
        set "GEHEUGEN_BESTAND=!EXAMPLE_METARGUMENTEN_BESTAND!"
        set "GEHEUGEN_TEST=!TEST_NAAM! | metArgumenten"
        set "GEHEUGEN_STATUS=OK"
        call :REGISTREER_GEHEUGENGEBRUIK "%TEMP%\GA_ReleasedInputCompileMetArgumenten.txt"
        set /A OK+=1
    ) else (
        echo [FOUT][OFFICIEEL] %BOARD% ^| !TEST_NAAM! ^| metArgumenten
        set /A FAIL+=1
    )
    del "%TEMP%\GA_ReleasedInputCompileMetArgumenten.txt" >nul 2>&1
)
goto :eof

:COMPILE_STABIELE_INPUT_STIMULUS
for %%E in (examples\Systeem\Input\Input_Test_Scenario1_EnkelTik examples\Systeem\Input\Input_Test_Scenario2_Simultaan examples\Systeem\Input\Input_Test_Scenario3_Ineenstortend examples\Systeem\Input\Input_Test_Scenario4_Cocktail examples\Systeem\Input\Input_Test_Tik_Enkele_Samen_Instortend_Cocktail) do (
    echo ------------------------------------------------------------
    echo Compileren van: %%E\%%~nxE.ino
    echo INPUT + STIMULUS ^| %%~nxE
    set /A TESTS+=1
    "%CLI_PATH%" compile --jobs 1 --fqbn "%BOARD%" --build-property "compiler.cpp.extra_flags=-DGROEIACADEMIE_IGNORE_USER_CONFIG !BOARD_FLAGS!" "%%E" >"%TEMP%\GA_ReleasedInputStimulus.txt" 2>&1
    set "COMPILE_RESULT=!errorlevel!"
    type "%TEMP%\GA_ReleasedInputStimulus.txt"
    if "!COMPILE_RESULT!"=="0" (
        set "GEHEUGEN_BOARD=%BOARD%"
        set "GEHEUGEN_BESTAND=%%~nxE.ino"
        set "GEHEUGEN_TEST=INPUT + STIMULUS | %%~nxE"
        set "GEHEUGEN_STATUS=OK"
        call :REGISTREER_GEHEUGENGEBRUIK "%TEMP%\GA_ReleasedInputStimulus.txt"
        set /A OK+=1
    ) else (
        echo [FOUT][OFFICIEEL] %BOARD% ^| %%~nxE
        set /A FAIL+=1
    )
    del "%TEMP%\GA_ReleasedInputStimulus.txt" >nul 2>&1
)
goto :eof

:TEST_MINIMAAL_BOARD
set "MIN_BOARD=%~1"
set "MIN_MODE=%~2"
set "MIN_BOARD_FLAGS="
if "%MIN_BOARD%"=="arduino:avr:uno" set "MIN_BOARD_FLAGS=-DBOARD_VERSION=BOARD_UNO_R3"
if "%MIN_BOARD%"=="arduino:renesas_uno:unor4wifi" set "MIN_BOARD_FLAGS=-DBOARD_VERSION=BOARD_UNO_R4_WIFI"
if "%MIN_BOARD%"=="esp32:esp32:d1_uno32" set "MIN_BOARD_FLAGS=-DBOARD_VERSION=BOARD_ESP32_UNO"
if "%MIN_BOARD%"=="rp2040:rp2040:cytron_maker_uno_rp2040" set "MIN_BOARD_FLAGS=-DBOARD_VERSION=BOARD_CYTRON_MAKER_UNO_RP2040"
if "%MIN_BOARD%"=="STMicroelectronics:stm32:Nucleo_64:pnum=NUCLEO_F401RE" set "MIN_BOARD_FLAGS=-DBOARD_VERSION=BOARD_NUCLEO_F401RE"
if "%MIN_BOARD%"=="esp32:esp32:esp32s3" set "MIN_BOARD_FLAGS=-DBOARD_VERSION=BOARD_ARDI32"
if not defined MIN_BOARD_FLAGS (
    echo [FOUT] Geen BOARD_VERSION gekoppeld aan %MIN_BOARD%.
    if "%MIN_MODE%"=="OFFICIEEL" (set /A FAIL+=1) else set /A ACCEPTATIE_FAIL+=1
    goto :eof
)

set "MIN_VERWACHT_GEHEUGEN=0"
set "MIN_VERWACHT_DEPENDENCY=0"
if "%MIN_BOARD%"=="esp32:esp32:esp32s3" (
    echo [N.V.T.][%MIN_MODE%] %MIN_BOARD% ^| AUTOMATISCHE BOARDDETECTIE ^| generieke ESP32S3 Dev Module identificeert een fysieke Ardi32 niet automatisch
) else (
    set "MIN_TEST_NAAM=AUTOMATISCHE BOARDDETECTIE | DIGITAL"
    set "MIN_TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_DIGITAL -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 -DSCREEN_OUTPUT_CONFIG=SCREEN_TYPE_NONE"
    set "MIN_EXAMPLE=examples\Systeem\Input\InputkanalenDIGITAL"
    set "MIN_GEBRUIK_BOARD_FLAGS=0"
    call :COMPILE_MINIMAAL
)

set "MIN_TEST_NAAM=CharacterScreen"
set "MIN_TEST_FLAGS=-DSCREEN_OUTPUT_CONFIG=2"
set "MIN_EXAMPLE=examples\Systeem\Screen\Default_CharacterScreen"
set "MIN_GEBRUIK_BOARD_FLAGS=1"
call :COMPILE_MINIMAAL

set "MIN_TEST_NAAM=PixelScreen"
set "MIN_TEST_FLAGS=-DSCREEN_OUTPUT_CONFIG=4"
set "MIN_EXAMPLE=examples\Systeem\Screen\Default_PixelScreen"
set "MIN_GEBRUIK_BOARD_FLAGS=1"
set "MIN_VERWACHT_DEPENDENCY=0"
if "%MIN_BOARD%"=="STMicroelectronics:stm32:Nucleo_64:pnum=NUCLEO_F401RE" set "MIN_VERWACHT_DEPENDENCY=1"
call :COMPILE_MINIMAAL
set "MIN_VERWACHT_DEPENDENCY=0"

set "MIN_TEST_NAAM=Stimulus basis | SCREEN_TYPE_NONE"
set "MIN_TEST_FLAGS=-DSCREEN_OUTPUT_CONFIG=0"
set "MIN_EXAMPLE=examples\Toepassingsgebieden\Stimulus\Scenario1_EnkelTik"
set "MIN_GEBRUIK_BOARD_FLAGS=1"
call :COMPILE_MINIMAAL

set "MIN_TEST_NAAM=Input DIGITAL | KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4"
set "MIN_TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_DIGITAL -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4"
set "MIN_EXAMPLE=examples\Systeem\Input\InputkanalenDIGITAL"
set "MIN_GEBRUIK_BOARD_FLAGS=1"
call :COMPILE_MINIMAAL

set "MIN_TEST_NAAM=Input DIGITAL | KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 | metArgumenten"
set "MIN_TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_DIGITAL -DKEYPAD_TYPE=KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4"
set "MIN_EXAMPLE=examples\Systeem\Input\InputkanalenDIGITALmetArgumenten"
set "MIN_GEBRUIK_BOARD_FLAGS=1"
call :COMPILE_MINIMAAL

set "MIN_TEST_NAAM=Input PCF8574 | KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4"
set "MIN_TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4"
set "MIN_EXAMPLE=examples\Systeem\Input\InputkanalenPCF8574"
set "MIN_GEBRUIK_BOARD_FLAGS=1"
call :COMPILE_MINIMAAL

set "MIN_TEST_NAAM=Input PCF8574 | KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4 | metArgumenten"
set "MIN_TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4"
set "MIN_EXAMPLE=examples\Systeem\Input\InputkanalenPCF8574metArgumenten"
set "MIN_GEBRUIK_BOARD_FLAGS=1"
call :COMPILE_MINIMAAL

set "MIN_TEST_NAAM=Input PCF8574 | KEYPAD_TYPE_USER_DEFINED_DIRECT"
set "MIN_TEST_FLAGS=-DINPUT_KANAAL_CONFIG=INPUT_TYPE_PCF8574 -DKEYPAD_TYPE=KEYPAD_TYPE_USER_DEFINED_DIRECT -DKEYPAD_GENERIEK_AANTAL_PINNEN=4 -DKEYPAD_GENERIEK_PINNEN={PCF8574_PIN_P0,PCF8574_PIN_P1,PCF8574_PIN_P2,PCF8574_PIN_P3} -DKEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED=KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW -DKEYPAD_GENERIEK_KEY_LAYOUT={{_LABEL_OPSCHRIFT_1,LABEL_TOETS_1},{_LABEL_OPSCHRIFT_2,LABEL_TOETS_2},{_LABEL_OPSCHRIFT_3,LABEL_TOETS_3},{_LABEL_OPSCHRIFT_4,LABEL_TOETS_4}}"
set "MIN_EXAMPLE=examples\Systeem\Input\InputkanalenPCF8574UserDefinedDirect"
set "MIN_GEBRUIK_BOARD_FLAGS=1"
call :COMPILE_MINIMAAL

set "MIN_TEST_NAAM=Input + Stimulus representatief"
set "MIN_TEST_FLAGS="
set "MIN_EXAMPLE=examples\Systeem\Input\Input_Test_Scenario1_EnkelTik"
set "MIN_GEBRUIK_BOARD_FLAGS=1"
call :COMPILE_MINIMAAL

if "%MIN_BOARD%"=="arduino:avr:uno" (
    set "MIN_VERWACHT_GEHEUGEN=1"
    set "MIN_TEST_NAAM=UNO R3 geheugengrens | gecombineerde Stimulus | SCREEN_OUTPUT_CONFIG=7"
    set "MIN_TEST_FLAGS=-DSCREEN_OUTPUT_CONFIG=7"
    set "MIN_EXAMPLE=examples\Toepassingsgebieden\Stimulus\Tik_Enkele_Samen_Instortend_Cocktail"
    set "MIN_GEBRUIK_BOARD_FLAGS=1"
    call :COMPILE_MINIMAAL
    set "MIN_VERWACHT_GEHEUGEN=0"
)
goto :eof

:COMPILE_MINIMAAL
set "MIN_EXTRA=!MIN_TEST_FLAGS!"
if "!MIN_GEBRUIK_BOARD_FLAGS!"=="1" set "MIN_EXTRA=!MIN_EXTRA! !MIN_BOARD_FLAGS!"
for %%X in ("!MIN_EXAMPLE!") do set "MIN_EXAMPLE_BESTAND=%%~nxX.ino"
echo ------------------------------------------------------------
echo Compileren van: !MIN_EXAMPLE!\!MIN_EXAMPLE_BESTAND!
echo !MIN_TEST_NAAM!
if "%MIN_MODE%"=="OFFICIEEL" (set /A TESTS+=1) else set /A ACCEPTATIE_TESTS+=1
"%CLI_PATH%" compile --jobs 1 --fqbn "%MIN_BOARD%" --build-property "compiler.cpp.extra_flags=-DGROEIACADEMIE_IGNORE_USER_CONFIG !MIN_EXTRA!" "%MIN_EXAMPLE%" >"%TEMP%\GA_ReleasedMinimal.txt" 2>&1
set "MIN_RESULT=!errorlevel!"
type "%TEMP%\GA_ReleasedMinimal.txt"
if "!MIN_RESULT!"=="0" (
    set "GEHEUGEN_BOARD=%MIN_BOARD%"
    set "GEHEUGEN_BESTAND=!MIN_EXAMPLE_BESTAND!"
    set "GEHEUGEN_TEST=!MIN_TEST_NAAM!"
    set "GEHEUGEN_STATUS=OK"
    call :REGISTREER_GEHEUGENGEBRUIK "%TEMP%\GA_ReleasedMinimal.txt"
    if "%MIN_MODE%"=="OFFICIEEL" (set /A OK+=1) else set /A ACCEPTATIE_OK+=1
) else (
    set "MIN_MEMORY_MATCH=0"
    if "!MIN_VERWACHT_GEHEUGEN!"=="1" (
        findstr /C:"text section exceeds available space in board" "%TEMP%\GA_ReleasedMinimal.txt" >nul
        if not errorlevel 1 set "MIN_MEMORY_MATCH=1"
    )
    set "MIN_DEPENDENCY_MATCH=0"
    if "!MIN_VERWACHT_DEPENDENCY!"=="1" (
        findstr /C:"wiring_private.h: No such file or directory" "%TEMP%\GA_ReleasedMinimal.txt" >nul
        if not errorlevel 1 set "MIN_DEPENDENCY_MATCH=1"
    )
    if "!MIN_MEMORY_MATCH!"=="1" (
        set "GEHEUGEN_BOARD=%MIN_BOARD%"
        set "GEHEUGEN_BESTAND=!MIN_EXAMPLE_BESTAND!"
        set "GEHEUGEN_TEST=!MIN_TEST_NAAM!"
        set "GEHEUGEN_STATUS=VERWACHTE_GEHEUGENBEPERKING"
        call :REGISTREER_GEHEUGENGEBRUIK "%TEMP%\GA_ReleasedMinimal.txt"
        echo [VERWACHTE GEHEUGENBEPERKING][%MIN_MODE%] %MIN_BOARD% ^| !MIN_TEST_NAAM!
        set /A EXPECTED_MEMORY+=1
    ) else if "!MIN_DEPENDENCY_MATCH!"=="1" (
        echo [VERWACHTE DEPENDENCYBEPERKING][%MIN_MODE%] %MIN_BOARD% ^| !MIN_TEST_NAAM! ^| Adafruit ST77xx verwacht wiring_private.h
        if "%MIN_MODE%"=="ACCEPTATIE" set /A ACCEPTATIE_EXPECTED_DEPENDENCY+=1
    ) else (
        echo [FOUT][%MIN_MODE%] %MIN_BOARD% ^| !MIN_TEST_NAAM!
        if "%MIN_MODE%"=="OFFICIEEL" (set /A FAIL+=1) else set /A ACCEPTATIE_FAIL+=1
    )
)
del "%TEMP%\GA_ReleasedMinimal.txt" >nul 2>&1
goto :eof

:REGISTREER_GEHEUGENGEBRUIK
set "PROGRAM_USED="
set "PROGRAM_PERCENT="
set "PROGRAM_MAX="
set "RAM_USED="
set "RAM_PERCENT="
set "RAM_MAX="
for /f "tokens=3,5,12" %%A in ('findstr /B /C:"Sketch uses " "%~1"') do (
  set "PROGRAM_USED=%%A"
  set "PROGRAM_PERCENT=%%B"
  set "PROGRAM_MAX=%%C"
)
for /f "tokens=4,6,18" %%A in ('findstr /B /C:"Global variables use " "%~1"') do (
  set "RAM_USED=%%A"
  set "RAM_PERCENT=%%B"
  set "RAM_MAX=%%C"
)
set "PROGRAMMA_WEERGAVE=NIET_GEVONDEN"
set "RAM_WEERGAVE=NIET_GEVONDEN"
if defined PROGRAM_USED if defined PROGRAM_MAX set "PROGRAMMA_WEERGAVE=!PROGRAM_USED!/!PROGRAM_MAX! !PROGRAM_PERCENT!"
if defined RAM_USED if defined RAM_MAX set "RAM_WEERGAVE=!RAM_USED!/!RAM_MAX! !RAM_PERCENT!"
echo [GEHEUGEN] Board=!GEHEUGEN_BOARD! ^| Bestand=!GEHEUGEN_BESTAND! ^| Test=!GEHEUGEN_TEST! ^| Programma=!PROGRAMMA_WEERGAVE! ^| RAM=!RAM_WEERGAVE! ^| Status=!GEHEUGEN_STATUS!
goto :eof
