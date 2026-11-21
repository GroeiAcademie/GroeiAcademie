# Eenmalige Cppcheck-rookproef. Geen wijziging aan het FrameWork of de bestaande scripts.
# Uitvoeren in een PowerShell-venster waarvan de werkmap de bestaande extras-map is.
param(
    [switch]$Child,
    [string]$Domein = 'Tools',
    [string]$Fqbn,
    [string]$Hashes,
    [string]$Resultaatmap
)

Clear-Host
$ErrorActionPreference = 'Stop'
$extras = (Get-Location).ProviderPath
$script = Join-Path $extras 'CppcheckBijwerken.ps1'
$lokale = Join-Path $extras 'LokalePaden.cmd'

if (-not (Test-Path -LiteralPath $script -PathType Leaf)) { throw 'Start vanuit de bestaande extras-map: CppcheckBijwerken.ps1 ontbreekt.' }

if (-not $Child) {
    if (-not (Test-Path -LiteralPath $lokale -PathType Leaf)) { throw 'LokalePaden.cmd ontbreekt.' }

    # Lees uitsluitend de ingestelde paden uit de bestaande CMD, wijzig die CMD niet.
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
    $uitvoer = @(& powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $script -TestScript $Domein -SnapshotRoot $opslag -ListHashes -Finalize 2>&1)
    if ($LASTEXITCODE -ne 0) { throw "Bestaande hashselectie mislukt: $($uitvoer -join '; ')" }

    # Alleen de echte hashes voor de afgesproken, bestaande .ino-sketch.
    $rijen = @(
        foreach ($regel in $uitvoer) {
            $tekst = [string]$regel
            if ($tekst -notmatch '^[0-9a-f]{64}\|') { continue }
            $delen = $tekst -split '\|'
            if ($delen.Count -ne 4 -or $delen[1] -notmatch '(?i)(?:^|[\\/])I2C_Scan\.ino$') { continue }
            [pscustomobject]@{ Hash = $delen[0]; Ino = $delen[1]; Fqbn = $delen[2]; Board = $delen[3] }
        }
    )
    if ($rijen.Count -eq 0) { throw 'I2C_Scan.ino is niet geselecteerd in de bestaande TestLibraryTools-v2.0.0.cmd.' }

    # Eén configuratie per unieke FQBN/BOARD_VERSION, niet meerdere testvarianten stapelen.
    $rijen = @($rijen | Group-Object -Property { $_.Fqbn + '|' + $_.Board } | ForEach-Object { $_.Group | Select-Object -First 1 })
    $groepen = @($rijen | Group-Object -Property Fqbn | Sort-Object Name)
    Write-Host "Gevonden: $($groepen.Count) FQBN's en $($rijen.Count) boardconfiguraties voor I2C_Scan.ino."
    foreach ($groep in $groepen) { Write-Host "  $($groep.Name): $($groep.Count) configuratie(s)" }
    if ($groepen.Count -lt 7 -or $groepen.Count -gt 8 -or $rijen.Count -lt 7 -or $rijen.Count -gt 8) {
        throw 'Niet de verwachte 7 of 8 boardvarianten. Er wordt niets gestart; controleer de bestaande testselectie.'
    }

    $basis = Join-Path $env:CCPCHECK_DATA_PATH 'BOARD_PROEF'
    $map = Join-Path $basis ([guid]::NewGuid().ToString('N'))
    New-Item -ItemType Directory -Path $map -Force | Out-Null
    $rijen | Export-Csv -LiteralPath (Join-Path $map 'Selectie.csv') -NoTypeInformation -Encoding UTF8
    Write-Host "Resultatenmap: $map"

    # Eén afzonderlijk PowerShell-proces per FQBN, ook bij twee varianten van dezelfde FQBN.
    if (-not (Get-Command 'wt.exe' -ErrorAction SilentlyContinue)) { throw 'Windows Terminal (wt.exe) ontbreekt of is niet beschikbaar via PATH. De boardproef wordt niet gestart.' }

    $processen = @()
    $nummer = 0
    foreach ($groep in $groepen) {
        $nummer++
        $doelmap = Join-Path $map ('B{0:d2}' -f $nummer)
        New-Item -ItemType Directory -Path $doelmap -Force | Out-Null
        $groep.Group | Export-Csv -LiteralPath (Join-Path $doelmap 'Board.csv') -NoTypeInformation -Encoding UTF8
        $hashesVoorBoard = (@($groep.Group | ForEach-Object { $_.Hash }) -join ',')
        $argumenten = '-w 0 new-tab --title "B{0:d2} {1}" -d "{2}" powershell.exe -NoExit -NoProfile -ExecutionPolicy Bypass -File "{3}" -Child -Domein "{4}" -Fqbn "{5}" -Hashes "{6}" -Resultaatmap "{7}"' -f $nummer, $groep.Name, $extras, $PSCommandPath, $Domein, $groep.Name, $hashesVoorBoard, $doelmap
        $proces = Start-Process -FilePath 'wt.exe' -ArgumentList $argumenten -WorkingDirectory $extras -PassThru
        $processen += [pscustomobject]@{ Proces = $proces; Fqbn = $groep.Name; Map = $doelmap }
        Write-Host "Gestart $nummer/$($groepen.Count): $($groep.Name)"
    }

    Write-Host "`nDe boardtabs draaien parallel en blijven open. De resultaten worden per board bewaard."
    # wt.exe geeft de tab door aan Windows Terminal en eindigt eerder dan de test in die tab.
    # Wacht daarom op de bestaande statusbestanden in plaats van op het wt.exe-proces.
    do {
        $nummer = 0
        foreach ($item in $processen) {
            if (Test-Path -LiteralPath (Join-Path $item.Map 'status.txt') -PathType Leaf) { $nummer++ }
        }
        if ($nummer -lt $processen.Count) { Start-Sleep -Milliseconds 500 }
    } while ($nummer -lt $processen.Count)
    Write-Host "`n=== CPPCHECK BOARD_PROEF RESULTATEN ==="
    $mislukt = 0
    foreach ($item in $processen) {
        $statusPad = Join-Path $item.Map 'status.txt'
        $status = if (Test-Path -LiteralPath $statusPad) { (Get-Content -LiteralPath $statusPad -First 1) } else { 'MISLUKT: geen statusbestand' }
        if ($status -ne 'OK') { $mislukt++ }
        Write-Host "$($item.Fqbn) | $status | $($item.Map)"
    }
    Write-Host "Resultaten: $map"
    if ($mislukt -gt 0) { throw "$mislukt van $($groepen.Count) boardvensters niet geslaagd. Zie de afzonderlijke logs." }
    Write-Host 'Alle boardproeven technisch afgerond. De inhoudelijke Cppcheck-meldingen staan in de XML-rapporten.'
    return
}

