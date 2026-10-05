@echo off
setlocal EnableDelayedExpansion
cd /d "%~dp0"

powershell -NoProfile -Command "Get-ChildItem -Path '..\src' -Include '*.h','*.cpp' -Recurse | Select-String -Pattern 'PrintToScreen\(\x22','Serial\.print\(\x22','Serial\.println\(\x22','GA_SERIAL\.print\(\x22','GA_SERIAL\.println\(\x22' | Where-Object { $_.Line -notmatch 'Serial\.(?:print|println)\(\x22\s*\x22\)' -and $_.Line -notmatch 'GA_SERIAL\.print(?:ln)?\(\x22(?:TRACE|DEBUG):' } | ForEach-Object { '{0}:{1}:{2}' -f $_.Path,$_.LineNumber,$_.Line }" >"%TEMP%\GA_Hardcoded.txt"

set "HARDCODED_SIZE=0"
for %%A in ("%TEMP%\GA_Hardcoded.txt") do set "HARDCODED_SIZE=%%~zA"

if !HARDCODED_SIZE! GTR 0 (
    echo WARNING: Hardcoded gebruikerstekst gevonden.
    type "%TEMP%\GA_Hardcoded.txt"
) else (
    echo OK: Geen hardcoded gebruikerstekst gevonden.
)

del "%TEMP%\GA_Hardcoded.txt" >nul 2>&1

echo.
pause
endlocal
