@echo off
cls
setlocal EnableExtensions EnableDelayedExpansion

set "REPORT=%~dp0TestLibraryStatusReport.txt"
set "MARKER==== TEST VOLLEDIG AFGEROND ==="
set "LOG1=%~dp0TestLibraryGereleased.txt"
set "LOG2=%~dp0TestLibraryGereleasedOngeldig.txt"
set "LOG3=%~dp0TestLibraryNieuw.txt"
set "LOG4=%~dp0TestLibraryNieuwOngeldig.txt"
set "MAPPING_SCRIPT=%~dp0TestLibraryMappingControle.cmd"
set "MAPPING_LOG=%TEMP%\GroeiAcademieMappingControle_%RANDOM%_%RANDOM%.txt"
set "RESULTAAT_MAPPING=NIET BEOORDEELD"

if exist "%REPORT%" del /q "%REPORT%" >nul 2>&1

set /a GELDIG=0
set /a ONTBREEKT=0
set /a ONVOLLEDIG=0

call :Controleer "%LOG1%" STATUS1
call :Controleer "%LOG2%" STATUS2
call :Controleer "%LOG3%" STATUS3
call :Controleer "%LOG4%" STATUS4

set "RESULTAAT1=NIET BEOORDEELD"
set "RESULTAAT2=NIET BEOORDEELD"
set "RESULTAAT3_OFFICIEEL=NIET BEOORDEELD"
set "RESULTAAT3_ACCEPTATIE=NIET BEOORDEELD"
set "RESULTAAT3_STATISCH=NIET BEOORDEELD"
set "RESULTAAT4=NIET BEOORDEELD"
set "RESULTAAT_GEHEUGEN1=NIET BEOORDEELD"
set "RESULTAAT_GEHEUGEN3=NIET BEOORDEELD"
set "GEHEUGEN1_AANTAL=0"
set "GEHEUGEN3_AANTAL=0"
set "GEHEUGEN1_FOUTEN=0"
set "GEHEUGEN3_FOUTEN=0"

set "SAMENVATTING1="
set "SAMENVATTING1_LINT="
set "SAMENVATTING2="
set "SAMENVATTING3_OFFICIEEL="
set "SAMENVATTING3_ACCEPTATIE="
set "SAMENVATTING3_STATISCH="
set "SAMENVATTING4="

set "LIB_VERSION=ONBEKEND"
set "RELEASE_DATUM=ONBEKEND"
set "GERELEASED_TOTAAL=ONBEKEND"
set "GERELEASED_OK=ONBEKEND"
set "GERELEASED_EXPECTED_MEMORY=ONBEKEND"
set "GERELEASED_UNEXPECTED_FAIL=ONBEKEND"
set "NIEUW_OFFICIEEL_TOTAAL=ONBEKEND"
set "NIEUW_OFFICIEEL_OK=ONBEKEND"
set "NIEUW_OFFICIEEL_FAIL=ONBEKEND"
set "RELEASE_TOTAAL=ONBEKEND"
set "RELEASE_OK=ONBEKEND"
set "RELEASE_EXPECTED_MEMORY=ONBEKEND"
set "RELEASE_UNEXPECTED_FAIL=ONBEKEND"
set "RELEASE_LINT=ONBEKEND"

if exist "%~dp0..\library.properties" (
  for /f "usebackq tokens=1,* delims==" %%A in (`findstr /B /C:"version=" "%~dp0..\library.properties"`) do set "LIB_VERSION=%%B"
)
for /f "usebackq delims=" %%D in (`powershell -NoProfile -Command "Get-Date -Format 'dd-MM-yyyy'"`) do set "RELEASE_DATUM=%%D"

set /a RELEASE_FOUT=0

if exist "%MAPPING_SCRIPT%" (
  call "%MAPPING_SCRIPT%" --no-pause >"%MAPPING_LOG%" 2>&1
  if errorlevel 1 (
    set "RESULTAAT_MAPPING=MISLUKT"
    set /a RELEASE_FOUT+=1
  ) else (
    set "RESULTAAT_MAPPING=GESLAAGD"
  )
) else (
  set "RESULTAAT_MAPPING=ONTBREEKT"
  >"%MAPPING_LOG%" echo TestLibraryMappingControle.cmd ontbreekt.
  set /a RELEASE_FOUT+=1
)

if "!STATUS1!"=="VOLLEDIG AFGEROND" call :BeoordeelGereleased
if "!STATUS2!"=="VOLLEDIG AFGEROND" call :BeoordeelOngeldig "%LOG2%" RESULTAAT2 SAMENVATTING2
if "!STATUS3!"=="VOLLEDIG AFGEROND" call :BeoordeelNieuw
if "!STATUS4!"=="VOLLEDIG AFGEROND" call :BeoordeelOngeldig "%LOG4%" RESULTAAT4 SAMENVATTING4

