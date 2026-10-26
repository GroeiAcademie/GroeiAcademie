@echo off
cls
setlocal EnableExtensions EnableDelayedExpansion

set "REPORT=%~dp0TestLibraryStatusReport.txt"
set "MARKER=[EINDE] Testen voltooid. Druk op een toets om af te sluiten..."
set "DOMEINEN=Data Extenders GedeeldeBus Input Screen Sensoren Sturingen Tools"
set "AANTAL_TESTCYCLI=16"
set "MAPPING_SCRIPT=%~dp0TestLibraryMappingControle.cmd"
set "MAPPING_LOG=%TEMP%\GroeiAcademieMappingControle_%RANDOM%_%RANDOM%.txt"
set "RESULTAAT_MAPPING=NIET BEOORDEELD"

if exist "%REPORT%" del /q "%REPORT%" >nul 2>&1

set /a GELDIG=0
set /a ONTBREEKT=0
set /a ONVOLLEDIG=0
set /a RELEASE_FOUT=0

set "LIB_VERSION=ONBEKEND"
set "RELEASE_DATUM=ONBEKEND"
set "RELEASE_LINT=GESLAAGD"
set /a RELEASE_TOTAAL=0
set /a RELEASE_OK=0
set /a RELEASE_EXPECTED_MEMORY=0
set /a RELEASE_UNEXPECTED_FAIL=0

if exist "%~dp0..\library.properties" (
  for /f "usebackq tokens=1,* delims==" %%A in (`findstr /B /C:"version=" "%~dp0..\library.properties"`) do set "LIB_VERSION=%%B"
)
for /f "usebackq delims=" %%D in (`powershell -NoProfile -Command "Get-Date -Format 'dd-MM-yyyy'"`) do set "RELEASE_DATUM=%%D"

for %%D in (%DOMEINEN%) do (
  set "LOG_%%D=%~dp0TestLibrary%%D-v2.0.0.txt"
  set "LOG_%%D_ONGELDIG=%~dp0TestLibrary%%DOngeldig-v2.0.0.txt"

  set "STATUS_%%D=ONTBREEKT"
  set "STATUS_%%D_ONGELDIG=ONTBREEKT"

  set "RESULTAAT_%%D_BASIC_EXTENDED=NIET BEOORDEELD"
  set "RESULTAAT_%%D_EXPERIMENTAL=NIET BEOORDEELD"
  set "RESULTAAT_%%D_STATISCH=NIET BEOORDEELD"
  set "RESULTAAT_%%D_LINT=NIET BEOORDEELD"
  set "RESULTAAT_%%D_GEHEUGEN=NIET BEOORDEELD"
  set "RESULTAAT_%%D_ONGELDIG=NIET BEOORDEELD"

  set "SAMENVATTING_%%D_BASIC_EXTENDED="
  set "SAMENVATTING_%%D_EXPERIMENTAL="
  set "SAMENVATTING_%%D_STATISCH="
  set "SAMENVATTING_%%D_LINT="
  set "SAMENVATTING_%%D_ONGELDIG="

  set "GEHEUGEN_%%D_AANTAL=0"
  set "GEHEUGEN_%%D_FOUTEN=0"

  call :Controleer "!LOG_%%D!" "STATUS_%%D"
  call :Controleer "!LOG_%%D_ONGELDIG!" "STATUS_%%D_ONGELDIG"
)

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

for %%D in (%DOMEINEN%) do (
  if "!STATUS_%%D!"=="VOLLEDIG AFGEROND" (
    call :BeoordeelDomein "!LOG_%%D!" "%%D"
    call :BeoordeelGeheugen "!LOG_%%D!" "RESULTAAT_%%D_GEHEUGEN" "GEHEUGEN_%%D_AANTAL" "GEHEUGEN_%%D_FOUTEN"
  )
  if "!STATUS_%%D_ONGELDIG!"=="VOLLEDIG AFGEROND" (
    call :BeoordeelOngeldig "!LOG_%%D_ONGELDIG!" "RESULTAAT_%%D_ONGELDIG" "SAMENVATTING_%%D_ONGELDIG"
  )
)

