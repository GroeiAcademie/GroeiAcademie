# Compilatiedatabases van de bestaande FrameWork-tests bewaren en samenvoegen.
# De losse Cppcheck-update bewaart ook de echte Arduino .ino.cpp-preprocessing.
param(
    [string]$Capture,
    [ValidateSet('Gewoon', 'Ongeldig')]
    [string]$Mode = 'Gewoon',
    [switch]$Finalize,
    [switch]$UpdateAll,
    [switch]$CountOnly,
    [switch]$ListHashes,
    [string]$TestScript,
    [string]$SnapshotRoot,
    [string]$CliPath,
    [string]$OutputDatabase,
    [string]$ProefHashes,
    [string]$RapportInoRegels,
    [switch]$CacheResponsbestanden
)

$ErrorActionPreference = 'Stop'
if ($SnapshotRoot) { $SnapshotRoot = [System.IO.Path]::GetFullPath($SnapshotRoot) }
$utf8 = [System.Text.UTF8Encoding]::new($false)
$database = if ($OutputDatabase) { $OutputDatabase } elseif ($env:CCPCHECK_DATA_PATH) { Join-Path $env:CCPCHECK_DATA_PATH 'compile_commands.json' } else { $null }
$databaseOngeldig = if ($env:CCPCHECK_DATA_PATH) { Join-Path $env:CCPCHECK_DATA_PATH 'compile_commands_ongeldig.json' } else { $null }
$opslag = if ($SnapshotRoot) { $SnapshotRoot } elseif ($env:CCPCHECK_DATA_PATH) { Join-Path $env:CCPCHECK_DATA_PATH 'Databases' } else { $null }
if (-not $opslag -or (-not $TestScript -and -not $Capture -and -not $database)) { throw 'CCPCHECK_DATA_PATH ontbreekt. Stel het pad in via extras\LokalePaden.cmd.' }
if ($PSBoundParameters.ContainsKey('ProefHashes')) {
    if (-not (($Finalize -and -not $TestScript -and -not $SnapshotRoot) -or
        ($TestScript -and -not $Finalize -and $SnapshotRoot -and $CliPath)) -or
        $Capture -or $UpdateAll -or $CountOnly -or $ListHashes) {
        throw '-ProefHashes is uitsluitend toegestaan met -Finalize zonder -SnapshotRoot, of met -TestScript, -SnapshotRoot en -CliPath.'
    }
    if (-not $ProefHashes -or $ProefHashes -notmatch '^[0-9a-f]{64}(?:,[0-9a-f]{64})*$') {
        throw '-ProefHashes verwacht een of meer bestaande SHA-256-hashes, gescheiden door komma''s.'
    }
    if ($Finalize) {
        if (-not $OutputDatabase) { throw '-ProefHashes vereist -OutputDatabase voor een afzonderlijke proefdatabase.' }
        if ([System.IO.Path]::GetFullPath($database).Equals([System.IO.Path]::GetFullPath((Join-Path $env:CCPCHECK_DATA_PATH 'compile_commands.json')), [System.StringComparison]::OrdinalIgnoreCase) -or
            [System.IO.Path]::GetFullPath($database).Equals([System.IO.Path]::GetFullPath((Join-Path $env:CCPCHECK_DATA_PATH 'compile_commands_ongeldig.json')), [System.StringComparison]::OrdinalIgnoreCase) -or
            [System.IO.Path]::GetFullPath($database).StartsWith([System.IO.Path]::GetFullPath($opslag).TrimEnd('\', '/') + [System.IO.Path]::DirectorySeparatorChar, [System.StringComparison]::OrdinalIgnoreCase)) {
            throw '-ProefHashes mag geen bestaande volledige database of snapshot overschrijven.'
        }
    }
}


# Alleen de leesbare rapportweergave corrigeren: de ruwe XML, snapshots en .ino.cpp blijven intact.
if ($PSBoundParameters.ContainsKey('RapportInoRegels')) {
    if (-not $RapportInoRegels -or -not $OutputDatabase -or $Capture -or $Finalize -or $UpdateAll -or
        $CountOnly -or $ListHashes -or $TestScript -or $SnapshotRoot -or $ProefHashes) {
        throw '-RapportInoRegels vereist een XML-rapport en -OutputDatabase, zonder andere verwerkingsmodi.'
    }
    if (-not (Test-Path -LiteralPath $RapportInoRegels -PathType Leaf)) { throw "Cppcheck-XML ontbreekt: $RapportInoRegels" }
    if (-not (Test-Path -LiteralPath $database -PathType Leaf)) { throw "Cppcheck-compilatiedatabase ontbreekt: $database" }

    [xml]$geladen = Get-Content -LiteralPath $RapportInoRegels -Raw -Encoding UTF8
    $items = @(ConvertFrom-Json -InputObject (Get-Content -LiteralPath $database -Raw -Encoding UTF8))
    if ($items.Count -eq 1 -and $items[0] -is [array]) { $items = @($items[0]) }
    $inoRegelCorrectiePerSketch = [System.Collections.Generic.Dictionary[string,object]]::new([System.StringComparer]::OrdinalIgnoreCase)

    # Meet de #line-afwijking afzonderlijk voor iedere gegenereerde .ino.cpp.
    # Dezelfde .ino kan voor verschillende boards anders zijn gegenereerd.
    foreach ($item in $items) {
        $bron = ([string]$item.file) -replace '[\\/]+', '/'
        if (-not $bron.EndsWith('.ino.cpp', [System.StringComparison]::OrdinalIgnoreCase)) { continue }
        if (-not (Test-Path -LiteralPath $bron -PathType Leaf)) { throw "Gegenereerde .ino.cpp voor regelrapport ontbreekt: $bron" }
        $a = -1
        $b = ''
        foreach ($regel in (Select-String -LiteralPath $bron -Pattern '^\s*#line\s+\d+\s+"[^"]+\.ino"' -Encoding UTF8)) {
            $patroon = [regex]::Match($regel.Line, '^\s*#line\s+(\d+)\s+"([^"]+\.ino)"')
            if (-not $patroon.Success) { continue }
            $pad = $patroon.Groups[2].Value -replace '[\\/]+', '/'
            if ($a -ge 0 -and $b.Equals($pad, [System.StringComparison]::OrdinalIgnoreCase) -and
                [int]$patroon.Groups[1].Value -lt $a) {
                $delen = @(
                    ($a - [int]$patroon.Groups[1].Value + 2)
                    ([int]$patroon.Groups[1].Value)
                )
                if ($inoRegelCorrectiePerSketch.ContainsKey($bron)) {
                    if ($inoRegelCorrectiePerSketch[$bron][0] -ne $delen[0] -or
                        $inoRegelCorrectiePerSketch[$bron][1] -ne $delen[1] -or
                        -not $inoRegelCorrectiePerSketch[$bron][3].Equals($pad, [System.StringComparison]::OrdinalIgnoreCase)) {
                        throw "Verschillende #line-regelcorrecties voor hetzelfde gegenereerde bestand: $bron. Rapport niet aangemaakt."
                    }
                } else {
                    $inoRegelCorrectiePerSketch[$bron] = @(
                        $delen[0]
                        $delen[1]
                        $(if (Test-Path -LiteralPath $pad -PathType Leaf) { @(Get-Content -LiteralPath $pad -Encoding UTF8).Count } else { 0 })
                        $pad
                    )
                }
                break
            }
            $a = [int]$patroon.Groups[1].Value
            $b = $pad
        }
    }

    $uitvoer = [System.IO.Path]::ChangeExtension([System.IO.Path]::GetFullPath($RapportInoRegels), '.ino-regels.txt')
    $aantal = 0
    $writer = [System.IO.StreamWriter]::new($uitvoer, $false, $utf8)
    try {
        $writer.WriteLine('Cppcheck-rapportweergave met controle van originele .ino-regelnummers')
        $writer.WriteLine("Ruwe XML (ongewijzigd): $RapportInoRegels")
        $writer.WriteLine("Compilatiedatabase: $database")
        $writer.WriteLine('Bij een correctie staat het oorspronkelijke Cppcheck-regelnummer tussen haakjes.')
        $writer.WriteLine('')
        foreach ($item in @($geladen.results.errors.error)) {
            if (-not $item) { continue }
            if (-not $item.location) {
                $writer.WriteLine("$($item.severity) | $($item.id) | $($item.msg)")
                continue
            }
            foreach ($regel in @($item.location)) {
                $pad = [string]$regel.file
                $bron = $pad -replace '[\\/]+', '/'
                $a = 0
                if (-not [int]::TryParse([string]$regel.line, [ref]$a)) {
                    $writer.WriteLine("$pad | $($item.severity) | $($item.id) | $($item.msg)")
                    continue
                }
                if ($bron.EndsWith('.ino', [System.StringComparison]::OrdinalIgnoreCase)) {
                    $delen = $null
                    # file0 verwijst naar de exacte .ino.cpp (en dus de juiste boardconfiguratie).
                    $b = ([string]$item.file0) -replace '[\\/]+', '/'
                    if ($b) {
                        if ($inoRegelCorrectiePerSketch.ContainsKey($b)) {
                            $delen = $inoRegelCorrectiePerSketch[$b]
                            if (-not $delen[3].Equals($bron, [System.StringComparison]::OrdinalIgnoreCase)) { $delen = $null }
                        }
                    } else {
                        # Zonder file0 alleen corrigeren als alle configuraties dezelfde afwijking tonen.
                        foreach ($b in $inoRegelCorrectiePerSketch.Keys) {
                            if (-not $inoRegelCorrectiePerSketch[$b][3].Equals($bron, [System.StringComparison]::OrdinalIgnoreCase)) { continue }
                            if ($null -eq $delen) {
                                $delen = $inoRegelCorrectiePerSketch[$b]
                            } elseif ($delen[0] -ne $inoRegelCorrectiePerSketch[$b][0] -or
                                $delen[1] -ne $inoRegelCorrectiePerSketch[$b][1]) {
                                $delen = $null
                                break
                            }
                        }
                    }
                    if ($null -ne $delen) {
                        $b = $a - [int]$delen[0]
                        if ($a -ge ([int]$delen[0] + [int]$delen[1]) -and $b -ge 1 -and $b -le [int]$delen[2]) {
                            $writer.WriteLine("${pad}:$b (Cppcheck: $a) | $($item.severity) | $($item.id) | $($item.msg)")
                            $aantal++
                            continue
                        }
                    }
                }
                $writer.WriteLine("${pad}:$a | $($item.severity) | $($item.id) | $($item.msg)")
            }
        }
        $writer.WriteLine('')
        $writer.WriteLine("Gecorrigeerde .ino-locaties: $aantal")
        $writer.WriteLine('Locaties zonder bewezen #line-correctie zijn ongewijzigd weergegeven.')
    } finally {
        $writer.Dispose()
    }
    Write-Host "[CCPCHECK] Leesbaar regelrapport: $uitvoer | $aantal .ino-locaties gecorrigeerd."
    exit 0
}

# De Cppcheck-update is losgekoppeld van de bestaande compilatie-CMD's.
# Lees de bestaande testlijsten; voer nooit TestLibraryCommon.cmd uit.
if ($TestScript) {
    if (-not $SnapshotRoot -or (-not $CliPath -and -not $ListHashes -and -not $CountOnly)) { throw 'SnapshotRoot of CliPath ontbreekt.' }

    $framework = (Resolve-Path -LiteralPath (Join-Path $PSScriptRoot '..')).Path
    $common = Join-Path $framework 'extras\TestLibraryCommon.cmd'
    if ($TestScript -notin @('Data','Extenders','GedeeldeBus','Input','Screen','Sensoren','Sturingen','Tools','Foutinjecties')) {
        throw "Geen gewoon testdomein: $TestScript"
    }
    $testbestand = Join-Path $framework ('extras\TestLibrary' + $TestScript + '-v2.0.0.cmd')
    if (-not (Test-Path -LiteralPath $testbestand)) { throw "Test-CMD ontbreekt: $testbestand" }
    if (-not $CountOnly -and -not $ListHashes) {
        if (-not (Test-Path -LiteralPath $CliPath) -and (Test-Path -LiteralPath ($CliPath + '.exe'))) { $CliPath += '.exe' }
        if (-not (Test-Path -LiteralPath $CliPath -PathType Leaf)) { throw "Arduino CLI ontbreekt: $CliPath" }
    }

    # De bestaande FQBN's en BOARD_VERSION-koppeling letterlijk uitlezen.
    $instellingen = @{}
    foreach ($regel in (Get-Content -LiteralPath $common -Encoding UTF8)) {
        if ($regel -match '^set (BASIC|EXTENDED|EXPERIMENTAL)_FQBN(_MAPPING_BOARDS)?=(.*)$') {
            $instellingen[$Matches[1] + '_FQBN' + $Matches[2]] = @(
                [regex]::Matches($Matches[3], '"([^"]*)"') | ForEach-Object { $_.Groups[1].Value }
            )
        }
    }
    foreach ($groep in @('BASIC','EXTENDED','EXPERIMENTAL')) {
        $a = @($instellingen[$groep + '_FQBN'])
        $b = @($instellingen[$groep + '_FQBN_MAPPING_BOARDS'])
        if ($a.Count -eq 0 -or $a.Count -ne $b.Count) {
            throw "FQBN-mapping ontbreekt of is gewijzigd in TestLibraryCommon.cmd: $groep"
        }
    }

    # bronHash blijft voor momentopname/consistentie bij generatie; niet voor database-hergebruik.
    # Cppcheck leest de actuele FrameWork-broncode; compileropdrachten en .ino.cpp hebben aparte controles.
    if (-not $CountOnly -and -not $ListHashes) {
        $sha = [System.Security.Cryptography.SHA256]::Create()
        try {
            $bronHash = [BitConverter]::ToString($sha.ComputeHash([System.Text.Encoding]::UTF8.GetBytes((
                (Get-ChildItem -LiteralPath (Join-Path $framework 'src') -Recurse -File |
                    Where-Object { $_.Extension -in @('.h', '.hh', '.hpp', '.hxx', '.cpp', '.c', '.cc', '.cxx', '.ino', '.pde', '.tpp', '.ipp', '.inc') } |
                    Sort-Object FullName |
                    ForEach-Object { $_.FullName.Substring($framework.Length + 1) + ':' + (Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256).Hash }) -join "`n"
            )))).Replace('-', '').ToLowerInvariant()
        } finally {
            $sha.Dispose()
        }
        $sketchHashes = @{}
    }

    $regels = @(Get-Content -LiteralPath $testbestand -Encoding UTF8)
    $start = [array]::IndexOf($regels, ':TESTLIJST')
    if ($start -lt 0) { throw "TESTLIJST ontbreekt in $testbestand" }
    $waarden = @{}
    $aantal = 0
    $geslaagd = 0
    $hergebruikt = 0
    $overgeslagen = 0
    $mislukt = 0

    if (-not $CountOnly -and -not $ListHashes) {
        if ($ProefHashes) {
            $uitvoer = @(& powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $PSCommandPath -TestScript $TestScript -SnapshotRoot $SnapshotRoot -ListHashes 2>&1)
            if ($LASTEXITCODE -ne 0) { throw "Proefhashes opvragen mislukt: $TestScript`n$($uitvoer -join "`n")" }
            foreach ($hash in $ProefHashes.Split(',')) {
                if ($hash -notin $uitvoer) { throw "Proefhash komt niet voor in domein ${TestScript}: $hash" }
            }
            $totaal = @($ProefHashes.Split(',') | Sort-Object -Unique).Count
        } else {
            $uitvoer = & powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $PSCommandPath -TestScript $TestScript -SnapshotRoot $SnapshotRoot -CountOnly 2>&1
            if ($LASTEXITCODE -ne 0 -or ($uitvoer -join '').Trim() -notmatch '^\d+$') {
                throw "Configuraties tellen mislukt: $TestScript`n$($uitvoer -join "`n")"
            }
            $totaal = [int](($uitvoer -join '').Trim())
        }
        Write-Host "[CCPCHECK] $TestScript : 0/$totaal"
    }

    foreach ($regel in $regels[($start+1)..($regels.Count-1)]) {
        if ($regel -match '^set ([A-Z_]+)=(.*)$') {
            $waarden[$Matches[1]] = $Matches[2].Replace('"', '')
            continue
        }
        if ($regel -notmatch '^call .*TestLibraryCommon\.cmd"? --VOER_DEZE_TEST_UIT\s*$') { continue }

        # Zelfde INCLUDE-selectie als TestLibraryCommon.cmd.
        if ($waarden['TEST_TYPE'] -notin @('NIEUW', 'EXPERIMENTEEL') -or $waarden['TEST_NAAM'] -eq 'SCREEN_OUTPUT_CONFIG=1') {
            if (-not $ProefHashes) { $overgeslagen++ }
            [void]$waarden.Remove('WELKE_FQBN_SKIPPEN')
            [void]$waarden.Remove('WELKE_FQBN_INCOMPATIBLE')
            continue
        }
        $ino = $waarden['TEST_INO']
        $pad = Join-Path $framework $ino
        if (-not (Test-Path -LiteralPath $pad -PathType Leaf)) { throw "Sketch ontbreekt: $pad" }
        $selectie = $waarden['WELKE_FQBN_TESTEN']
        $groepen = switch ($selectie) {
            'BIJ_BASIC_FQBN' { @('BASIC'); break }
            'BIJ_EXTENDED_FQBN' { @('EXTENDED'); break }
            'BIJ_EXPERIMENTAL_FQBN' { @('EXPERIMENTAL'); break }
            'BIJ_ALLE_FQBN' { @('BASIC','EXTENDED','EXPERIMENTAL'); break }
            default { throw "Onbekende boardselectie: $selectie" }
        }

        foreach ($groep in $groepen) {
            $fqbnLijst = @($instellingen[$groep + '_FQBN'])
            $mappingLijst = @($instellingen[$groep + '_FQBN_MAPPING_BOARDS'])
            for ($i=0; $i -lt $fqbnLijst.Count; $i++) {
                $fqbn = $fqbnLijst[$i]
                $teSkippen = @($waarden['WELKE_FQBN_SKIPPEN'], $waarden['WELKE_FQBN_INCOMPATIBLE']) -join ','
                if ((',' + $teSkippen + ',').IndexOf(',' + $fqbn + ',', [StringComparison]::OrdinalIgnoreCase) -ge 0) {
                    if (-not $ProefHashes) { $overgeslagen++ }
                    continue
                }
                foreach ($board in ($mappingLijst[$i] -split '\|')) {
                    if (-not $ProefHashes) { $aantal++ }
                    if ($CountOnly) { continue }

                    # Identieke hash als de bestaande -Capture-code.
                    $bestandsnaam = 'TestLibrary' + $TestScript + '-v2.0.0'
                    $sleutel = @('Gewoon', $bestandsnaam, $ino, $waarden['TEST_NAAM'], $waarden['TEST_FLAGS'], $fqbn, $board) -join "`n"
                    $sha = [System.Security.Cryptography.SHA256]::Create()
                    try {
                        $hash = [BitConverter]::ToString($sha.ComputeHash([System.Text.Encoding]::UTF8.GetBytes($sleutel))).Replace('-', '').ToLowerInvariant()
                    } finally {
                        $sha.Dispose()
                    }
                    if ($ListHashes) {
                        if ($Finalize) {
                            Write-Output "$hash|$ino|$fqbn|$board"
                        } else {
                            Write-Output $hash
                        }
                        continue
                    }
                    if ($ProefHashes) {
                        if ($hash -notin $ProefHashes.Split(',')) { continue }
                        $aantal++
                    }

                    $doelmap = Join-Path (Join-Path $SnapshotRoot 'Gewoon') $hash
                    $metadataPad = Join-Path $doelmap 'metadata.json'
                    $databasePad = Join-Path $doelmap 'compile_commands.json'
                    # Alle sketch-tabs en bijhorende codebestanden tellen mee, niet alleen het hoofd-.ino.
                    if (-not $sketchHashes.ContainsKey($pad)) {
                        $sha = [System.Security.Cryptography.SHA256]::Create()
                        try {
                            $sketchHashes[$pad] = [BitConverter]::ToString($sha.ComputeHash([System.Text.Encoding]::UTF8.GetBytes((
                                (Get-ChildItem -LiteralPath (Split-Path -Parent $pad) -Recurse -File |
                                    Where-Object { $_.Extension -in @('.ino', '.pde', '.h', '.hh', '.hpp', '.hxx', '.cpp', '.c', '.cc', '.cxx', '.tpp', '.ipp', '.inc') } |
                                    Sort-Object FullName |
                                    ForEach-Object { $_.FullName.Substring($framework.Length + 1) + ':' + (Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256).Hash }) -join "`n"
                            )))).Replace('-', '').ToLowerInvariant()
                        } finally {
                            $sha.Dispose()
                        }
                    }
                    $sketchHash = $sketchHashes[$pad]
                    $herbruikbaar = $false
                    if ((Test-Path -LiteralPath $metadataPad -PathType Leaf) -and (Test-Path -LiteralPath $databasePad -PathType Leaf)) {
                        try {
                            $metadata = ConvertFrom-Json -InputObject (Get-Content -LiteralPath $metadataPad -Raw -Encoding UTF8)
                            if ($metadata.databaseBeschikbaar -and $metadata.compileResult -eq '0' -and
                                $metadata.testscript -eq $bestandsnaam -and $metadata.test -eq $ino -and
                                $metadata.testNaam -eq $waarden['TEST_NAAM'] -and $metadata.flags -eq $waarden['TEST_FLAGS'] -and
                                $metadata.fqbn -eq $fqbn -and $metadata.boardVersion -eq $board -and
                                $metadata.PSObject.Properties['sketchHash'] -and $metadata.sketchHash -eq $sketchHash) {
                                $geladen = ConvertFrom-Json -InputObject (Get-Content -LiteralPath $databasePad -Raw -Encoding UTF8)
                                $items = if ($geladen.PSObject.Properties['value'] -and $geladen.value) { @($geladen.value) } else { @($geladen) }
                                if ($items.Count -eq 1 -and $items[0] -is [array]) { $items = @($items[0]) }
                                $sketchItem = @($items | Where-Object { $_.PSObject.Properties['file'] -and ([string]$_.file) -like '*.ino.cpp' })
                                if ($sketchItem.Count -gt 0 -and -not (Test-Path -LiteralPath ([string]$sketchItem[0].file) -PathType Leaf)) {
                                    # Herstel uitsluitend oude relatieve snapshotpaden wanneer de .ino.cpp lokaal bestaat.
                                    $lokaleSketch = Join-Path (Join-Path $doelmap 'sketch') (Split-Path -Leaf ([string]$sketchItem[0].file))
                                    $patroon = [regex]::Match(([string]$sketchItem[0].file), '(?i)^(.*?)[\\/]sketch[\\/][^\\/]+\.ino\.cpp$')
                                    if ($patroon.Success -and (Test-Path -LiteralPath $lokaleSketch -PathType Leaf)) {
                                        $patroon = $patroon.Groups[1].Value
                                        $doelPad = $doelmap.Replace('\', '/')
                                        foreach ($item in $items) {
                                            foreach ($veld in @('directory', 'file', 'command', 'output')) {
                                                if ($item.PSObject.Properties[$veld] -and $null -ne $item.$veld) {
                                                    $item.$veld = ([string]$item.$veld).Replace($patroon, $doelPad)
                                                }
                                            }
                                            if ($item.PSObject.Properties['arguments'] -and $item.arguments) {
                                                $item.arguments = @($item.arguments | ForEach-Object { ([string]$_).Replace($patroon, $doelPad) })
                                            }
                                        }
                                        if (Test-Path -LiteralPath ([string]$sketchItem[0].file) -PathType Leaf) {
                                            $bestand = $databasePad + '.padcorrectie.tmp'
                                            [System.IO.File]::WriteAllText($bestand, (ConvertTo-Json -InputObject $items -Depth 30 -Compress), $utf8)
                                            # Zelfde veilige mapwissel als bij Capture: oude snapshot terugzetten als het vervangen mislukt.
                                            if (Test-Path -LiteralPath ($databasePad + '.padcorrectie.vorig')) {
                                                throw "Back-up van compilatiedatabase bestaat nog: $($databasePad + '.padcorrectie.vorig')"
                                            }
                                            Move-Item -LiteralPath $databasePad -Destination ($databasePad + '.padcorrectie.vorig')
                                            try {
                                                Move-Item -LiteralPath $bestand -Destination $databasePad
                                            } catch {
                                                Move-Item -LiteralPath ($databasePad + '.padcorrectie.vorig') -Destination $databasePad
                                                throw
                                            }
                                            Remove-Item -LiteralPath ($databasePad + '.padcorrectie.vorig') -Force
                                        }
                                    }
                                }
                                if ($items.Count -gt 0 -and $sketchItem.Count -gt 0 -and
                                    @($items | Where-Object {
                                        ($_.PSObject.Properties['arguments'] -and @($_.arguments | Where-Object { [string]$_ -match '^(?:@|-imacros|-iprefix|-iwithprefix)' }).Count -gt 0) -or
                                        ($_.PSObject.Properties['command'] -and $_.command -match '(?<!\S)(?:"?@|-imacros|-iprefix|-iwithprefix)')
                                    }).Count -eq 0 -and
                                    @($sketchItem | Where-Object { -not (Test-Path -LiteralPath ([string]$_.file) -PathType Leaf) }).Count -eq 0 -and
                                    @($sketchItem | Where-Object { -not (Select-String -LiteralPath ([string]$_.file) -Pattern '^\s*#line\s+\d+\s+.*\.ino' -Quiet) }).Count -eq 0) {
                                    $herbruikbaar = $true
                                } elseif ($items.Count -eq 0) {
                                    Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: compile_commands.json bevat geen compileropdrachten."
                                } elseif ($sketchItem.Count -eq 0) {
                                    Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: .ino.cpp ontbreekt in compile_commands.json."
                                } else {
                                    Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: .ino.cpp bestaat niet op het pad $($sketchItem[0].file)."
                                }
                            } elseif (-not $metadata.databaseBeschikbaar) {
                                Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: databaseBeschikbaar is niet True."
                            } elseif ($metadata.compileResult -ne '0') {
                                Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: compileResult is '$($metadata.compileResult)', verwacht '0'."
                            } elseif ($metadata.testscript -ne $bestandsnaam) {
                                Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: testscript is '$($metadata.testscript)', verwacht '$bestandsnaam'."
                            } elseif ($metadata.test -ne $ino) {
                                Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: test is '$($metadata.test)', verwacht '$ino'."
                            } elseif ($metadata.testNaam -ne $waarden['TEST_NAAM']) {
                                Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: testNaam verschilt van de actuele TEST_NAAM."
                            } elseif ($metadata.flags -ne $waarden['TEST_FLAGS']) {
                                Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: flags verschillen van de actuele TEST_FLAGS."
                            } elseif ($metadata.fqbn -ne $fqbn) {
                                Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: fqbn is '$($metadata.fqbn)', verwacht '$fqbn'."
                            } elseif ($metadata.boardVersion -ne $board) {
                                Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: boardVersion is '$($metadata.boardVersion)', verwacht '$board'."
                            } elseif (-not $metadata.PSObject.Properties['sketchHash']) {
                                Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: sketchHash ontbreekt in metadata.json."
                            } elseif ($metadata.sketchHash -ne $sketchHash) {
                                Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: sketchHash verschilt."
                            } else {
                                Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: onbekend metadata-verschil."
                            }
                        } catch {
                            $herbruikbaar = $false
                            Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: fout bij lezen of controleren van snapshot: $($_.Exception.Message)"
                        }
                    } elseif (-not (Test-Path -LiteralPath $metadataPad -PathType Leaf)) {
                        Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: metadata.json ontbreekt."
                    } else {
                        Write-Warning "[CCPCHECK] $TestScript | $hash | afkeur: compile_commands.json ontbreekt."
                    }
                    if ($herbruikbaar) {
                        $hergebruikt++
                        Write-Output "[CCPCHECK_STAP] $TestScript : $aantal/$totaal | hergebruikt | $ino | $fqbn | $board"
                        continue
                    }

                    $env:BESTANDSNAAM = $bestandsnaam
                    $env:TEST_INO_ACTIEF = $ino
                    $env:TEST_NAAM_ACTIEF = $waarden['TEST_NAAM']
                    $env:TEST_FLAGS_ACTIEF = $waarden['TEST_FLAGS']
                    $env:FQBN_ACTIEF = $fqbn
                    $env:BOARD_VERSION_ACTIEF = $board
                    $bouwmap = Join-Path $env:TEMP ('GA_Ccpcheck_' + [guid]::NewGuid().ToString('N'))
                    $herbruikbaar = $false
                    try {
                        $property = 'compiler.cpp.extra_flags=-DGROEIACADEMIE_IGNORE_USER_CONFIG ' + $waarden['TEST_FLAGS'] + ' -DBOARD_VERSION=' + $board
                        # Arduino maakt de sketch/.ino.cpp tijdens de officiële preprocessing.
                        $oudeErrorActionPreference = $ErrorActionPreference
                        try {
                            $ErrorActionPreference = 'Continue'
                            $uitvoer = & $CliPath compile --fqbn $fqbn --build-property $property --build-path $bouwmap --preprocess (Split-Path -Parent $pad) 2>&1 | Select-Object -Last 30
                            $compileExitCode = $LASTEXITCODE
                        } finally {
                            $ErrorActionPreference = $oudeErrorActionPreference
                        }
                        if ($compileExitCode -ne 0) {
                            throw "Sketch-preprocessing mislukt: $ino | $fqbn | $board`n$($uitvoer -join "`n")"
                        }
                        $gegenereerd = @(Get-ChildItem -LiteralPath (Join-Path $bouwmap 'sketch') -Recurse -File -Filter '*.ino.cpp' -ErrorAction SilentlyContinue)
                        if ($gegenereerd.Count -eq 0) {
                            # Alleen wanneer de preprocessmodus geen fysiek .ino.cpp bewaarde.
                            $oudeErrorActionPreference = $ErrorActionPreference
                            try {
                                $ErrorActionPreference = 'Continue'
                                $uitvoer = & $CliPath compile --fqbn $fqbn --build-property $property --build-path $bouwmap (Split-Path -Parent $pad) 2>&1
                                $compileExitCode = $LASTEXITCODE
                            } finally {
                                $ErrorActionPreference = $oudeErrorActionPreference
                            }
                            if ($compileExitCode -ne 0) {
                                throw "Genereren van .ino.cpp via Arduino-compilatie mislukt: $ino | $fqbn | $board`n$($uitvoer -join "`n")"
                            }
                        }
                        $oudeErrorActionPreference = $ErrorActionPreference
                        try {
                            $ErrorActionPreference = 'Continue'
                            $uitvoer = & $CliPath compile --fqbn $fqbn --build-property $property --build-path $bouwmap --only-compilation-database (Split-Path -Parent $pad) 2>&1
                            $compileExitCode = $LASTEXITCODE
                        } finally {
                            $ErrorActionPreference = $oudeErrorActionPreference
                        }
                        if ($compileExitCode -ne 0) {
                            throw "Databaseopbouw mislukt: $ino | $fqbn | $board`n$($uitvoer -join "`n")"
                        }
                        if (-not (Test-Path -LiteralPath (Join-Path $bouwmap 'compile_commands.json'))) {
                            throw "Arduino CLI heeft geen compile_commands.json aangemaakt: $ino | $fqbn | $board"
                        }
                        $gegenereerd = @(Get-ChildItem -LiteralPath (Join-Path $bouwmap 'sketch') -Recurse -File -Filter '*.ino.cpp' -ErrorAction SilentlyContinue)
                        if ($gegenereerd.Count -eq 0) { throw "Gegenereerde .ino.cpp ontbreekt na databaseopbouw: $ino | $fqbn | $board" }
                        # De broncode mag tijdens de generatie niet onopgemerkt gewijzigd zijn.
                        # Anders zou de snapshot met een verouderde bronHash als geldig worden opgeslagen.
                        $sha = [System.Security.Cryptography.SHA256]::Create()
                        try {
                            $env:BRON_HASH_ACTIEF = [BitConverter]::ToString($sha.ComputeHash([System.Text.Encoding]::UTF8.GetBytes((
                                (Get-ChildItem -LiteralPath (Join-Path $framework 'src') -Recurse -File |
                                    Where-Object { $_.Extension -in @('.h', '.hh', '.hpp', '.hxx', '.cpp', '.c', '.cc', '.cxx', '.ino', '.pde', '.tpp', '.ipp', '.inc') } |
                                    Sort-Object FullName |
                                    ForEach-Object { $_.FullName.Substring($framework.Length + 1) + ':' + (Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256).Hash }) -join "`n"
                            )))).Replace('-', '').ToLowerInvariant()
                        } finally {
                            $sha.Dispose()
                        }
                        if ($env:BRON_HASH_ACTIEF -ne $bronHash) {
                            throw "FrameWork-broncode gewijzigd tijdens Cppcheck-verwerking: oorspronkelijk $bronHash, nu $($env:BRON_HASH_ACTIEF). Bestaande snapshots blijven behouden."
                        }
                        $env:SKETCH_HASH_ACTIEF = $sketchHash
                        $env:COMPILE_RESULT = '0'
                        & $PSCommandPath -Capture $bouwmap -Mode Gewoon -SnapshotRoot $SnapshotRoot -CacheResponsbestanden:$CacheResponsbestanden
                        if ($LASTEXITCODE -ne 0) { throw "Snapshot opslaan mislukt: $ino | $fqbn | $board" }
                        $herbruikbaar = $true
                        $geslaagd++
                        Write-Output "[CCPCHECK_STAP] $TestScript : $aantal/$totaal | nieuw | $ino | $fqbn | $board"
                    } catch {
                        $mislukt++
                        $herbruikbaar = $false
                        Write-Warning "[CCPCHECK] $TestScript | $hash | fout: $($_.Exception.Message)"
                        Write-Output "[CCPCHECK_STAP] $TestScript : $aantal/$totaal | mislukt | $ino | $fqbn | $board"
                    } finally {
                        if ($herbruikbaar) {
                            if (Test-Path -LiteralPath $bouwmap) { Remove-Item -LiteralPath $bouwmap -Recurse -Force }
                        } else {
                            Write-Warning "Arduino-buildmap voor diagnose behouden: $bouwmap"
                        }
                    }
                }
            }
        }
        [void]$waarden.Remove('WELKE_FQBN_SKIPPEN')
        [void]$waarden.Remove('WELKE_FQBN_INCOMPATIBLE')
    }
    if ($aantal -eq 0) { throw "Geen Cppcheck-configuraties gevonden in $testbestand" }
    if ($CountOnly) { Write-Output $aantal; exit 0 }
    if ($ListHashes) { exit 0 }
    Write-Output "[CCPCHECK] $TestScript : $geslaagd nieuw, $hergebruikt hergebruikt, $mislukt mislukt, $overgeslagen overgeslagen"
    if ($mislukt -gt 0) { throw "$mislukt Cppcheck-configuratie(s) mislukt in $TestScript. Geldige snapshots behouden." }
    exit 0
}