if "!STATUS1!"=="VOLLEDIG AFGEROND" call :BeoordeelGeheugen "%LOG1%" RESULTAAT_GEHEUGEN1 GEHEUGEN1_AANTAL GEHEUGEN1_FOUTEN
if "!STATUS3!"=="VOLLEDIG AFGEROND" call :BeoordeelGeheugen "%LOG3%" RESULTAAT_GEHEUGEN3 GEHEUGEN3_AANTAL GEHEUGEN3_FOUTEN
call :BerekenReleaseTotalen

if !GELDIG! NEQ 4 set /a RELEASE_FOUT+=1

> "%REPORT%" (
  echo ============================================================
  echo GROEIACADEMIE FRAMEWORK - OFFICIEEL TESTRAPPORT
  echo ============================================================
  echo Datum en tijd rapport: %DATE% %TIME%
  echo.
  echo ============================================================
  echo TOTAALRESULTAAT OFFICIELE RELEASEVALIDATIE
  echo ============================================================
  if !RELEASE_FOUT! EQU 0 (
    echo EINDSTATUS: RELEASEVALIDATIE GESLAAGD
  ) else (
    echo EINDSTATUS: RELEASEVALIDATIE MISLUKT
  )
  echo.
  echo Alle testcycli volledig : !GELDIG! / 4
  echo Ontbrekend               : !ONTBREEKT!
  echo Onvolledig / afgebroken  : !ONVOLLEDIG!
  echo.
  echo TestLibraryGereleased.cmd
  echo   Testcyclus              : !STATUS1!
  echo   Release-resultaat       : !RESULTAAT1!
  if defined SAMENVATTING1 echo   !SAMENVATTING1!
  if defined SAMENVATTING1_LINT echo   !SAMENVATTING1_LINT!
  echo   Geheugengegevens        : !RESULTAAT_GEHEUGEN1! ^(!GEHEUGEN1_AANTAL! records, !GEHEUGEN1_FOUTEN! ongeldig^)
  echo.
  echo TestLibraryGereleasedOngeldig.cmd
  echo   Testcyclus              : !STATUS2!
  echo   Release-resultaat       : !RESULTAAT2!
  if defined SAMENVATTING2 echo   !SAMENVATTING2!
  echo.
  echo TestLibraryNieuw.cmd
  echo   Testcyclus              : !STATUS3!
  echo   Officiele tests         : !RESULTAAT3_OFFICIEEL!
  if defined SAMENVATTING3_OFFICIEEL echo   !SAMENVATTING3_OFFICIEEL!
  echo   Acceptatietests         : !RESULTAAT3_ACCEPTATIE! - GEEN RELEASE-IMPACT
  if defined SAMENVATTING3_ACCEPTATIE echo   !SAMENVATTING3_ACCEPTATIE!
  echo   Statische controles     : !RESULTAAT3_STATISCH!
  if defined SAMENVATTING3_STATISCH echo   !SAMENVATTING3_STATISCH!
  echo   Geheugengegevens        : !RESULTAAT_GEHEUGEN3! ^(!GEHEUGEN3_AANTAL! records, !GEHEUGEN3_FOUTEN! ongeldig^)
  echo.
  echo TestLibraryNieuwOngeldig.cmd
  echo   Testcyclus              : !STATUS4!
  echo   Release-resultaat       : !RESULTAAT4!
  if defined SAMENVATTING4 echo   !SAMENVATTING4!
  echo.
  echo TestLibraryMappingControle.cmd
  echo   Verplichte mappingcontrole : !RESULTAAT_MAPPING!
  echo.
  echo ============================================================
  echo RELEASECRITERIA
  echo ============================================================
  echo TestLibraryGereleased              : !RESULTAAT1!
  echo TestLibraryGereleasedOngeldig      : !RESULTAAT2!
  echo TestLibraryNieuw OFFICIEEL         : !RESULTAAT3_OFFICIEEL!
  echo TestLibraryNieuw STATISCH          : !RESULTAAT3_STATISCH!
  echo TestLibraryNieuw ACCEPTATIE        : !RESULTAAT3_ACCEPTATIE! - GEEN RELEASE-IMPACT
  echo TestLibraryNieuwOngeldig           : !RESULTAAT4!
  echo TestLibraryMappingControle         : !RESULTAAT_MAPPING!
  echo Geheugen TestLibraryGereleased      : !RESULTAAT_GEHEUGEN1! ^(!GEHEUGEN1_AANTAL! records, !GEHEUGEN1_FOUTEN! ongeldig^)
  echo Geheugen TestLibraryNieuw           : !RESULTAAT_GEHEUGEN3! ^(!GEHEUGEN3_AANTAL! records, !GEHEUGEN3_FOUTEN! ongeldig^)
  echo Alle vier testcycli volledig       : !GELDIG! / 4
  echo.
)

