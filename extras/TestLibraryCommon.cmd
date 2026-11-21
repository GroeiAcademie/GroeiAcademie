@echo off
if /I "%~1"=="--VOER_DEZE_TEST_UIT" goto :VOER_DEZE_TEST_UIT

set BASIC_FQBN="arduino:renesas_uno:minima" "arduino:renesas_uno:unor4wifi"
set BASIC_FQBN_MAPPING_BOARDS="BOARD_UNO_R4_MINIMA" "BOARD_UNO_R4_WIFI"

set EXTENDED_FQBN="esp32:esp32:d1_uno32" "esp32:esp32:esp32s3" "rp2040:rp2040:cytron_maker_uno_rp2040" "STMicroelectronics:stm32:Nucleo_64:pnum=NUCLEO_F401RE"
set EXTENDED_FQBN_MAPPING_BOARDS="BOARD_ESP32_D1_UNO_R32" "BOARD_ESP32S3_ARDI32|BOARD_ESP32S3_DEV" "BOARD_RP2040_CYTRON_MAKER_UNO" "BOARD_STM32F4_NUCLEO64_F401RE"

set EXPERIMENTAL_FQBN="arduino:zephyr:unoq"
set EXPERIMENTAL_FQBN_MAPPING_BOARDS="BOARD_UNO_Q"

set "REGRESSIETEST=FALSE"
set "NO_PAUSE=FALSE"
set "IS_LOGGED=FALSE"
set "SPLIT_TEST_PER_FQBN=FALSE"

set "LOG_BESTAND=%~dp0!BESTANDSNAAM!.txt"
set "CONTROLE_BESTAND=%~dp0!BESTANDSNAAM!.ino"
set "TEST_SCRIPT_BESTAND=%~dp0!BESTANDSNAAM!.cmd"

for %%A in ("%~1" "%~2" "%~3" "%~4") do (
    if /I "%%~A"=="REGRESSIETEST=TRUE" set "REGRESSIETEST=TRUE"
    if /I "%%~A"=="--no-pause" set "NO_PAUSE=TRUE"
    if /I "%%~A"=="--logged" set "IS_LOGGED=TRUE"
    if /I "%%~A"=="--split-test-per-fqbn" set "SPLIT_TEST_PER_FQBN=TRUE"
)

rem Alleen een tab van --split-test-per-fqbn krijgt een eigen log, met de FQBN in de naam en _ in plaats van : en =.
rem Zonder --split-test-per-fqbn blijft het log altijd de ene gewone txt, ook met BEPERK_TEST_TOT_FQBN.
if /I "!SPLIT_TEST_PER_FQBN!"=="TRUE" if defined BEPERK_TEST_TOT_FQBN for /f "delims=" %%S in ('powershell -NoProfile -Command "$env:BEPERK_TEST_TOT_FQBN -replace '[:=]','_'"') do set "LOG_BESTAND=%~dp0!BESTANDSNAAM!_%%S.txt"

rem Splitsen gebeurt voor de opstart met Tee-Object, zodat het gewone log vrij blijft voor het samenvoegen.
if /I "!SPLIT_TEST_PER_FQBN!"=="TRUE" if /I "!IS_LOGGED!"=="FALSE" if not defined BEPERK_TEST_TOT_FQBN goto :VOER_SPLIT_TEST_PER_FQBN_UIT

if /I "!IS_LOGGED!"=="FALSE" (
    cls
    if exist "%LOG_BESTAND%" del /q "%LOG_BESTAND%" >nul 2>&1
    powershell -NoProfile -Command "& { & '%TEST_SCRIPT_BESTAND%' --logged %* 2>&1 | Tee-Object -FilePath '%LOG_BESTAND%'; exit $LASTEXITCODE }"
    exit /b !errorlevel!
)

goto :MAIN_SCRIPT

:VOER_SPLIT_TEST_PER_FQBN_UIT
rem Start per FQBN een tab met hetzelfde testscript, beperkt tot die FQBN via BEPERK_TEST_TOT_FQBN.
rem De tab geeft zelf ook --split-test-per-fqbn mee, zodat alleen die tab een eigen log krijgt.
rem Elke tab schrijft na het script zijn foutcode in een eigen statusbestand.
rem REGRESSIETEST gaat expliciet als argument mee; REGRESSIE_PADEN blijft hetzelfde als bij een gewone run.
for /f %%I in ('powershell -NoProfile -Command "[guid]::NewGuid().ToString('N')"') do set "GA_RUN_ID=%%I"
if exist "!LOG_BESTAND!" del /q "!LOG_BESTAND!" >nul 2>&1
powershell -NoProfile -Command "$ErrorActionPreference='Stop'; $q=[char]34; $map=$env:TEMP+'\GA_Split_'+$env:GA_RUN_ID; New-Item -ItemType Directory -Path $map -Force | Out-Null; $dir=Split-Path -Parent $env:TEST_SCRIPT_BESTAND; $i=0; foreach($k in @('BASIC','EXTENDED','EXPERIMENTAL')){foreach($x in [regex]::Matches([Environment]::GetEnvironmentVariable($k+'_FQBN'),'\x22([^\x22]+)\x22')){$i++; $fq=$x.Groups[1].Value; $a='-w 0 new-tab --title {0}{1}: {2}{0} -d {0}{3}{0} powershell.exe -NoLogo -NoProfile -NoExit -Command {0}$env:BEPERK_TEST_TOT_FQBN=''{2}''\; $env:REGRESSIE_PADEN=''{7}''\; & ''{4}'' --no-pause --split-test-per-fqbn REGRESSIETEST={6}\; Set-Content -LiteralPath ''{5}'' -Value $LASTEXITCODE{0}' -f $q,($env:BESTANDSNAAM -replace '^TestLibrary','' -replace '-v2\.0\.0$',''),$fq,$dir,$env:TEST_SCRIPT_BESTAND,($map+'\'+$i+'.status'),$env:REGRESSIETEST,($env:REGRESSIE_PADEN -replace ';','\;'); Start-Process -FilePath 'wt.exe' -ArgumentList $a | Out-Null; Write-Host ('[SPLIT_TEST_PER_FQBN] Gestart '+$i+': '+$fq)}}; if($i -eq 0){throw 'Geen FQBN gevonden.'}"
if errorlevel 1 (
    echo [FOUT] De tabs per FQBN konden niet worden gestart.
    exit /b 1
)
call :VOEG_SPLIT_LOGS_SAMEN
set /A TOTAL_FAIL=!errorlevel!
rmdir /s /q "%TEMP%\GA_Split_!GA_RUN_ID!" >nul 2>&1
if /I "!NO_PAUSE!"=="FALSE" (
    <nul set /p =[EINDE] Testen voltooid. Druk op een toets om af te sluiten... & pause >nul
    echo.
) else (
    echo [EINDE] Testen voltooid. Druk op een toets om af te sluiten...
)
exit /b !TOTAL_FAIL!

