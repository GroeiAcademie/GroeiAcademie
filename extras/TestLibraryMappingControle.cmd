@echo off
if /I not "%~1"=="--no-pause" cls
setlocal enabledelayedexpansion
pushd "%~dp0.."

:: Bepaal welk pad bestaat
echo ============================================================
echo MAPPING-VOLLEDIGHEIDSCONTROLE
echo ============================================================
echo Dit script controleert of mappingTestMenu[] in de drie
echo Inputkanalen-voorbeelden elk opschrift bevat dat het KEY_LAYOUT[]
echo van het bijhorende KEYPAD_TYPE in Input.cpp kan opleveren.
echo.
echo LET OP: de referentietabel hieronder is handmatig opgebouwd uit
echo Input.cpp op %date%. Bij een nieuw KEYPAD_TYPE of een nieuwe
echo HX1838_TOETSENINDELING moet deze tabel ook zelf worden uitgebreid,
echo dit script leidt niets automatisch af uit Input.cpp zelf.
echo ------------------------------------------------------------

set /A FOUT=0
set /A GECONTROLEERD=0

:: ============================================================
:: InputkanalenDIGITAL.ino
:: ============================================================
set "DIGITAL_INO=examples\Systeem\Input\InputkanalenDIGITAL\InputkanalenDIGITAL.ino"

call :ControleerOpschriften "%DIGITAL_INO%" "KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4" "S1 S2 S3 S4"
call :ControleerOpschriften "%DIGITAL_INO%" "KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2" "S1 S2 S3 S4"
call :ControleerOpschriften "%DIGITAL_INO%" "KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4" "1 2 3 4"
call :ControleerOpschriften "%DIGITAL_INO%" "KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1" "1 2 3 4"
call :ControleerOpschriften "%DIGITAL_INO%" "KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4" "1 2 3 4"

:: ============================================================
:: InputkanalenPCF8574.ino
:: ============================================================
set "PCF8574_INO=examples\Systeem\Input\InputkanalenPCF8574\InputkanalenPCF8574.ino"

call :ControleerOpschriften "%PCF8574_INO%" "KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4" "S1 S2 S3 S4"
call :ControleerOpschriften "%PCF8574_INO%" "KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2" "S1 S2 S3 S4"
call :ControleerOpschriften "%PCF8574_INO%" "KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4" "S1 S2 S3 S4 S5 S6 S7 S8 S9 S10 S11 S12 S13 S14 S15 S16"
call :ControleerOpschriften "%PCF8574_INO%" "KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4" "1 2 3 4 5 6 7 8"
call :ControleerOpschriften "%PCF8574_INO%" "KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4" "S1 S2 S3 S4 S5 S6 S7 S8"
call :ControleerOpschriften "%PCF8574_INO%" "KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4" "1 2 3 4"
call :ControleerOpschriften "%PCF8574_INO%" "KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1" "1 2 3 4"
call :ControleerOpschriften "%PCF8574_INO%" "KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4" "1 2 3 4"
call :ControleerOpschriften "%PCF8574_INO%" "KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4" "1 2 3 4 5 6 7 8"
call :ControleerOpschriften "%PCF8574_INO%" "KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3" "1 2 3 4 5 6 7 8 9 STER 0 HEKJE"
call :ControleerOpschriften "%PCF8574_INO%" "KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4" "1 2 3 A 4 5 6 B 7 8 9 C STER 0 HEKJE D"
call :ControleerOpschriften "%PCF8574_INO%" "KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4" "1 2 3 4"

:: ============================================================
:: InputkanalenHX1838.ino
:: ============================================================
set "HX1838_INO=examples\Systeem\Input\InputkanalenHX1838\InputkanalenHX1838.ino"

call :ControleerOpschriften "%HX1838_INO%" "HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN" "UP DOWN OK LEFT RIGHT 1 2 3 4 5 6 7 8 9 STER 0 HEKJE"
call :ControleerOpschriften "%HX1838_INO%" "HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN" "1 2 3 4 5 6 7 8 9 STER 0 HEKJE UP DOWN OK LEFT RIGHT"
call :ControleerOpschriften "%HX1838_INO%" "HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3" "CH_MINUS CH CH_PLUS PREV NEXT PLAY MINUS PLUS EQ 0 100_PLUS 200_PLUS 1 2 3 4 5 6 7 8 9"