call :SchrijfReleaseUpdate >> "%REPORT%"
call :SchrijfGeheugentabellen >> "%REPORT%"
call :SchrijfMappingResultaat >> "%REPORT%"
if !RELEASE_FOUT! NEQ 0 call :SchrijfFoutdetails >> "%REPORT%"

>> "%REPORT%" (
  echo.
  echo ============================================================
  echo VOLLEDIGE TESTLOGS
  echo ============================================================
  echo.
)

call :VoegToe "%LOG1%" "TestLibraryGereleased.cmd"
call :VoegToe "%LOG2%" "TestLibraryGereleasedOngeldig.cmd"
call :VoegToe "%LOG3%" "TestLibraryNieuw.cmd"
call :VoegToe "%LOG4%" "TestLibraryNieuwOngeldig.cmd"

echo.

if !GELDIG! EQU 4 if !RELEASE_FOUT! EQU 0 (
  call :SchrijfReleaseUpdate
  del /q "%LOG1%" "%LOG2%" "%LOG3%" "%LOG4%" >nul 2>&1
  echo.
  echo TestLibraryStatusReport.txt is aangemaakt.
  echo De releasevalidatie is geslaagd.
  echo De vier individuele logbestanden zijn verwijderd.
) else (
  echo ============================================================
  echo LEESBAAR STATUSRAPPORT - RELEASEVALIDATIE NIET GESLAAGD
  echo ============================================================
  powershell -NoProfile -Command "$l=Get-Content -LiteralPath '%REPORT%'; $stop=($l ^| Select-String -SimpleMatch 'VOLLEDIGE TESTLOGS' ^| Select-Object -First 1).LineNumber; if($stop){$l[0..([Math]::Max(0,$stop-2))]}else{$l}"
  echo.
  echo OPNIEUW UIT TE VOEREN:
  set "HERUITVOER1=0"
  set "HERUITVOER2=0"
  set "HERUITVOER3=0"
  set "HERUITVOER4=0"
  if not "!STATUS1!"=="VOLLEDIG AFGEROND" set "HERUITVOER1=1"
  if not "!STATUS2!"=="VOLLEDIG AFGEROND" set "HERUITVOER2=1"
  if not "!STATUS3!"=="VOLLEDIG AFGEROND" set "HERUITVOER3=1"
  if not "!STATUS4!"=="VOLLEDIG AFGEROND" set "HERUITVOER4=1"
  if "!STATUS1!"=="VOLLEDIG AFGEROND" if "!RESULTAAT1!"=="MISLUKT" set "HERUITVOER1=1"
  if "!STATUS1!"=="VOLLEDIG AFGEROND" if not "!RESULTAAT_GEHEUGEN1!"=="GESLAAGD" set "HERUITVOER1=1"
  if "!STATUS2!"=="VOLLEDIG AFGEROND" if "!RESULTAAT2!"=="MISLUKT" set "HERUITVOER2=1"
  if "!STATUS3!"=="VOLLEDIG AFGEROND" if "!RESULTAAT3_OFFICIEEL!"=="MISLUKT" set "HERUITVOER3=1"
  if "!STATUS3!"=="VOLLEDIG AFGEROND" if "!RESULTAAT3_STATISCH!"=="MISLUKT" set "HERUITVOER3=1"
  if "!STATUS3!"=="VOLLEDIG AFGEROND" if not "!RESULTAAT_GEHEUGEN3!"=="GESLAAGD" set "HERUITVOER3=1"
  if "!STATUS4!"=="VOLLEDIG AFGEROND" if "!RESULTAAT4!"=="MISLUKT" set "HERUITVOER4=1"
  if "!HERUITVOER1!"=="1" echo TestLibraryGereleased.cmd
  if "!HERUITVOER2!"=="1" echo TestLibraryGereleasedOngeldig.cmd
  if "!HERUITVOER3!"=="1" echo TestLibraryNieuw.cmd
  if "!HERUITVOER4!"=="1" echo TestLibraryNieuwOngeldig.cmd
  if not "!RESULTAAT_MAPPING!"=="GESLAAGD" echo TestLibraryMappingControle.cmd
  echo.
  echo De overige geldige logbestanden blijven behouden en hoeven niet opnieuw uitgevoerd te worden.
)

del /q "%MAPPING_LOG%" >nul 2>&1

if !RELEASE_FOUT! EQU 0 (
  endlocal & exit /b 0
) else (
  endlocal & exit /b 1
)

