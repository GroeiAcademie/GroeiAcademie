@echo off
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0FIX_Libraries_COMPILE_ISSUES.ps1" %*
pause