if !GELDIG! NEQ !AANTAL_TESTCYCLI! set /a RELEASE_FOUT+=1

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
  echo Alle testcycli volledig : !GELDIG! / !AANTAL_TESTCYCLI!
  echo Ontbrekend               : !ONTBREEKT!
  echo Onvolledig / afgebroken  : !ONVOLLEDIG!
  echo.
)

for %%D in (%DOMEINEN%) do (
  >> "%REPORT%" (
    echo TestLibrary%%D-v2.0.0.cmd
    echo   Testcyclus              : !STATUS_%%D!
    echo   BASIC + EXTENDED        : !RESULTAAT_%%D_BASIC_EXTENDED!
    if defined SAMENVATTING_%%D_BASIC_EXTENDED echo   !SAMENVATTING_%%D_BASIC_EXTENDED!
    echo   EXPERIMENTAL_FQBN       : !RESULTAAT_%%D_EXPERIMENTAL! - GEEN RELEASE-IMPACT
    if defined SAMENVATTING_%%D_EXPERIMENTAL echo   !SAMENVATTING_%%D_EXPERIMENTAL!
    echo   Statische controles     : !RESULTAAT_%%D_STATISCH!
    if defined SAMENVATTING_%%D_STATISCH echo   !SAMENVATTING_%%D_STATISCH!
    echo   Arduino LINT            : !RESULTAAT_%%D_LINT!
    if defined SAMENVATTING_%%D_LINT echo   !SAMENVATTING_%%D_LINT!
    echo   Geheugengegevens        : !RESULTAAT_%%D_GEHEUGEN! ^(!GEHEUGEN_%%D_AANTAL! records, !GEHEUGEN_%%D_FOUTEN! ongeldig^)
    echo.
    echo TestLibrary%%DOngeldig-v2.0.0.cmd
    echo   Testcyclus              : !STATUS_%%D_ONGELDIG!
    echo   Release-resultaat       : !RESULTAAT_%%D_ONGELDIG!
    if defined SAMENVATTING_%%D_ONGELDIG echo   !SAMENVATTING_%%D_ONGELDIG!
    echo.
  )
)

>> "%REPORT%" (
  echo TestLibraryMappingControle.cmd
  echo   Verplichte mappingcontrole : !RESULTAAT_MAPPING!
  echo.
  echo ============================================================
  echo RELEASECRITERIA
  echo ============================================================
)

for %%D in (%DOMEINEN%) do (
  >> "%REPORT%" (
    echo TestLibrary%%D-v2.0.0 BASIC + EXTENDED : !RESULTAAT_%%D_BASIC_EXTENDED!
    echo TestLibrary%%D-v2.0.0 STATISCH         : !RESULTAAT_%%D_STATISCH!
    echo TestLibrary%%D-v2.0.0 LINT             : !RESULTAAT_%%D_LINT!
    echo TestLibrary%%D-v2.0.0 EXPERIMENTAL     : !RESULTAAT_%%D_EXPERIMENTAL! - GEEN RELEASE-IMPACT
    echo TestLibrary%%D-v2.0.0 GEHEUGEN         : !RESULTAAT_%%D_GEHEUGEN! ^(!GEHEUGEN_%%D_AANTAL! records, !GEHEUGEN_%%D_FOUTEN! ongeldig^)
    echo TestLibrary%%DOngeldig-v2.0.0           : !RESULTAAT_%%D_ONGELDIG!
  )
)

