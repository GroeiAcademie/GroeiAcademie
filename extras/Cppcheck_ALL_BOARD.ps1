# Volledige Cppcheck-controle van de bestaande negen gewone testdomeinen.
# Acht boardconfiguraties in acht Windows Terminal-tabs. Geen bestaande scripts wijzigen.
param(
    [switch]$Child,
    [string]$Resultaatmap
)

Clear-Host
$ErrorActionPreference = 'Stop'
$extras = $PSScriptRoot
$script = Join-Path $extras 'CppcheckBijwerken.ps1'
$lokale = Join-Path $extras 'LokalePaden.cmd'
$domeinen = @('Data', 'Extenders', 'GedeeldeBus', 'Input', 'Screen', 'Sensoren', 'Sturingen', 'Tools', 'Foutinjecties')

# Bij een opstartfout in een tab ook een statusbestand achterlaten.
try {
if (-not (Test-Path -LiteralPath $script -PathType Leaf)) { throw 'CppcheckBijwerken.ps1 ontbreekt in extras.' }
if (-not (Test-Path -LiteralPath $lokale -PathType Leaf)) { throw 'LokalePaden.cmd ontbreekt in extras.' }

# Zelfde bestaande padinstellingen als de geslaagde BOARD_PROEF, ook opnieuw in elke tab.
$regels = @(& $env:ComSpec /d /c "call `"$lokale`" >nul && set" 2>&1)
if ($LASTEXITCODE -ne 0) { throw 'LokalePaden.cmd kan niet worden ingelezen.' }
foreach ($regel in $regels) {
    if ($regel -match '^(CCPCHECK_DATA_PATH|CLI_PATH|CPPCHECK_PATH)=(.*)$') {
        [Environment]::SetEnvironmentVariable($Matches[1], $Matches[2], 'Process')
    }
}
foreach ($naam in @('CCPCHECK_DATA_PATH', 'CLI_PATH', 'CPPCHECK_PATH')) {
    if (-not [Environment]::GetEnvironmentVariable($naam, 'Process')) { throw "Bestaande instelling ontbreekt: $naam" }
}
if (-not (Test-Path -LiteralPath $env:CLI_PATH -PathType Leaf)) {
    if (Test-Path -LiteralPath ($env:CLI_PATH + '.exe') -PathType Leaf) { $env:CLI_PATH += '.exe' }
    else { throw "Arduino CLI ontbreekt: $env:CLI_PATH" }
}
if (-not (Test-Path -LiteralPath $env:CPPCHECK_PATH -PathType Leaf)) { throw "Cppcheck ontbreekt: $env:CPPCHECK_PATH" }
$opslag = Join-Path $env:CCPCHECK_DATA_PATH 'Databases'
} catch {
    if ($Child -and $Resultaatmap -and (Test-Path -LiteralPath $Resultaatmap -PathType Container)) {
        Set-Content -LiteralPath (Join-Path $Resultaatmap 'status.txt') -Value @('MISLUKT', $_.Exception.Message) -Encoding UTF8
    }
    throw
}

if (-not $Child) {
    # Alleen de bestaande testlijsten lezen; hier worden nog geen builds uitgevoerd.
    $rijen = @(
        foreach ($domein in $domeinen) {
            Write-Host "[SELECTIE] $domein" -ForegroundColor Cyan
            $uitvoer = @(& powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $script -TestScript $domein -SnapshotRoot $opslag -ListHashes -Finalize 2>&1)
            if ($LASTEXITCODE -ne 0) { throw "Hashselectie mislukt voor ${domein}: $($uitvoer -join '; ')" }
            foreach ($regel in $uitvoer) {
                $delen = ([string]$regel).Trim() -split '\|', 4
                if ($delen.Count -ne 4 -or $delen[0] -notmatch '^[0-9a-f]{64}$') {
                    throw "Ongeldige configuratieregel voor ${domein}: $regel"
                }
                [pscustomobject]@{
                    Domein = $domein
                    Hash = $delen[0]
                    Ino = $delen[1]
                    Fqbn = $delen[2]
                    Board = $delen[3]
                }
            }
        }
    )
    if ($rijen.Count -eq 0) { throw 'Geen testconfiguraties gevonden. Er wordt niets gestart.' }
    $rijen = @($rijen | Sort-Object -Property Hash -Unique)
    $groepen = @($rijen | Group-Object -Property { $_.Fqbn + '|' + $_.Board } | Sort-Object Name)
    $fqbnAantal = @($rijen | Select-Object -ExpandProperty Fqbn -Unique).Count
    Write-Host "Gevonden: $($rijen.Count) configuraties uit $($domeinen.Count) testdomeinen, $fqbnAantal FQBN's, $($groepen.Count) boardvarianten."
    if ($fqbnAantal -ne 7 -or $groepen.Count -ne 8) {
        throw 'Niet de verwachte zeven FQBNs en acht boardconfiguraties. Geen tabs gestart.'
    }
    if (-not (Get-Command 'wt.exe' -ErrorAction SilentlyContinue)) {
        throw 'Windows Terminal (wt.exe) ontbreekt. Geen tabs gestart.'
    }

    $basis = Join-Path $env:CCPCHECK_DATA_PATH 'ALL_BOARDS'
    $map = Join-Path $basis ([guid]::NewGuid().ToString('N'))
    New-Item -ItemType Directory -Path $map -Force | Out-Null
    $rijen | Export-Csv -LiteralPath (Join-Path $map 'Selectie.csv') -NoTypeInformation -Encoding UTF8
    Write-Host "Resultatenmap: $map"

    $processen = @()
    $nummer = 0
    foreach ($groep in $groepen) {
        $nummer++
        $doelmap = Join-Path $map ('B{0:d2}' -f $nummer)
        New-Item -ItemType Directory -Path $doelmap -Force | Out-Null
        $groep.Group | Export-Csv -LiteralPath (Join-Path $doelmap 'Board.csv') -NoTypeInformation -Encoding UTF8
        $voorbeeld = $groep.Group | Select-Object -First 1
        $titel = 'B{0:d2} {1} ({2})' -f $nummer, $voorbeeld.Fqbn, $voorbeeld.Board
        $argumenten = '-w 0 new-tab --title "{0}" -d "{1}" powershell.exe -NoExit -NoProfile -ExecutionPolicy Bypass -File "{2}" -Child -Resultaatmap "{3}"' -f $titel, $extras, $PSCommandPath, $doelmap
        $proces = Start-Process -FilePath 'wt.exe' -ArgumentList $argumenten -WorkingDirectory $extras -PassThru
        $processen += [pscustomobject]@{ Fqbn = $voorbeeld.Fqbn; Board = $voorbeeld.Board; Map = $doelmap; Proces = $proces }
        Write-Host "Gestart $nummer/$($groepen.Count): $($voorbeeld.Fqbn) | $($voorbeeld.Board) | $($groep.Count) configuraties"
    }

    Write-Host "`nAcht boardtabs draaien parallel. Wachten op de acht statusbestanden."
    do {
        $gereed = @($processen | Where-Object { Test-Path -LiteralPath (Join-Path $_.Map 'status.txt') -PathType Leaf }).Count
        if ($gereed -lt $processen.Count) { Start-Sleep -Seconds 2 }
    } while ($gereed -lt $processen.Count)

    Write-Host "`n=== CPPCHECK ALL_BOARDS RESULTATEN ==="
    $mislukt = 0
    $overzicht = @(
        foreach ($item in $processen) {
            $status = @(Get-Content -LiteralPath (Join-Path $item.Map 'status.txt') -Encoding UTF8)[0]
            if ($status -ne 'OK') { $mislukt++ }
            Write-Host "$($item.Fqbn) | $($item.Board) | $status | $($item.Map)"
            [pscustomobject]@{ Fqbn = $item.Fqbn; Board = $item.Board; Status = $status; Map = $item.Map }
        }
    )
    $overzicht | Export-Csv -LiteralPath (Join-Path $map 'Resultaten.csv') -NoTypeInformation -Encoding UTF8
    Write-Host "Alle rapporten: $map"
    if ($mislukt -gt 0) { throw "$mislukt van de acht boardconfiguraties mislukt. Zie de afzonderlijke logs." }
    Write-Host 'Acht volledige Cppcheck-runs technisch afgerond; inhoudelijke meldingen staan in de XML-rapporten.'
    return
}

# De child draait per unieke FQBN + BOARD_VERSION in zijn eigen Terminal-tab.
try {
    if (-not $Resultaatmap -or -not (Test-Path -LiteralPath $Resultaatmap -PathType Container)) { throw 'Resultatenmap ontbreekt.' }
    $rijen = @(Import-Csv -LiteralPath (Join-Path $Resultaatmap 'Board.csv') -Encoding UTF8)
    if ($rijen.Count -eq 0) { throw 'Board.csv bevat geen configuraties.' }
    $fqbn = $rijen[0].Fqbn
    $board = $rijen[0].Board
    if (@($rijen | Where-Object { $_.Fqbn -ne $fqbn -or $_.Board -ne $board }).Count -ne 0) {
        throw 'Board.csv bevat meer dan een boardconfiguratie.'
    }

    $nummer = 0
    foreach ($domein in $domeinen) {
        $selectie = @($rijen | Where-Object { $_.Domein -eq $domein } | Select-Object -ExpandProperty Hash -Unique)
        if ($selectie.Count -eq 0) { continue }
        $nummer += $selectie.Count
        $hashes = $selectie -join ','
        Write-Host "[$fqbn | $board] Snapshots $domein : $($selectie.Count) configuraties" -ForegroundColor Cyan
        & powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $script -TestScript $domein -SnapshotRoot $opslag -CliPath $env:CLI_PATH -ProefHashes $hashes 2>&1 |
            Tee-Object -FilePath (Join-Path $Resultaatmap ("Snapshots_$domein.log"))
        if ($LASTEXITCODE -ne 0) { throw "Snapshotopbouw mislukt in domein $domein." }
    }
    if ($nummer -ne $rijen.Count) { throw "Aantal snapshots verschilt van selectie: $nummer / $($rijen.Count)." }

    $database = Join-Path $Resultaatmap 'compile_commands.json'
    $rapport = Join-Path $Resultaatmap 'Cppcheck.xml'
    $cache = Join-Path $Resultaatmap 'CppcheckCache'
    New-Item -ItemType Directory -Path $cache -Force | Out-Null
    $hashes = (@($rijen | Select-Object -ExpandProperty Hash -Unique) -join ',')

    Write-Host "[$fqbn | $board] Samenvoegen van $nummer configuraties" -ForegroundColor Cyan
    & powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $script -Finalize -ProefHashes $hashes -OutputDatabase $database 2>&1 |
        Tee-Object -FilePath (Join-Path $Resultaatmap 'Database.log')
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $database -PathType Leaf)) { throw 'Samenvoegen van de boarddatabase mislukt.' }

    Write-Host "[$fqbn | $board] Volledige Cppcheck-analyse" -ForegroundColor Cyan
    & $env:CPPCHECK_PATH "--project=$database" "--cppcheck-build-dir=$cache" '--platform=unix32' '--enable=all' '--check-level=exhaustive' '--inline-suppr' '--xml' '--xml-version=2' "--output-file=$rapport" 2>&1 |
        Tee-Object -FilePath (Join-Path $Resultaatmap 'Cppcheck.stdout.log')
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $rapport -PathType Leaf)) { throw 'Cppcheck kon geen geldig XML-rapport maken.' }

    Write-Host "[$fqbn | $board] Originele .ino-regelnummers" -ForegroundColor Cyan
    & powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $script -RapportInoRegels $rapport -OutputDatabase $database 2>&1 |
        Tee-Object -FilePath (Join-Path $Resultaatmap 'InoRegels.log')
    if ($LASTEXITCODE -ne 0) { throw 'Omzetten van .ino-regelnummers mislukt.' }

    [xml]$xml = Get-Content -LiteralPath $rapport -Raw -Encoding UTF8
    $meldingen = @($xml.results.errors.error | Where-Object { $null -ne $_ })
    $fouten = @($meldingen | Where-Object { $_.severity -eq 'error' }).Count
    Set-Content -LiteralPath (Join-Path $Resultaatmap 'status.txt') -Value 'OK' -Encoding UTF8
    Write-Host "[$fqbn | $board] KLAAR: $nummer configuraties, $($meldingen.Count) meldingen, $fouten errors." -ForegroundColor Green
} catch {
    $foutmelding = $_.Exception.Message
    if ($Resultaatmap -and (Test-Path -LiteralPath $Resultaatmap -PathType Container)) {
        Set-Content -LiteralPath (Join-Path $Resultaatmap 'status.txt') -Value @('MISLUKT', $foutmelding) -Encoding UTF8
    }
    Write-Host "[ALL_BOARDS] MISLUKT: $foutmelding" -ForegroundColor Red
    exit 1
}
