@echo off
:: LokalePaden_template.cmd
::
:: Kopieer dit bestand in dezelfde map (extras/) naar LokalePaden.cmd en vul de paden hieronder
:: in naar waar arduino-cli(.exe) en arduino-lint(.exe) op DEZE machine staan.
::
:: LokalePaden.cmd zelf wordt nooit gedeeld of gepubliceerd (zie .gitignore); dit
:: template-bestand wel. Enkel nodig wanneer arduino-cli niet via PATH gevonden wordt
:: (test dat eerst met "where arduino-cli" in een terminal).
::
:: LINT_PATH mag leeg blijven wanneer arduino-lint niet geïnstalleerd is; TestLibraryGereleased.cmd
:: valt dan terug op dezelfde map als CLI_PATH.

set "PATH_OPTION1="
set "PATH_OPTION2="
set "PATH_OPTION3="
set "CCPCHECK_DATA_PATH="

if exist "%PATH_OPTION1%\arduino-cli.exe" (
    set "BASE_PATH=%PATH_OPTION1%"
) else if exist "%PATH_OPTION2%\arduino-cli.exe" (
    set "BASE_PATH=%PATH_OPTION2%"
) else if defined PATH_OPTION3 if exist "%PATH_OPTION3%\arduino-cli.exe" (
    set "BASE_PATH=%PATH_OPTION3%"
)

if defined BASE_PATH (
    set "CLI_PATH=%BASE_PATH%\arduino-cli"
    set "LINT_PATH=%BASE_PATH%\arduino-lint"
)
