@echo off

:: goto root
cd ..

:: fisch
rmdir /q /s _build\fisch\debug
mkdir    _build\fisch\debug
rmdir /q /s _build\fisch\release
mkdir    _build\fisch\release
rmdir /q /s _build\fisch\vs19
mkdir    _build\fisch\vs19
echo fisch

:: fisch_editor
rmdir /q /s _build\fisch_editor\debug
mkdir    _build\fisch_editor\debug
rmdir /q /s _build\fisch_editor\release
mkdir    _build\fisch_editor\release
rmdir /q /s _build\fisch_editor\vs19
mkdir    _build\fisch_editor\vs19
echo fisch_editor

:: fisch_external
rmdir /q /s _build\fisch_external\debug
mkdir    _build\fisch_external\debug
rmdir /q /s _build\fisch_external\release
mkdir    _build\fisch_external\release
rmdir /q /s _build\fisch_external\vs19
mkdir    _build\fisch_external\vs19
echo fisch_external

:: editor
rmdir /q /s _build\editor\debug
mkdir    _build\editor\debug
rmdir /q /s _build\editor\release
mkdir    _build\editor\release
rmdir /q /s _build\editor_vs19
mkdir    _build\editor_vs19
echo editor

:: game 
rmdir /q /s _build\game\debug
mkdir    _build\game\debug
rmdir /q /s _build\game\release
mkdir    _build\game\release
rmdir /q /s _build\game_vs19
mkdir    _build\game_vs19
echo game

:: :: tex_viewer 
:: rmdir /s _build\tex_viewer\debug
:: mkdir    _build\tex_viewer\debug
:: rmdir /s _build\tex_viewer\release
:: mkdir    _build\tex_viewer\release
:: rmdir /s _build\tex_viewer_vs19
:: mkdir    _build\tex_viewer_vs19
:: echo tex_viewer