:VOEG_SPLIT_LOGS_SAMEN
rem Wacht tot elke tab een statusbestand heeft, zonder tijdslimiet.
rem Voeg daarna de logs per FQBN samen in het gewone log, met een TESTRESULTAAT met de opgetelde tellers.
rem De regel [EINDE] komt alleen in het gewone log als alle logs per FQBN volledig zijn.
powershell -NoProfile -Command "$map=$env:TEMP+'\GA_Split_'+$env:GA_RUN_ID; $fq=@(); foreach($k in @('BASIC','EXTENDED','EXPERIMENTAL')){foreach($x in [regex]::Matches([Environment]::GetEnvironmentVariable($k+'_FQBN'),'\x22([^\x22]+)\x22')){$fq+=$x.Groups[1].Value}}; Write-Host ('[SPLIT_TEST_PER_FQBN] Wachten op '+$fq.Count+' statusbestanden in '+$map); while(@(Get-ChildItem -LiteralPath $map -Filter '*.status' -File -ErrorAction SilentlyContinue).Count -lt $fq.Count){Start-Sleep -Seconds 2}; $basis=$env:LOG_BESTAND.Substring(0,$env:LOG_BESTAND.Length-4); $einde='[EINDE] Testen voltooid. Druk op een toets om af te sluiten...'; $over=New-Object System.Collections.Generic.List[string]; $uit=New-Object System.Collections.Generic.List[string]; $som=@{}; foreach($g in @('BASIC','EXTENDED','EXPERIMENTAL')){$som[$g]=@(0,0,0,0)}; $onvolledig=0; $gefaald=$false; $lintfout=$false; $statisch=New-Object System.Collections.Generic.List[string]; for($i=0;$i -lt $fq.Count;$i++){$log=$basis+'_'+($fq[$i] -replace '[:=]','_')+'.txt'; $code=@(Get-Content -LiteralPath ($map+'\'+($i+1)+'.status'))[0]; $over.Add('[SPLIT_TEST_PER_FQBN] '+$fq[$i]+' | foutcode '+$code+' | '+$log); if(([string]$code).Trim() -ne '0'){$gefaald=$true}; if(-not (Test-Path -LiteralPath $log -PathType Leaf)){$onvolledig++; $over.Add('[SPLIT_TEST_PER_FQBN] Log ontbreekt: '+$log); continue}; $r=@(Get-Content -LiteralPath $log); $raw=$r -join [char]10; if(-not ($r -contains $einde)){$onvolledig++; $over.Add('[SPLIT_TEST_PER_FQBN] Log onvolledig: '+$log)}; $uit.Add('============================================================'); $uit.Add('[SPLIT_TEST_PER_FQBN] '+$fq[$i]); $uit.Add('============================================================'); foreach($l in $r){if($l -ne $einde){$uit.Add($l)}}; if(-not ($r -contains 'Arduino LINT is geslaagd.')){$lintfout=$true}; $s=[array]::IndexOf($r,'[STATISCHE CONFIGURATIECONTROLES]'); if($s -lt 0){$statisch.Add('FOUT: geen statische controles in '+$log)}else{for($j=$s+1;$j -lt $r.Count;$j++){if($r[$j] -match '^\[(?:OK|FOUT)\]'){break}; if($r[$j] -match '^FOUT:' -and -not $statisch.Contains($r[$j])){$statisch.Add($r[$j])}}}; foreach($g in @('BASIC','EXTENDED','EXPERIMENTAL')){$m=[regex]::Matches($raw,'(?ms)^\['+$g+'_FQBN\].*?^Totaal getest\s*:\s*(\d+).*?^OK bevonden\s*:\s*(\d+).*?^Verwachte geheugenbeperkingen\s*:\s*(\d+).*?^Onverwacht mislukt\s*:\s*(\d+)'); if($m.Count -eq 0){$onvolledig++; continue}; $y=$m[$m.Count-1]; for($j=0;$j -lt 4;$j++){$som[$g][$j]+=[int]$y.Groups[$j+1].Value}}; if($raw -match '(?m)^Eindstatus BASIC \+ EXTENDED\s*:\s*GEFAALD'){$gefaald=$true}}; $st=@{}; foreach($g in @('BASIC','EXTENDED','EXPERIMENTAL')){$st[$g]=$(if($som[$g][3] -gt 0){'GEFAALD'}else{'GESLAAGD'})}; if($onvolledig -gt 0 -or $lintfout -or $statisch.Count -gt 0 -or $som['BASIC'][3] -gt 0 -or $som['EXTENDED'][3] -gt 0){$gefaald=$true}; $uit.Add('============================================================'); $uit.Add('[SPLIT_TEST_PER_FQBN] LINT en statische controles van alle tabs'); $uit.Add('============================================================'); $uit.Add($(if($lintfout){'Arduino LINT is mislukt.'}else{'Arduino LINT is geslaagd.'})); $uit.Add('------------------------------------------------------------'); $uit.Add('[STATISCHE CONFIGURATIECONTROLES]'); foreach($l in $statisch){$uit.Add($l)}; $eind=$(if($gefaald){'GEFAALD'}else{'GESLAAGD'}); $rap=New-Object System.Collections.Generic.List[string]; $rap.Add(''); $rap.Add('============================================================'); $rap.Add('TESTRESULTAAT'); $rap.Add('============================================================'); $rap.Add('Script                               : '+$env:BESTANDSNAAM); $rap.Add('CONTROLE_MODUS                       : '+$env:CONTROLE_MODUS); if($env:CONTROLE_MODUS -eq 'EXCLUDE'){$rap.Add('REGRESSIETEST                         : '+$env:REGRESSIETEST)}; $rap.Add(''); foreach($g in @('BASIC','EXTENDED')){$rap.Add('['+$g+'_FQBN]'); $rap.Add('Totaal getest                        : '+$som[$g][0]); $rap.Add('OK bevonden                          : '+$som[$g][1]); $rap.Add('Verwachte geheugenbeperkingen        : '+$som[$g][2]); $rap.Add('Onverwacht mislukt                   : '+$som[$g][3]); $rap.Add('------------------------------------------------------------'); $rap.Add($(if($g -eq 'BASIC'){'Tussenstatus BASIC_FQBN               : '}else{'Tussenstatus EXTENDED_FQBN           : '})+$st[$g]); $rap.Add('')}; $rap.Add('============================================================'); $rap.Add('Eindstatus BASIC + EXTENDED          : '+$eind); $rap.Add('============================================================'); $rap.Add(''); $rap.Add('[EXPERIMENTAL_FQBN]'); $rap.Add('Totaal getest                        : '+$som['EXPERIMENTAL'][0]); $rap.Add('OK bevonden                          : '+$som['EXPERIMENTAL'][1]); $rap.Add('Verwachte geheugenbeperkingen        : '+$som['EXPERIMENTAL'][2]); $rap.Add('Onverwacht mislukt                   : '+$som['EXPERIMENTAL'][3]); $rap.Add('------------------------------------------------------------'); $rap.Add('Status EXPERIMENTAL_FQBN             : '+$st['EXPERIMENTAL']); $rap.Add('============================================================'); $rap.Add('EINDE'); $rap.Add('============================================================'); $alles=New-Object System.Collections.Generic.List[string]; $alles.AddRange($over); $alles.AddRange($uit); $alles.AddRange($rap); if($onvolledig -eq 0){$alles.Add($einde)}; [IO.File]::WriteAllLines($env:LOG_BESTAND,[string[]]$alles.ToArray(),[Text.Encoding]::Unicode); foreach($l in $over){Write-Host $l}; foreach($l in $rap){Write-Host $l}; Write-Host ('[SPLIT_TEST_PER_FQBN] Samengevoegd log: '+$env:LOG_BESTAND); $c=$som['BASIC'][3]+$som['EXTENDED'][3]; if($gefaald -and $c -eq 0){$c=1}; exit $c"
exit /b !errorlevel!

