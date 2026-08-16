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

set "CLI_PATH=C:\Pad\Naar\map\met\arduino-cli"
set "LINT_PATH=C:\Pad\Naar\map\met\arduino-lint"