:Controleer
set "%~2=ONTBREEKT"
if not exist "%~1" (
  set /a ONTBREEKT+=1
  exit /b
)
powershell -NoProfile -Command "if (Select-String -LiteralPath '%~1' -SimpleMatch -Pattern '=== TEST VOLLEDIG AFGEROND ===' -Quiet) { exit 0 } else { exit 1 }" >nul 2>&1
if errorlevel 1 (
  set "%~2=ONVOLLEDIG / AFGEBROKEN"
  set /a ONVOLLEDIG+=1
) else (
  set "%~2=VOLLEDIG AFGEROND"
  set /a GELDIG+=1
)
exit /b

:BeoordeelGereleased
for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG1%' -SimpleMatch -Pattern 'Totaal getest' ^| Select-Object -Last 1; if($m){$m.Line}"`) do set "SAMENVATTING1=%%L"
for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG1%' -SimpleMatch -Pattern 'Arduino LINT' ^| Select-Object -Last 1; if($m){$m.Line}"`) do set "SAMENVATTING1_LINT=%%L"
for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG1%' -Pattern '^Totaal getest\s*:\s*(\d+)' ^| Select-Object -Last 1; if($m){$m.Matches[0].Groups[1].Value}"`) do set "GERELEASED_TOTAAL=%%V"
for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG1%' -Pattern '^OK bevonden\s*:\s*(\d+)' ^| Select-Object -Last 1; if($m){$m.Matches[0].Groups[1].Value}"`) do set "GERELEASED_OK=%%V"
for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG1%' -Pattern '^Verwachte UNO R3-geheugenbeperkingen\s*:\s*(\d+)' ^| Select-Object -Last 1; if($m){$m.Matches[0].Groups[1].Value}"`) do set "GERELEASED_EXPECTED_MEMORY=%%V"
for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG1%' -Pattern '^Onverwacht mislukt\s*:\s*(\d+)' ^| Select-Object -Last 1; if($m){$m.Matches[0].Groups[1].Value}"`) do set "GERELEASED_UNEXPECTED_FAIL=%%V"
for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG1%' -Pattern '^Arduino LINT\s*:\s*(.+)$' ^| Select-Object -Last 1; if($m){$m.Matches[0].Groups[1].Value.Trim()}"`) do set "RELEASE_LINT=%%V"
set "GERELEASED_PARSE_OK=1"
if "!GERELEASED_TOTAAL!"=="ONBEKEND" set "GERELEASED_PARSE_OK=0"
if "!GERELEASED_OK!"=="ONBEKEND" set "GERELEASED_PARSE_OK=0"
if "!GERELEASED_EXPECTED_MEMORY!"=="ONBEKEND" set "GERELEASED_PARSE_OK=0"
if "!GERELEASED_UNEXPECTED_FAIL!"=="ONBEKEND" set "GERELEASED_PARSE_OK=0"
if "!RELEASE_LINT!"=="ONBEKEND" set "GERELEASED_PARSE_OK=0"
if "!GERELEASED_PARSE_OK!"=="1" (
  set /a GERELEASED_SOM=GERELEASED_OK+GERELEASED_EXPECTED_MEMORY+GERELEASED_UNEXPECTED_FAIL
  if not !GERELEASED_SOM! EQU !GERELEASED_TOTAAL! set "GERELEASED_PARSE_OK=0"
  if not !GERELEASED_UNEXPECTED_FAIL! EQU 0 set "GERELEASED_PARSE_OK=0"
  if /I not "!RELEASE_LINT!"=="GESLAAGD" set "GERELEASED_PARSE_OK=0"
)
if "!GERELEASED_PARSE_OK!"=="0" (
  set "RESULTAAT1=MISLUKT"
  set /a RELEASE_FOUT+=1
) else (
  set "RESULTAAT1=GESLAAGD"
)
exit /b
:BeoordeelOngeldig
set "BO_LOG=%~1"
set "BO_RESULTAAT=%~2"
set "BO_SAMENVATTING=%~3"
for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%BO_LOG%' -Pattern '^Totaal ' ^| Select-Object -Last 1; if($m){$m.Line}"`) do set "%BO_SAMENVATTING%=%%L"
powershell -NoProfile -Command "$m=Select-String -LiteralPath '%BO_LOG%' -Pattern '^Totaal\s+(\d+)\s+-\s+OK\s+(\d+)\s+-\s+FOUT\s+(\d+)$' ^| Select-Object -Last 1; if(-not $m){exit 1}; $t=[int]$m.Matches[0].Groups[1].Value; $o=[int]$m.Matches[0].Groups[2].Value; $f=[int]$m.Matches[0].Groups[3].Value; if($t -eq $o -and $f -eq 0){exit 0}else{exit 1}" >nul 2>&1
if errorlevel 1 (
  set "%BO_RESULTAAT%=MISLUKT"
  set /a RELEASE_FOUT+=1
) else (
  set "%BO_RESULTAAT%=GESLAAGD"
)
exit /b
:BeoordeelNieuw
for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG3%' -Pattern '^OFFICIEEL:' ^| Select-Object -Last 1; if($m){$m.Line}"`) do set "SAMENVATTING3_OFFICIEEL=%%L"
for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG3%' -Pattern '^ACCEPTATIE:' ^| Select-Object -Last 1; if($m){$m.Line}"`) do set "SAMENVATTING3_ACCEPTATIE=%%L"
for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG3%' -Pattern '^STATISCH:' ^| Select-Object -Last 1; if($m){$m.Line}"`) do set "SAMENVATTING3_STATISCH=%%L"
for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG3%' -Pattern '^OFFICIEEL:\s*(\d+)/(\d+)\s+OK,\s*(\d+)\s+FOUT$' ^| Select-Object -Last 1; if($m){$m.Matches[0].Groups[1].Value}"`) do set "NIEUW_OFFICIEEL_OK=%%V"
for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG3%' -Pattern '^OFFICIEEL:\s*(\d+)/(\d+)\s+OK,\s*(\d+)\s+FOUT$' ^| Select-Object -Last 1; if($m){$m.Matches[0].Groups[2].Value}"`) do set "NIEUW_OFFICIEEL_TOTAAL=%%V"
for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG3%' -Pattern '^OFFICIEEL:\s*(\d+)/(\d+)\s+OK,\s*(\d+)\s+FOUT$' ^| Select-Object -Last 1; if($m){$m.Matches[0].Groups[3].Value}"`) do set "NIEUW_OFFICIEEL_FAIL=%%V"
set "NIEUW_PARSE_OK=1"
if "!NIEUW_OFFICIEEL_OK!"=="ONBEKEND" set "NIEUW_PARSE_OK=0"
if "!NIEUW_OFFICIEEL_TOTAAL!"=="ONBEKEND" set "NIEUW_PARSE_OK=0"
if "!NIEUW_OFFICIEEL_FAIL!"=="ONBEKEND" set "NIEUW_PARSE_OK=0"
if "!NIEUW_PARSE_OK!"=="1" (
  if not !NIEUW_OFFICIEEL_OK! EQU !NIEUW_OFFICIEEL_TOTAAL! set "NIEUW_PARSE_OK=0"
  if not !NIEUW_OFFICIEEL_FAIL! EQU 0 set "NIEUW_PARSE_OK=0"
)
if "!NIEUW_PARSE_OK!"=="0" (
  set "RESULTAAT3_OFFICIEEL=MISLUKT"
  set /a RELEASE_FOUT+=1
) else (
  set "RESULTAAT3_OFFICIEEL=GESLAAGD"
)
powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG3%' -Pattern '^STATISCH:\s*(\d+)\s+FOUT$' ^| Select-Object -Last 1; if($m -and [int]$m.Matches[0].Groups[1].Value -eq 0){exit 0}else{exit 1}" >nul 2>&1
if errorlevel 1 (
  set "RESULTAAT3_STATISCH=MISLUKT"
  set /a RELEASE_FOUT+=1
) else (
  set "RESULTAAT3_STATISCH=GESLAAGD"
)
powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG3%' -Pattern '^ACCEPTATIE:\s*(\d+)/(\d+)\s+OK,\s*(\d+)\s+FOUT(?:\s+-\s+GEEN RELEASE-IMPACT)?$' ^| Select-Object -Last 1; if(-not $m){exit 1}; $o=[int]$m.Matches[0].Groups[1].Value; $t=[int]$m.Matches[0].Groups[2].Value; $f=[int]$m.Matches[0].Groups[3].Value; if($o -eq $t -and $f -eq 0){exit 0}else{exit 1}" >nul 2>&1
if errorlevel 1 (
  set "RESULTAAT3_ACCEPTATIE=NIET VOLLEDIG GESLAAGD"
) else (
  set "RESULTAAT3_ACCEPTATIE=GESLAAGD"
)
exit /b
:BeoordeelGeheugen
set "BG_LOG=%~1"
set "BG_RESULTAAT=%~2"
set "BG_AANTAL=%~3"
set "BG_FOUTEN=%~4"
set "BG_COUNT=0"
set "BG_INVALID=0"
for /f "tokens=1,2 delims=|" %%A in ('powershell -NoProfile -Command "$rows=Get-Content -LiteralPath '%BG_LOG%' ^| Where-Object {$_ -match '^\[GEHEUGEN\]'}; $bad=0; foreach($line in $rows){ if($line -notmatch '^\[GEHEUGEN\]\s+Board=(.*?) \| Bestand=(.*?) \| Test=(.*?) \| Programma=(.*?) \| RAM=(.*?) \| Status=(.*?)$'){ $bad++; continue }; $prog=$matches[4].Trim(); $ram=$matches[5].Trim(); $status=$matches[6].Trim(); if($status -eq 'OK'){ if($prog -eq 'NIET_GEVONDEN' -or $ram -eq 'NIET_GEVONDEN'){ $bad++ } } elseif($status -eq 'VERWACHTE_GEHEUGENBEPERKING'){ } else { $bad++ } }; Write-Output ($rows.Count.ToString()+'|'+$bad.ToString())"') do (
  set "BG_COUNT=%%A"
  set "BG_INVALID=%%B"
)
set "%BG_AANTAL%=!BG_COUNT!"
set "%BG_FOUTEN%=!BG_INVALID!"
if "!BG_COUNT!"=="0" (
  set "%BG_RESULTAAT%=ONTBREEKT"
  set /a RELEASE_FOUT+=1
) else if not "!BG_INVALID!"=="0" (
  set "%BG_RESULTAAT%=MISLUKT"
  set /a RELEASE_FOUT+=1
) else (
  set "%BG_RESULTAAT%=GESLAAGD"
)
exit /b
:BerekenReleaseTotalen
set "RELEASE_TOTAAL=ONBEKEND"
set "RELEASE_OK=ONBEKEND"
set "RELEASE_EXPECTED_MEMORY=!GERELEASED_EXPECTED_MEMORY!"
set "RELEASE_UNEXPECTED_FAIL=ONBEKEND"
if not "!GERELEASED_TOTAAL!"=="ONBEKEND" if not "!NIEUW_OFFICIEEL_TOTAAL!"=="ONBEKEND" set /a RELEASE_TOTAAL=GERELEASED_TOTAAL+NIEUW_OFFICIEEL_TOTAAL
if not "!GERELEASED_OK!"=="ONBEKEND" if not "!NIEUW_OFFICIEEL_OK!"=="ONBEKEND" set /a RELEASE_OK=GERELEASED_OK+NIEUW_OFFICIEEL_OK
if not "!GERELEASED_UNEXPECTED_FAIL!"=="ONBEKEND" if not "!NIEUW_OFFICIEEL_FAIL!"=="ONBEKEND" set /a RELEASE_UNEXPECTED_FAIL=GERELEASED_UNEXPECTED_FAIL+NIEUW_OFFICIEEL_FAIL
exit /b

