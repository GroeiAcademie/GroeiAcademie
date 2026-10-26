# Minimale correcties voor lokaal geïnstalleerde externe libraries.
# Verandert geen bestanden van GROEI ACADEMIE FrameWork.
$ErrorActionPreference = 'Stop'
$root = Join-Path $env:USERPROFILE 'Documents\Arduino\libraries'
$targets = @(
    (Join-Path $root 'I2C_SCANNER\I2C_SCANNER.h'),
    (Join-Path $root 'I2C_SCANNER\I2C_SCANNER.cpp'),
    (Join-Path $root 'Adafruit_ST7735_and_ST7789_Library\Adafruit_ST7796S.h'),
    (Join-Path $root 'Adafruit_ST7735_and_ST7789_Library\Adafruit_ST7796S.cpp'),
    (Join-Path $root 'Adafruit_ST7735_and_ST7789_Library\Adafruit_ST77xx.cpp')
)
foreach ($path in $targets) { if (-not (Test-Path -LiteralPath $path)) { throw "Ontbrekend bibliotheekbestand: $path. Niets aangepast." } }
$encoding = [System.Text.Encoding]::GetEncoding(28591)
$original = @{}
$modified = @{}
foreach ($path in $targets) { $original[$path] = $encoding.GetString([System.IO.File]::ReadAllBytes($path)); $modified[$path] = $original[$path] }

# I2C_SCANNER 0.3.3: controleer exact de bestaande functiedeclaratie.
$h = $targets[0]; $cpp = $targets[1]
$old = 'void releaseI2CBus(uint8_t SDA, uint8_t SCL);'
$new = 'void releaseI2CBus(uint8_t sdaPin, uint8_t sclPin);'
if ($modified[$h].Contains($old)) { $modified[$h] = $modified[$h].Replace($old,$new) }
elseif (-not $modified[$h].Contains($new)) { throw "Onbekende I2C_SCANNER.h-versie: $h. Niets aangepast." }
$pattern = '(?s)void\s+I2C_SCANNER::releaseI2CBus\s*\(\s*uint8_t\s+SDA\s*,\s*uint8_t\s+SCL\s*\).*?(?=////////////////////////////////////////////////////)'
$m = [regex]::Match($modified[$cpp],$pattern)
if ($m.Success) {
    $replacement = [regex]::Replace($m.Value,'\bSDA\b','sdaPin')
    $replacement = [regex]::Replace($replacement,'\bSCL\b','sclPin')
    $modified[$cpp] = $modified[$cpp].Substring(0,$m.Index) + $replacement + $modified[$cpp].Substring($m.Index+$m.Length)
} elseif (-not ($modified[$cpp] -match 'void\s+I2C_SCANNER::releaseI2CBus\s*\(\s*uint8_t\s+sdaPin\s*,\s*uint8_t\s+sclPin\s*\)')) {
    throw "Onbekende I2C_SCANNER.cpp-versie: $cpp. Niets aangepast."
}

# Adafruit 1.12.0: wijzig alleen software-SPI-constructordeclaratie en -definitie.
$h = $targets[2]; $cpp = $targets[3]
$pattern = '(?s)(Adafruit_ST7796S\s*\(\s*int16_t\s+CS\s*,\s*int16_t\s+RS\s*,\s*int16_t\s+)MOSI(\s*,\s*int16_t\s+SCLK\s*,\s*int16_t\s+RST\s*=\s*-1\s*\))'
if ([regex]::IsMatch($modified[$h],$pattern)) { $modified[$h] = [regex]::Replace($modified[$h],$pattern,'${1}mosiPin${2}') }
elseif (-not ($modified[$h] -match 'int16_t\s+mosiPin')) { throw "Onbekende Adafruit_ST7796S.h-versie: $h. Niets aangepast." }
$pattern = '(?s)(Adafruit_ST7796S::Adafruit_ST7796S\s*\(\s*int16_t\s+CS\s*,\s*int16_t\s+RS\s*,\s*int16_t\s+)MOSI(\s*,\s*int16_t\s+SCLK\s*,\s*int16_t\s+RST\s*\)\s*:\s*Adafruit_ST77xx\s*\(\s*ST7796S_TFTWIDTH\s*,\s*ST7796S_TFTHEIGHT\s*,\s*CS\s*,\s*RS\s*,\s*)MOSI(\s*,\s*SCLK\s*,\s*RST\s*\))'
if ([regex]::IsMatch($modified[$cpp],$pattern)) { $modified[$cpp] = [regex]::Replace($modified[$cpp],$pattern,'${1}mosiPin${2}mosiPin${3}') }
elseif (-not ($modified[$cpp] -match 'int16_t\s+mosiPin')) { throw "Onbekende Adafruit_ST7796S.cpp-versie: $cpp. Niets aangepast." }

# Adafruit 1.12.0: STM32-core 3.0.0 heeft geen wiring_private.h; sluit STM32 uit in deze ene regel.
$cpp = $targets[4]
$old = '#if !defined(ARDUINO_STM32_FEATHER) && !defined(ARDUINO_UNOR4_WIFI)'
$new = '#if !defined(ARDUINO_STM32_FEATHER) && !defined(ARDUINO_UNOR4_WIFI) && !defined(ARDUINO_ARCH_STM32)'
$pattern = '(?m)^' + [regex]::Escape($old) + '[ \t]*(?=\r?$)'
if ($modified[$cpp].Contains($new)) { }
elseif ([regex]::Matches($modified[$cpp],$pattern).Count -eq 1) { $modified[$cpp] = [regex]::Replace($modified[$cpp],$pattern,$new) }
else { throw "Onbekende Adafruit_ST77xx.cpp-versie: $cpp. Niets aangepast." }

$changed = @($targets | Where-Object { $modified[$_] -cne $original[$_] })
foreach ($path in $changed) {
    $backup = "$path.RUN33.bak"
    if (Test-Path -LiteralPath $backup) { throw "Back-up bestaat al: $backup. Niets aangepast." }
}
foreach ($path in $changed) { [System.IO.File]::WriteAllBytes("$path.RUN33.bak", [System.IO.File]::ReadAllBytes($path)) }
foreach ($path in $changed) { [System.IO.File]::WriteAllBytes($path, $encoding.GetBytes($modified[$path])); Write-Host "GECORRIGEERD: $path" }
Write-Host "GEREED: $($changed.Count) bibliotheekbestanden gecorrigeerd. Bestaande back-ups hebben suffix .RUN33.bak."
Write-Host 'Voer nu de bestaande Tools- en Screen-test-CMD uit. Geen tests gewijzigd.'