>> "%REPORT%" (
  echo TestLibraryMappingControle               : !RESULTAAT_MAPPING!
  echo Alle !AANTAL_TESTCYCLI! testcycli volledig             : !GELDIG! / !AANTAL_TESTCYCLI!
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

for %%D in (%DOMEINEN%) do (
  call :VoegToe "!LOG_%%D!" "TestLibrary%%D-v2.0.0.cmd"
  call :VoegToe "!LOG_%%D_ONGELDIG!" "TestLibrary%%DOngeldig-v2.0.0.cmd"
)

echo.

if !GELDIG! EQU !AANTAL_TESTCYCLI! if !RELEASE_FOUT! EQU 0 (
  call :SchrijfReleaseUpdate
  for %%D in (%DOMEINEN%) do (
    del /q "!LOG_%%D!" "!LOG_%%D_ONGELDIG!" >nul 2>&1
  )
  echo.
  echo TestLibraryStatusReport.txt is aangemaakt.
  echo De releasevalidatie is geslaagd.
  echo De !AANTAL_TESTCYCLI! individuele logbestanden zijn verwijderd.
) else (
  echo ============================================================
  echo LEESBAAR STATUSRAPPORT - RELEASEVALIDATIE NIET GESLAAGD
  echo ============================================================
  powershell -NoProfile -Command "$l=Get-Content -LiteralPath '%REPORT%'; $stop=($l | Select-String -SimpleMatch 'VOLLEDIGE TESTLOGS' | Select-Object -First 1).LineNumber; if($stop){$l[0..([Math]::Max(0,$stop-2))]}else{$l}"
  echo.
  echo OPNIEUW UIT TE VOEREN:
  for %%D in (%DOMEINEN%) do (
    set "HERUITVOER=0"
    if not "!STATUS_%%D!"=="VOLLEDIG AFGEROND" set "HERUITVOER=1"
    if "!STATUS_%%D!"=="VOLLEDIG AFGEROND" if not "!RESULTAAT_%%D_BASIC_EXTENDED!"=="GESLAAGD" set "HERUITVOER=1"
    if "!STATUS_%%D!"=="VOLLEDIG AFGEROND" if not "!RESULTAAT_%%D_STATISCH!"=="GESLAAGD" set "HERUITVOER=1"
    if "!STATUS_%%D!"=="VOLLEDIG AFGEROND" if not "!RESULTAAT_%%D_LINT!"=="GESLAAGD" set "HERUITVOER=1"
    if "!STATUS_%%D!"=="VOLLEDIG AFGEROND" if not "!RESULTAAT_%%D_GEHEUGEN!"=="GESLAAGD" set "HERUITVOER=1"
    if "!HERUITVOER!"=="1" echo TestLibrary%%D-v2.0.0.cmd

    set "HERUITVOER=0"
    if not "!STATUS_%%D_ONGELDIG!"=="VOLLEDIG AFGEROND" set "HERUITVOER=1"
    if "!STATUS_%%D_ONGELDIG!"=="VOLLEDIG AFGEROND" if not "!RESULTAAT_%%D_ONGELDIG!"=="GESLAAGD" set "HERUITVOER=1"
    if "!HERUITVOER!"=="1" echo TestLibrary%%DOngeldig-v2.0.0.cmd
  )
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
powershell -NoProfile -Command "if (Select-String -LiteralPath '%~1' -SimpleMatch -Pattern '[EINDE] Testen voltooid. Druk op een toets om af te sluiten...' -Quiet) { exit 0 } else { exit 1 }" >nul 2>&1
if errorlevel 1 (
  set "%~2=ONVOLLEDIG / AFGEBROKEN"
  set /a ONVOLLEDIG+=1
) else (
  set "%~2=VOLLEDIG AFGEROND"
  set /a GELDIG+=1
)
exit /b

:BeoordeelDomein
set "BD_LOG=%~1"
set "BD_DOMEIN=%~2"
set "BD_BASIC_TOTAAL="
set "BD_BASIC_OK="
set "BD_BASIC_EXPECTED_MEMORY="
set "BD_BASIC_FAIL="
set "BD_EXTENDED_TOTAAL="
set "BD_EXTENDED_OK="
set "BD_EXTENDED_EXPECTED_MEMORY="
set "BD_EXTENDED_FAIL="
set "BD_EINDSTATUS="
set "BD_EXPERIMENTAL_STATUS="
set "BD_LINT=ONBEKEND"

for /f "usebackq tokens=1-4 delims=|" %%A in (`powershell -NoProfile -Command "$raw=Get-Content -LiteralPath '%BD_LOG%' -Raw; $p='(?ms)^\[BASIC_FQBN\].*?^Totaal getest\s*:\s*(\d+).*?^OK bevonden\s*:\s*(\d+).*?^Verwachte geheugenbeperkingen\s*:\s*(\d+).*?^Onverwacht mislukt\s*:\s*(\d+)'; $m=[regex]::Matches($raw,$p); if($m.Count){$x=$m[$m.Count-1]; Write-Output ($x.Groups[1].Value+'|'+$x.Groups[2].Value+'|'+$x.Groups[3].Value+'|'+$x.Groups[4].Value)}"`) do (
  set "BD_BASIC_TOTAAL=%%A"
  set "BD_BASIC_OK=%%B"
  set "BD_BASIC_EXPECTED_MEMORY=%%C"
  set "BD_BASIC_FAIL=%%D"
)
for /f "usebackq tokens=1-4 delims=|" %%A in (`powershell -NoProfile -Command "$raw=Get-Content -LiteralPath '%BD_LOG%' -Raw; $p='(?ms)^\[EXTENDED_FQBN\].*?^Totaal getest\s*:\s*(\d+).*?^OK bevonden\s*:\s*(\d+).*?^Verwachte geheugenbeperkingen\s*:\s*(\d+).*?^Onverwacht mislukt\s*:\s*(\d+)'; $m=[regex]::Matches($raw,$p); if($m.Count){$x=$m[$m.Count-1]; Write-Output ($x.Groups[1].Value+'|'+$x.Groups[2].Value+'|'+$x.Groups[3].Value+'|'+$x.Groups[4].Value)}"`) do (
  set "BD_EXTENDED_TOTAAL=%%A"
  set "BD_EXTENDED_OK=%%B"
  set "BD_EXTENDED_EXPECTED_MEMORY=%%C"
  set "BD_EXTENDED_FAIL=%%D"
)
for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%BD_LOG%' -Pattern '^Eindstatus BASIC \+ EXTENDED\s*:\s*(.+)$' | Select-Object -Last 1; if($m){$m.Matches[0].Groups[1].Value.Trim()}"`) do set "BD_EINDSTATUS=%%V"
for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%BD_LOG%' -Pattern '^Eindstatus BASIC \+ EXTENDED\s*:' | Select-Object -Last 1; if($m){$m.Line}"`) do set "SAMENVATTING_!BD_DOMEIN!_BASIC_EXTENDED=%%L"

set "BD_PARSE_OK=1"
if not defined BD_BASIC_TOTAAL set "BD_PARSE_OK=0"
if not defined BD_EXTENDED_TOTAAL set "BD_PARSE_OK=0"
if not defined BD_EINDSTATUS set "BD_PARSE_OK=0"
if "!BD_PARSE_OK!"=="1" (
  set /a BD_TOTAAL=BD_BASIC_TOTAAL+BD_EXTENDED_TOTAAL
  set /a BD_OK=BD_BASIC_OK+BD_EXTENDED_OK
  set /a BD_EXPECTED_MEMORY=BD_BASIC_EXPECTED_MEMORY+BD_EXTENDED_EXPECTED_MEMORY
  set /a BD_FAIL=BD_BASIC_FAIL+BD_EXTENDED_FAIL
  set /a RELEASE_TOTAAL+=BD_TOTAAL
  set /a RELEASE_OK+=BD_OK
  set /a RELEASE_EXPECTED_MEMORY+=BD_EXPECTED_MEMORY
  set /a RELEASE_UNEXPECTED_FAIL+=BD_FAIL
  if /I not "!BD_EINDSTATUS!"=="GESLAAGD" set "BD_PARSE_OK=0"
  if not !BD_FAIL! EQU 0 set "BD_PARSE_OK=0"
)
if "!BD_PARSE_OK!"=="0" (
  set "RESULTAAT_!BD_DOMEIN!_BASIC_EXTENDED=MISLUKT"
  set /a RELEASE_FOUT+=1
) else (
  set "RESULTAAT_!BD_DOMEIN!_BASIC_EXTENDED=GESLAAGD"
)

for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%BD_LOG%' -Pattern '^Status EXPERIMENTAL_FQBN\s*:' | Select-Object -Last 1; if($m){$m.Line}"`) do set "SAMENVATTING_!BD_DOMEIN!_EXPERIMENTAL=%%L"
for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%BD_LOG%' -Pattern '^Status EXPERIMENTAL_FQBN\s*:\s*(.+)$' | Select-Object -Last 1; if($m){$m.Matches[0].Groups[1].Value.Trim()}"`) do set "BD_EXPERIMENTAL_STATUS=%%V"
if /I "!BD_EXPERIMENTAL_STATUS!"=="GESLAAGD" (
  set "RESULTAAT_!BD_DOMEIN!_EXPERIMENTAL=GESLAAGD"
) else (
  set "RESULTAAT_!BD_DOMEIN!_EXPERIMENTAL=NIET VOLLEDIG GESLAAGD"
)

powershell -NoProfile -Command "$l=Get-Content -LiteralPath '%BD_LOG%'; $s=($l | Select-String -SimpleMatch '[STATISCHE CONFIGURATIECONTROLES]' | Select-Object -Last 1).LineNumber; if(-not $s){exit 1}; $e=($l | Select-String -Pattern '^\[(?:OK|FOUT)\]' | Where-Object {$_.LineNumber -gt $s} | Select-Object -First 1).LineNumber; if(-not $e){$e=$l.Count+1}; $blok=$l[$s..($e-2)]; if($blok | Where-Object {$_ -match '^FOUT:'}){exit 1}else{exit 0}" >nul 2>&1
if errorlevel 1 (
  set "RESULTAAT_!BD_DOMEIN!_STATISCH=MISLUKT"
  set /a RELEASE_FOUT+=1
) else (
  set "RESULTAAT_!BD_DOMEIN!_STATISCH=GESLAAGD"
)
for %%Z in (!BD_DOMEIN!) do set "SAMENVATTING_%%Z_STATISCH=STATISCH: !RESULTAAT_%%Z_STATISCH!"

for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%BD_LOG%' -Pattern '^Arduino LINT is ' | Select-Object -Last 1; if($m){$m.Line}"`) do set "SAMENVATTING_!BD_DOMEIN!_LINT=%%L"
for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%BD_LOG%' -Pattern '^Arduino LINT is (geslaagd|mislukt)\.$' | Select-Object -Last 1; if($m){if($m.Matches[0].Groups[1].Value -eq 'geslaagd'){'GESLAAGD'}else{'MISLUKT'}}"`) do set "BD_LINT=%%V"
if /I "!BD_LINT!"=="GESLAAGD" (
  set "RESULTAAT_!BD_DOMEIN!_LINT=GESLAAGD"
) else (
  set "RESULTAAT_!BD_DOMEIN!_LINT=MISLUKT"
  set "RELEASE_LINT=MISLUKT"
  set /a RELEASE_FOUT+=1
)
exit /b

:BeoordeelOngeldig
set "BO_LOG=%~1"
set "BO_RESULTAAT=%~2"
set "BO_SAMENVATTING=%~3"
for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%BO_LOG%' -Pattern '^Totaal ' | Select-Object -Last 1; if($m){$m.Line}"`) do set "%BO_SAMENVATTING%=%%L"
powershell -NoProfile -Command "$m=Select-String -LiteralPath '%BO_LOG%' -Pattern '^Totaal\s+(\d+)\s+-\s+OK\s+(\d+)\s+-\s+FOUT\s+(\d+)$' | Select-Object -Last 1; if(-not $m){exit 1}; $t=[int]$m.Matches[0].Groups[1].Value; $o=[int]$m.Matches[0].Groups[2].Value; $f=[int]$m.Matches[0].Groups[3].Value; if($t -eq $o -and $f -eq 0){exit 0}else{exit 1}" >nul 2>&1
if errorlevel 1 (
  set "%BO_RESULTAAT%=MISLUKT"
  set /a RELEASE_FOUT+=1
) else (
  set "%BO_RESULTAAT%=GESLAAGD"
)
exit /b

:BeoordeelGeheugen
set "BG_LOG=%~1"
set "BG_RESULTAAT=%~2"
set "BG_AANTAL=%~3"
set "BG_FOUTEN=%~4"
set "BG_COUNT=0"
set "BG_INVALID=0"
for /f "tokens=1,2 delims=|" %%A in ('powershell -NoProfile -Command "$rows=Get-Content -LiteralPath '%BG_LOG%' | Where-Object {$_ -match '^\[GEHEUGEN\]'}; $bad=0; foreach($line in $rows){ if($line -notmatch '^\[GEHEUGEN\]\s+FQBN=(.*?) \| BOARD_VERSION=(.*?) \| Bestand=(.*?) \| Test=(.*?) \| Programma=(.*?) \| RAM=(.*?) \| Status=(.*?)$'){ $bad++; continue }; $prog=$matches[5].Trim(); $ram=$matches[6].Trim(); $status=$matches[7].Trim(); if($status -eq 'OK'){ if($prog -eq 'NIET_GEVONDEN' -or $ram -eq 'NIET_GEVONDEN'){ $bad++ } } elseif($status -eq 'VERWACHTE_GEHEUGENBEPERKING'){ } else { $bad++ } }; Write-Output ($rows.Count.ToString()+'|'+$bad.ToString())"') do (
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
for %%D in (%DOMEINEN%) do (
  echo.
  echo ============================================================
  echo GEHEUGENGEBRUIK - TestLibrary%%D-v2.0.0.cmd
  echo ============================================================
  call :SchrijfGeheugentabel "!LOG_%%D!"
)
exit /b

:SchrijfGeheugentabel
if not exist "%~1" (
  echo GEEN LOGBESTAND AANWEZIG
  exit /b
)
powershell -NoProfile -Command "$rows = Get-Content -LiteralPath '%~1' | ForEach-Object { if ($_ -match '^\[GEHEUGEN\]\s+FQBN=(.*?) \| BOARD_VERSION=(.*?) \| Bestand=(.*?) \| Test=(.*?) \| Programma=(.*?) \| RAM=(.*?) \| Status=(.*?)$') { [pscustomobject]@{ Board=($matches[1] + ' / ' + $matches[2]); Bestand=$matches[3]; Test=$matches[4]; Programma=$matches[5]; RAM=$matches[6]; Status=$matches[7] } } }; if (-not $rows) { Write-Output 'Geen geheugenrecords gevonden.'; exit }; Write-Output 'Board | Bestand / example | Test / configuratie | Programma | RAM | Status'; Write-Output '----- | ----------------- | ------------------- | --------- | --- | ------'; foreach ($r in $rows) { Write-Output ('{0} | {1} | {2} | {3} | {4} | {5}' -f $r.Board,$r.Bestand,$r.Test,$r.Programma,$r.RAM,$r.Status) }"
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
for %%D in (%DOMEINEN%) do (
  if not "!STATUS_%%D!"=="VOLLEDIG AFGEROND" (
    echo.
    echo TestLibrary%%D-v2.0.0.cmd
    echo   Testcyclus : !STATUS_%%D!
    if "!STATUS_%%D!"=="ONTBREEKT" echo   Reden      : TestLibrary%%D-v2.0.0.txt ontbreekt.
    if "!STATUS_%%D!"=="ONVOLLEDIG / AFGEBROKEN" echo   Reden      : log bevat geen eindmarker.
    set "FOUTDETAILS_GEVONDEN=1"
  ) else (
    if not "!RESULTAAT_%%D_BASIC_EXTENDED!"=="GESLAAGD" (
      call :SchrijfFoutenUitLog "!LOG_%%D!" "TestLibrary%%D-v2.0.0.cmd - BASIC + EXTENDED" "BASIC_EXTENDED"
      set "FOUTDETAILS_GEVONDEN=1"
    )
    if not "!RESULTAAT_%%D_STATISCH!"=="GESLAAGD" (
      call :SchrijfFoutenUitLog "!LOG_%%D!" "TestLibrary%%D-v2.0.0.cmd - STATISCH" "STATISCH"
      set "FOUTDETAILS_GEVONDEN=1"
    )
    if not "!RESULTAAT_%%D_LINT!"=="GESLAAGD" (
      call :SchrijfFoutenUitLog "!LOG_%%D!" "TestLibrary%%D-v2.0.0.cmd - LINT" "ALLE"
      set "FOUTDETAILS_GEVONDEN=1"
    )
    if not "!RESULTAAT_%%D_GEHEUGEN!"=="GESLAAGD" (
      echo.
      echo TestLibrary%%D-v2.0.0.cmd - GEHEUGENGEGEVENS
      echo ------------------------------------------------------------
      if "!RESULTAAT_%%D_GEHEUGEN!"=="ONTBREEKT" echo   Reden: log bevat geen [GEHEUGEN]-records.
      if "!RESULTAAT_%%D_GEHEUGEN!"=="MISLUKT" echo   Reden: !GEHEUGEN_%%D_FOUTEN! geheugenrecord^(s^) bevatten ontbrekende Programma/RAM-gegevens of een ongeldige status.
      set "FOUTDETAILS_GEVONDEN=1"
    )
  )

  if not "!STATUS_%%D_ONGELDIG!"=="VOLLEDIG AFGEROND" (
    echo.
    echo TestLibrary%%DOngeldig-v2.0.0.cmd
    echo   Testcyclus : !STATUS_%%D_ONGELDIG!
    if "!STATUS_%%D_ONGELDIG!"=="ONTBREEKT" echo   Reden      : TestLibrary%%DOngeldig-v2.0.0.txt ontbreekt.
    if "!STATUS_%%D_ONGELDIG!"=="ONVOLLEDIG / AFGEBROKEN" echo   Reden      : log bevat geen eindmarker.
    set "FOUTDETAILS_GEVONDEN=1"
  ) else if not "!RESULTAAT_%%D_ONGELDIG!"=="GESLAAGD" (
    call :SchrijfFoutenUitLog "!LOG_%%D_ONGELDIG!" "TestLibrary%%DOngeldig-v2.0.0.cmd" "ALLE"
    set "FOUTDETAILS_GEVONDEN=1"
  )
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
powershell -NoProfile -Command "$l=Get-Content -LiteralPath '%~1'; $cat='%~3'; $hits=@(); for($i=0; $i -lt $l.Count; $i++){ $line=$l[$i]; $hit=$false; if($cat -eq 'BASIC_EXTENDED'){ if($line -match '^\[FOUT\]\[(BASIC|EXTENDED)\]'){ $hit=$true } } elseif($cat -eq 'STATISCH'){ if($line -match '^\[FOUT\](?!\[)'){ $hit=$true } } else { if($line -match '^\[FOUT\]' -or $line -match '^FOUT:' -or $line -eq 'COMPILATIEFOUT' -or $line -match '^Arduino LINT.*(?:MISLUKT|FOUT)'){ $hit=$true } }; if($hit){ $hits += $i } }; if($hits.Count -eq 0){ $sum=$l | Where-Object { $_ -match 'FOUT [1-9][0-9]*$' -or $_ -match 'Onverwacht mislukt\s*:\s*[1-9]' } | Select-Object -Last 5; if($sum){$sum}else{'Geen afzonderlijke foutregel gevonden; raadpleeg de volledige log hieronder in TestLibraryStatusReport.txt.'}; exit }; foreach($i in $hits){ Write-Output $l[$i]; $detail=@(); $max=[Math]::Min($l.Count-1,$i+30); for($j=$i+1;$j -le $max;$j++){ if($l[$j] -match '^\[OK\]' -or $l[$j] -match '^\[FOUT\]' -or $l[$j] -match '^-{20,}$'){ break }; if($l[$j] -match '(?i)(fatal error:|error:|#error|undefined reference|collect2:|compilation terminated|failed|mislukt|onverwacht)'){ $detail += $l[$j]; if($detail.Count -ge 4){break} } }; if($detail.Count -gt 0){ $detail | ForEach-Object { Write-Output ('  Reden: ' + $_) } } else { Write-Output '  Reden: zie bijbehorende compile-uitvoer in de volledige log.' }; Write-Output '' }"
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
