@echo off
cls
setlocal EnableExtensions EnableDelayedExpansion

set "REPORT=TestLibraryStatusReport.txt"
set "MARKER==== TEST VOLLEDIG AFGEROND ==="
set "LOG1=TestLibraryGereleased.txt"
set "LOG2=TestLibraryGereleasedOngeldig.txt"
set "LOG3=TestLibraryNieuw.txt"
set "LOG4=TestLibraryNieuwOngeldig.txt"

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

set "SAMENVATTING1="
set "SAMENVATTING1_LINT="
set "SAMENVATTING2="
set "SAMENVATTING3_OFFICIEEL="
set "SAMENVATTING3_ACCEPTATIE="
set "SAMENVATTING3_STATISCH="
set "SAMENVATTING4="

set "LIB_VERSION=ONBEKEND"
set "RELEASE_DATUM=ONBEKEND"
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

if "!STATUS1!"=="VOLLEDIG AFGEROND" call :BeoordeelGereleased
if "!STATUS2!"=="VOLLEDIG AFGEROND" call :BeoordeelOngeldig "%LOG2%" RESULTAAT2 SAMENVATTING2
if "!STATUS3!"=="VOLLEDIG AFGEROND" call :BeoordeelNieuw
if "!STATUS4!"=="VOLLEDIG AFGEROND" call :BeoordeelOngeldig "%LOG4%" RESULTAAT4 SAMENVATTING4

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
  echo.
  echo TestLibraryNieuwOngeldig.cmd
  echo   Testcyclus              : !STATUS4!
  echo   Release-resultaat       : !RESULTAAT4!
  if defined SAMENVATTING4 echo   !SAMENVATTING4!
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
  echo Alle vier testcycli volledig       : !GELDIG! / 4
  echo.
)

call :SchrijfReleaseUpdate >> "%REPORT%"

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
call :SchrijfReleaseUpdate
echo.

if !GELDIG! EQU 4 if !RELEASE_FOUT! EQU 0 (
  del /q "%LOG1%" "%LOG2%" "%LOG3%" "%LOG4%" >nul 2>&1
  echo.
  echo TestLibraryStatusReport.txt is aangemaakt.
  echo De releasevalidatie is geslaagd.
  echo De vier individuele logbestanden zijn verwijderd.
) else (
  echo.
  echo TestLibraryStatusReport.txt is aangemaakt.
  echo De releasevalidatie is niet volledig geslaagd.
  echo.
  echo STATUS TESTCYCLI:
  echo TestLibraryGereleased.cmd          : !STATUS1!
  echo TestLibraryGereleasedOngeldig.cmd  : !STATUS2!
  echo TestLibraryNieuw.cmd               : !STATUS3!
  echo TestLibraryNieuwOngeldig.cmd       : !STATUS4!
  echo.
  echo OPNIEUW UIT TE VOEREN:
  if not "!STATUS1!"=="VOLLEDIG AFGEROND" echo TestLibraryGereleased.cmd
  if not "!STATUS2!"=="VOLLEDIG AFGEROND" echo TestLibraryGereleasedOngeldig.cmd
  if not "!STATUS3!"=="VOLLEDIG AFGEROND" echo TestLibraryNieuw.cmd
  if not "!STATUS4!"=="VOLLEDIG AFGEROND" echo TestLibraryNieuwOngeldig.cmd
  if "!STATUS1!"=="VOLLEDIG AFGEROND" if "!RESULTAAT1!"=="MISLUKT" echo TestLibraryGereleased.cmd
  if "!STATUS2!"=="VOLLEDIG AFGEROND" if "!RESULTAAT2!"=="MISLUKT" echo TestLibraryGereleasedOngeldig.cmd
  if "!STATUS3!"=="VOLLEDIG AFGEROND" if "!RESULTAAT3_OFFICIEEL!"=="MISLUKT" echo TestLibraryNieuw.cmd
  if "!STATUS3!"=="VOLLEDIG AFGEROND" if "!RESULTAAT3_STATISCH!"=="MISLUKT" echo TestLibraryNieuw.cmd
  if "!STATUS4!"=="VOLLEDIG AFGEROND" if "!RESULTAAT4!"=="MISLUKT" echo TestLibraryNieuwOngeldig.cmd
  echo.
  echo De overige geldige logbestanden blijven behouden en hoeven niet opnieuw uitgevoerd te worden.
)

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
for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG1%' -SimpleMatch -Pattern 'Totaal getest' | Select-Object -Last 1; if($m){$m.Line}"`) do set "SAMENVATTING1=%%L"
for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG1%' -SimpleMatch -Pattern 'Arduino LINT' | Select-Object -Last 1; if($m){$m.Line}"`) do set "SAMENVATTING1_LINT=%%L"

