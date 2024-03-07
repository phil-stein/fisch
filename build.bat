@echo off

:: check if arg was given
IF "%~1"=="" GOTO editor

:: build game
echo building GAME
cd scripts
build_game
cd ..
GOTO end

:: build editor
:editor
echo building EDITOR 
cd scripts
build_editor
cd ..

:end