:MAIN_SCRIPT
setlocal enabledelayedexpansion
for /f %%I in ('powershell -NoProfile -Command "[guid]::NewGuid().ToString('N')"') do set "GA_RUN_ID=%%I"
pushd "%~dp0.."

:: Bepaal welk pad bestaat
:: Zoek arduino-cli via PATH; anders via extras\LokalePaden.cmd (lokaal, niet gedeeld - zie extras\LokalePaden_template.cmd om aan te maken). 
:: Geen paden van specifieke personen/machines in dit gedeelde script.
where arduino-cli >nul 2>&1
if exist "extras\LokalePaden.cmd" (
    call "extras\LokalePaden.cmd"
) else (
    where arduino-cli >nul 2>&1
    if not errorlevel 1 (
        for /f "delims=" %%P in ('where arduino-cli') do set "CLI_PATH=%%~dpnP"
    ) else (
        echo arduino-cli niet gevonden via PATH, en extras\LokalePaden.cmd bestaat niet.
        exit /b 1
    )
)

set /A BASIC_TESTS=0
set /A BASIC_OK=0
set /A BASIC_EXPECTED_MEMORY=0
set /A BASIC_FAIL=0
set /A EXTENDED_TESTS=0
set /A EXTENDED_OK=0
set /A EXTENDED_EXPECTED_MEMORY=0
set /A EXTENDED_FAIL=0
set /A EXPERIMENTAL_TESTS=0
set /A EXPERIMENTAL_OK=0
set /A EXPERIMENTAL_EXPECTED_MEMORY=0
set /A EXPERIMENTAL_FAIL=0
set /A LINT_FAIL=0
set /A STATIC_FAIL=0

call :CONTROLEER_TESTLIJST
if errorlevel 1 (
    if /I "!NO_PAUSE!"=="FALSE" pause
    popd
    exit /b 1
)

echo ------------------------------------------------------------
"%LINT_PATH%" --library-manager update
if errorlevel 1 (
    set /A LINT_FAIL=1
    echo Arduino LINT is mislukt.
) else (
    echo Arduino LINT is geslaagd.
)

echo ------------------------------------------------------------
echo [STATISCHE CONFIGURATIECONTROLES]
set "STATIC_CHECK_LOG=%TEMP%\GroeiAcademieStaticCheck_!GA_RUN_ID!.txt"
findstr /S /N /R /C:"^[ ]*#define[ ]*PIN_TOETS_[1-4][ ]*D[0-9][0-9]*" examples\*.ino >"!STATIC_CHECK_LOG!"
if not errorlevel 1 (
    echo FOUT: rechtstreekse D-pinmapping voor PIN_TOETS gevonden:
    type "!STATIC_CHECK_LOG!"
    set /A STATIC_FAIL+=1
)
del "!STATIC_CHECK_LOG!" >nul 2>&1

findstr /C:"PCF8574" library.properties >nul || (
    echo FOUT: PCF8574 dependency ontbreekt in library.properties.
    set /A STATIC_FAIL+=1
)
findstr /C:"IRremote" library.properties >nul || (
    echo FOUT: IRremote dependency ontbreekt in library.properties.
    set /A STATIC_FAIL+=1
)
findstr /B /C:"INPUT_TYPE_HX1838	" keywords.txt >nul || (
    echo FOUT: keywords.txt mist INPUT_TYPE_HX1838
    set /A STATIC_FAIL+=1
)

for %%K in (InputConfigureren OpvragenHuidigeToetsAanslag OpvragenHuidigeToetsAanslagen OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag InputResultaat InputResultaten InputKanaal StatusOpvragenToetsAanslagen MAX_AANTAL_SIMULTANE_TOETSAANSLAGEN INPUT_TYPE_PCF8574) do (
    findstr /B /C:"%%K	" keywords.txt >nul || (
        echo FOUT: keywords.txt mist %%K
        set /A STATIC_FAIL+=1
    )
)