for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG1%' -Pattern '^Totaal getest\s*:\s*(\d+)' | Select-Object -Last 1; if($m){$m.Matches[0].Groups[1].Value}"`) do set "RELEASE_TOTAAL=%%V"
for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG1%' -Pattern '^OK bevonden\s*:\s*(\d+)' | Select-Object -Last 1; if($m){$m.Matches[0].Groups[1].Value}"`) do set "RELEASE_OK=%%V"
for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG1%' -Pattern '^Verwachte UNO R3-geheugenbeperkingen\s*:\s*(\d+)' | Select-Object -Last 1; if($m){$m.Matches[0].Groups[1].Value}"`) do set "RELEASE_EXPECTED_MEMORY=%%V"
for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG1%' -Pattern '^Onverwacht mislukt\s*:\s*(\d+)' | Select-Object -Last 1; if($m){$m.Matches[0].Groups[1].Value}"`) do set "RELEASE_UNEXPECTED_FAIL=%%V"
for /f "usebackq delims=" %%V in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG1%' -Pattern '^Arduino LINT\s*:\s*(.+)$' | Select-Object -Last 1; if($m){$m.Matches[0].Groups[1].Value.Trim()}"`) do set "RELEASE_LINT=%%V"

powershell -NoProfile -Command "if (Select-String -LiteralPath '%LOG1%' -Pattern 'Onverwacht mislukt\s*:\s*0' -Quiet) { exit 0 } else { exit 1 }" >nul 2>&1
if errorlevel 1 (
  set "RESULTAAT1=MISLUKT"
  set /a RELEASE_FOUT+=1
  exit /b
)

powershell -NoProfile -Command "if (Select-String -LiteralPath '%LOG1%' -Pattern 'Arduino LINT.*GESLAAGD|Arduino LINT is geslaagd' -Quiet) { exit 0 } else { exit 1 }" >nul 2>&1
if errorlevel 1 (
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

for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%BO_LOG%' -Pattern '^Totaal ' | Select-Object -Last 1; if($m){$m.Line}"`) do set "%BO_SAMENVATTING%=%%L"

powershell -NoProfile -Command "if (Select-String -LiteralPath '%BO_LOG%' -Pattern '^Totaal .* - OK .* - FOUT 0$' -Quiet) { exit 0 } else { exit 1 }" >nul 2>&1
if errorlevel 1 (
  set "%BO_RESULTAAT%=MISLUKT"
  set /a RELEASE_FOUT+=1
) else (
  set "%BO_RESULTAAT%=GESLAAGD"
)
exit /b

:BeoordeelNieuw
for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG3%' -Pattern '^OFFICIEEL:' | Select-Object -Last 1; if($m){$m.Line}"`) do set "SAMENVATTING3_OFFICIEEL=%%L"
for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG3%' -Pattern '^ACCEPTATIE:' | Select-Object -Last 1; if($m){$m.Line}"`) do set "SAMENVATTING3_ACCEPTATIE=%%L"
for /f "usebackq delims=" %%L in (`powershell -NoProfile -Command "$m=Select-String -LiteralPath '%LOG3%' -Pattern '^STATISCH:' | Select-Object -Last 1; if($m){$m.Line}"`) do set "SAMENVATTING3_STATISCH=%%L"

powershell -NoProfile -Command "if (Select-String -LiteralPath '%LOG3%' -Pattern '^OFFICIEEL: .* OK, 0 FOUT$' -Quiet) { exit 0 } else { exit 1 }" >nul 2>&1
if errorlevel 1 (
  set "RESULTAAT3_OFFICIEEL=MISLUKT"
  set /a RELEASE_FOUT+=1
) else (
  set "RESULTAAT3_OFFICIEEL=GESLAAGD"
)

powershell -NoProfile -Command "if (Select-String -LiteralPath '%LOG3%' -Pattern '^STATISCH: 0 FOUT$' -Quiet) { exit 0 } else { exit 1 }" >nul 2>&1
if errorlevel 1 (
  set "RESULTAAT3_STATISCH=MISLUKT"
  set /a RELEASE_FOUT+=1
) else (
  set "RESULTAAT3_STATISCH=GESLAAGD"
)

powershell -NoProfile -Command "if (Select-String -LiteralPath '%LOG3%' -Pattern '^ACCEPTATIE: .* OK, 0 FOUT(?: - GEEN RELEASE-IMPACT)?$' -Quiet) { exit 0 } else { exit 1 }" >nul 2>&1
if errorlevel 1 (
  set "RESULTAAT3_ACCEPTATIE=NIET VOLLEDIG GESLAAGD"
) else (
  set "RESULTAAT3_ACCEPTATIE=GESLAAGD"
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
  echo ^| v!LIB_VERSION! ^| !RELEASE_DATUM! ^| GESLAAGD ^(!RELEASE_LINT!^) ^| !RELEASE_TOTAAL! ^| !RELEASE_OK! ^| !RELEASE_EXPECTED_MEMORY! ^| !RELEASE_UNEXPECTED_FAIL! ^| GESLAAGD ^|
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
