@echo off

set args=""

:: check if arg was given
IF "%~1"=="no_run" (
  set args="no_run"
  GOTO editor
) 
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
:: echo %args%
build_editor %args%
cd ..

:end