# Alleen deze tak draait in het afzonderlijke boardvenster.
try {
    if (-not $Fqbn -or -not $Hashes -or -not $Resultaatmap) { throw 'Ontbrekende boardparameters.' }
    if ($Hashes -notmatch '^[0-9a-f]{64}(?:,[0-9a-f]{64})*$') { throw 'Ongeldige proefhashes.' }
    $opslag = Join-Path $env:CCPCHECK_DATA_PATH 'Databases'
    $database = Join-Path $Resultaatmap 'compile_commands.json'
    $rapport = Join-Path $Resultaatmap 'Cppcheck.xml'
    $cache = Join-Path $Resultaatmap 'CppcheckCache'
    New-Item -ItemType Directory -Path $cache -Force | Out-Null

    Write-Host "[$Fqbn] 1/3: Arduino .ino.cpp en snapshots" -ForegroundColor Cyan
    & powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $script -TestScript $Domein -SnapshotRoot $opslag -CliPath $env:CLI_PATH -ProefHashes $Hashes 2>&1 | Tee-Object -FilePath (Join-Path $Resultaatmap 'Snapshots.log')
    if ($LASTEXITCODE -ne 0) { throw 'Snapshotopbouw mislukt.' }

    Write-Host "[$Fqbn] 2/3: afzonderlijke compilatiedatabase" -ForegroundColor Cyan
    & powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $script -Finalize -ProefHashes $Hashes -OutputDatabase $database 2>&1 | Tee-Object -FilePath (Join-Path $Resultaatmap 'Database.log')
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $database -PathType Leaf)) { throw 'Samenvoegen van de proefdatabase mislukt.' }

    Write-Host "[$Fqbn] 3/3: korte Cppcheck-analyse" -ForegroundColor Cyan
    & $env:CPPCHECK_PATH "--project=$database" "--cppcheck-build-dir=$cache" '--platform=unix32' '--enable=all' '--check-level=normal' '--xml' '--xml-version=2' "--output-file=$rapport" 2>&1 | Tee-Object -FilePath (Join-Path $Resultaatmap 'Cppcheck.stdout.log')
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $rapport -PathType Leaf)) { throw 'Cppcheck kon geen geldig XML-rapport maken.' }

    & powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $script -RapportInoRegels $rapport -OutputDatabase $database 2>&1 | Tee-Object -FilePath (Join-Path $Resultaatmap 'InoRegels.log')
    if ($LASTEXITCODE -ne 0) { throw 'Omzetten van .ino-regelnummers mislukt.' }

    [xml]$xml = Get-Content -LiteralPath $rapport -Raw -Encoding UTF8
    $meldingen = @($xml.results.errors.error | Where-Object { $null -ne $_ })
    $fouten = @($meldingen | Where-Object { $_.severity -eq 'error' }).Count
    Set-Content -LiteralPath (Join-Path $Resultaatmap 'status.txt') -Value 'OK' -Encoding UTF8
    Write-Host "[$Fqbn] KLAAR: $($meldingen.Count) meldingen, waaronder $fouten met ernst 'error'." -ForegroundColor Green
} catch {
    $foutmelding = $_.Exception.Message
    Set-Content -LiteralPath (Join-Path $Resultaatmap 'status.txt') -Value @('MISLUKT', $foutmelding) -Encoding UTF8
    Write-Host "[$Fqbn] MISLUKT: $foutmelding" -ForegroundColor Red
    exit 1
}