rem Een onbekende BEPERK_TEST_TOT_FQBN zou nul compilaties geven en toch slagen.
if defined BEPERK_TEST_TOT_FQBN (
    powershell -NoProfile -Command "$f=@(); foreach($k in @('BASIC','EXTENDED','EXPERIMENTAL')){foreach($x in [regex]::Matches([Environment]::GetEnvironmentVariable($k+'_FQBN'),'\x22([^\x22]+)\x22')){$f+=$x.Groups[1].Value}}; if($f -contains $env:BEPERK_TEST_TOT_FQBN){exit 0}else{exit 1}"
    if errorlevel 1 (
        echo FOUT: BEPERK_TEST_TOT_FQBN is geen FQBN uit BASIC_FQBN, EXTENDED_FQBN of EXPERIMENTAL_FQBN: !BEPERK_TEST_TOT_FQBN!
        set /A STATIC_FAIL+=1
    )
)

set "TOTAAL_COMPILATIES="
for /f %%N in ('powershell -NoProfile -Command "$b=@{};foreach($k in @('BASIC','EXTENDED','EXPERIMENTAL')){$f=[regex]::Matches([Environment]::GetEnvironmentVariable($k+'_FQBN'),'\x22([^\x22]+)\x22');$m=[regex]::Matches([Environment]::GetEnvironmentVariable($k+'_FQBN_MAPPING_BOARDS'),'\x22([^\x22]+)\x22');$a=@();for($i=0;$i -lt $f.Count;$i++){for($j=0;$j -lt $m[$i].Groups[1].Value.Split([char]124).Count;$j++){$a+=$f[$i].Groups[1].Value}}$b['BIJ_'+$k+'_FQBN']=$a};$b['BIJ_ALLE_FQBN']=$b['BIJ_BASIC_FQBN']+$b['BIJ_EXTENDED_FQBN']+$b['BIJ_EXPERIMENTAL_FQBN'];$p=@{};$n=0;foreach($l in [IO.File]::ReadAllLines($env:TEST_SCRIPT_BESTAND)){if($l -match '^set ([A-Z_]+)=(.*)$'){$p[$matches[1]]=$matches[2].Trim([char]34)}elseif($l -match '^call .*--VOER_DEZE_TEST_UIT'){$t=$p['TEST_TYPE'];$ok=($env:CONTROLE_MODUS -eq 'INCLUDE' -and $t -in @('NIEUW','EXPERIMENTEEL')) -or ($env:CONTROLE_MODUS -eq 'EXCLUDE' -and ($t -eq 'GERELEASED' -or ($t -eq 'REGRESSIE' -and $env:REGRESSIETEST -eq 'TRUE')));if($ok -and $t -eq 'REGRESSIE'){$ok=$false;foreach($r in ($env:REGRESSIE_PADEN -split '\s+')){if($r -and ([string]$p['TEST_INO']).StartsWith($r,[StringComparison]::OrdinalIgnoreCase)){$ok=$true;break}}};if($ok){$skip=','+$p['WELKE_FQBN_SKIPPEN']+',';$inc=','+$p['WELKE_FQBN_INCOMPATIBLE']+',';foreach($fq in $b[$p['WELKE_FQBN_TESTEN']]){if(-not $skip.Contains(','+$fq+',') -and -not $inc.Contains(','+$fq+',') -and (-not $env:BEPERK_TEST_TOT_FQBN -or $fq -eq $env:BEPERK_TEST_TOT_FQBN)){$n++}}};$p['WELKE_FQBN_SKIPPEN']='';$p['WELKE_FQBN_INCOMPATIBLE']=''}};$n"') do set "TOTAAL_COMPILATIES=%%N"
call "!TEST_SCRIPT_BESTAND!" --TESTLIJST
call :RAPPORT
set /A TOTAL_FAIL=BASIC_FAIL+EXTENDED_FAIL+LINT_FAIL+STATIC_FAIL
popd
if /I "!NO_PAUSE!"=="FALSE" (
    <nul set /p =[EINDE] Testen voltooid. Druk op een toets om af te sluiten... & pause >nul
    echo.
) else (
    echo [EINDE] Testen voltooid. Druk op een toets om af te sluiten...
)
exit /b !TOTAL_FAIL!

:CONTROLEER_TESTLIJST
set "LIJST_DIR=%TEMP%\GA_Testlijst_Dir_!GA_RUN_ID!.txt"
set "LIJST_CMD=%TEMP%\GA_Testlijst_Cmd_!GA_RUN_ID!.txt"
set "VERSCHIL_DIR=%TEMP%\GA_Testlijst_MistCmd_!GA_RUN_ID!.txt"
set "VERSCHIL_CMD=%TEMP%\GA_Testlijst_MistDir_!GA_RUN_ID!.txt"
set "TYPE_FOUT=%TEMP%\GA_TestType_Fout_!GA_RUN_ID!.txt"
set "TYPE_NIEUW=%TEMP%\GA_TestType_Nieuw_!GA_RUN_ID!.txt"
set "GA_CONTROLE_MODUS=!CONTROLE_MODUS!"
set "GA_CONTROLE_PADEN=!CONTROLE_PADEN!"
set "GA_TEST_SCRIPT=!TEST_SCRIPT_BESTAND!"

powershell -NoProfile -Command "$root=(Get-Location).Path; $mode=$env:GA_CONTROLE_MODUS; $paths=$env:GA_CONTROLE_PADEN -split ';' | Where-Object { $_ }; if($mode -eq 'INCLUDE'){ $r=@(); foreach($p in $paths){ if(Test-Path -LiteralPath $p){ $r += Get-ChildItem -LiteralPath $p -Recurse -Filter '*.ino' -File | ForEach-Object { $_.FullName.Substring($root.Length+1) } } } } elseif($mode -eq 'EXCLUDE'){ $r=Get-ChildItem -LiteralPath 'examples' -Recurse -Filter '*.ino' -File | ForEach-Object { $rel=$_.FullName.Substring($root.Length+1); $skip=$false; foreach($p in $paths){ if($rel.StartsWith($p,[System.StringComparison]::OrdinalIgnoreCase)){ $skip=$true; break } }; if(-not $skip){$rel} } } else { Write-Error ('Onbekende CONTROLE_MODUS: '+$mode); exit 2 }; $r | Sort-Object -Unique" >"!LIJST_DIR!"
if errorlevel 1 (
    echo FOUT: controlelijst van .ino-bestanden kon niet worden samengesteld.
    goto :CONTROLEER_TESTLIJST_FOUT
)

