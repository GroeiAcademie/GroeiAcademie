# Alleen verouderde gegevens opruimen. Geen builds, scans of database-samenvoeging.
# Bestaande structuur, met afzonderlijke tellers voor OK en onbetrouwbare selecties.
$ErrorActionPreference = 'Stop'
$extras = $PSScriptRoot
$lokale = Join-Path $extras 'LokalePaden.cmd'
$script = Join-Path $extras 'CppcheckBijwerken.ps1'
$domeinen = @('Data', 'Extenders', 'GedeeldeBus', 'Input', 'Screen', 'Sensoren', 'Sturingen', 'Tools', 'Foutinjecties')
$mutex = [System.Threading.Mutex]::new($false, 'Local\GroeiAcademieCcpcheckMerge')
$vergrendeld = $false

try {
    try { $vergrendeld = $mutex.WaitOne(0) }
    catch [System.Threading.AbandonedMutexException] { $vergrendeld = $true }
    if (-not $vergrendeld) { throw 'Cppcheck is aan het samenvoegen of opruimen. Opruimen geweigerd.' }
    if (-not (Test-Path -LiteralPath $lokale -PathType Leaf)) { throw 'LokalePaden.cmd ontbreekt in extras.' }
    if (-not (Test-Path -LiteralPath $script -PathType Leaf)) { throw 'CppcheckBijwerken.ps1 ontbreekt in extras.' }

    # Bestaande lokale paden inlezen, zonder er nieuwe of vaste Windows-paden aan toe te voegen.
    $regels = @(& $env:ComSpec /d /c "call `"$lokale`" >nul && set" 2>&1)
    if ($LASTEXITCODE -ne 0) { throw 'LokalePaden.cmd kan niet worden ingelezen.' }
    foreach ($regel in $regels) {
        if ($regel -match '^CCPCHECK_DATA_PATH=(.*)$') { $env:CCPCHECK_DATA_PATH = $Matches[1] }
    }
    if (-not $env:CCPCHECK_DATA_PATH) { throw 'CCPCHECK_DATA_PATH ontbreekt in LokalePaden.cmd.' }
    $env:CCPCHECK_DATA_PATH = [System.IO.Path]::GetFullPath($env:CCPCHECK_DATA_PATH)
    if ($env:CCPCHECK_DATA_PATH -eq [System.IO.Path]::GetPathRoot($env:CCPCHECK_DATA_PATH)) {
        throw 'CCPCHECK_DATA_PATH verwijst naar de hoofdmap van een schijf. Opruimen geweigerd.'
    }
    if (-not (Test-Path -LiteralPath $env:CCPCHECK_DATA_PATH -PathType Container)) {
        throw 'De bestaande Cppcheck-datamap ontbreekt. Er is niets om op te ruimen.'
    }

    $basis = Join-Path $env:CCPCHECK_DATA_PATH 'ALL_BOARDS'
    $opslag = Join-Path (Join-Path $env:CCPCHECK_DATA_PATH 'Databases') 'Gewoon'
    foreach ($pad in @($env:CCPCHECK_DATA_PATH, $basis, $opslag)) {
        if (-not (Test-Path -LiteralPath $pad)) { continue }
        $bestand = Get-Item -LiteralPath $pad -ErrorAction Stop
        if (($bestand.Attributes -band [System.IO.FileAttributes]::ReparsePoint) -ne 0) {
            throw "Opruimen via een gekoppelde map (reparse point) is niet toegestaan: $pad"
        }
    }

    # Geen opruiming tijdens actieve Cppcheck-/CLI-/regressietaken.
    # Een Arduino IDE-daemon is geen actieve compilatie. Afgeronde -NoExit-tabs zijn geen actieve test.
    $processen = @(Get-CimInstance Win32_Process -ErrorAction Stop)
    $processen = @($processen | Where-Object {
        if ($_.ProcessId -eq $PID) { return $false }
        if ($_.Name -eq 'cppcheck.exe') { return $true }
        if ($_.Name -eq 'arduino-cli.exe') {
            return -not ([string]$_.CommandLine -match '(?i)(?:^|\s)daemon(?:\s|$)')
        }
        if ($_.Name -notin @('powershell.exe', 'pwsh.exe', 'cmd.exe') -or
            [string]$_.CommandLine -notmatch '(?i)(?:Cppcheck_ALL_BOARD|Cppcheck_BOARD_PROEF|CppcheckBijwerken|ccpcheckCreateDatabaseTestLibraryAllCmdParallel|ccpcheckPrepareDatabaseTestLibraryAllCmdParallel|TestLibrary[^\/\s"]+)\.(?:ps1|cmd)') { return $false }
        if ([string]$_.CommandLine -match '(?i)\s-NoExit\b') {
            # Child-tab: de eigen status wordt pas bij het afsluiten van de test geschreven.
            if ([string]$_.CommandLine -match '(?i)\s-Child\b' -and
                [string]$_.CommandLine -match '(?i)\s-Resultaatmap\s+"([^"]+)"' -and
                (Test-Path -LiteralPath (Join-Path $Matches[1] 'status.txt') -PathType Leaf)) { return $false }
            # Regressietabs met -NoExit -Command blijven open; een actieve CMD is hun child.
            if ([string]$_.CommandLine -match '(?i)\s-Command\b') {
                $nummer = $_.ProcessId
                if (@($processen | Where-Object { $_.ParentProcessId -eq $nummer -and $_.Name -notin @('conhost.exe', 'OpenConsole.exe') }).Count -eq 0) { return $false }
            }
        }
        return $true
    })
    if ($processen.Count -gt 0) {
        $processen | Select-Object Name, ProcessId, CommandLine | Format-Table -AutoSize
        throw 'Er draait een Cppcheck-, Arduino CLI- of regressietaak. Opruimen geweigerd.'
    }

    Write-Host "`n=== CPPCHECK OUDE DATA: CONTROLE, NOG NIETS VERWIJDERD ===" -ForegroundColor Cyan
    Write-Host "Datamap: $($env:CCPCHECK_DATA_PATH)"

    # Elke ALL_BOARDS-GUID is een afzonderlijke uitvoeringsmap, geen broncodehash.
    $groepen = @()
    if (Test-Path -LiteralPath $basis -PathType Container) {
        $groepen = @(Get-ChildItem -LiteralPath $basis -Directory -ErrorAction Stop |
            Where-Object { $_.Name -match '^[0-9a-fA-F]{32}$' } |
            Sort-Object CreationTime -Descending)
    }
    $overzicht = @(
        for ($nummer = 0; $nummer -lt $groepen.Count; $nummer++) {
            $map = $groepen[$nummer]
            $aantalOK = 0
            $mislukt = 0
            $gereed = 0
            foreach ($groep in @('B01','B02','B03','B04','B05','B06','B07','B08')) {
                $doelmap = Join-Path $map.FullName $groep
                $status = Join-Path $doelmap 'status.txt'
                if (-not (Test-Path -LiteralPath $status -PathType Leaf)) { continue }
                $waarde = @(Get-Content -LiteralPath $status -Encoding UTF8 -TotalCount 1)[0]
                if ($waarde -eq 'OK') { $aantalOK++; $gereed++ }
                elseif ($waarde -eq 'MISLUKT') { $mislukt++; $gereed++ }
            }
            [pscustomobject]@{
                Nummer = $nummer + 1
                Datum = $map.CreationTime
                Map = $map.FullName
                OK = $aantalOK
                Mislukt = $mislukt
                Volledig = ($gereed -eq 8 -and (Test-Path -LiteralPath (Join-Path $map.FullName 'Selectie.csv') -PathType Leaf))
                Nieuwste = ($nummer -eq 0)
            }
        }
    )

    if ($overzicht.Count -gt 0) {
        $overzicht | Select-Object Nummer, Datum, OK, Mislukt, Volledig, Nieuwste, Map | Format-Table -AutoSize -Wrap
    } else {
        Write-Host 'Geen ALL_BOARDS-GUID-mappen gevonden.'
    }
    Write-Host 'De nieuwste run en elke onvolledige run blijven altijd behouden.'
    Write-Host 'Volledig betekent acht eindstatussen (OK of MISLUKT), niet acht geslaagde boards.'
    Write-Host 'Alle andere runs blijven behouden tenzij je hieronder zelf hun nummers selecteert.'
    $uitvoer = (Read-Host 'Te verwijderen voltooide oude runnummers, gescheiden door komma (Enter = geen)').Trim()
    $rijen = @()
    if ($uitvoer) {
        if ($uitvoer -notmatch '^\d+(\s*,\s*\d+)*$') { throw 'Ongeldige selectie. Niets verwijderd.' }
        foreach ($nummer in @($uitvoer -split '\s*,\s*' | ForEach-Object { [int]$_ } | Sort-Object -Unique)) {
            $item = @($overzicht | Where-Object { $_.Nummer -eq $nummer })
            if ($item.Count -ne 1 -or $item[0].Nieuwste -or -not $item[0].Volledig) {
                throw "Runnummer $nummer is niet beschikbaar voor opruiming. Niets verwijderd."
            }
            $rijen += $item[0]
        }
    }

    # De actuele negen testlijsten zijn de enige bron voor de huidige configuratiehashes.
    # De parameter -ListHashes -Finalize leest uitsluitend de selectie; hij ruimt niets op.
    $actueel = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::OrdinalIgnoreCase)
    foreach ($domein in $domeinen) {
        $uitvoer = @(& powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $script -TestScript $domein -SnapshotRoot (Split-Path -Path $opslag -Parent) -ListHashes -Finalize 2>&1)
        if ($LASTEXITCODE -ne 0) { throw "Huidige selectie van $domein kon niet worden vastgesteld. Niets verwijderd: $($uitvoer -join '; ')" }
        foreach ($regel in $uitvoer) {
            $delen = ([string]$regel).Trim() -split '\|', 4
            if ($delen.Count -ne 4 -or $delen[0] -notmatch '^[0-9a-f]{64}$') {
                throw "Ongeldige huidige hash in $domein. Niets verwijderd: $regel"
            }
            [void]$actueel.Add($delen[0])
        }
    }
    if ($actueel.Count -eq 0) { throw 'Geen actuele testconfiguraties gevonden. Niets verwijderd.' }

    # Ook alle niet geselecteerde/historisch bewaarde ALL_BOARDS- en BOARD_PROEF-runs beschermen.
    # Als een bewaarde map geen betrouwbare selectie heeft, niet aan snapshots komen.
    $onbetrouwbareSelecties = 0
    foreach ($doelmap in @($basis, (Join-Path $env:CCPCHECK_DATA_PATH 'BOARD_PROEF'))) {
        if (-not (Test-Path -LiteralPath $doelmap -PathType Container)) { continue }
        foreach ($groep in @(Get-ChildItem -LiteralPath $doelmap -Directory -ErrorAction Stop)) {
            if (@($rijen | Where-Object { $_.Map -eq $groep.FullName }).Count -gt 0) { continue }
            $bestand = Join-Path $groep.FullName 'Selectie.csv'
            if (-not (Test-Path -LiteralPath $bestand -PathType Leaf)) {
                Write-Warning "Bewaarde run zonder Selectie.csv: $($groep.FullName). Snapshots worden niet verwijderd."
                $onbetrouwbareSelecties++
                continue
            }
            $selectie = @(Import-Csv -LiteralPath $bestand -Encoding UTF8)
            if ($selectie.Count -eq 0) {
                Write-Warning "Lege selectie in bewaarde run: $bestand. Snapshots worden niet verwijderd."
                $onbetrouwbareSelecties++
                continue
            }
            foreach ($regel in $selectie) {
                if ([string]$regel.Hash -notmatch '^[0-9a-fA-F]{64}$') {
                    Write-Warning "Ongeldige hash in bewaarde run: $bestand. Snapshots worden niet verwijderd."
                    $onbetrouwbareSelecties++
                    break
                }
                [void]$actueel.Add([string]$regel.Hash)
            }
        }
    }

    # Ook een bestaande centrale compilatiedatabase mag geen verweesde snapshotverwijzingen krijgen.
    $bestand = Join-Path $env:CCPCHECK_DATA_PATH 'compile_commands.json'
    if (Test-Path -LiteralPath $bestand -PathType Leaf) {
        $regels = Get-Content -LiteralPath $bestand -Raw -Encoding UTF8
        $selectie = @([regex]::Matches($regels, '(?i)Databases[\\/]+Gewoon[\\/]+([0-9a-f]{64})'))
        if ($selectie.Count -eq 0) {
            Write-Warning 'Centrale compilatiedatabase zonder herkenbare snapshotverwijzingen. Snapshots worden niet verwijderd.'
            $onbetrouwbareSelecties++
        }
        foreach ($regel in $selectie) { [void]$actueel.Add($regel.Groups[1].Value) }
    }

    $groepen = @()
    if ($onbetrouwbareSelecties -eq 0 -and (Test-Path -LiteralPath $opslag -PathType Container)) {
        $groepen = @(Get-ChildItem -LiteralPath $opslag -Directory -ErrorAction Stop |
            Where-Object { $_.Name -match '^[0-9a-fA-F]{64}$' -and -not $actueel.Contains($_.Name) })
    }

    Write-Host "`n=== VOORGESTELDE OPRUIMING ===" -ForegroundColor Cyan
    Write-Host "Geselecteerde oude, voltooide runs: $($rijen.Count)"
    foreach ($item in $rijen) { Write-Host "  RUN: $($item.Map)" }
    if ($onbetrouwbareSelecties -gt 0) {
        Write-Warning 'Er zijn bewaarde runs met onvolledige selectie-informatie: snapshots blijven volledig behouden.'
    }
    Write-Host "Aantoonbaar niet-geselecteerde snapshot-hashmappen: $($groepen.Count)"
    foreach ($groep in $groepen) { Write-Host "  HASH: $($groep.FullName)" }

    if ($rijen.Count -eq 0 -and $groepen.Count -eq 0) {
        Write-Host 'Geen opruiming geselecteerd of mogelijk. Er is niets gewijzigd.' -ForegroundColor Green
        exit 0
    }

    $uitvoer = (Read-Host "Typ exact VERWIJDEREN om alleen bovenstaande mappen te verwijderen (Enter = annuleren)").Trim()
    if ($uitvoer -cne 'VERWIJDEREN') {
        Write-Host 'Geannuleerd. Er is niets verwijderd.'
        exit 0
    }

    # Vlak voor de eerste verwijdering opnieuw op actieve processen controleren.
    # Een Arduino IDE-daemon is geen actieve compilatie. Afgeronde -NoExit-tabs zijn geen actieve test.
    $processen = @(Get-CimInstance Win32_Process -ErrorAction Stop)
    $processen = @($processen | Where-Object {
        if ($_.ProcessId -eq $PID) { return $false }
        if ($_.Name -eq 'cppcheck.exe') { return $true }
        if ($_.Name -eq 'arduino-cli.exe') {
            return -not ([string]$_.CommandLine -match '(?i)(?:^|\s)daemon(?:\s|$)')
        }
        if ($_.Name -notin @('powershell.exe', 'pwsh.exe', 'cmd.exe') -or
            [string]$_.CommandLine -notmatch '(?i)(?:Cppcheck_ALL_BOARD|Cppcheck_BOARD_PROEF|CppcheckBijwerken|ccpcheckCreateDatabaseTestLibraryAllCmdParallel|ccpcheckPrepareDatabaseTestLibraryAllCmdParallel|TestLibrary[^\/\s"]+)\.(?:ps1|cmd)') { return $false }
        if ([string]$_.CommandLine -match '(?i)\s-NoExit\b') {
            # Child-tab: de eigen status wordt pas bij het afsluiten van de test geschreven.
            if ([string]$_.CommandLine -match '(?i)\s-Child\b' -and
                [string]$_.CommandLine -match '(?i)\s-Resultaatmap\s+"([^"]+)"' -and
                (Test-Path -LiteralPath (Join-Path $Matches[1] 'status.txt') -PathType Leaf)) { return $false }
            # Regressietabs met -NoExit -Command blijven open; een actieve CMD is hun child.
            if ([string]$_.CommandLine -match '(?i)\s-Command\b') {
                $nummer = $_.ProcessId
                if (@($processen | Where-Object { $_.ParentProcessId -eq $nummer -and $_.Name -notin @('conhost.exe', 'OpenConsole.exe') }).Count -eq 0) { return $false }
            }
        }
        return $true
    })
    if ($processen.Count -gt 0) { throw 'Intussen is een Cppcheck-, Arduino CLI- of regressietaak gestart. Niets verwijderd.' }

    foreach ($item in $rijen) {
        $map = Get-Item -LiteralPath $item.Map -ErrorAction Stop
        if (($map.Attributes -band [System.IO.FileAttributes]::ReparsePoint) -ne 0 -or
            @(Get-ChildItem -LiteralPath $map.FullName -Force -Recurse -Attributes ReparsePoint -ErrorAction Stop).Count -gt 0) {
            throw "Gekoppelde map of inhoud geweigerd: $($map.FullName)"
        }
        if ($map.Parent.FullName -ne $basis -or $map.Name -notmatch '^[0-9a-fA-F]{32}$') { throw 'Onverwachte runmap. Opruimen onderbroken.' }
        Write-Host "[VERWIJDEREN] RUN $($map.FullName)"
        Remove-Item -LiteralPath $map.FullName -Recurse -Force -ErrorAction Stop
    }
    foreach ($groep in $groepen) {
        $map = Get-Item -LiteralPath $groep.FullName -ErrorAction Stop
        if (($map.Attributes -band [System.IO.FileAttributes]::ReparsePoint) -ne 0 -or
            @(Get-ChildItem -LiteralPath $map.FullName -Force -Recurse -Attributes ReparsePoint -ErrorAction Stop).Count -gt 0) {
            throw "Gekoppelde map of inhoud geweigerd: $($map.FullName)"
        }
        if ($map.Parent.FullName -ne $opslag -or $map.Name -notmatch '^[0-9a-fA-F]{64}$' -or $actueel.Contains($map.Name)) {
            throw 'Snapshotselectie is niet meer veilig. Opruimen onderbroken.'
        }
        Write-Host "[VERWIJDEREN] HASH $($map.Name)"
        Remove-Item -LiteralPath $map.FullName -Recurse -Force -ErrorAction Stop
    }
    Write-Host "`n[CCPCHECK] Klaar: $($rijen.Count) oude runs en $($groepen.Count) ongebruikte snapshots verwijderd." -ForegroundColor Green
    Write-Host 'Geen broncode, actuele snapshots, onvolledige runs of centrale compilatiedatabases gewijzigd.'
} catch {
    Write-Host "`n[CCPCHECK] STOP: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
} finally {
    if ($vergrendeld) { $mutex.ReleaseMutex() }
    $mutex.Dispose()
}
