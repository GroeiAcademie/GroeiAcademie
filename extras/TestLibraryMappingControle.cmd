@echo off
cls
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

call :ControleerOpschriften "%HX1838_INO%" "HX1838_TOETSENINDELING_3x4" "1 2 3 4 5 6 7 8 9 STER 0 HEKJE"
call :ControleerOpschriften "%HX1838_INO%" "HX1838_TOETSENINDELING_REMOTE_17_TOETSEN" "1 2 3 4 5 6 7 8 9 STER 0 HEKJE UP DOWN LEFT RIGHT OK"
call :ControleerOpschriften "%HX1838_INO%" "HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3" "CH_MINUS CH CH_PLUS PREV NEXT PLAY MINUS PLUS EQ 0 100_PLUS 200_PLUS 1 2 3 4 5 6 7 8 9"

echo ------------------------------------------------------------
echo Gecontroleerd: !GECONTROLEERD! combinaties van bestand en type.
if !FOUT! EQU 0 (
    echo Alle mappingTestMenu[]-tabellen zijn volledig voor hun gedocumenteerde types.
) else (
    echo !FOUT! ontbrekende opschriften gevonden. Zie [FOUT]-regels hierboven.
)
echo ============================================================
popd
pause
exit /b !FOUT!

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