powershell -NoProfile -Command "Get-Content -LiteralPath $env:GA_TEST_SCRIPT | ForEach-Object { if($_ -match '^set TEST_INO=\x22(examples\\.*?\.ino)\x22$'){ $matches[1] } } | Sort-Object -Unique" >"!LIJST_CMD!"
set "GA_LIJST_DIR=!LIJST_DIR!"
set "GA_LIJST_CMD=!LIJST_CMD!"
set "GA_VERSCHIL_DIR=!VERSCHIL_DIR!"
set "GA_VERSCHIL_CMD=!VERSCHIL_CMD!"
powershell -NoProfile -Command "$a=@(Get-Content -LiteralPath $env:GA_LIJST_DIR -ErrorAction SilentlyContinue); $b=@(Get-Content -LiteralPath $env:GA_LIJST_CMD -ErrorAction SilentlyContinue); @($a | Where-Object { $_ -notin $b }) | Set-Content -LiteralPath $env:GA_VERSCHIL_DIR; @($b | Where-Object { -not (Test-Path -LiteralPath $_ -PathType Leaf) }) | Set-Content -LiteralPath $env:GA_VERSCHIL_CMD"

powershell -NoProfile -Command "$lines=Get-Content -LiteralPath $env:GA_TEST_SCRIPT; $ino=''; $naam=''; $type=''; $result=@(); foreach($l in $lines){ if($l -match '^set TEST_INO=\x22(.*)\x22$'){ $ino=$matches[1]; $naam=''; $type='' } elseif($l -match '^set TEST_NAAM=\x22(.*)\x22$'){ $naam=$matches[1] } elseif($l -match '^set TEST_TYPE=\x22(.*)\x22$'){ $type=$matches[1] } elseif($l -match '^call .*--VOER_DEZE_TEST_UIT'){ if($env:GA_CONTROLE_MODUS -eq 'INCLUDE'){ if($type -notin @('NIEUW','EXPERIMENTEEL')){ $result += ($ino+' | '+$naam+' | TEST_TYPE='+$type) } } elseif($env:GA_CONTROLE_MODUS -eq 'EXCLUDE'){ if($type -notin @('GERELEASED','REGRESSIE','NIEUW','EXPERIMENTEEL')){ $result += ($ino+' | '+$naam+' | TEST_TYPE='+$type) } }; $ino=''; $naam=''; $type='' } }; $result | Set-Content -LiteralPath $env:TYPE_FOUT"

powershell -NoProfile -Command "$lines=Get-Content -LiteralPath $env:GA_TEST_SCRIPT; $ino=''; $naam=''; $type=''; $result=@(); foreach($l in $lines){ if($l -match '^set TEST_INO=\x22(.*)\x22$'){ $ino=$matches[1]; $naam=''; $type='' } elseif($l -match '^set TEST_NAAM=\x22(.*)\x22$'){ $naam=$matches[1] } elseif($l -match '^set TEST_TYPE=\x22(.*)\x22$'){ $type=$matches[1] } elseif($l -match '^call .*--VOER_DEZE_TEST_UIT'){ if($env:GA_CONTROLE_MODUS -eq 'EXCLUDE' -and $type -in @('NIEUW','EXPERIMENTEEL')){ $result += ($ino+' | '+$naam) }; $ino=''; $naam=''; $type='' } }; $result | Set-Content -LiteralPath $env:TYPE_NIEUW"

set "TYPE_NIEUW_SIZE=0"
for %%A in ("!TYPE_NIEUW!") do set "TYPE_NIEUW_SIZE=%%~zA"
if !TYPE_NIEUW_SIZE! GTR 0 (
    echo ============================================================
    echo TEST_TYPE=NIEUW GEVONDEN IN !TEST_SCRIPT_NAAM!
    echo ============================================================
    type "!TYPE_NIEUW!"
    echo ============================================================
    echo GEEN ENKELE TEST WORDT GESTART
    goto :CONTROLEER_TESTLIJST_FOUT
)

set "TYPE_FOUT_SIZE=0"
for %%A in ("!TYPE_FOUT!") do set "TYPE_FOUT_SIZE=%%~zA"
if !TYPE_FOUT_SIZE! GTR 0 (
    echo ============================================================
    echo ONGELDIGE TEST_TYPE IN !TEST_SCRIPT_NAAM!
    echo ============================================================
    type "!TYPE_FOUT!"
    echo ============================================================
    echo GEEN ENKELE TEST WORDT GESTART
    goto :CONTROLEER_TESTLIJST_FOUT
)

if exist "!CONTROLE_BESTAND!" goto :CONTROLE_BESTAND_BESTAAT
>"!CONTROLE_BESTAND!" type nul

if exist "!VERSCHIL_DIR!" (
    for /f "usebackq delims=" %%I in ("!VERSCHIL_DIR!") do (
        >>"!CONTROLE_BESTAND!" echo set TEST_INO="%%I"
        >>"!CONTROLE_BESTAND!" echo set TEST_NAAM=""
        >>"!CONTROLE_BESTAND!" echo set TEST_FLAGS=" !EXTRA_FLAGS!"

        if /I "!CONTROLE_MODUS!"=="INCLUDE" (
            >>"!CONTROLE_BESTAND!" echo set TEST_TYPE="NIEUW"
            >>"!CONTROLE_BESTAND!" echo set WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"
        ) else (
            >>"!CONTROLE_BESTAND!" echo set TEST_TYPE="REGRESSIE"
            >>"!CONTROLE_BESTAND!" echo set WELKE_FQBN_TESTEN=""
        )

        >>"!CONTROLE_BESTAND!" echo call "%%~dp0TestLibraryCommon.cmd" --VOER_DEZE_TEST_UIT
        >>"!CONTROLE_BESTAND!" echo.
    )
)

for %%A in ("!VERSCHIL_CMD!") do if %%~zA GTR 0 (
    >>"!CONTROLE_BESTAND!" echo REM ============================================================
    >>"!CONTROLE_BESTAND!" echo REM TEST_INO staat in CMD maar ontbreekt in de directoryscan:
    for /f "usebackq delims=" %%I in ("!VERSCHIL_CMD!") do >>"!CONTROLE_BESTAND!" echo REM %%I
)