if ($UpdateAll) {
    if (-not $CliPath) { $CliPath = $env:CLI_PATH }
    if (-not $CliPath) { throw 'CLI_PATH is leeg; controleer de bestaande LokalePaden.cmd of PATH.' }
    if (-not (Test-Path -LiteralPath $CliPath) -and (Test-Path -LiteralPath ($CliPath + '.exe'))) { $CliPath += '.exe' }
    if (-not (Test-Path -LiteralPath $CliPath -PathType Leaf)) { throw "Arduino CLI ontbreekt: $CliPath" }

    # Snapshots blijven permanent bestaan. Geen stagingboom die bij afbreken wordt gewist.
    $snapshotRoot = $opslag
    New-Item -ItemType Directory -Path (Join-Path $snapshotRoot 'Gewoon') -Force | Out-Null
    $domeinen = @('Data','Extenders','GedeeldeBus','Input','Screen','Sensoren','Sturingen','Tools','Foutinjecties')
    $aantal = 0
    foreach ($domein in $domeinen) {
        $uitvoer = & powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $PSCommandPath -TestScript $domein -SnapshotRoot $snapshotRoot -CountOnly 2>&1
        if ($LASTEXITCODE -ne 0 -or ($uitvoer -join '').Trim() -notmatch '^\d+$') {
            throw "Configuraties tellen mislukt: $domein`n$($uitvoer -join "`n")"
        }
        $aantal += [int](($uitvoer -join '').Trim())
    }
    Write-Host "[CCPCHECK] Voortgang: 0/$aantal"

    # Negen gewone testdomeinen parallel. Geen Ongeldig-scripts en geen normale compilaties.
    $jobs = @()
    $voortgang = 0
    try {
        foreach ($domein in $domeinen) {
            $jobs += Start-Job -ScriptBlock {
                param($script, $domein, $snapshotRoot, $cli, $cache)
                if ($cache) {
                    & powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $script -TestScript $domein -SnapshotRoot $snapshotRoot -CliPath $cli -CacheResponsbestanden
                } else {
                    & powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $script -TestScript $domein -SnapshotRoot $snapshotRoot -CliPath $cli
                }
                if ($LASTEXITCODE -ne 0) { throw "Cppcheck-domein mislukt: $domein" }
            } -ArgumentList @($PSCommandPath, $domein, $snapshotRoot, $CliPath, $CacheResponsbestanden.IsPresent)
        }
        do {
            Start-Sleep -Seconds 2
            foreach ($job in $jobs) {
                foreach ($melding in @(Receive-Job -Job $job -ErrorAction SilentlyContinue)) {
                    if (([string]$melding).StartsWith('[CCPCHECK_STAP]')) {
                        $voortgang++
                        Write-Host "[CCPCHECK] Voortgang: $voortgang/$aantal | $(([string]$melding).Substring(16).Trim())"
                    } else {
                        Write-Host $melding
                    }
                }
            }
        } while (@($jobs | Where-Object { $_.State -in @('Running', 'NotStarted') }).Count -gt 0)

        foreach ($job in $jobs) {
            foreach ($melding in @(Receive-Job -Job $job -ErrorAction SilentlyContinue)) {
                if (([string]$melding).StartsWith('[CCPCHECK_STAP]')) {
                    $voortgang++
                    Write-Host "[CCPCHECK] Voortgang: $voortgang/$aantal | $(([string]$melding).Substring(16).Trim())"
                } else {
                    Write-Host $melding
                }
            }
        }
        $mislukt = @($jobs | Where-Object { $_.State -ne 'Completed' })
        if ($mislukt.Count -gt 0 -or $voortgang -ne $aantal) {
            foreach ($job in $mislukt) { Write-Host "[CCPCHECK] Fout in $($job.Name): $($job.ChildJobs[0].JobStateInfo.Reason)" }
            throw "Verzamelen onvolledig ($voortgang/$aantal). Geldige snapshots behouden. Samenvoegen nog niet uitvoeren."
        }
        Write-Host "[CCPCHECK] Verzamelen voltooid: $voortgang/$aantal. Start daarna extras\ccpcheckSamenvoegen.cmd."
    } finally {
        foreach ($job in $jobs) {
            if ($job.State -in @('Running','NotStarted')) { Stop-Job -Job $job -ErrorAction SilentlyContinue }
        }
        $jobs | Remove-Job -Force -ErrorAction SilentlyContinue
    }
    exit 0
}


if ($Finalize -and -not $SnapshotRoot) {
    # Lees de actuele hashes opnieuw uit exact de negen gewone test-CMD's.
    $framework = (Resolve-Path -LiteralPath (Join-Path $PSScriptRoot '..')).Path
    $basis = Join-Path $opslag 'Gewoon'
    $sha = [System.Security.Cryptography.SHA256]::Create()
    try {
        $bronHash = [BitConverter]::ToString($sha.ComputeHash([System.Text.Encoding]::UTF8.GetBytes((
            (Get-ChildItem -LiteralPath (Join-Path $framework 'src') -Recurse -File |
                Where-Object { $_.Extension -in @('.h', '.hh', '.hpp', '.hxx', '.cpp', '.c', '.cc', '.cxx', '.ino', '.pde', '.tpp', '.ipp', '.inc') } |
                Sort-Object FullName |
                ForEach-Object { $_.FullName.Substring($framework.Length + 1) + ':' + (Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256).Hash }) -join "`n"
        )))).Replace('-', '').ToLowerInvariant()
    } finally {
        $sha.Dispose()
    }
    $sketchHashes = @{}
    $actueel = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::OrdinalIgnoreCase)
    $beschrijving = @{}
    foreach ($domein in @('Data','Extenders','GedeeldeBus','Input','Screen','Sensoren','Sturingen','Tools','Foutinjecties')) {
        $uitvoer = @(& powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $PSCommandPath -TestScript $domein -SnapshotRoot $opslag -ListHashes -Finalize 2>&1)
        if ($LASTEXITCODE -ne 0) { throw "Actuele configuraties lezen mislukt: $domein`n$($uitvoer -join "`n")" }
        foreach ($regel in $uitvoer) {
            $delen = ([string]$regel) -split '\|', 4
            if ($delen.Count -ne 4 -or $delen[0] -notmatch '^[0-9a-f]{64}$') { throw "Ongeldige configuratie uit $domein : $regel" }
            $hash = $delen[0]
            [void]$actueel.Add($hash)
            $beschrijving[$hash] = "$domein | sketch=$($delen[1]) | FQBN=$($delen[2]) | board=$($delen[3])"
        }
    }
    if ($actueel.Count -eq 0) { throw 'Geen actuele gewone configuraties gevonden. Niets samengevoegd of opgeruimd.' }
    if ($ProefHashes) {
        foreach ($hash in $ProefHashes.Split(',')) {
            if (-not $actueel.Contains($hash)) { throw "Proefhash komt niet voor in de actuele testlijsten: $hash" }
        }
        $actueel.IntersectWith($ProefHashes.Split(','))
    }

    $mutex = [System.Threading.Mutex]::new($false, 'Local\GroeiAcademieCcpcheckMerge')
    $vergrendeld = $false
    $tijdelijkeUitvoer = $database + '.' + [guid]::NewGuid().ToString('N') + '.tmp'
    try {
        try { $vergrendeld = $mutex.WaitOne(600000) }
        catch [System.Threading.AbandonedMutexException] { $vergrendeld = $true }
        if (-not $vergrendeld) { throw 'Timeout bij samenvoegen van Cppcheck-databases.' }

        # Schrijf direct naar een tijdelijk JSON-bestand om duizenden snapshots
        # zonder een volledige database in het PowerShell-geheugen samen te voegen.
        $gezien = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::OrdinalIgnoreCase)
        $gecontroleerdeBronnen = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::OrdinalIgnoreCase)
        # Bestaande FQBN's uit TestLibraryCommon.cmd; definities letterlijk uit de cores.
        $notAnInterruptPerFqbn = @{
            'esp32:esp32:d1_uno32' = '-1'
            'esp32:esp32:esp32s3' = '-1'
            'rp2040:rp2040:cytron_maker_uno_rp2040' = '(-1)'
            'STMicroelectronics:stm32:Nucleo_64:pnum=NUCLEO_F401RE' = '(pin_size_t)NC'
        }
        # UNO Q: bewuste Cppcheck-vervangwaarden voor de terugvaltak, niet de echte devicetree-waarden; 70 = digital-pin-gpios uit arduino_uno_q_stm32u585xx.overlay (Zephyr 1.1.0).
        $zephyrMacroDefinitiesPerFqbn = @{
            'arduino:zephyr:unoq' = @(
                '-DDT_PATH(x)=0'
                '-DDT_NODE_HAS_PROP(n,p)=0'
                '-DDT_PROP_LEN(n,p)=0'
                '-DDT_NODE_EXISTS(n)=1'
                '-DDT_NODELABEL(x)=0'
                '-DDT_FOREACH_PROP_ELEM_SEP(n,p,f,s)=70'
            )
        }
        foreach ($fqbn in @($notAnInterruptPerFqbn.Keys) + @($zephyrMacroDefinitiesPerFqbn.Keys) + @('arduino:renesas_uno:minima')) {
            if (-not (Get-Content -LiteralPath (Join-Path $framework 'extras\TestLibraryCommon.cmd') -Encoding UTF8 |
                Where-Object { $_ -match '^set (?:BASIC|EXTENDED|EXPERIMENTAL)_FQBN=' -and $_.Contains('"' + $fqbn + '"') })) {
                throw "Cppcheck-definities: FQBN ontbreekt in TestLibraryCommon.cmd: $fqbn"
            }
        }
        $aantal = 0
        $verwerkt = 0
        $problemen = [System.Collections.Generic.List[string]]::new()
        $writer = $null
        Write-Host "[CCPCHECK] Samenvoegen: 0/$($actueel.Count)"
        try {
            $writer = [System.IO.StreamWriter]::new($tijdelijkeUitvoer, $false, $utf8)
            $writer.Write('[')
            foreach ($hash in @($actueel | Sort-Object)) {
                $snapshotmap = Join-Path $basis $hash
                $metadatapad = Join-Path $snapshotmap 'metadata.json'
                $bestand = Join-Path $snapshotmap 'compile_commands.json'
                try {
                    if (-not (Test-Path -LiteralPath $metadatapad -PathType Leaf) -or -not (Test-Path -LiteralPath $bestand -PathType Leaf)) {
                        throw "Actuele Cppcheck-snapshot ontbreekt: $hash. Niets opgeruimd."
                    }
                    $metadata = ConvertFrom-Json -InputObject (Get-Content -LiteralPath $metadatapad -Raw -Encoding UTF8)
                    if (-not $metadata.databaseBeschikbaar -or $metadata.compileResult -ne '0' -or
                        -not $metadata.PSObject.Properties['sketchHash']) {
                        throw "Snapshot is verouderd of ongeldig: $hash. Voer eerst het verzamelscript uit."
                    }
                    $sketch = Join-Path $framework $metadata.test
                    if (-not (Test-Path -LiteralPath $sketch -PathType Leaf)) {
                        throw "Sketch ontbreekt sinds snapshot: $($metadata.test). Voer eerst het verzamelscript uit."
                    }
                    if (-not $sketchHashes.ContainsKey($sketch)) {
                        $sha = [System.Security.Cryptography.SHA256]::Create()
                        try {
                            $sketchHashes[$sketch] = [BitConverter]::ToString($sha.ComputeHash([System.Text.Encoding]::UTF8.GetBytes((
                                (Get-ChildItem -LiteralPath (Split-Path -Parent $sketch) -Recurse -File |
                                    Where-Object { $_.Extension -in @('.ino', '.pde', '.h', '.hh', '.hpp', '.hxx', '.cpp', '.c', '.cc', '.cxx', '.tpp', '.ipp', '.inc') } |
                                    Sort-Object FullName |
                                    ForEach-Object { $_.FullName.Substring($framework.Length + 1) + ':' + (Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256).Hash }) -join "`n"
                            )))).Replace('-', '').ToLowerInvariant()
                        } finally {
                            $sha.Dispose()
                        }
                    }
                    if ($sketchHashes[$sketch] -ne $metadata.sketchHash) {
                        throw "Sketch gewijzigd sinds snapshot: $($metadata.test). Voer eerst het verzamelscript uit."
                    }
                    $geladen = ConvertFrom-Json -InputObject (Get-Content -LiteralPath $bestand -Raw -Encoding UTF8)
                    $items = if ($geladen.PSObject.Properties['value'] -and $geladen.value) { @($geladen.value) } else { @($geladen) }
                    if ($items.Count -eq 1 -and $items[0] -is [array]) { $items = @($items[0]) }
                    if ($items.Count -eq 0) { throw "Lege compilatiedatabase: $bestand" }
                    if (@($items | Where-Object {
                                        ($_.PSObject.Properties['arguments'] -and @($_.arguments | Where-Object { [string]$_ -match '^(?:@|-imacros|-iprefix|-iwithprefix)' }).Count -gt 0) -or
                                        ($_.PSObject.Properties['command'] -and $_.command -match '(?<!\S)(?:"?@|-imacros|-iprefix|-iwithprefix)')
                                    }).Count -gt 0) {
                        throw "Onverwerkte compiler-responsbestanden of -imacros in snapshot: $bestand. Voer eerst het verzamelscript uit."
                    }
                    $sketchItems = @($items | Where-Object { $_.PSObject.Properties['file'] -and ([string]$_.file) -like '*.ino.cpp' })
                    if ($sketchItems.Count -eq 0) {
                        throw "Gegenereerde .ino.cpp ontbreekt in compilatiedatabase: $bestand"
                    }
                    # Herstel relatieve paden en absolute snapshotpaden na verplaatsing van Databases.
                    $patroon = [regex]::Match(([string]$sketchItems[0].file), '(?i)^(.*?)[\\/]sketch[\\/][^\\/]+\.ino\.cpp$')
                    if ($patroon.Success -and $patroon.Groups[1].Value.Replace('\', '/') -ne $snapshotmap.Replace('\', '/')) {
                        $patroon = [regex]::Escape($patroon.Groups[1].Value.Replace('\', '/')).Replace('/', '[\\/]')
                        $doelPad = $snapshotmap.Replace('\', '/')
                        foreach ($item in $items) {
                            foreach ($veld in @('directory', 'file', 'command', 'output')) {
                                if ($item.PSObject.Properties[$veld] -and $null -ne $item.$veld) {
                                    $item.$veld = [regex]::Replace([string]$item.$veld, $patroon, $doelPad, [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)
                                }
                            }
                            if ($item.PSObject.Properties['arguments'] -and $item.arguments) {
                                $item.arguments = @($item.arguments | ForEach-Object {
                                    [regex]::Replace([string]$_, $patroon, $doelPad, [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)
                                })
                            }
                        }
                        foreach ($item in $sketchItems) {
                            if (-not (Test-Path -LiteralPath ([string]$item.file) -PathType Leaf)) {
                                throw "Gegenereerde .ino.cpp ontbreekt: $($item.file)"
                            }
                        }
                        # In de proef uitsluitend de in-memory paden corrigeren; snapshots blijven onaangeroerd.
                        if (-not $ProefHashes) {
                            $databasePad = $bestand + '.' + [guid]::NewGuid().ToString('N') + '.tmp'
                            try {
                                [System.IO.File]::WriteAllText($databasePad, (ConvertTo-Json -InputObject $items -Depth 30 -Compress), $utf8)
                                [System.IO.File]::Replace($databasePad, $bestand, [System.Management.Automation.Language.NullString]::Value)
                            } finally {
                                if (Test-Path -LiteralPath $databasePad) { Remove-Item -LiteralPath $databasePad -Force }
                            }
                        }
                    }
                    foreach ($item in $sketchItems) {
                        if (-not (Test-Path -LiteralPath ([string]$item.file) -PathType Leaf)) {
                            throw "Gegenereerde .ino.cpp ontbreekt: $($item.file)"
                        }
                    }
                    $patroon = [regex]::Escape($snapshotmap.Replace('\', '/')).Replace('/', '[\\/]')
                    foreach ($item in $items) {
                        if (-not $item.PSObject.Properties['file']) { throw "Ongeldig compileritem in $bestand" }
                        $bron = ([string]$item.file).Replace('\', '/')
                        # Alleen FrameWork-.cpp, gegenereerde .ino.cpp en eigen sketch-.cpp.
                        $eigenBron = ($bron.StartsWith($framework.Replace('\', '/') + '/src/', [System.StringComparison]::OrdinalIgnoreCase) -and $bron.EndsWith('.cpp', [System.StringComparison]::OrdinalIgnoreCase)) -or
                            ($bron.StartsWith($snapshotmap.Replace('\', '/') + '/sketch/', [System.StringComparison]::OrdinalIgnoreCase) -and $bron.EndsWith('.cpp', [System.StringComparison]::OrdinalIgnoreCase)) -or
                            ($bron.StartsWith((Split-Path -Parent $sketch).Replace('\', '/') + '/', [System.StringComparison]::OrdinalIgnoreCase) -and $bron.EndsWith('.cpp', [System.StringComparison]::OrdinalIgnoreCase))
                        if ($bron.EndsWith('.ino.cpp', [System.StringComparison]::OrdinalIgnoreCase) -and -not $eigenBron) {
                            throw "Gegenereerde .ino.cpp valt buiten de eigen bronselectie: $bron"
                        }
                        if (-not $eigenBron) { continue }
                        if (-not $item.PSObject.Properties['arguments'] -or @($item.arguments).Count -eq 0) {
                            throw "Compileropdracht zonder arguments in $bestand; voer eerst het verzamelscript uit."
                        }

                        # Behoud -D/-U uit de uitgewerkte responsbestanden. Geen volledige core/SDK-preprocessing.
                        $behoudenArgumenten = [System.Collections.Generic.List[string]]::new()
                        for ($i = 0; $i -lt $item.arguments.Count; $i++) {
                            $argument = [string]$item.arguments[$i]
                            if ($argument -cmatch '^(-I|-isystem|-iquote|-idirafter|-include|-imacros)(.*)$') {
                                $waarde = [string]$Matches[2]
                                if ($waarde.Length -eq 0) {
                                    $i++
                                    if ($i -ge $item.arguments.Count) { throw "Include-optie zonder pad voor $($item.file): $argument" }
                                    $waarde = [string]$item.arguments[$i]
                                }
                                $includePad = $waarde.Trim('"').Replace('\', '/')
                                if (-not [System.IO.Path]::IsPathRooted($includePad)) {
                                    $includePad = [System.IO.Path]::GetFullPath((Join-Path ([string]$item.directory) $includePad)).Replace('\', '/')
                                }
                                $includePad = $includePad.TrimEnd('/')
                                if ($argument -cmatch '^(-include|-imacros)' -and -not (Test-Path -LiteralPath $includePad -PathType Leaf)) { continue }
                                # Eigen library/sketch, lokale Arduino-libraries en libraries van de geïnstalleerde core.
                                if ($includePad.Equals((Split-Path -Parent $framework).Replace('\', '/'), [System.StringComparison]::OrdinalIgnoreCase) -or
                                    $includePad.StartsWith((Split-Path -Parent $framework).Replace('\', '/') + '/', [System.StringComparison]::OrdinalIgnoreCase) -or
                                    $includePad.Equals($snapshotmap.Replace('\', '/') + '/sketch', [System.StringComparison]::OrdinalIgnoreCase) -or
                                    $includePad.StartsWith($snapshotmap.Replace('\', '/') + '/sketch/', [System.StringComparison]::OrdinalIgnoreCase) -or
                                    $includePad -match '(?i)/packages/[^/]+/hardware/[^/]+/[^/]+/libraries(?:/|$)') {
                                    if ($argument -cmatch '^(-include|-imacros)') {
                                        [void]$behoudenArgumenten.Add('-include')
                                        [void]$behoudenArgumenten.Add($includePad)
                                    } else {
                                        [void]$behoudenArgumenten.Add('-I' + $includePad)
                                    }
                                }
                                continue
                            }
                            if ($argument -cmatch '^(?:-isysroot.*|--sysroot(?:=.*)?)$') {
                                if ($argument -ceq '-isysroot' -or $argument -ceq '--sysroot') {
                                    $i++
                                    if ($i -ge $item.arguments.Count) { throw "Sysroot-optie zonder pad voor $($item.file)" }
                                }
                                continue
                            }
                            [void]$behoudenArgumenten.Add($argument)
                        }
                        if ($notAnInterruptPerFqbn.ContainsKey([string]$metadata.fqbn) -and
                            @($behoudenArgumenten | Where-Object { $_ -match '^-DNOT_AN_INTERRUPT(?:=|$)' }).Count -eq 0) {
                            [void]$behoudenArgumenten.Add('-DNOT_AN_INTERRUPT=' + $notAnInterruptPerFqbn[$metadata.fqbn])
                        }
                        if ($zephyrMacroDefinitiesPerFqbn.ContainsKey([string]$metadata.fqbn)) {
                            $behoudenArgumenten.AddRange([string[]]$zephyrMacroDefinitiesPerFqbn[[string]$metadata.fqbn])
                        }
                        if ([string]$metadata.fqbn -eq 'arduino:renesas_uno:minima') {
                            if (@($behoudenArgumenten | Where-Object { $_ -match '^-DPROGMEM(?:=|$)' }).Count -eq 0) { [void]$behoudenArgumenten.Add('-DPROGMEM=') }
                            if (@($behoudenArgumenten | Where-Object { $_ -match '^-DNULL(?:=|$)' }).Count -eq 0) { [void]$behoudenArgumenten.Add('-DNULL=0') }
                        }
                        $item.arguments = @($behoudenArgumenten.ToArray())
                        # Een eventuele oude 'command' mag niet opnieuw ongefilterde SDK-flags aanleveren.
                        if ($item.PSObject.Properties['command']) { [void]$item.PSObject.Properties.Remove('command') }

                        [void]$gecontroleerdeBronnen.Add($bron)
                        $bron = [regex]::Replace($bron, $patroon, '<BUILD>', [Text.RegularExpressions.RegexOptions]::IgnoreCase)
                        $opdracht = @($item.arguments) -join ' '
                        $opdracht = [regex]::Replace($opdracht, $patroon, '<BUILD>', [Text.RegularExpressions.RegexOptions]::IgnoreCase)
                        if ($gezien.Add($bron + "`n" + $opdracht)) {
                            if ($aantal -gt 0) { $writer.Write(',') }
                            $writer.Write((ConvertTo-Json -InputObject $item -Depth 30 -Compress))
                            $aantal++
                        }
                    }
                } catch {
                    $soort = if (-not (Test-Path -LiteralPath $metadatapad -PathType Leaf) -or -not (Test-Path -LiteralPath $bestand -PathType Leaf)) { 'ONTBREEKT' } else { 'ONGELDIG' }
                    [void]$problemen.Add("[CCPCHECK_PROBLEEM] $soort | $hash | $($beschrijving[$hash]) | reden: $($_.Exception.Message)")
                } finally {
                    $verwerkt++
                    if (($verwerkt % 25) -eq 0 -or $verwerkt -eq $actueel.Count) {
                        Write-Host "[CCPCHECK] Samenvoegen: $verwerkt/$($actueel.Count)"
                    }
                }
            }
            $ontbrekend = @($problemen | Where-Object { $_ -match '^\[CCPCHECK_PROBLEEM\] ONTBREEKT' }).Count
            Write-Host "[CCPCHECK] Controle afgerond: $verwerkt/$($actueel.Count) | geldig $($verwerkt - $problemen.Count) | ontbrekend $ontbrekend | ongeldig $($problemen.Count - $ontbrekend)"
            foreach ($probleem in $problemen) { Write-Host $probleem }
            if ($problemen.Count -gt 0) {
                Write-Host "[CCPCHECK] Database niet aangemaakt: $($problemen.Count) problemen."
                Write-Host "[CCPCHECK] Bestaande database en snapshots behouden."
                exit 1
            }
            if ($aantal -eq 0) { throw 'Geen compileropdrachten gevonden. Niets opgeruimd.' }
            if ($ProefHashes) {
                Write-Host "[CCPCHECK] Proefmodus: $($actueel.Count) hashes, $($gecontroleerdeBronnen.Count) eigen bronpaden in de database."
            } else {
                $ontbrekend = @(Get-ChildItem -LiteralPath (Join-Path $framework 'src') -Recurse -File -Filter '*.cpp' |
                    Where-Object { -not $gecontroleerdeBronnen.Contains($_.FullName.Replace('\', '/')) })
                if ($ontbrekend.Count -gt 0) {
                    foreach ($bestand in $ontbrekend) { Write-Host "[CCPCHECK_PROBLEEM] FrameWork-.cpp niet geanalyseerd: $($bestand.FullName)" }
                    throw "$($ontbrekend.Count) FrameWork-.cpp-bestand(en) ontbreken in de gezamenlijke compilatiedatabase."
                }
                Write-Host "[CCPCHECK] FrameWork-.cpp-dekking: $($gecontroleerdeBronnen.Count) compilerpaden gecontroleerd, geen ontbrekende src/*.cpp."
            }
            $writer.Write(']')
            $writer.Flush()
        } finally {
            if ($null -ne $writer) { $writer.Dispose() }
        }

        # Pas na succesvolle validatie en samenvoeging de vorige database vervangen.
        if (Test-Path -LiteralPath $database -PathType Leaf) {
            [System.IO.File]::Replace($tijdelijkeUitvoer, $database, [System.Management.Automation.Language.NullString]::Value)
        } else {
            [System.IO.File]::Move($tijdelijkeUitvoer, $database)
        }
        Write-Host "[CCPCHECK] Samenvoegen OK: $($actueel.Count) actuele hashes, $aantal compileropdrachten."

        # Proefmodus ruimt nooit snapshots op; normale volledige samenvoeging blijft ongewijzigd.
        if ($ProefHashes) {
            Write-Host '[CCPCHECK] Proefmodus: geen snapshots opgeruimd.'
        } else {
            # Verwijder alleen echte verouderde hashmappen na succesvolle samenvoeging.
            $opgeruimd = 0
            foreach ($map in @(Get-ChildItem -LiteralPath $basis -Directory -ErrorAction SilentlyContinue)) {
                if ($map.Name -match '^[0-9a-f]{64}$' -and -not $actueel.Contains($map.Name)) {
                    Remove-Item -LiteralPath $map.FullName -Recurse -Force
                    $opgeruimd++
                }
            }
            Write-Host "[CCPCHECK] Opruimen OK: $opgeruimd verouderde hashmappen verwijderd."
        }
    } finally {
        if (Test-Path -LiteralPath $tijdelijkeUitvoer) { Remove-Item -LiteralPath $tijdelijkeUitvoer -Force }
        if ($vergrendeld) { $mutex.ReleaseMutex() }
        $mutex.Dispose()
    }
    exit 0
}

if ($Capture) {
    if (-not (Test-Path -LiteralPath $Capture -PathType Container)) {
        throw "Tijdelijke Arduino-buildmap ontbreekt: $Capture"
    }

    $bronmap = (Resolve-Path -LiteralPath $Capture).Path.TrimEnd('\', '/')
    $bronDatabase = Join-Path $bronmap 'compile_commands.json'
    $test = if ($env:TEST_INO_ACTIEF) { $env:TEST_INO_ACTIEF } else { $env:TEST_INO }
    $testNaam = if ($env:TEST_NAAM_ACTIEF) { $env:TEST_NAAM_ACTIEF } else { $env:TEST_NAAM }
    $flags = if ($env:TEST_FLAGS_ACTIEF) { $env:TEST_FLAGS_ACTIEF } else { [string]$env:TEST_FLAGS }
    $fqbn = if ($env:FQBN_ACTIEF) { $env:FQBN_ACTIEF } else { $env:TEST_FQBN }
    $boardVersion = if ($env:BOARD_VERSION_ACTIEF) { $env:BOARD_VERSION_ACTIEF } else { $env:TEST_BOARD_VERSION }
    $sleutel = @($Mode, $env:BESTANDSNAAM, $test, $testNaam, $flags, $fqbn, $boardVersion) -join "`n"

    $sha = [System.Security.Cryptography.SHA256]::Create()
    try {
        $hash = [BitConverter]::ToString($sha.ComputeHash([System.Text.Encoding]::UTF8.GetBytes($sleutel))).Replace('-', '').ToLowerInvariant()
    } finally {
        $sha.Dispose()
    }

    $basis = Join-Path $opslag $Mode
    New-Item -ItemType Directory -Path $basis -Force | Out-Null
    $doelmap = Join-Path $basis $hash
    $tijdelijkeMap = Join-Path $basis ($hash + '.' + [guid]::NewGuid().ToString('N') + '.tmp')
    New-Item -ItemType Directory -Path $tijdelijkeMap -Force | Out-Null

    try {
        $metadata = [ordered]@{
            testscript = $env:BESTANDSNAAM
            test = $test
            testNaam = $testNaam
            flags = $flags
            fqbn = $fqbn
            boardVersion = $boardVersion
            mode = $Mode
            compileResult = $env:COMPILE_RESULT
            databaseBeschikbaar = (Test-Path -LiteralPath $bronDatabase -PathType Leaf)
            sketchHash = $env:SKETCH_HASH_ACTIEF
            bronHash = $env:BRON_HASH_ACTIEF
        }

        if (-not $metadata.databaseBeschikbaar -and $env:COMPILE_RESULT -eq '0') {
            throw "Arduino-compilatie is geslaagd, maar compile_commands.json ontbreekt in $bronmap"
        }

        if ($metadata.databaseBeschikbaar) {
            $geladen = ConvertFrom-Json -InputObject (Get-Content -LiteralPath $bronDatabase -Raw -Encoding UTF8)
            $items = if ($geladen.PSObject.Properties['value'] -and $geladen.value) {
                @($geladen.value)
            } else {
                @($geladen)
            }
            if ($items.Count -eq 1 -and $items[0] -is [array]) { $items = @($items[0]) }
            if ($items.Count -eq 0 -or -not $items[0].PSObject.Properties['file']) {
                throw "Ongeldige compilatiedatabase: $bronDatabase"
            }

            # Bewaar alleen gegenereerde broncode en headers. Grote binaire buildbestanden zijn niet nodig.
            foreach ($bestand in (Get-ChildItem -LiteralPath $bronmap -Recurse -File)) {
                if ($bestand.Extension -notin @('.h', '.hpp', '.hxx', '.c', '.cc', '.cpp', '.cxx', '.ino', '.inc')) { continue }
                $relatief = $bestand.FullName.Substring($bronmap.Length + 1)
                $doel = Join-Path $tijdelijkeMap $relatief
                New-Item -ItemType Directory -Path (Split-Path -Parent $doel) -Force | Out-Null
                Copy-Item -LiteralPath $bestand.FullName -Destination $doel
            }

            # Werk uitsluitend op de echte compilerargumenten (Arduino CLI levert een JSON-array 'arguments').
            # Responsbestanden kunnen onder andere -iwithprefixbefore en -imacros bevatten.
            # Optionele cache geldt uitsluitend tijdens deze Capture; inhoudshash voorkomt verouderde treffers.
            $responsbestandCache = $null
            if ($CacheResponsbestanden) {
                $responsbestandCache = [System.Collections.Generic.Dictionary[string,object]]::new([System.StringComparer]::OrdinalIgnoreCase)
            }
            foreach ($item in $items) {
                if (-not $item.PSObject.Properties['arguments'] -or @($item.arguments).Count -eq 0) {
                    throw "Compileropdracht zonder arguments in $bronDatabase; kan niet veilig worden verwerkt."
                }
                $opdracht = @($item.arguments | ForEach-Object { [string]$_ })
                for ($poging = 0; $poging -lt 8; $poging++) {
                    if (@($opdracht | Where-Object { $_ -match '^@' }).Count -eq 0) { break }
                    $uitvoer = [System.Collections.Generic.List[string]]::new()
                    foreach ($argument in $opdracht) {
                        if ($argument -notmatch '^@(.+)$') {
                            [void]$uitvoer.Add($argument)
                            continue
                        }
                        $responsePad = $Matches[1].Trim('"')
                        if (-not [System.IO.Path]::IsPathRooted($responsePad)) {
                            $responsePad = Join-Path ([string]$item.directory) $responsePad
                        }
                        if (-not (Test-Path -LiteralPath $responsePad -PathType Leaf)) {
                            throw "Compiler-responsbestand ontbreekt: $responsePad"
                        }
                        if ($null -ne $responsbestandCache -and $responsbestandCache.ContainsKey($responsePad) -and
                            $responsbestandCache[$responsePad].Hash -eq (Get-FileHash -LiteralPath $responsePad -Algorithm SHA256).Hash) {
                            $overeenkomsten = $responsbestandCache[$responsePad].Argumenten
                        } else {
                            $inhoud = Get-Content -LiteralPath $responsePad -Raw -Encoding UTF8
                            # GCC-responsbestanden: backslash escaped het volgende teken; enkele en dubbele quotes groeperen.
                            $overeenkomsten = [System.Collections.Generic.List[string]]::new()
                            $opgebouwdArgument = $null
                            $openAanhalingsteken = $null
                            for ($i = 0; $i -lt $inhoud.Length; $i++) {
                                $huidigTeken = [string]$inhoud[$i]
                                if ($huidigTeken -eq '\') {
                                    $i++
                                    if ($i -ge $inhoud.Length) { throw "Onvolledige escape in compiler-responsbestand: $responsePad" }
                                    if ($null -eq $opgebouwdArgument) { $opgebouwdArgument = '' }
                                    $opgebouwdArgument += [string]$inhoud[$i]
                                    continue
                                }
                                if ($huidigTeken -eq '"' -or $huidigTeken -eq "'") {
                                    if ($null -eq $openAanhalingsteken) {
                                        $openAanhalingsteken = $huidigTeken
                                        if ($null -eq $opgebouwdArgument) { $opgebouwdArgument = '' }
                                        continue
                                    }
                                    if ($openAanhalingsteken -eq $huidigTeken) {
                                        $openAanhalingsteken = $null
                                        continue
                                    }
                                }
                                if ([char]::IsWhiteSpace($inhoud[$i]) -and $null -eq $openAanhalingsteken) {
                                    if ($null -ne $opgebouwdArgument) {
                                        [void]$overeenkomsten.Add($opgebouwdArgument)
                                        $opgebouwdArgument = $null
                                    }
                                    continue
                                }
                                if ($null -eq $opgebouwdArgument) { $opgebouwdArgument = '' }
                                $opgebouwdArgument += $huidigTeken
                            }
                            if ($null -ne $openAanhalingsteken) { throw "Niet afgesloten quotes in compiler-responsbestand: $responsePad" }
                            if ($null -ne $opgebouwdArgument) { [void]$overeenkomsten.Add($opgebouwdArgument) }
                            if ($null -ne $responsbestandCache) {
                                $responsbestandCache[$responsePad] = [pscustomobject]@{
                                    Hash = (Get-FileHash -LiteralPath $responsePad -Algorithm SHA256).Hash
                                    Argumenten = [string[]]$overeenkomsten.ToArray()
                                }
                            }
                        }
                        foreach ($overeenkomst in $overeenkomsten) {
                            [void]$uitvoer.Add($overeenkomst)
                        }
                    }
                    $opdracht = @($uitvoer.ToArray())
                }
                if (@($opdracht | Where-Object { $_ -match '^@' }).Count -gt 0) {
                    throw "Niet alle compiler-responsbestanden verwerkt voor $($item.file)."
                }

                $uitvoer = [System.Collections.Generic.List[string]]::new()
                $includeArgumentenAchteraan = [System.Collections.Generic.List[string]]::new()
                $iprefix = $null
                for ($i = 0; $i -lt $opdracht.Count; $i++) {
                    $argument = [string]$opdracht[$i]
                    if ($argument -match '^-iprefix(.*)$') {
                        $waarde = $Matches[1]
                        if (-not $waarde) {
                            $i++
                            if ($i -ge $opdracht.Count) { throw "Ontbrekende waarde bij -iprefix: $($item.file)" }
                            $waarde = [string]$opdracht[$i]
                        }
                        $iprefix = $waarde
                        continue
                    }
                    if ($argument -match '^-iwithprefix(before)?(.*)$') {
                        $waarde = $Matches[2]
                        if (-not $waarde) {
                            $i++
                            if ($i -ge $opdracht.Count) { throw "Ontbrekende waarde bij -iwithprefix: $($item.file)" }
                            $waarde = [string]$opdracht[$i]
                        }
                        if (-not $iprefix) { throw "-iwithprefix zonder -iprefix: $($item.file)" }
                        # GCC plakt de suffix aan -iprefix; dit is niet hetzelfde als Join-Path.
                        $responsePad = ($iprefix + $waarde).Replace('\', '/')
                        if (-not [System.IO.Path]::IsPathRooted($responsePad)) {
                            $responsePad = Join-Path ([string]$item.directory) $responsePad
                        }
                        if (-not (Test-Path -LiteralPath $responsePad -PathType Container)) {
                            Write-Warning "Include-map uit -iprefix/-iwithprefix ontbreekt en wordt overgeslagen: $responsePad"
                            continue
                        }
                        if ($argument.StartsWith('-iwithprefixbefore')) {
                            [void]$uitvoer.Add('-I' + $responsePad)
                        } else {
                            [void]$includeArgumentenAchteraan.Add('-I' + $responsePad)
                        }
                        continue
                    }
                    if ($argument -match '^-imacros(.*)$') {
                        $waarde = $Matches[1]
                        if (-not $waarde) {
                            $i++
                            if ($i -ge $opdracht.Count) { throw "Ontbrekende waarde bij -imacros: $($item.file)" }
                            $waarde = [string]$opdracht[$i]
                        }
                        # Cppcheck 2.22 leest geforceerde -include uit compilatiedatabases.
                        [void]$uitvoer.Add('-include')
                        [void]$uitvoer.Add($waarde)
                        continue
                    }
                    [void]$uitvoer.Add($argument)
                }
                foreach ($overeenkomst in $includeArgumentenAchteraan) { [void]$uitvoer.Add($overeenkomst) }
                $item.arguments = @($uitvoer.ToArray())
            }

            # Arduino CLI verwijst in de JSON nog naar de tijdelijke buildmap.
            # Vervang die verwijzingen door de definitieve snapshotmap.
            $patroon = '(?i)[A-Za-z]:[\\/](?:[^\\/"\r\n]+[\\/])*' + [regex]::Escape((Split-Path -Leaf $bronmap))
            $doelPad = $doelmap.Replace('\', '/')
            foreach ($item in $items) {
                foreach ($veld in @('directory', 'file', 'command', 'output')) {
                    if ($item.PSObject.Properties[$veld] -and $null -ne $item.$veld) {
                        $item.$veld = [regex]::Replace([string]$item.$veld, $patroon, $doelPad, [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)
                    }
                }
                if ($item.PSObject.Properties['arguments'] -and $item.arguments) {
                    $item.arguments = @($item.arguments | ForEach-Object {
                        [regex]::Replace([string]$_, $patroon, $doelPad, [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)
                    })
                }
            }
            $gegenereerd = @($items | Where-Object { $_.PSObject.Properties['file'] -and ([string]$_.file) -like '*.ino.cpp' })
            if ($gegenereerd.Count -eq 0) { throw "Gegenereerde .ino.cpp ontbreekt in compilatiedatabase: $bronDatabase" }
            $lokaleSketch = ([string]$gegenereerd[0].file).Replace($doelmap.Replace('\', '/'), $tijdelijkeMap.Replace('\', '/'))
            if (-not (Test-Path -LiteralPath $lokaleSketch -PathType Leaf)) {
                throw "Gegenereerde .ino.cpp ontbreekt in snapshot: $lokaleSketch"
            }
            foreach ($item in $gegenereerd) {
                $lokaleSketch = ([string]$item.file).Replace($doelmap.Replace('\', '/'), $tijdelijkeMap.Replace('\', '/'))
                if (-not (Test-Path -LiteralPath $lokaleSketch -PathType Leaf) -or
                    -not (Select-String -LiteralPath $lokaleSketch -Pattern '^\s*#line\s+\d+\s+.*\.ino' -Quiet)) {
                    throw "Gegenereerde .ino.cpp zonder controleerbare oorspronkelijke .ino-regels: $lokaleSketch"
                }
            }
            [System.IO.File]::WriteAllText((Join-Path $tijdelijkeMap 'compile_commands.json'), (ConvertTo-Json -InputObject $items -Depth 30 -Compress), $utf8)
        }

        [System.IO.File]::WriteAllText((Join-Path $tijdelijkeMap 'metadata.json'), (ConvertTo-Json -InputObject $metadata -Depth 5), $utf8)
        $oudeMap = $doelmap + '.vorig'
        if (Test-Path -LiteralPath $oudeMap) { throw "Back-up van snapshot bestaat nog: $oudeMap" }
        if (Test-Path -LiteralPath $doelmap) { Move-Item -LiteralPath $doelmap -Destination $oudeMap }
        try {
            Move-Item -LiteralPath $tijdelijkeMap -Destination $doelmap
        } catch {
            if (Test-Path -LiteralPath $oudeMap) { Move-Item -LiteralPath $oudeMap -Destination $doelmap }
            throw
        }
        if (Test-Path -LiteralPath $oudeMap) { Remove-Item -LiteralPath $oudeMap -Recurse -Force }

        if ($metadata.databaseBeschikbaar) {
            Write-Host "[CCPCHECK] $Mode opgeslagen: $test | $fqbn | $boardVersion"
        } else {
            Write-Host "[CCPCHECK] $Mode zonder JSON geregistreerd: $test | $fqbn | $boardVersion"
        }
    } finally {
        if (Test-Path -LiteralPath $tijdelijkeMap) { Remove-Item -LiteralPath $tijdelijkeMap -Recurse -Force }
    }
    exit 0
}

# Zonder parameters, of met -Finalize: bestaande snapshots opnieuw samenvoegen.
# Een Windows-mutex beschermt beide uitvoerbestanden bij parallelle testdomeinen.
$mutex = [System.Threading.Mutex]::new($false, 'Local\GroeiAcademieCcpcheckMerge')
$vergrendeld = $false
try {
    try {
        $vergrendeld = $mutex.WaitOne(600000)
    } catch [System.Threading.AbandonedMutexException] {
        $vergrendeld = $true
    }
    if (-not $vergrendeld) { throw 'Timeout bij samenvoegen parallelle Cppcheck-databases.' }

    foreach ($soort in $(if ($SnapshotRoot) { @('Gewoon') } else { @('Gewoon', 'Ongeldig') })) {
        $basis = Join-Path $opslag $soort
        $bestanden = @(Get-ChildItem -LiteralPath $basis -Filter 'compile_commands.json' -Recurse -File -ErrorAction SilentlyContinue |
            Where-Object { $_.Directory.Name -notlike '*.tmp' } | Sort-Object FullName)
        if ($bestanden.Count -eq 0) {
            Write-Host "[CCPCHECK] Geen snapshots voor $soort. Bestaande database niet overschreven."
            continue
        }

        $lijst = New-Object 'System.Collections.Generic.List[object]'
        $gezien = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::OrdinalIgnoreCase)
        foreach ($bestand in $bestanden) {
            $geladen = ConvertFrom-Json -InputObject (Get-Content -LiteralPath $bestand.FullName -Raw -Encoding UTF8)
            $items = if ($geladen.PSObject.Properties['value'] -and $geladen.value) {
                @($geladen.value)
            } else {
                @($geladen)
            }
            if ($items.Count -eq 1 -and $items[0] -is [array]) { $items = @($items[0]) }
            $snapshotmap = $bestand.Directory.FullName.Replace('\', '/')
            $patroon = [regex]::Escape($snapshotmap).Replace('/', '[\\/]')
            foreach ($item in $items) {
                if (-not $item.PSObject.Properties['file']) { throw "Ongeldig compileritem in $($bestand.FullName)" }
                $bron = [string]$item.file
                $opdracht = if ($item.PSObject.Properties['command']) {
                    [string]$item.command
                } else {
                    (@($item.arguments) -join ' ')
                }
                # Alleen gelijkwaardige compileopdrachten samenvoegen; verschillen in
                # board, compilerdefinities of configuratie blijven afzonderlijk.
                $genormaliseerd = [regex]::Replace($opdracht, $patroon, '<BUILD>', [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)
                $bron = [regex]::Replace($bron, $patroon, '<BUILD>', [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)
                $sleutel = $bron + "`n" + $genormaliseerd
                if ($gezien.Add($sleutel)) { [void]$lijst.Add($item) }
            }
        }

        $uitvoer = if ($soort -eq 'Gewoon') { $database } else { $databaseOngeldig }
        $tijdelijkeUitvoer = $uitvoer + '.' + [guid]::NewGuid().ToString('N') + '.tmp'
        try {
            [System.IO.File]::WriteAllText($tijdelijkeUitvoer, (ConvertTo-Json -InputObject $lijst.ToArray() -Depth 30 -Compress), $utf8)
            if (Test-Path -LiteralPath $uitvoer) {
                [System.IO.File]::Replace($tijdelijkeUitvoer, $uitvoer, [System.Management.Automation.Language.NullString]::Value)
            } else {
                [System.IO.File]::Move($tijdelijkeUitvoer, $uitvoer)
            }
        } finally {
            if (Test-Path -LiteralPath $tijdelijkeUitvoer) { Remove-Item -LiteralPath $tijdelijkeUitvoer -Force }
        }
        Write-Host "[CCPCHECK] $soort : $($bestanden.Count) snapshots, $($lijst.Count) compileropdrachten in $(Split-Path -Leaf $uitvoer)"
    }
} finally {
    if ($vergrendeld) { $mutex.ReleaseMutex() }
    $mutex.Dispose()
}