:SchrijfReleaseUpdate
echo ============================================================
echo UPDATE OFFICIELE TESTRESULTATEN
echo ============================================================
echo Bij te werken document: extras\TESTRESULTATEN.md
echo Plaats: ## Overzicht
echo.
if !RELEASE_FOUT! EQU 0 (
  echo Nieuwe release-regel:
  echo ^| v!LIB_VERSION! ^| !RELEASE_DATUM! ^| !RELEASE_LINT! ^| !RELEASE_TOTAAL! ^| !RELEASE_OK! ^| !RELEASE_EXPECTED_MEMORY! ^| !RELEASE_UNEXPECTED_FAIL! ^| GESLAAGD ^|
  echo.
  echo REMINDER:
  echo Voeg bovenstaande regel bovenaan de bestaande versietabel toe
  echo wanneer deze validatie bij de officiele release hoort.
) else (
  echo GEEN release-regel toevoegen.
  echo De releasevalidatie is niet volledig geslaagd.
)
echo ============================================================
exit /b


:SchrijfGeheugentabellen
echo.
echo ============================================================
echo GEHEUGENGEBRUIK - TestLibraryGereleased.cmd
echo ============================================================
call :SchrijfGeheugentabel "%LOG1%"
echo.
echo ============================================================
echo GEHEUGENGEBRUIK - TestLibraryNieuw.cmd
echo ============================================================
call :SchrijfGeheugentabel "%LOG3%"
exit /b