:CONTROLE_BESTAND_BESTAAT
set "CONTROLE_SIZE=0"
for %%A in ("!CONTROLE_BESTAND!") do set "CONTROLE_SIZE=%%~zA"
if !CONTROLE_SIZE! GTR 0 (
    echo ============================================================
    echo TESTLIJST NIET VOLLEDIG - GEEN ENKELE TEST WORDT GESTART
    echo ============================================================
    echo Vul het bestand aan, knip de ingevulde blokken en plak ze in :TESTLIJST:
    echo !CONTROLE_BESTAND!
    echo ------------------------------------------------------------
    type "!CONTROLE_BESTAND!"
    goto :CONTROLEER_TESTLIJST_FOUT
)

echo [CONTROLE TESTLIJST] OK - geen ontbrekende of ongeldige TEST_INO-vermeldingen.
del "!LIJST_DIR!" "!LIJST_CMD!" "!VERSCHIL_DIR!" "!VERSCHIL_CMD!" "!TYPE_FOUT!" "!TYPE_NIEUW!" >nul 2>&1
exit /b 0

:CONTROLEER_TESTLIJST_FOUT
del "!LIJST_DIR!" "!LIJST_CMD!" "!VERSCHIL_DIR!" "!VERSCHIL_CMD!" "!TYPE_FOUT!" "!TYPE_NIEUW!" >nul 2>&1
exit /b 1

:VOER_DEZE_TEST_UIT
set "TEST_INO_ACTIEF=!TEST_INO:"=!"
set "TEST_NAAM_ACTIEF=!TEST_NAAM:"=!"
set "TEST_FLAGS_ACTIEF=!TEST_FLAGS:"=!"
set "TEST_TYPE_ACTIEF=!TEST_TYPE:"=!"
set "WELKE_FQBN_TESTEN_ACTIEF=!WELKE_FQBN_TESTEN:"=!"
set "WELKE_FQBN_INCOMPATIBLE_ACTIEF=!WELKE_FQBN_INCOMPATIBLE:"=!"
set "WELKE_FQBN_SKIPPEN_ACTIEF=!WELKE_FQBN_SKIPPEN:"=!"
set "WELKE_FQBN_INCOMPATIBLE="
set "WELKE_FQBN_SKIPPEN="

