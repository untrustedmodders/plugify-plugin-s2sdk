@echo off
REM build.bat - For Windows builds

if not exist "%PREFIX%\bin" mkdir "%PREFIX%\bin"

copy bin\plugify-plugin-s2sdk.dll "%PREFIX%\bin\" || exit 1
copy plugify-plugin-s2sdk.pplugin "%PREFIX%\" || exit 1
copy s2sdk.gamedata.jsonc "%PREFIX%\" || exit 1
copy s2sdk.settings.jsonc "%PREFIX%\" || exit 1
copy s2sdk.translations.jsonc "%PREFIX%\" || exit 1

exit 0