:SchrijfGeheugentabel
if not exist "%~1" (
  echo GEEN LOGBESTAND AANWEZIG
  exit /b
)
powershell -NoProfile -Command "$rows = Get-Content -LiteralPath '%~1' ^| ForEach-Object { if ($_ -match '^\[GEHEUGEN\]\s+Board=(.*?) \^| Bestand=(.*?) \^| Test=(.*?) \^| Programma=(.*?) \^| RAM=(.*?) \^| Status=(.*?)$') { [pscustomobject]@{ Board=$matches[1]; Bestand=$matches[2]; Test=$matches[3]; Programma=$matches[4]; RAM=$matches[5]; Status=$matches[6] } } }; if (-not $rows) { Write-Output 'Geen geheugenrecords gevonden.'; exit }; Write-Output 'Board ^| Bestand / example ^| Test / configuratie ^| Programma ^| RAM ^| Status'; Write-Output '----- ^| ----------------- ^| ------------------- ^| --------- ^| --- ^| ------'; foreach ($r in $rows) { Write-Output ('{0} ^| {1} ^| {2} ^| {3} ^| {4} ^| {5}' -f $r.Board,$r.Bestand,$r.Test,$r.Programma,$r.RAM,$r.Status) }"
exit /b

:SchrijfMappingResultaat
echo.
echo ============================================================
echo MAPPINGCONTROLE - TestLibraryMappingControle.cmd
echo ============================================================
echo Resultaat: !RESULTAAT_MAPPING!
if exist "%MAPPING_LOG%" type "%MAPPING_LOG%"
exit /b