rem EXPERIMENTEEL wordt ook gecompileerd, maar telt in de tellers EXPERIMENTAL_* en blokkeert de vrijgave niet.
if /I "!CONTROLE_MODUS!"=="INCLUDE" (
    if /I not "!TEST_TYPE_ACTIEF!"=="NIEUW" if /I not "!TEST_TYPE_ACTIEF!"=="EXPERIMENTEEL" exit /b 0
) else if /I "!CONTROLE_MODUS!"=="EXCLUDE" (
    if /I "!TEST_TYPE_ACTIEF!"=="NIEUW" exit /b 0
    if /I "!TEST_TYPE_ACTIEF!"=="REGRESSIE" if /I not "!REGRESSIETEST!"=="TRUE" exit /b 0
    if /I "!TEST_TYPE_ACTIEF!"=="REGRESSIE" if /I "!REGRESSIETEST!"=="TRUE" (
        set "REGRESSIE_PAD_GEVONDEN=FALSE"
        for %%R in (!REGRESSIE_PADEN!) do (
            echo(!TEST_INO_ACTIEF!| findstr /I /B /L /C:"%%~R" >nul && set "REGRESSIE_PAD_GEVONDEN=TRUE"
        )
        if /I not "!REGRESSIE_PAD_GEVONDEN!"=="TRUE" exit /b 0
    )
    if /I not "!TEST_TYPE_ACTIEF!"=="GERELEASED" if /I not "!TEST_TYPE_ACTIEF!"=="REGRESSIE" exit /b 0
)

if "!WELKE_FQBN_TESTEN_ACTIEF!"=="BIJ_BASIC_FQBN" (
    call :VOER_TEST_UIT_OP_GROEP BASIC
    exit /b 0
)
if "!WELKE_FQBN_TESTEN_ACTIEF!"=="BIJ_EXTENDED_FQBN" (
    call :VOER_TEST_UIT_OP_GROEP EXTENDED
    exit /b 0
)
if "!WELKE_FQBN_TESTEN_ACTIEF!"=="BIJ_EXPERIMENTAL_FQBN" (
    call :VOER_TEST_UIT_OP_GROEP EXPERIMENTAL
    exit /b 0
)
if "!WELKE_FQBN_TESTEN_ACTIEF!"=="BIJ_ALLE_FQBN" (
    call :VOER_TEST_UIT_OP_GROEP BASIC
    call :VOER_TEST_UIT_OP_GROEP EXTENDED
    call :VOER_TEST_UIT_OP_GROEP EXPERIMENTAL
    exit /b 0
)
echo [FOUT] Onbekende WELKE_FQBN_TESTEN: !WELKE_FQBN_TESTEN_ACTIEF! ^| !TEST_NAAM_ACTIEF!
set /A BASIC_FAIL+=1
exit /b 0

:VOER_TEST_UIT_OP_GROEP
set "FQBN_GROEP=%~1"
set "FQBN_LIJST="
set "MAPPING_LIJST="
if "!FQBN_GROEP!"=="BASIC" (
    set "FQBN_LIJST=!BASIC_FQBN!"
    set "MAPPING_LIJST=!BASIC_FQBN_MAPPING_BOARDS!"
)
if "!FQBN_GROEP!"=="EXTENDED" (
    set "FQBN_LIJST=!EXTENDED_FQBN!"
    set "MAPPING_LIJST=!EXTENDED_FQBN_MAPPING_BOARDS!"
)
if "!FQBN_GROEP!"=="EXPERIMENTAL" (
    set "FQBN_LIJST=!EXPERIMENTAL_FQBN!"
    set "MAPPING_LIJST=!EXPERIMENTAL_FQBN_MAPPING_BOARDS!"
)
if not defined FQBN_LIJST exit /b 0

set "MAPPING_REST=!MAPPING_LIJST!"
for %%F in (!FQBN_LIJST!) do (
    set "FQBN_ACTIEF=%%~F"
    set "MAPPING_ACTIEF="
    for /f "tokens=1,*" %%M in ("!MAPPING_REST!") do (
        set "MAPPING_ACTIEF=%%~M"
        set "MAPPING_REST=%%N"
    )
    if not defined MAPPING_ACTIEF (
        echo [FOUT] Geen mapping voor FQBN !FQBN_ACTIEF! in groep !FQBN_GROEP!.
        call :TEL_TEST
        call :TEL_FAIL
    ) else (
        set "BOARD_VERSIONS_LOOP=!MAPPING_ACTIEF:|= !"
        for %%V in (!BOARD_VERSIONS_LOOP!) do call :COMPILEER_DEZE_TEST "%%~V"
    )
)
exit /b 0

:COMPILEER_DEZE_TEST
set "BOARD_VERSION_ACTIEF=%~1"
if not exist "!TEST_INO_ACTIEF!" (
    echo [FOUT] TEST_INO bestaat niet: !TEST_INO_ACTIEF!
    call :TEL_TEST
    call :TEL_FAIL
    exit /b 0
)

for %%X in ("!TEST_INO_ACTIEF!") do (
    set "TEST_BESTAND=%%~nxX"
    set "TEST_DIR=%%~dpX"
)

set "TEST_DIR=!TEST_DIR:~0,-1!"
set "OVERSLAAN=FALSE"
if defined WELKE_FQBN_INCOMPATIBLE_ACTIEF echo(,!WELKE_FQBN_INCOMPATIBLE_ACTIEF!,| findstr /I /L /C:",!FQBN_ACTIEF!," >nul && set "OVERSLAAN=TRUE"
if defined WELKE_FQBN_SKIPPEN_ACTIEF echo(,!WELKE_FQBN_SKIPPEN_ACTIEF!,| findstr /I /L /C:",!FQBN_ACTIEF!," >nul && set "OVERSLAAN=TRUE"
rem Met BEPERK_TEST_TOT_FQBN wordt alleen op die ene FQBN gecompileerd.
rem Leeg of niet gezet betekent: alle FQBN.
if defined BEPERK_TEST_TOT_FQBN if /I not "!FQBN_ACTIEF!"=="!BEPERK_TEST_TOT_FQBN!" set "OVERSLAAN=TRUE"
if /I "!OVERSLAAN!"=="TRUE" exit /b 0
call :TEL_TEST
set /A COMPILATIE_NUMMER=BASIC_TESTS+EXTENDED_TESTS+EXPERIMENTAL_TESTS
set "COMPILE_LOG=%TEMP%\GroeiAcademieCompile_!GA_RUN_ID!.txt"
echo ------------------------------------------------------------
echo Compileren !COMPILATIE_NUMMER! van !TOTAAL_COMPILATIES!: !TEST_INO_ACTIEF!
echo TEST_TYPE: !TEST_TYPE_ACTIEF!
echo FQBN: !FQBN_ACTIEF!
echo BOARD_VERSION: !BOARD_VERSION_ACTIEF!
echo !TEST_NAAM_ACTIEF!
"%CLI_PATH%" compile --jobs 2 --fqbn "!FQBN_ACTIEF!" --build-property "compiler.cpp.extra_flags=-DGROEIACADEMIE_IGNORE_USER_CONFIG !TEST_FLAGS_ACTIEF! -DBOARD_VERSION=!BOARD_VERSION_ACTIEF!" --build-path "%TEMP%\GA_Build_!GA_RUN_ID!" "!TEST_DIR!" >"!COMPILE_LOG!" 2>&1
set "COMPILE_RESULT=!errorlevel!"
type "!COMPILE_LOG!"

if "!COMPILE_RESULT!"=="0" (
    if /I "!TEST_NAAM_ACTIEF!"=="SCREEN_OUTPUT_CONFIG=1" (
        call :TEL_FAIL
        echo [FOUT][!FQBN_GROEP!] !FQBN_ACTIEF! ^| !BOARD_VERSION_ACTIEF! ^| !TEST_NAAM_ACTIEF! ^| compileerde terwijl een compile-fout verwacht werd
    ) else (
        set "GEHEUGEN_FQBN=!FQBN_ACTIEF!"
        set "GEHEUGEN_BOARD_VERSION=!BOARD_VERSION_ACTIEF!"
        set "GEHEUGEN_BESTAND=!TEST_BESTAND!"
        set "GEHEUGEN_TEST=!TEST_NAAM_ACTIEF!"
        set "GEHEUGEN_STATUS=OK"
        call :REGISTREER_GEHEUGENGEBRUIK "!COMPILE_LOG!"
        call :TEL_OK
    )
) else (
    if /I "!TEST_NAAM_ACTIEF!"=="SCREEN_OUTPUT_CONFIG=1" (
        findstr /C:"Stel SCREEN_OUTPUT_CONFIG in UserConfig.h of SystemConfig.h in op (minstens) SCREEN_TYPE_CHARACTER." "!COMPILE_LOG!" >nul
        if not errorlevel 1 (
            call :TEL_OK
            echo [OK][!FQBN_GROEP!] !FQBN_ACTIEF! ^| !BOARD_VERSION_ACTIEF! ^| !TEST_NAAM_ACTIEF! ^| verwachte compile-fout
        ) else (
            call :TEL_FAIL
            echo [FOUT][!FQBN_GROEP!] !FQBN_ACTIEF! ^| !BOARD_VERSION_ACTIEF! ^| !TEST_NAAM_ACTIEF! ^| andere compile-fout dan verwacht
        )
    ) else (
        set "EXPECTED_MEMORY_LIMIT=0"

        if "!EXPECTED_MEMORY_LIMIT!"=="1" (
            set "GEHEUGEN_FQBN=!FQBN_ACTIEF!"
            set "GEHEUGEN_BOARD_VERSION=!BOARD_VERSION_ACTIEF!"
            set "GEHEUGEN_BESTAND=!TEST_BESTAND!"
            set "GEHEUGEN_TEST=!TEST_NAAM_ACTIEF!"
            set "GEHEUGEN_STATUS=VERWACHTE_GEHEUGENBEPERKING"
            call :REGISTREER_GEHEUGENGEBRUIK "!COMPILE_LOG!"
            call :TEL_EXPECTED_MEMORY
            echo [VERWACHTE GEHEUGENBEPERKING][!FQBN_GROEP!] !FQBN_ACTIEF! ^| !BOARD_VERSION_ACTIEF! ^| !TEST_NAAM_ACTIEF!
        ) else (
            call :TEL_FAIL
            echo [FOUT][!FQBN_GROEP!] !FQBN_ACTIEF! ^| !BOARD_VERSION_ACTIEF! ^| !TEST_NAAM_ACTIEF!
        )
    )
)
del /Q "!COMPILE_LOG!" >nul 2>&1
rmdir /s /q "%TEMP%\GA_Build_!GA_RUN_ID!" >nul 2>&1
exit /b 0

:TEL_TEST
rem Een EXPERIMENTEEL-test telt op elk board in EXPERIMENTAL_*, nooit in BASIC_* of EXTENDED_*.
if /I "!TEST_TYPE_ACTIEF!"=="EXPERIMENTEEL" (
    set /A EXPERIMENTAL_TESTS+=1
    exit /b 0
)
if "!FQBN_GROEP!"=="BASIC" set /A BASIC_TESTS+=1
if "!FQBN_GROEP!"=="EXTENDED" set /A EXTENDED_TESTS+=1
if "!FQBN_GROEP!"=="EXPERIMENTAL" set /A EXPERIMENTAL_TESTS+=1
exit /b 0

:TEL_OK
rem Een EXPERIMENTEEL-test telt op elk board in EXPERIMENTAL_*, nooit in BASIC_* of EXTENDED_*.
if /I "!TEST_TYPE_ACTIEF!"=="EXPERIMENTEEL" (
    set /A EXPERIMENTAL_OK+=1
    exit /b 0
)
if "!FQBN_GROEP!"=="BASIC" set /A BASIC_OK+=1
if "!FQBN_GROEP!"=="EXTENDED" set /A EXTENDED_OK+=1
if "!FQBN_GROEP!"=="EXPERIMENTAL" set /A EXPERIMENTAL_OK+=1
exit /b 0

:TEL_EXPECTED_MEMORY
rem Een EXPERIMENTEEL-test telt op elk board in EXPERIMENTAL_*, nooit in BASIC_* of EXTENDED_*.
if /I "!TEST_TYPE_ACTIEF!"=="EXPERIMENTEEL" (
    set /A EXPERIMENTAL_EXPECTED_MEMORY+=1
    exit /b 0
)
if "!FQBN_GROEP!"=="BASIC" set /A BASIC_EXPECTED_MEMORY+=1
if "!FQBN_GROEP!"=="EXTENDED" set /A EXTENDED_EXPECTED_MEMORY+=1
if "!FQBN_GROEP!"=="EXPERIMENTAL" set /A EXPERIMENTAL_EXPECTED_MEMORY+=1
exit /b 0

:TEL_FAIL
rem Een EXPERIMENTEEL-test telt op elk board in EXPERIMENTAL_*, nooit in BASIC_* of EXTENDED_*.
if /I "!TEST_TYPE_ACTIEF!"=="EXPERIMENTEEL" (
    set /A EXPERIMENTAL_FAIL+=1
    exit /b 0
)
if "!FQBN_GROEP!"=="BASIC" set /A BASIC_FAIL+=1
if "!FQBN_GROEP!"=="EXTENDED" set /A EXTENDED_FAIL+=1
if "!FQBN_GROEP!"=="EXPERIMENTAL" set /A EXPERIMENTAL_FAIL+=1
exit /b 0

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
echo [GEHEUGEN] FQBN=!GEHEUGEN_FQBN! ^| BOARD_VERSION=!GEHEUGEN_BOARD_VERSION! ^| Bestand=!GEHEUGEN_BESTAND! ^| Test=!GEHEUGEN_TEST! ^| Programma=!PROGRAMMA_WEERGAVE! ^| RAM=!RAM_WEERGAVE! ^| Status=!GEHEUGEN_STATUS!
exit /b 0

:RAPPORT
set "BASIC_STATUS=GESLAAGD"
if !BASIC_FAIL! GTR 0 set "BASIC_STATUS=GEFAALD"
set "EXTENDED_STATUS=GESLAAGD"
if !EXTENDED_FAIL! GTR 0 set "EXTENDED_STATUS=GEFAALD"
set "EXPERIMENTAL_STATUS=GESLAAGD"
if !EXPERIMENTAL_FAIL! GTR 0 set "EXPERIMENTAL_STATUS=GEFAALD"
set "EINDSTATUS=GESLAAGD"
if !BASIC_FAIL! GTR 0 set "EINDSTATUS=GEFAALD"
if !EXTENDED_FAIL! GTR 0 set "EINDSTATUS=GEFAALD"
if !LINT_FAIL! GTR 0 set "EINDSTATUS=GEFAALD"
if !STATIC_FAIL! GTR 0 set "EINDSTATUS=GEFAALD"

echo.
echo ============================================================
echo TESTRESULTAAT
echo ============================================================
echo Script                               : !TEST_SCRIPT_NAAM!
echo CONTROLE_MODUS                       : !CONTROLE_MODUS!
if /I "!CONTROLE_MODUS!"=="EXCLUDE" echo REGRESSIETEST                         : !REGRESSIETEST!
echo.
echo [BASIC_FQBN]
echo Totaal getest                        : !BASIC_TESTS!
echo OK bevonden                          : !BASIC_OK!
echo Verwachte geheugenbeperkingen        : !BASIC_EXPECTED_MEMORY!
echo Onverwacht mislukt                   : !BASIC_FAIL!
echo ------------------------------------------------------------
echo Tussenstatus BASIC_FQBN               : !BASIC_STATUS!
echo.
echo [EXTENDED_FQBN]
echo Totaal getest                        : !EXTENDED_TESTS!
echo OK bevonden                          : !EXTENDED_OK!
echo Verwachte geheugenbeperkingen        : !EXTENDED_EXPECTED_MEMORY!
echo Onverwacht mislukt                   : !EXTENDED_FAIL!
echo ------------------------------------------------------------
echo Tussenstatus EXTENDED_FQBN           : !EXTENDED_STATUS!
echo.
echo ============================================================
echo Eindstatus BASIC + EXTENDED          : !EINDSTATUS!
echo ============================================================
echo.
echo [EXPERIMENTAL_FQBN]
echo Totaal getest                        : !EXPERIMENTAL_TESTS!
echo OK bevonden                          : !EXPERIMENTAL_OK!
echo Verwachte geheugenbeperkingen        : !EXPERIMENTAL_EXPECTED_MEMORY!
echo Onverwacht mislukt                   : !EXPERIMENTAL_FAIL!
echo ------------------------------------------------------------
echo Status EXPERIMENTAL_FQBN             : !EXPERIMENTAL_STATUS!
echo ============================================================
echo EINDE
echo ============================================================
exit /b 0