:: Extra HX1838-controle: behoud de brede controle hierboven, maar controleer
:: aanvullend de juiste #if/#elif-tak en de volgorde binnen mappingTestMenu[].
call :ControleerOpschriftenInTak "%HX1838_INO%" "HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN" "UP DOWN OK LEFT RIGHT 1 2 3 4 5 6 7 8 9 STER 0 HEKJE"
call :ControleerOpschriftenInTak "%HX1838_INO%" "HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN" "1 2 3 4 5 6 7 8 9 STER 0 HEKJE UP DOWN OK LEFT RIGHT"
call :ControleerOpschriftenInTak "%HX1838_INO%" "HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3" "CH_MINUS CH CH_PLUS PREV NEXT PLAY MINUS PLUS EQ 0 100_PLUS 200_PLUS 1 2 3 4 5 6 7 8 9"

echo ------------------------------------------------------------
echo Gecontroleerd: !GECONTROLEERD! combinaties van bestand en type.
if !FOUT! EQU 0 (
    echo Alle mappingTestMenu[]-tabellen zijn volledig voor hun gedocumenteerde types.
) else (
    echo !FOUT! ontbrekende opschriften gevonden. Zie [FOUT]-regels hierboven.
)
echo ============================================================
set "MAPPING_RESULT=!FOUT!"
popd
if /I not "%~1"=="--no-pause" pause
exit /b !MAPPING_RESULT!


:: ============================================================
:: :ControleerOpschriftenInTak <ino-pad> <HX1838_TOETSENINDELING> <verwachte volgorde>
:: ============================================================
:ControleerOpschriftenInTak
set "INO_PAD=%~1"
set "TYPE_NAAM=%~2"
set "VERWACHTE_OPSCHRIFTEN=%~3"
set /A GECONTROLEERD+=1

if not exist "%INO_PAD%" (
    echo [FOUT] Bestand niet gevonden: %INO_PAD%
    set /A FOUT+=1
    goto :eof
)

set "TAK_START="
for /f "tokens=1 delims=:" %%L in ('findstr /N /C:"HX1838_TOETSENINDELING == %TYPE_NAAM%" "%INO_PAD%"') do (
    if not defined TAK_START set "TAK_START=%%L"
)

if not defined TAK_START (
    echo [FOUT] %INO_PAD% ^| %TYPE_NAAM% ^| #if/#elif-tak niet gevonden
    set /A FOUT+=1
    goto :eof
)

set "TAK_EIND="
for /f "tokens=1 delims=:" %%L in ('findstr /N /R /C:"^[ ]*#elif HX1838_TOETSENINDELING" /C:"^[ ]*#else" /C:"^[ ]*#endif" "%INO_PAD%"') do (
    if %%L GTR !TAK_START! if not defined TAK_EIND set "TAK_EIND=%%L"
)
if not defined TAK_EIND set "TAK_EIND=2147483647"

set /A VORIGE_REGEL=TAK_START
for %%O in (%VERWACHTE_OPSCHRIFTEN%) do (
    set "GEVONDEN_REGEL="
    for /f "tokens=1 delims=:" %%L in ('findstr /N /C:"_LABEL_OPSCHRIFT_%%O," "%INO_PAD%"') do (
        if %%L GTR !TAK_START! if %%L LSS !TAK_EIND! if not defined GEVONDEN_REGEL set "GEVONDEN_REGEL=%%L"
    )

    if not defined GEVONDEN_REGEL (
        echo [FOUT] %INO_PAD% ^| %TYPE_NAAM% ^| opschrift %%O ontbreekt in de eigen #if/#elif-tak
        set /A FOUT+=1
    ) else (
        if !GEVONDEN_REGEL! LEQ !VORIGE_REGEL! (
            echo [FOUT] %INO_PAD% ^| %TYPE_NAAM% ^| opschrift %%O staat niet in de verwachte volgorde
            set /A FOUT+=1
        )
        set /A VORIGE_REGEL=GEVONDEN_REGEL
    )
)
goto :eof

:: ============================================================
:: :ControleerOpschriften <ino-pad> <keypadtype-naam> <lijst met opschriften gescheiden door spatie>
:: ============================================================
:ControleerOpschriften
set "INO_PAD=%~1"
set "TYPE_NAAM=%~2"
set "VERWACHTE_OPSCHRIFTEN=%~3"
set /A GECONTROLEERD+=1

if not exist "%INO_PAD%" (
    echo [FOUT] Bestand niet gevonden: %INO_PAD%
    set /A FOUT+=1
    goto :eof
)

for %%O in (%VERWACHTE_OPSCHRIFTEN%) do (
    findstr /C:"_LABEL_OPSCHRIFT_%%O," "%INO_PAD%" >nul
    if errorlevel 1 (
        echo [FOUT] %INO_PAD% ^| %TYPE_NAAM% ^| opschrift %%O ontbreekt in mappingTestMenu[]
        set /A FOUT+=1
    )
)
goto :eof