:SchrijfFoutdetails
echo ============================================================
echo MISLUKTE TESTEN - DETAILS
echo ============================================================
set "FOUTDETAILS_GEVONDEN=0"
if not "!STATUS1!"=="VOLLEDIG AFGEROND" (
  echo.
  echo TestLibraryGereleased.cmd
  echo   Testcyclus : !STATUS1!
  if "!STATUS1!"=="ONTBREEKT" echo   Reden      : TestLibraryGereleased.txt ontbreekt.
  if "!STATUS1!"=="ONVOLLEDIG / AFGEBROKEN" echo   Reden      : TestLibraryGereleased.txt bevat geen marker === TEST VOLLEDIG AFGEROND ===.
  set "FOUTDETAILS_GEVONDEN=1"
) else (
  if "!RESULTAAT1!"=="MISLUKT" (
    call :SchrijfFoutenUitLog "%LOG1%" "TestLibraryGereleased.cmd" "ALLE"
    set "FOUTDETAILS_GEVONDEN=1"
  )
  if not "!RESULTAAT_GEHEUGEN1!"=="GESLAAGD" (
    echo.
    echo TestLibraryGereleased.cmd - GEHEUGENGEGEVENS
    echo ------------------------------------------------------------
    if "!RESULTAAT_GEHEUGEN1!"=="ONTBREEKT" echo   Reden: TestLibraryGereleased.txt bevat geen [GEHEUGEN]-records.
    if "!RESULTAAT_GEHEUGEN1!"=="MISLUKT" echo   Reden: !GEHEUGEN1_FOUTEN! geheugenrecord^(s^) bevatten ontbrekende Programma/RAM-gegevens of een ongeldige status.
    set "FOUTDETAILS_GEVONDEN=1"
  )
)
if not "!STATUS2!"=="VOLLEDIG AFGEROND" (
  echo.
  echo TestLibraryGereleasedOngeldig.cmd
  echo   Testcyclus : !STATUS2!
  if "!STATUS2!"=="ONTBREEKT" echo   Reden      : TestLibraryGereleasedOngeldig.txt ontbreekt.
  if "!STATUS2!"=="ONVOLLEDIG / AFGEBROKEN" echo   Reden      : TestLibraryGereleasedOngeldig.txt bevat geen marker === TEST VOLLEDIG AFGEROND ===.
  set "FOUTDETAILS_GEVONDEN=1"
) else if "!RESULTAAT2!"=="MISLUKT" (
  call :SchrijfFoutenUitLog "%LOG2%" "TestLibraryGereleasedOngeldig.cmd" "ALLE"
  set "FOUTDETAILS_GEVONDEN=1"
)
if not "!STATUS3!"=="VOLLEDIG AFGEROND" (
  echo.
  echo TestLibraryNieuw.cmd
  echo   Testcyclus : !STATUS3!
  if "!STATUS3!"=="ONTBREEKT" echo   Reden      : TestLibraryNieuw.txt ontbreekt.
  if "!STATUS3!"=="ONVOLLEDIG / AFGEBROKEN" echo   Reden      : TestLibraryNieuw.txt bevat geen marker === TEST VOLLEDIG AFGEROND ===.
  set "FOUTDETAILS_GEVONDEN=1"
) else (
  if "!RESULTAAT3_OFFICIEEL!"=="MISLUKT" (
    call :SchrijfFoutenUitLog "%LOG3%" "TestLibraryNieuw.cmd - OFFICIEEL" "OFFICIEEL"
    set "FOUTDETAILS_GEVONDEN=1"
  )
  if "!RESULTAAT3_STATISCH!"=="MISLUKT" (
    call :SchrijfFoutenUitLog "%LOG3%" "TestLibraryNieuw.cmd - STATISCH" "STATISCH"
    set "FOUTDETAILS_GEVONDEN=1"
  )
  if not "!RESULTAAT_GEHEUGEN3!"=="GESLAAGD" (
    echo.
    echo TestLibraryNieuw.cmd - GEHEUGENGEGEVENS
    echo ------------------------------------------------------------
    if "!RESULTAAT_GEHEUGEN3!"=="ONTBREEKT" echo   Reden: TestLibraryNieuw.txt bevat geen [GEHEUGEN]-records.
    if "!RESULTAAT_GEHEUGEN3!"=="MISLUKT" echo   Reden: !GEHEUGEN3_FOUTEN! geheugenrecord^(s^) bevatten ontbrekende Programma/RAM-gegevens of een ongeldige status.
    set "FOUTDETAILS_GEVONDEN=1"
  )
)
if not "!STATUS4!"=="VOLLEDIG AFGEROND" (
  echo.
  echo TestLibraryNieuwOngeldig.cmd
  echo   Testcyclus : !STATUS4!
  if "!STATUS4!"=="ONTBREEKT" echo   Reden      : TestLibraryNieuwOngeldig.txt ontbreekt.
  if "!STATUS4!"=="ONVOLLEDIG / AFGEBROKEN" echo   Reden      : TestLibraryNieuwOngeldig.txt bevat geen marker === TEST VOLLEDIG AFGEROND ===.
  set "FOUTDETAILS_GEVONDEN=1"
) else if "!RESULTAAT4!"=="MISLUKT" (
  call :SchrijfFoutenUitLog "%LOG4%" "TestLibraryNieuwOngeldig.cmd" "ALLE"
  set "FOUTDETAILS_GEVONDEN=1"
)
if not "!RESULTAAT_MAPPING!"=="GESLAAGD" (
  echo.
  echo TestLibraryMappingControle.cmd
  echo ------------------------------------------------------------
  echo   Resultaat: !RESULTAAT_MAPPING!
  if exist "%MAPPING_LOG%" type "%MAPPING_LOG%"
  set "FOUTDETAILS_GEVONDEN=1"
)
if "!FOUTDETAILS_GEVONDEN!"=="0" echo Geen releaseblokkerende foutdetails gevonden.
exit /b

:SchrijfFoutenUitLog
echo.
echo %~2
echo ------------------------------------------------------------
if not exist "%~1" (
  echo Geen logbestand aanwezig.
  exit /b
)
powershell -NoProfile -Command "$l=Get-Content -LiteralPath '%~1'; $cat='%~3'; $hits=@(); for($i=0; $i -lt $l.Count; $i++){ $line=$l[$i]; $hit=$false; if($cat -eq 'OFFICIEEL'){ if($line -match '^\[FOUT\]\[(OFFICIAL|OFFICIEEL)\]'){ $hit=$true } } elseif($cat -eq 'STATISCH'){ if($line -match '^\[FOUT\](?!\[)'){ $hit=$true } } else { if($line -match '^\[FOUT\]' -or $line -match '^FOUT:' -or $line -eq 'COMPILATIEFOUT' -or $line -match '^Arduino LINT.*(?:MISLUKT|FOUT)'){ $hit=$true } }; if($hit){ $hits += $i } }; if($hits.Count -eq 0){ $sum=$l ^| Where-Object { $_ -match 'FOUT [1-9][0-9]*$' -or $_ -match 'Onverwacht mislukt\s*:\s*[1-9]' } ^| Select-Object -Last 5; if($sum){$sum}else{'Geen afzonderlijke foutregel gevonden; raadpleeg de volledige log hieronder in TestLibraryStatusReport.txt.'}; exit }; foreach($i in $hits){ Write-Output $l[$i]; $detail=@(); $max=[Math]::Min($l.Count-1,$i+30); for($j=$i+1;$j -le $max;$j++){ if($l[$j] -match '^\[OK\]' -or $l[$j] -match '^\[FOUT\]' -or $l[$j] -match '^-{20,}$'){ break }; if($l[$j] -match '(?i)(fatal error:|error:|#error|undefined reference|collect2:|compilation terminated|failed|mislukt|onverwacht)'){ $detail += $l[$j]; if($detail.Count -ge 4){break} } }; if($detail.Count -gt 0){ $detail ^| ForEach-Object { Write-Output ('  Reden: ' + $_) } } else { Write-Output '  Reden: zie bijbehorende compile-uitvoer in de volledige log.' }; Write-Output '' }"
exit /b

:VoegToe
>> "%REPORT%" echo ============================================================
>> "%REPORT%" echo %~2
>> "%REPORT%" echo ============================================================
if exist "%~1" (
  powershell -NoProfile -Command "Get-Content -LiteralPath '%~1' | Add-Content -LiteralPath '%REPORT%'"
) else (
  >> "%REPORT%" echo GEEN LOGBESTAND AANWEZIG
)
>> "%REPORT%" echo.
exit /b
